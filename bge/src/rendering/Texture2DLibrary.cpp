#include "rendering/Texture2DLibrary.h"

#include "logging/Log.h"

#include <stb/stb_image.h>

namespace bge
{

Texture2DHandle Texture2DLibrary::GetTexture(const std::string& filepath,
                                             bool invertY,
                                             TextureParameters parameters)
{
  auto foundIt = m_TextureMap.find(filepath);
  if (foundIt != m_TextureMap.end())
  {
    return foundIt->second;
  }

  int32 width = 0;
  int32 height = 0;
  int32 channels = 0;

  int32 desiredChannels = 4;
  if (parameters.m_Format == TextureFormat::RGB)
  {
    desiredChannels = 3;
  }

  stbi_set_flip_vertically_on_load(invertY);
  unsigned char* data =
      stbi_load(filepath.c_str(), &width, &height, &channels, desiredChannels);

  BGE_CORE_ASSERT(data != nullptr, "Unable to load texture data");

  Texture2DHandle result =
      RenderDevice::CreateTexture2D(width, height, data, parameters);

  free(data);

  m_TextureMap.insert(std::make_pair(filepath, result));

  return result;
}

void Texture2DLibrary::ClearLibrary()
{
  for (auto&& instance : m_TextureMap)
  {
    RenderDevice::DestroyTexture2D(instance.second);
  }

  m_TextureMap.clear();
}

} // namespace bge
