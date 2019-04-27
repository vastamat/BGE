#if defined BGE_PLATFORM_UNIX || BGE_PLATFORM_APPLE || BGE_PLATFORM_WINDOWS

#include "logging/Log.h"
#include "rendering/RenderDevice.h"
#include "util/FileIO.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <string>
#include <vector>

uint32 GLCheckError() { return glGetError(); }

bool GLLogCall(const char* function, const char* file, int line)
{
  uint32 error = GLCheckError();
  if (error != GL_NO_ERROR)
  {
    BGE_CORE_ERROR("Code: {0} \n\tFile: {1}\n\tFunction: {2}\n\tLine: {3}\n",
                   std::to_string(error), file, function, std::to_string(line));
    return false;
  }
  return true;
}

#if defined BGE_DEBUG
#define GLCall(expr)                                                           \
  GLCheckError();                                                              \
  expr;                                                                        \
  BGE_CORE_ASSERT(GLLogCall(#expr, __FILE__, __LINE__), "OpenGL Error")
#else
#define GLCall(expr) expr
#endif

namespace bge
{

TextureParameters::TextureParameters()
    : m_Format(TextureFormat::RGBA)
    , m_Filter(TextureFilter::Linear)
    , m_Wrap(TextureWrap::ClampToEdge)
{
}

TextureParameters::TextureParameters(TextureFormat format, TextureFilter filter,
                                     TextureWrap wrap)
    : m_Format(format)
    , m_Filter(filter)
    , m_Wrap(wrap)
{
}

VertexBufferLayout::BufferElement::BufferElement(uint32 type, uint32 size,
                                                 uint32 count, uint32 offset,
                                                 bool normalized)
    : m_Type(type)
    , m_Size(size)
    , m_Count(count)
    , m_Offset(offset)
    , m_Normalized(normalized)
{
}

void VertexBufferLayout::PushFloat(uint32 count, bool normalized)
{
  m_Elements.emplace_back(GL_FLOAT, sizeof(float), count, m_CurrentSizeInBytes,
                          normalized);

  m_CurrentSizeInBytes += sizeof(float) * count;
}
void VertexBufferLayout::PushUint32(uint32 count, bool normalized)
{
  m_Elements.emplace_back(GL_UNSIGNED_INT, sizeof(uint32), count,
                          m_CurrentSizeInBytes, normalized);

  m_CurrentSizeInBytes += sizeof(uint32) * count;
}
void VertexBufferLayout::PushUint8(uint32 count, bool normalized)
{
  m_Elements.emplace_back(GL_UNSIGNED_BYTE, sizeof(uint8), count,
                          m_CurrentSizeInBytes, normalized);

  m_CurrentSizeInBytes += sizeof(uint8) * count;
}

void VertexBufferLayout::Apply() const
{
  BGE_CORE_ASSERT(!m_Elements.empty(), "Can't apply an empty layout.");

  for (std::size_t i = 0; i < m_Elements.size(); i++)
  {
    const BufferElement& element = m_Elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(
        i, element.m_Count, element.m_Type, element.m_Normalized,
        m_CurrentSizeInBytes, reinterpret_cast<const void*>(element.m_Offset)));
  }
}

namespace RenderDevice
{

static GLenum GetGLTextureFilter(TextureFilter filter);
static GLenum GetGLTextureFormat(TextureFormat format);
static GLenum GetGLTextureWrap(TextureWrap wrap);

static GLuint AddShader(GLuint shaderProgram, const char* src, GLenum type);
static bool CheckShaderError(GLuint shader, int flag, bool isProgram,
                             const std::string& errorMessage);

static constexpr uint32 c_MaxBuffersAllocated = 1 << 8;
static constexpr uint32 c_MaxVertexArrayBuffersAllocated = 1 << 8;
static constexpr uint32 c_MaxShaderProgramsAllocated = 1 << 8;
static constexpr uint32 c_MaxTexturesAllocated = 1 << 8;

static GLuint s_Buffers[c_MaxBuffersAllocated];
static GLuint s_VAOBuffers[c_MaxVertexArrayBuffersAllocated];
static GLuint s_ShaderPrograms[c_MaxShaderProgramsAllocated];
static GLuint s_Textures[c_MaxTexturesAllocated];

static std::vector<uint32> s_BufferGenerations;
static std::vector<uint32> s_VAOBufferGenerations;
static std::vector<uint32> s_ShaderProgramGenerations;
static std::vector<uint32> s_TextureGenerations;

static std::vector<uint32> s_FreeBufferIds;
static std::vector<uint32> s_FreeVAOBufferIds;
static std::vector<uint32> s_FreeShaderIds;
static std::vector<uint32> s_FreeTextureIds;

void Initialize()
{
  // Initialize glad
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  BGE_CORE_ASSERT(status, "Failed to initialize Glad!");

  GLCall(auto version = glGetString(GL_VERSION));
  GLCall(auto vendor = glGetString(GL_VENDOR));
  GLCall(auto renderer = glGetString(GL_RENDERER));

  BGE_CORE_INFO(
      "Initialized OpenGL:\n\tVersion: {0}\n\tVendor: {1}\n\tRenderer: {2}",
      version, vendor, renderer);

  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
  GLCall(glEnable(GL_CULL_FACE));
  GLCall(glFrontFace(GL_CCW));
  GLCall(glCullFace(GL_BACK));

  GLCall(glGenBuffers(c_MaxBuffersAllocated, s_Buffers));
  GLCall(glGenVertexArrays(c_MaxVertexArrayBuffersAllocated, s_VAOBuffers));
  GLCall(glGenTextures(c_MaxTexturesAllocated, s_Textures));
}

void Shutdown()
{
  GLCall(glDeleteBuffers(c_MaxBuffersAllocated, s_Buffers));
  GLCall(glDeleteVertexArrays(c_MaxVertexArrayBuffersAllocated, s_VAOBuffers));
  GLCall(glDeleteTextures(c_MaxTexturesAllocated, s_Textures));
}

void SetClearColor(float r, float g, float b, float a)
{
  GLCall(glClearColor(r, g, b, a));
}

void ClearBuffers(bool color, bool depth)
{
  uint32 flags = 0;

  if (color)
  {
    flags |= GL_COLOR_BUFFER_BIT;
  }

  if (depth)
  {
    flags |= GL_DEPTH_BUFFER_BIT;
  }

  GLCall(glClear(flags));
}

void SetDepthTesting(bool enabled)
{
  if (enabled)
  {
    GLCall(glEnable(GL_DEPTH_TEST));
  }
  else
  {
    GLCall(glDisable(GL_DEPTH_TEST));
  }
}

void SetBlend(bool enabled)
{
  if (enabled)
  {
    GLCall(glEnable(GL_BLEND));
  }
  else
  {
    GLCall(glDisable(GL_BLEND));
  }
}

void SetCulling(bool enabled)
{
  if (enabled)
  {
    GLCall(glEnable(GL_CULL_FACE));
  }
  else
  {
    GLCall(glDisable(GL_CULL_FACE));
  }
}

void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
  GLCall(glViewport(x, y, width, height));
}

VertexArrayHandle CreateVertexArray(VertexBufferHandle* vertexBuffers,
                                    VertexBufferLayout* layouts, uint32 count)
{
  uint32 vaoBufferId = 0;

  if (!s_FreeVAOBufferIds.empty())
  {
    vaoBufferId = s_FreeVAOBufferIds.back();
    s_FreeVAOBufferIds.pop_back();
  }
  else
  {
    s_VAOBufferGenerations.emplace_back(0);
    vaoBufferId = s_VAOBufferGenerations.size() - 1;
    BGE_CORE_ASSERT(vaoBufferId < c_MaxVertexArrayBuffersAllocated,
                    "Can't have more than max buffers.");
  }

  GLCall(glBindVertexArray(s_VAOBuffers[vaoBufferId]));

  for (size_t i = 0; i < count; i++)
  {
    VertexBufferHandle& vboHandle = vertexBuffers[i];
    VertexBufferLayout& vboLayout = layouts[i];

    BGE_CORE_ASSERT(vboHandle.m_Generation ==
                        s_BufferGenerations[vboHandle.m_Index],
                    "Trying to use an invalid vertex buffer");

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Buffers[vboHandle.m_Index]));
    vboLayout.Apply();
  }

  return VertexArrayHandle{vaoBufferId, s_VAOBufferGenerations[vaoBufferId]};
}

