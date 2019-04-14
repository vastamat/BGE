#pragma once

#include <string>
#include <vector>

namespace bge
{

std::vector<std::string> SplitString(const std::string& str, char delim);

std::string GetFilePath(const std::string& fileName);

std::string LoadTextFile(const std::string& fileName);

std::string LoadTextFileWithIncludes(const std::string& fileName,
                                     const std::string& includeKeyword);

} // namespace bge