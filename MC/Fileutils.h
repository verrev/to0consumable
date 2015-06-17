#pragma once
#include <windows.h>
#include <string>
#include <fstream>

class Fileutils
{
public:
	static bool readFile(const std::string &filePath, std::ifstream &file,const bool &binary = 1);
	static bool writeFile(const std::string &filePath, const void *const data, const UINT &len, const std::ios_base::openmode &mode = 0, const bool &binary = 1);
private:
	Fileutils() {}
};