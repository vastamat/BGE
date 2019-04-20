#pragma once

#include "CameraManager.h"
#include "MeshLibrary.h"
#include "MeshSystem.h"
#include "PhysicalMeshSystem.h"
#include "ShaderLibrary.h"
#include "Texture2DLibrary.h"

namespace bge
{

class RenderWorld
{
public:
  RenderWorld();

  void UpdateTransforms(TransformsContainer transforms);
  void Render(float interpolation);
  void OnExit();

  Mesh LoadMesh(const std::string& filepath);
  ShaderProgramHandle LoadShader(const std::string& filepath);
  Texture2DHandle LoadTexture2D(const std::string& filepath);

  uint32 AddCamera(const Vec4i32& viewport, float fov, float near, float far);
  uint32 AddCamera(const Vec4i32& viewport, float near, float far);

  template <typename T> void DestroyComponent(ComponentHandle handle)
  {
    using SystemType = typename ComponentIdToSystem<T>::Type;
    return GetComponentSystem<SystemType>()->DestroyComponent(handle);
  }

  template <typename T> ComponentHandle AddComponent(const T& data)
  {
    using SystemType = typename ComponentIdToSystem<T>::Type;
    return GetComponentSystem<SystemType>()->AddComponent(data);
  }

  template <typename T> T LookUpComponent(ComponentHandle handle)
  {
    using SystemType = typename ComponentIdToSystem<T>::Type;
    return GetComponentSystem<SystemType>()->LookUpComponent(handle);
  }

  template <typename T> T* GetComponentSystem() { return nullptr; }

private:
  // Systems
  MeshSystem m_MeshSystem;
  PhysicalMeshSystem m_PhysicalMeshSystem;

  // Resource Libraries
  MeshLibrary m_MeshLibrary;
  ShaderLibrary m_ShaderLibrary;
  Texture2DLibrary m_TextureLibrary;

  // Cameras
  CameraManager m_CameraManager;
};

// helper struct that defines the corresponding world type for each component ID
// in an inner Type typedef
template <> struct ComponentIdToWorld<MeshData>
{
  using WorldType = RenderWorld;
  using Type = WorldType;
};

template <> struct ComponentIdToWorld<PhysicalMeshData>
{
  using WorldType = RenderWorld;
  using Type = WorldType;
};

// helper struct that defines the corresponding system for each component ID in
// an inner Type typedef
template <> struct ComponentIdToSystem<MeshData>
{
  using SystemType = MeshSystem;
  using Type = SystemType;
};

template <> struct ComponentIdToSystem<PhysicalMeshData>
{
  using SystemType = PhysicalMeshSystem;
  using Type = SystemType;
};

template <> struct ComponentIdToComponent<ComponentTypeToId<MeshData>::ID>
{
};

} // namespace bge
