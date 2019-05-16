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

/**
 * Supported texture wrap types
 */
enum class TextureWrap
{
  Repeat,
  MirroredRepeat,
  ClampToEdge,
  ClampToBorder
};

/**
 * Supported texture filter types
 */
enum class TextureFilter
{
  Linear,
  Nearest
};

/**
 * Supported texture format types
 */
enum class TextureFormat
{
  RGB,
  RGBA,
  Depth,
  DepthStencil
};

/**
 * Supported texture parameter types
 */
struct TextureParameters
{
  TextureParameters();
  TextureParameters(TextureFormat format, TextureFilter filter,
                    TextureWrap wrap);

  TextureFormat m_Format;
  TextureFilter m_Filter;
  TextureWrap m_Wrap;
};

/**
 * Represents the layout of a vertex buffer
 */
class VertexBufferLayout
{
public:
  /**
   * pushes a float to the layout stack
   * @param count number of floats
   * @param normalized flag whether the value is normalized
   */
  void PushFloat(uint32 count, bool normalized);

  /**
   * pushes a uint32 to the layout stack
   * @param count number of uint32s
   * @param normalized flag whether the value is normalized
   */
  void PushUint32(uint32 count, bool normalized);

  /**
   * pushes a uint8 to the layout stack
   * @param count number of uint8
   * @param normalized flag whether the value is normalized
   */
  void PushUint8(uint32 count, bool normalized);

  /**
   * Applies the layout to the currently bound VBO
   */
  void Apply() const;

private:
  struct BufferElement
  {
    BufferElement(uint32 type, uint32 size, uint32 count, uint32 offset,
                  bool normalized);

    uint32 m_Type;     /**< The type of the layout element */
    uint32 m_Size;     /**< The size of the layout element */
    uint32 m_Count;    /**< The count of the layout element */
    uint32 m_Offset;   /**< The offset of the layout element */
    bool m_Normalized; /**< The normalize flag of the element */
  };
  std::vector<BufferElement> m_Elements;
  uint32 m_CurrentSizeInBytes = 0;
};

namespace RenderDevice
{

/**
 * Initializes the render device
 */
void Initialize();

/**
 * Shuts down the render device
 */
void Shutdown();

/**
 * Sets the clear color (0.0 - 1.0 range)
 * @param r red value
 * @param g green value
 * @param b blue value
 * @param a alpha value
 */
void SetClearColor(float r, float g, float b, float a);

/**
 * Clears the currently bound framebuffer
 * @param color flag to clear the color buffer
 * @param depth flag to clear the depth buffer
 */
void ClearBuffers(bool color, bool depth);

/**
 * Sets the depth testing state
 * @param enabled flag whether to enable it or not
 */
void SetDepthTesting(bool enabled);

/**
 * Sets the blending state
 * @param enabled flag whether to enable it or not
 */
void SetBlend(bool enabled);

/**
 * Sets the culling state
 * @param enabled flag whether to enable it or not
 */
void SetCulling(bool enabled);

/**
 * Sets the viewport for the currently bound framebuffer
 * @param x sets the x-axis origin
 * @param y sets the y-axis origin
 * @param width sets the width of the viewport
 * @param height sets the height of the viewport
 */
void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);

/**
 * Create a vertex array
 * @param vertexBuffers array of vertex buffers to bind to the VAO
 * @param layouts array of vertex buffer layouts for each vertex buffer
 * @param count number of vertex buffers & layouts
 * @return a handle to the vertex array
 */
VertexArrayHandle CreateVertexArray(VertexBufferHandle* vertexBuffers,
                                    VertexBufferLayout* layouts, uint32 count);

/**
 * Destroy a vertex array
 * @param handle reference to the vertex array
 */
void DestroyVertexArray(VertexArrayHandle handle);

/**
 * Bind a vertex array
 * @param handle reference to the vertex array
 */
void BindVertexArray(VertexArrayHandle handle);

/**
 * Unbind the vertex array
 */
void UnbindVertexArray();

/**
 * Create a vertex buffer
 * @param vertexCount number of vertices
 * @param stride the stride of the vertices
 * @param initialData the vertex data
 * @return handle to the vertex buffer
 */
VertexBufferHandle CreateVertexBuffer(uint32 vertexCount, uint32 stride,
                                      const void* initialData);

/**
 * Create a dynamic vertex buffer (can upload its data at a later point)
 * @param vertexCount number of vertices
 * @param stride the stride of the vertices
 * @return handle to the vertex buffer
 */
VertexBufferHandle CreateDynamicVertexBuffer(uint32 vertexCount, uint32 stride);

/**
 * Destroy a vertex buffer
 * @param handle reference to the vertex buffer
 */
