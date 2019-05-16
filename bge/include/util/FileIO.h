#pragma once

#include <string>
#include <vector>

namespace bge
{

/**
 * Splits a string into a vector of sub-strings split by passed delim
 * @param str the string to split
 * @param delim the character to split the string by
 * @return array of strings split
 */
std::vector<std::string> SplitString(const std::string& str, char delim);

/**
 * Returns the filepath of a file
 * @param fileName the name of the file
 * @return the filepath
 */
std::string GetFilePath(const std::string& fileName);

/**
 * Loads text from a file into a string
 * @param fileName the name of the file loaded
 * @return the string with the file contents
 */
std::string LoadTextFile(const std::string& fileName);

/**
 * Loads text from a file into a string and can include other files
 * if the contents of the file match an include keyword
 * @param fileName the name of the file loaded
 * @param includeKeyword the keyword to look for when processing the file
 * @return the string with the file contents
 */
std::string LoadTextFileWithIncludes(const std::string& fileName,
                                     const std::string& includeKeyword);

} // namespace bge