void DestroyVertexArray(VertexArrayHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_VAOBufferGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  uint32 index = handle.m_Index;
  ++s_VAOBufferGenerations[index];
  s_FreeVAOBufferIds.emplace_back(index);
}

void BindVertexArray(VertexArrayHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_VAOBufferGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  GLCall(glBindVertexArray(s_VAOBuffers[handle.m_Index]));
}

void UnbindVertexArray() { GLCall(glBindVertexArray(0)); }

VertexBufferHandle CreateVertexBuffer(uint32 vertexCount, uint32 stride,
                                      const void* initialData)
{
  uint32 bufferId = 0;

  if (!s_FreeBufferIds.empty())
  {
    bufferId = s_FreeBufferIds.back();
    s_FreeBufferIds.pop_back();
  }
  else
  {
    s_BufferGenerations.emplace_back(0);
    bufferId = s_BufferGenerations.size() - 1;
    BGE_CORE_ASSERT(bufferId < c_MaxBuffersAllocated,
                    "Can't have more than max buffers.");
  }
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Buffers[bufferId]));
  GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * stride, initialData,
                      GL_STATIC_DRAW));

  return VertexBufferHandle{bufferId, s_BufferGenerations[bufferId]};
}

VertexBufferHandle CreateDynamicVertexBuffer(uint32 vertexCount, uint32 stride)
{
  uint32 bufferId = 0;

  if (!s_FreeBufferIds.empty())
  {
    bufferId = s_FreeBufferIds.back();
    s_FreeBufferIds.pop_back();
  }
  else
  {
    s_BufferGenerations.emplace_back(0);
    bufferId = s_BufferGenerations.size() - 1;
    BGE_CORE_ASSERT(bufferId < c_MaxBuffersAllocated,
                    "Can't have more than max buffers.");
  }
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Buffers[bufferId]));
  GLCall(glBufferData(GL_ARRAY_BUFFER, vertexCount * stride, nullptr,
                      GL_DYNAMIC_DRAW));

  return VertexBufferHandle{bufferId, s_BufferGenerations[bufferId]};
}

