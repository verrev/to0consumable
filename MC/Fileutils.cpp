#include "Fileutils.h"

bool Fileutils::readFile(const std::string &filePath, std::ifstream &file, const bool &binary)
{
	int flags = 0;
	if (binary) flags |= std::ios::binary;
	file.open(filePath, flags);
	if (file.good() && file.is_open()) {
		return 1;
	}
	else {
		//MessageBox(0, filePath.c_str(), L"Error loading file", MB_OK);
		return 0;
	}
}

bool Fileutils::writeFile(const std::string &filePath,const void *const data, const UINT &len, const std::ios_base::openmode &mode, const bool &binary)
{
	int flags = mode;
	if (binary) flags |= std::ios::binary;
	std::ofstream outFile(filePath, flags);
	if (outFile.is_open() && outFile.good()) {
		outFile.write((char*)data, len);
		outFile.flush();
		outFile.close();
		return 1;
	}
	return 0;
}