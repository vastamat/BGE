#include "rendering/ShaderLibrary.h"

namespace bge
{

ShaderProgramHandle ShaderLibrary::GetShader(const std::string& filepath)
{
  auto foundIt = m_ShaderMap.find(filepath);
  if (foundIt != m_ShaderMap.end())
  {
    return foundIt->second;
  }

  ShaderProgramHandle result =
      RenderDevice::CreateShaderProgram(filepath.c_str());

  m_ShaderMap.insert(std::make_pair(filepath, result));
  return result;
}

void ShaderLibrary::ClearLibrary()
{
  RenderDevice::UnbindShaderProgram();

  for (auto&& shader : m_ShaderMap)
  {
    RenderDevice::DestroyShaderProgram(shader.second);
  }

  m_ShaderMap.clear();
}

} // namespace bge
