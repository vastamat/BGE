#pragma once

#include "core/Common.h"
#include "math/Mat.h"

#include <vector>

namespace bge
{

using VertexBufferHandle = GenericHandle<8, 24>;
using IndexBufferHandle = GenericHandle<8, 24>;
using VertexArrayHandle = GenericHandle<8, 24>;
using ShaderProgramHandle = GenericHandle<8, 24>;
using Texture2DHandle = GenericHandle<8, 24>;

enum class TextureWrap
{
  Repeat,
  MirroredRepeat,
  ClampToEdge,
  ClampToBorder
};

enum class TextureFilter
{
  Linear,
  Nearest
};

enum class TextureFormat
{
  RGB,
  RGBA,
  Depth,
  DepthStencil
};

struct TextureParameters
{
  TextureParameters();
  TextureParameters(TextureFormat format, TextureFilter filter,
                    TextureWrap wrap);

  TextureFormat m_Format;
  TextureFilter m_Filter;
  TextureWrap m_Wrap;
};

class VertexBufferLayout
{
public:
  void PushFloat(uint32 count, bool normalized);
  void PushUint32(uint32 count, bool normalized);
  void PushUint8(uint32 count, bool normalized);

  void Apply() const;

private:
  struct BufferElement
  {
    BufferElement(uint32 type, uint32 size, uint32 count, uint32 offset,
                  bool normalized);

    uint32 m_Type;
    uint32 m_Size;
    uint32 m_Count;
    uint32 m_Offset;
    bool m_Normalized;
  };
  std::vector<BufferElement> m_Elements;
  uint32 m_CurrentSizeInBytes = 0;
};

namespace RenderDevice
{

void Initialize();
void Shutdown();

void SetClearColor(float r, float g, float b, float a);
void ClearBuffers(bool color, bool depth);

void SetDepthTesting(bool enabled);
void SetBlend(bool enabled);
void SetCulling(bool enabled);
void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);

VertexArrayHandle CreateVertexArray(VertexBufferHandle* vertexBuffers,
                                    VertexBufferLayout* layouts, uint32 count);
void DestroyVertexArray(VertexArrayHandle handle);
void BindVertexArray(VertexArrayHandle handle);
void UnbindVertexArray();

VertexBufferHandle CreateVertexBuffer(uint32 vertexCount, uint32 stride,
                                      const void* initialData);
VertexBufferHandle CreateDynamicVertexBuffer(uint32 vertexCount, uint32 stride);
void DestroyVertexBuffer(VertexBufferHandle handle);

IndexBufferHandle CreateIndexBuffer(uint32 indexCount,
                                    const uint32* initialData);
void DestroyIndexBuffer(IndexBufferHandle handle);
void BindIndexBuffer(IndexBufferHandle handle);
void UnbindIndexBuffer();

ShaderProgramHandle CreateShaderProgram(const char* filepath);
ShaderProgramHandle CreateShaderProgram(const char* vertexShaderSource,
                                        const char* fragmentShaderSource);

void DestroyShaderProgram(ShaderProgramHandle handle);
void BindShaderProgram(ShaderProgramHandle handle);
void UnbindShaderProgram();

void SetUniform1f(ShaderProgramHandle handle, const std::string& name,
                  float value);
void SetUniform1fv(ShaderProgramHandle handle, const std::string& name,
                   float* value, int32 count);
void SetUniform1i(ShaderProgramHandle handle, const std::string& name,
                  int32 value);
void SetUniform1iv(ShaderProgramHandle handle, const std::string& name,
                   int32* value, int32 count);
void SetUniform2f(ShaderProgramHandle handle, const std::string& name,
                  const Vec2f& vector);
void SetUniform3f(ShaderProgramHandle handle, const std::string& name,
                  const Vec3f& vector);
void SetUniform4f(ShaderProgramHandle handle, const std::string& name,
                  const Vec4f& vector);
void SetUniformMat4(ShaderProgramHandle handle, const std::string& name,
                    Mat4f matrix, bool transpose = true);

Texture2DHandle CreateTexture2D(uint32 width, uint32 height, uint8* data,
                                TextureParameters parameters);

void DestroyTexture2D(Texture2DHandle handle);
void BindTexture2D(Texture2DHandle handle, uint32 slot);
void UnbindTexture2D(uint32 slot);

void Draw(VertexArrayHandle vao, IndexBufferHandle ibo, uint32 indicesCount);
void DrawWireframeLines(uint32 indicesCount);

} // namespace RenderDevice
} // namespace bge