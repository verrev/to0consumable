#pragma once
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;

enum VERTEX_ELEMENT
{
	POSITION = 1,
	NORMAL = 2,
	TEXCOORD = 4
};
#define MAX_ELEMENT_COUNT 3

struct RenderableHeader
{
	UINT vsInputLayout;
	char vshaderPath[75];
	char pshaderPath[75];
	UINT verticeCount;
	UINT indiceCount;
};

struct Vertex1P
{
	Vertex1P(float x, float y, float z) : pos(XMFLOAT3(x, y, z)) {}
	Vertex1P(XMFLOAT3 p) : pos(p) {}
	XMFLOAT3 pos;
};
struct Vertex1P1UV
{
	Vertex1P1UV(float x, float y, float z, float u, float v)
		: pos(XMFLOAT3(x, y, z)), texcoord(XMFLOAT2(u, v)) {}
	Vertex1P1UV(XMFLOAT3 p, XMFLOAT2 texc)
		: pos(p), texcoord(texc) {}
	Vertex1P1UV()
		: pos(XMFLOAT3(-1, -1, -1)), texcoord(XMFLOAT2(-1, -1)) {}

	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
};
struct Vertex1P1N1UV
{
	Vertex1P1N1UV(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(XMFLOAT3(x, y, z)), normal(XMFLOAT3(nx, ny, nz)), texcoord(XMFLOAT2(u, v)) {}
	Vertex1P1N1UV(XMFLOAT3 p, XMFLOAT3 nor, XMFLOAT2 texc)
		: pos(p), normal(nor), texcoord(texc) {}
	Vertex1P1N1UV()
		: pos(XMFLOAT3(-1, -1, -1)), normal(XMFLOAT3(-1, -1, -1)), texcoord(XMFLOAT2(-1, -1)) {}

	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
};