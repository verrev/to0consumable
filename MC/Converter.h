#pragma once
#include "Filestructure.h"
#include "Fileutils.h"
#include <string>
#include <vector>
#include <fbxsdk.h>
#pragma comment(lib,"libfbxsdk-md.lib")

#ifdef _DEBUG
#define LOG(x) MessageBox(0, TEXT(x), L"Error", MB_OK);
#else
#define LOG(x)
#endif

void getIndices(std::vector<UINT> &indices, UINT vcount);
void convertScene(std::string scenePath);
void getMeshes(FbxNode *node, std::vector<FbxMesh*> &meshes);
void processMesh(FbxMesh* m, std::vector<Vertex1P1N1UV> &vertices);
void getMaterials(FbxNode *n, std::vector<std::string> &textures);
void convertModel(FbxNode *rootNode);
void getNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
void getUV(FbxMesh* m, int i, int j, XMFLOAT2& outUV);
void convertModel(FbxNode *rootNode, std::string s);

void getIndices(std::vector<UINT> &indices, UINT vcount)
{
	for (int i = 0; i < vcount; ++i) {
		indices.push_back(i);
	}
}
void convertModel(FbxNode *rootNode, std::string s)
{
	std::vector<FbxMesh*> meshes;
	getMeshes(rootNode, meshes);
	for (int i = 0; i < meshes.size(); ++i) {
		std::vector<Vertex1P1N1UV> v;
		processMesh(meshes[i], v);

		RenderableHeader h;
		h.verticeCount = v.size();
		h.indiceCount = h.verticeCount;
		h.vsInputLayout = VERTEX_ELEMENT::POSITION | VERTEX_ELEMENT::NORMAL | VERTEX_ELEMENT::TEXCOORD;

		strcpy_s(h.vshaderPath, "C:/Users/Vootele/Desktop/0/Debug/VS2.cso");
		strcpy_s(h.pshaderPath, "C:/Users/Vootele/Desktop/0/Debug/PS2.cso");

		Fileutils::writeFile(s, &h, sizeof(RenderableHeader));
		Fileutils::writeFile(s, &v[0], sizeof(Vertex1P1N1UV) * h.verticeCount, std::ios::app);

		std::vector<UINT> indices;
		getIndices(indices, h.verticeCount);

		Fileutils::writeFile(s, &indices[0], sizeof(UINT) * indices.size(), std::ios::app);

		XMMATRIX world = XMMatrixIdentity();
		Fileutils::writeFile(s, &world, sizeof(XMMATRIX), std::ios::app);

		std::vector<std::string> textures;
		getMaterials(meshes[i]->GetNode(), textures);
		int count = textures.size();
		Fileutils::writeFile(s, &count , sizeof(int), std::ios::app);

		for (auto a : textures) {
			char tex[75];
			strcpy_s(tex, a.c_str());
			Fileutils::writeFile(s, tex, sizeof(char) * 75, std::ios::app);
		}

		s += "i";
	}
}
void convertScene(std::string scenePath)
{
	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(scenePath.c_str(), -1, manager->GetIOSettings());

	FbxScene* scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	importer->Destroy();

	FbxAxisSystem axisSystem = FbxAxisSystem::eDirectX;
	if (scene->GetGlobalSettings().GetAxisSystem() != axisSystem) axisSystem.ConvertScene(scene);

	scenePath.replace(scenePath.find(".FBX"), 4, ".aaa");

	convertModel(scene->GetRootNode(), scenePath);
}
void getMeshes(FbxNode *node, std::vector<FbxMesh*> &meshes)
{
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {
		if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh) {
			meshes.push_back(static_cast<FbxMesh*>(node->GetNodeAttributeByIndex(i)));
		}
	}
	for (int i = 0; i < node->GetChildCount(); ++i) {
		getMeshes(node->GetChild(i), meshes);
	}
}
void getNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}
	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void getMaterials(FbxNode *n, std::vector<std::string> &textures)
{
	for (int i = 0; i < n->GetMaterialCount(); ++i) {
		FbxSurfaceMaterial *material = n->GetMaterial(i);
		FbxProperty property = material->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
		if (property.IsValid()) {
			for (int j = 0; j < property.GetSrcObjectCount<FbxTexture>(); ++j) {
				if (!property.GetSrcObject<FbxLayeredTexture>(j)) {
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(j);
					if (texture){
						std::string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
						if (fileTexture){
							if (textureType == "DiffuseColor"){
								textures.push_back(fileTexture->GetRelativeFileName());
							}
						}
					}
				}
			}
		}
	}
}
void getUV(FbxMesh* m, int i, int j, XMFLOAT2& outUV)
{
	FbxStringList lUVSetNameList;
	m->GetUVSetNames(lUVSetNameList);
	FbxVector2 fbxUV;
	bool unmapped = 0;
	m->GetPolygonVertexUV(i, j, lUVSetNameList.GetStringAt(0), fbxUV, unmapped);
	outUV = XMFLOAT2(fbxUV.mData[0], 1 - fbxUV.mData[1]);
}
void processMesh(FbxMesh* m, std::vector<Vertex1P1N1UV> &vertices)
{
	FbxVector4 *fbxVertices = m->GetControlPoints();
	UINT vertexCounter = 0;
	for (int i = 0; i < m->GetPolygonCount(); i++) {
		for (int j = 0; j < 3; ++j) {
			int index = m->GetPolygonVertex(i, j);
			XMFLOAT3 nor;
			getNormal(m, index, vertexCounter, nor);
			XMFLOAT2 tex;
			getUV(m, i, j, tex);
			vertices.push_back(Vertex1P1N1UV(XMFLOAT3(fbxVertices[index].mData[0], fbxVertices[index].mData[1], fbxVertices[index].mData[2]), nor, tex));
			vertexCounter++;
		}
	}
}
