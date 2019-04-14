#pragma once

#include "RenderDevice.h"

#include <vector>

namespace bge
{

struct Material
{
  std::vector<Texture2DHandle> m_Textures;
  ShaderProgramHandle m_Shader;
};

} // namespace bge
