#pragma once

#include "RenderDevice.h"

#include "core/Common.h"

#include <unordered_map>

namespace bge
{

class Texture2DLibrary
{
public:
  Texture2DHandle
  GetTexture(const std::string& filepath, bool invertY = true,
             TextureParameters parameters = TextureParameters());

  void ClearLibrary();

private:
  std::unordered_map<std::string, Texture2DHandle> m_TextureMap;
};

} // namespace bge