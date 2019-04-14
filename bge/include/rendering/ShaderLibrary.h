#pragma once

#include "RenderDevice.h"

#include "core/Common.h"

#include <unordered_map>

namespace bge
{

class ShaderLibrary
{
public:
  ShaderProgramHandle GetShader(const std::string& filepath);

  void ClearLibrary();

private:
  std::unordered_map<std::string, ShaderProgramHandle> m_ShaderMap;
};

} // namespace bge