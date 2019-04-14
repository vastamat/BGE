#include "util/FileIO.h"

#include "core/Common.h"
#include "logging/Log.h"

#include <fstream>
#include <sstream>

namespace bge
{

std::vector<std::string> SplitString(const std::string& str, char delim)
{
  std::vector<std::string> subStrings;

  const char* cstr = str.c_str();
  uint32 strLength = (uint32)str.length();
  uint32 start = 0;
  uint32 end = 0;

  while (end <= strLength)
  {
    while (end <= strLength)
    {
      if (cstr[end] == delim)
      {
        break;
      }
      end++;
    }

    subStrings.push_back(str.substr(start, end - start));
    start = end + 1;
    end = start;
  }

  return subStrings;
}

std::string GetFilePath(const std::string& fileName)
{
  const char* cstr = fileName.c_str();
  uint32 strLength = (uint32)fileName.length();
  uint32 end = strLength - 1;

  while (end != 0)
  {
    if (cstr[end] == '/')
    {
      break;
    }
    end--;
  }

  if (end == 0)
  {
    return fileName;
  }
  else
  {
    uint32 start = 0;
    end = end + 1;
    return fileName.substr(start, end - start);
  }
}

std::string LoadTextFile(const std::string& fileName)
{
  std::ifstream file;
  file.open(fileName.c_str());

  std::ostringstream ss("");
  std::string line;

  BGE_CORE_ASSERT(file.is_open(), "Unable to open file: {0}", fileName.c_str());

  while (file.good())
  {
    getline(file, line);

    ss << line << "\n";
  }

  return ss.str();
}

std::string LoadTextFileWithIncludes(const std::string& fileName,
                                     const std::string& includeKeyword)
{
  std::ifstream file;
  file.open(fileName.c_str());

  std::string filePath = GetFilePath(fileName);
  std::stringstream ss;
  std::string line;

  BGE_CORE_ASSERT(file.is_open(), "Unable to open file: {0}", fileName.c_str());

  while (file.good())
  {
    getline(file, line);

    if (line.find(includeKeyword) == std::string::npos)
    {
      ss << line << "\n";
    }
    else
    {
      // get the filename after the keyword
      // eg. " #include "common.h" " will return "common.h"
      std::string includeFileName = SplitString(line, ' ')[1];
      // extract the name from the quotation marks by ommiting 1st and last char
      includeFileName = includeFileName.substr(1, includeFileName.length() - 2);

      std::string toAppend =
          LoadTextFileWithIncludes(filePath + includeFileName, includeKeyword);

      ss << toAppend << "\n";
    }
  }

  return ss.str();
}

} // namespace bge