void DestroyVertexBuffer(VertexBufferHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_BufferGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  uint32 index = handle.m_Index;
  ++s_BufferGenerations[index];
  s_FreeBufferIds.emplace_back(index);
}

IndexBufferHandle CreateIndexBuffer(uint32 indexCount,
                                    const uint32* initialData)
{
  uint32 bufferId = 0;

  if (!s_FreeBufferIds.empty())
  {
    bufferId = s_FreeBufferIds.back();
    s_FreeBufferIds.pop_back();
  }
  else
  {
    s_BufferGenerations.emplace_back(0);
    bufferId = s_BufferGenerations.size() - 1;
    BGE_CORE_ASSERT(bufferId < c_MaxBuffersAllocated,
                    "Can't have more than max buffers.");
  }
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Buffers[bufferId]));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32),
                      initialData, GL_STATIC_DRAW));

  return IndexBufferHandle{bufferId, s_BufferGenerations[bufferId]};
}

void DestroyIndexBuffer(IndexBufferHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_BufferGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  uint32 index = handle.m_Index;
  ++s_BufferGenerations[index];
  s_FreeBufferIds.emplace_back(index);
}

void BindIndexBuffer(IndexBufferHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_BufferGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Buffers[handle.m_Index]));
}

void UnbindIndexBuffer() { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

ShaderProgramHandle CreateShaderProgram(const char* filepath)
{
  std::string fullFilepath = filepath;
  fullFilepath.append(".glsl");

  auto src = LoadTextFileWithIncludes(fullFilepath, "#include");

  std::string vertexShaderSrc = "#version 330\n#define VS_BUILD\n" + src;
  std::string fragmentShaderSrc = "#version 330\n#define FS_BUILD\n" + src;

  return CreateShaderProgram(vertexShaderSrc.c_str(),
                             fragmentShaderSrc.c_str());
}

ShaderProgramHandle CreateShaderProgram(const char* vertexShaderSource,
                                        const char* fragmentShaderSource)
{
  uint32 programId = 0;

  if (!s_FreeShaderIds.empty())
  {
    programId = s_FreeShaderIds.back();
    s_FreeShaderIds.pop_back();
  }
  else
  {
    s_ShaderProgramGenerations.emplace_back(0);
    programId = s_ShaderProgramGenerations.size() - 1;
    BGE_CORE_ASSERT(programId < c_MaxShaderProgramsAllocated,
                    "Can't have more than max shader programs.");
  }

  GLCall(s_ShaderPrograms[programId] = glCreateProgram());

  GLuint vertexShader = AddShader(s_ShaderPrograms[programId],
                                  vertexShaderSource, GL_VERTEX_SHADER);
  BGE_CORE_ASSERT(vertexShader != 0, "Could not add Vertex Shader.");

  GLuint fragmentShader = AddShader(s_ShaderPrograms[programId],
                                    fragmentShaderSource, GL_FRAGMENT_SHADER);
  BGE_CORE_ASSERT(fragmentShader != 0, "Could not add Fragment Shader.");

  GLCall(glLinkProgram(s_ShaderPrograms[programId]));

  bool successfulLinkage =
      !CheckShaderError(s_ShaderPrograms[programId], GL_LINK_STATUS, true,
                        "Error linking shader program.");

  BGE_CORE_ASSERT(successfulLinkage, "Could not link shader program.");

  GLCall(glValidateProgram(s_ShaderPrograms[programId]));

  bool validProgram =
      !CheckShaderError(s_ShaderPrograms[programId], GL_VALIDATE_STATUS, true,
                        "Invalid shader program.");

  BGE_CORE_ASSERT(validProgram, "Could not validate shader program.");

  GLCall(glDetachShader(s_ShaderPrograms[programId], vertexShader));
  GLCall(glDetachShader(s_ShaderPrograms[programId], fragmentShader));
  GLCall(glDeleteShader(vertexShader));
  GLCall(glDeleteShader(fragmentShader));

  return ShaderProgramHandle{programId, s_ShaderProgramGenerations[programId]};
}

void DestroyShaderProgram(ShaderProgramHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation ==
                      s_ShaderProgramGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  uint32 index = handle.m_Index;
  ++s_ShaderProgramGenerations[index];
  s_FreeShaderIds.emplace_back(index);

  GLCall(glDeleteProgram(s_ShaderPrograms[index]));
}

void BindShaderProgram(ShaderProgramHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation ==
                      s_ShaderProgramGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  GLCall(glUseProgram(s_ShaderPrograms[handle.m_Index]));
}

void UnbindShaderProgram() { GLCall(glUseProgram(0)); }

int32 GetUniformLocation(ShaderProgramHandle handle, const std::string& name)
{
  GLCall(GLint result = glGetUniformLocation(s_ShaderPrograms[handle.m_Index],
                                             name.c_str()));

  BGE_CORE_ASSERT(result != -1, "Shader could not find uniform ");

  return result;
}

void SetUniform1f(ShaderProgramHandle handle, const std::string& name,
                  float value)
{
  GLCall(glUniform1f(GetUniformLocation(handle, name), value));
}
void SetUniform1fv(ShaderProgramHandle handle, const std::string& name,
                   float* value, int32 count)
{
  GLCall(glUniform1fv(GetUniformLocation(handle, name), count, value));
}
void SetUniform1i(ShaderProgramHandle handle, const std::string& name,
                  int32 value)
{
  GLCall(glUniform1i(GetUniformLocation(handle, name), value));
}
void SetUniform1iv(ShaderProgramHandle handle, const std::string& name,
                   int32* value, int32 count)
{
  GLCall(glUniform1iv(GetUniformLocation(handle, name), count, value));
}
void SetUniform2f(ShaderProgramHandle handle, const std::string& name,
                  const Vec2f& vector)
{
  GLCall(glUniform2f(GetUniformLocation(handle, name), vector[0], vector[1]));
}
void SetUniform3f(ShaderProgramHandle handle, const std::string& name,
                  const Vec3f& vector)
{
  GLCall(glUniform3f(GetUniformLocation(handle, name), vector[0], vector[1],
                     vector[2]));
}
void SetUniform4f(ShaderProgramHandle handle, const std::string& name,
                  const Vec4f& vector)
{
  GLCall(glUniform4f(GetUniformLocation(handle, name), vector[0], vector[1],
                     vector[2], vector[3]));
}
void SetUniformMat4(ShaderProgramHandle handle, const std::string& name,
                    Mat4f matrix, bool transpose)
{
  // Transpose flag is set to GL_TRUE, because the matrix class is row-major
  GLCall(glUniformMatrix4fv(GetUniformLocation(handle, name), 1, transpose,
                            &matrix[0]));
}

Texture2DHandle CreateTexture2D(uint32 width, uint32 height, uint8* data,
                                TextureParameters parameters)
{
  uint32 textureID = 0;

  if (!s_FreeTextureIds.empty())
  {
    textureID = s_FreeTextureIds.back();
    s_FreeTextureIds.pop_back();
  }
  else
  {
    s_TextureGenerations.emplace_back(0);
    textureID = s_TextureGenerations.size() - 1;
    BGE_CORE_ASSERT(textureID < c_MaxBuffersAllocated,
                    "Can't have more than max textures.");
  }

  GLCall(glBindTexture(GL_TEXTURE_2D, s_Textures[textureID]));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                         parameters.m_Filter == TextureFilter::Linear
                             ? GL_LINEAR_MIPMAP_LINEAR
                             : GL_NEAREST));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                         GetGLTextureFilter(parameters.m_Filter)));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                         GetGLTextureWrap(parameters.m_Wrap)));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                         GetGLTextureWrap(parameters.m_Wrap)));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GetGLTextureFormat(parameters.m_Format),
                      width, height, 0, GetGLTextureFormat(parameters.m_Format),
                      GL_UNSIGNED_BYTE, data));

  GLCall(glGenerateMipmap(GL_TEXTURE_2D));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  return Texture2DHandle{textureID, s_TextureGenerations[textureID]};
}

