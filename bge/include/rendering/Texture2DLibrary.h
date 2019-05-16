#pragma once

#include "RenderDevice.h"

#include "core/Common.h"

#include <unordered_map>

namespace bge
{

/**
 * Manages the loading and unloading of textures
 */
class Texture2DLibrary
{
public:
  /**
   * Loads a texture from file or returns an existing instance if already loaded
   * once
   * @param filepath the filepath to the texture
   * @param invertY flag to set the inversion of Y axis
   * @param parameters texture setup parameters
   */
  Texture2DHandle
  GetTexture(const std::string& filepath, bool invertY = true,
             TextureParameters parameters = TextureParameters());

  /**
   * Clears the resource map
   */
  void ClearLibrary();

private:
  // The resource map which maps filepaths to textures
  std::unordered_map<std::string, Texture2DHandle> m_TextureMap;
};

} // namespace bge