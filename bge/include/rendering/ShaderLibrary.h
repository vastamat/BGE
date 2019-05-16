#pragma once

#include "RenderDevice.h"

#include "core/Common.h"

#include <unordered_map>

namespace bge
{

/**
 * Manages the loading and unloading of shaders
 */
class ShaderLibrary
{
public:
  /**
   * Loads a shader from file or returns an existing instance if already loaded
   * once
   * @param filepath the path to the file including the file name, but excluding
   * the extension as that is added by the API-specific implementation
   * @return the handle to the shader loaded
   */
  ShaderProgramHandle GetShader(const std::string& filepath);

  /**
   * Clears the resource map
   */
  void ClearLibrary();

private:
  // The resource map which maps filepaths to shaders
  std::unordered_map<std::string, ShaderProgramHandle> m_ShaderMap;
};

} // namespace bge