void DestroyTexture2D(Texture2DHandle handle)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_TextureGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  uint32 index = handle.m_Index;
  ++s_TextureGenerations[index];
  s_FreeTextureIds.emplace_back(index);
}

void BindTexture2D(Texture2DHandle handle, uint32 slot)
{
  BGE_CORE_ASSERT(handle.m_Generation == s_TextureGenerations[handle.m_Index],
                  "Trying to destroy an invalid handle");

  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, s_Textures[handle.m_Index]));
}
void UnbindTexture2D(uint32 slot)
{
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Draw(VertexArrayHandle vao, IndexBufferHandle ibo, uint32 indicesCount)
{
  BindVertexArray(vao);
  BindIndexBuffer(ibo);
  GLCall(glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr));
}

void DrawWireframeLines(uint32 indicesCount)
{
  GLCall(glDrawElements(GL_LINE_STRIP, indicesCount, GL_UNSIGNED_INT, nullptr));
}

static GLuint AddShader(GLuint shaderProgram, const char* src, GLenum type)
{
  GLCall(GLuint shader = glCreateShader(type));

  if (shader == 0)
  {
    BGE_CORE_ERROR("Error creating shader type {0}", type);
    return 0;
  }

  GLCall(glShaderSource(shader, 1, &src, nullptr));
  GLCall(glCompileShader(shader));

  if (CheckShaderError(shader, GL_COMPILE_STATUS, false,
                       "Error compiling shader"))
  {
    return 0;
  }

  GLCall(glAttachShader(shaderProgram, shader));
  return shader;
}