void DestroyVertexBuffer(VertexBufferHandle handle);

/**
 * Create an index buffer
 * @param indexCount number of indices
 * @param initialData indices data ptr
 * @return a handle to the index buffer
 */
IndexBufferHandle CreateIndexBuffer(uint32 indexCount,
                                    const uint32* initialData);

/**
 * Destroy an index buffer
 * @param handle reference to the index buffer
 */
void DestroyIndexBuffer(IndexBufferHandle handle);

/**
 * Bind an index buffer
 * @param handle reference to the index buffer
 */
void BindIndexBuffer(IndexBufferHandle handle);

/**
 * Unbind the index buffer
 */
void UnbindIndexBuffer();

/**
 * Creates a shader program from a shader file
 * @param filepath the path to the file including the file name, but excluding
 * the extension as that is added by the API-specific implementation
 * @return handle to the shader program
 */
ShaderProgramHandle CreateShaderProgram(const char* filepath);

/**
 * Creates a shader program from a vertex and fragment shader source code
 * @param vertexShaderSource the source code for the vertex shader
 * @param fragmentShaderSource the source code for the fragment shader
 * @return handle to the shader program
 */
ShaderProgramHandle CreateShaderProgram(const char* vertexShaderSource,
                                        const char* fragmentShaderSource);

/**
 * Destroy an existing shader program
 * @param handle the handle to the shader program
 */
void DestroyShaderProgram(ShaderProgramHandle handle);

/**
 * Bind an existing shader program
 * @param handle the handle to the shader program
 */
void BindShaderProgram(ShaderProgramHandle handle);

/**
 * Unbind an the currently bound shader program
 */
void UnbindShaderProgram();

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param value the value to upload to the shader
 */
void SetUniform1f(ShaderProgramHandle handle, const std::string& name,
                  float value);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param value the array of values to upload to the shader
 * @param count the number of elements of the value array
 */
void SetUniform1fv(ShaderProgramHandle handle, const std::string& name,
                   float* value, int32 count);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param value the value to upload to the shader
 */
void SetUniform1i(ShaderProgramHandle handle, const std::string& name,
                  int32 value);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param value the array of values to upload to the shader
 * @param count the number of elements of the value array
 */
void SetUniform1iv(ShaderProgramHandle handle, const std::string& name,
                   int32* value, int32 count);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param vector the 2D vec to upload to the shader
 */
void SetUniform2f(ShaderProgramHandle handle, const std::string& name,
                  const Vec2f& vector);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param vector the 3D vec to upload to the shader
 */
void SetUniform3f(ShaderProgramHandle handle, const std::string& name,
                  const Vec3f& vector);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param vector the 4D vec to upload to the shader
 */
void SetUniform4f(ShaderProgramHandle handle, const std::string& name,
                  const Vec4f& vector);

/**
 * Upload uniform information to a shader
 * @param handle the handle to the shader which will receive the data
 * @param name the name of the uniform (must match the uniform in the shader)
 * @param matrix the 4x4 matrix to upload to the shader
 * @param transpose flag whether to transpose the matrix
 */
void SetUniformMat4(ShaderProgramHandle handle, const std::string& name,
                    Mat4f matrix, bool transpose = true);

/**
 * Create a 2D texture
 * @param width the width of the texture
 * @param height the height of the texture
 * @param data the array of pixel data
 * @param parameters texture setup parameters
 * @return the handle to the texture
 */
Texture2DHandle CreateTexture2D(uint32 width, uint32 height, uint8* data,
                                TextureParameters parameters);

/**
 * Destroy an existing 2D texture
 * @param handle the handle to the texture to destroy
 */
void DestroyTexture2D(Texture2DHandle handle);

/**
 * Bind an existing 2D texture
 * @param handle the handle to the texture to bind
 * @param slot the id of the texture in the shader
 */
void BindTexture2D(Texture2DHandle handle, uint32 slot);

/**
 * Unbind the currently bound texture at an index
 * @param slot the id of the texture in the shader
 */
void UnbindTexture2D(uint32 slot);

/**
 * Indexed draw call to the currently bound framebuffer
 * @param vao the vertex array to use for the draw call
 * @param ibo the index buffer to use for the draw call
 * @param indicesCount the number of indices the buffer contains
 */
void Draw(VertexArrayHandle vao, IndexBufferHandle ibo, uint32 indicesCount);

/**
 * Draws in wireframe mode the currently bound VAO to the framebuffer
 * @param indicesCount the number of indices of the bound IBO
 */
void DrawWireframeLines(uint32 indicesCount);

} // namespace RenderDevice
} // namespace bge