static bool CheckShaderError(GLuint shader, int flag, bool isProgram,
                             const std::string& errorMessage)
{
  GLint success = 0;
  GLchar error[1024] = {0};

  if (isProgram)
  {
    GLCall(glGetProgramiv(shader, flag, &success));
  }
  else
  {
    GLCall(glGetShaderiv(shader, flag, &success));
  }

  if (!success)
  {
    if (isProgram)
    {
      GLCall(glGetProgramInfoLog(shader, sizeof(error), NULL, error));
    }
    else
    {
      GLCall(glGetShaderInfoLog(shader, sizeof(error), NULL, error));
    }

    BGE_CORE_ERROR("{0}: {1}", errorMessage.c_str(), error);
    return true;
  }
  return false;
}

static GLenum GetGLTextureWrap(TextureWrap wrap)
{
  switch (wrap)
  {
    case TextureWrap::Repeat:
      return GL_REPEAT;
    case TextureWrap::MirroredRepeat:
      return GL_MIRRORED_REPEAT;
    case TextureWrap::ClampToEdge:
      return GL_CLAMP_TO_EDGE;
    case TextureWrap::ClampToBorder:
      return GL_CLAMP_TO_BORDER;
  }

  return 0;
}

static GLenum GetGLTextureFormat(TextureFormat format)
{
  switch (format)
  {
    case TextureFormat::RGBA:
      return GL_RGBA;
    case TextureFormat::RGB:
      return GL_RGB;
    case TextureFormat::Depth:
      return GL_DEPTH_COMPONENT;
    case TextureFormat::DepthStencil:
      return GL_DEPTH_STENCIL;
  }

  return 0;
}

static GLenum GetGLTextureFilter(TextureFilter filter)
{
  switch (filter)
  {
    case TextureFilter::Linear:
      return GL_LINEAR;
    case TextureFilter::Nearest:
      return GL_NEAREST;
  }

  return 0;
}

} // namespace RenderDevice
} // namespace bge

#endif