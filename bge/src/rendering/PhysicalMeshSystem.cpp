#include "rendering/PhysicalMeshSystem.h"

#include "math/Quat.h"

namespace bge
{

void PhysicalMeshSystem::UpdateTransforms(TransformsContainer transforms)
{
  BGE_CORE_ASSERT(transforms.m_Count == m_Meshes.size(),
                  "Uneven amount of physical transforms and graphic instances");

  m_Transforms.resize(transforms.m_Count);
  for (size_t i = 0; i < transforms.m_Count; i++)
  {
    m_Transforms[i] =
        GenTranslationMat(Vec3f(transforms.m_Transforms[i].m_Position)) *
        Quatf(transforms.m_Transforms[i].m_Rotation).ToMat4() *
        GenScalingMat(Vec3f(1.0f));
  }
}

void PhysicalMeshSystem::RenderMeshes(const Mat4f& projection,
                                      const Mat4f& view)
{
  for (size_t i = 0; i < m_Meshes.size(); ++i)
  {
    RenderDevice::BindShaderProgram(m_Meshes[i].m_Material.m_Shader);

    RenderDevice::SetUniformMat4(m_Meshes[i].m_Material.m_Shader,
                                 "in_Projection", projection);
    RenderDevice::SetUniformMat4(m_Meshes[i].m_Material.m_Shader, "in_View",
                                 view);
    RenderDevice::SetUniformMat4(m_Meshes[i].m_Material.m_Shader, "in_Model",
                                 m_Transforms[i]);

    for (size_t i = 0; i < m_Meshes[i].m_Material.m_Textures.size(); i++)
    {
      RenderDevice::BindTexture2D(m_Meshes[i].m_Material.m_Textures[i], i);
    }

    RenderDevice::Draw(m_Meshes[i].m_Mesh.m_VertexArray,
                       m_Meshes[i].m_Mesh.m_IndexBuffer,
                       m_Meshes[i].m_Mesh.m_IndicesCount);

    for (int i = m_Meshes[i].m_Material.m_Textures.size() - 1; i >= 0; i--)
    {
      RenderDevice::UnbindTexture2D(i);
    }
  }
}

template <>
ComponentHandle
PhysicalMeshSystem::AddComponent<PhysicalMeshData>(const PhysicalMeshData& data)
{
  ComponentHandle handle;
  if (m_FreeList.empty())
  {
    // if there are no free slots, just add a new instance of everything
    m_Meshes.push_back(data);                          // new mesh
    m_ComponentIdToIndex.emplace_back(m_IndexCounter); // new mapping index
    m_ComponentVersion.emplace_back(0u);               // new generation
    handle.m_Index = m_IndexCounter++; // set and increment the index counter
    handle.m_Generation = 0u;          // set the generation
  }
  else
  {
    // When there is a free slot, the index and generation mapping can be
    // reused
    uint32 freeSlot = m_FreeList.back();
    m_FreeList.pop_back();    // remove the slot that's about to be occupied
    m_Meshes.push_back(data); // new comp
    // set the mapping of the free slot to point to the new data
    m_ComponentIdToIndex[freeSlot] = m_Meshes.size() - 1;
    handle.m_Index = freeSlot; // set the index to the free slot
    handle.m_Generation = m_ComponentVersion[freeSlot]; // set the generation
  }
  return handle;
}

template <>
void PhysicalMeshSystem::DestroyComponent<PhysicalMeshData>(
    ComponentHandle handle)
{
  // assert this operation is valid
  BGE_CORE_ASSERT(m_ComponentVersion[handle.m_Index] == handle.m_Generation,
                  "Trying to destroy an already destroyed component");

  // Get the index to the component in the internal array
  uint32 compIndex = m_ComponentIdToIndex[handle.m_Index];
  uint32 lastIndex = m_Meshes.size() - 1;
  // swap the component to be removed with the last added component and pop
  std::swap(m_Meshes[compIndex], m_Meshes[lastIndex]);
  m_Meshes.pop_back();

  // Update the mapping so the id which pointed to the last component now
  // points to the initial index where the component was swapped to
  for (size_t i = 0; i < m_ComponentIdToIndex.size(); i++)
  {
    // Find which mapping instance "points" to the index of the last component
    // in the array
    if (m_ComponentIdToIndex[i] == lastIndex)
    {
      // set that instance to point to the removed component index where the
      // data was swapped to
      m_ComponentIdToIndex[i] = compIndex;
      break;
    }
  }

  // Set the removed component mapping to max uint so that it doesn't point to
  // any valid data instances which could affect the loop above.
  m_ComponentIdToIndex[handle.m_Index] = UINT_MAX;
  // Add the mapping which has been removed to the free list
  m_FreeList.push_back(handle.m_Index);

  // Update the generation to invalidate the mapping to the removed component
  ++m_ComponentVersion[handle.m_Index];
}

template <>
PhysicalMeshData
PhysicalMeshSystem::LookUpComponent<PhysicalMeshData>(ComponentHandle handle)
{
  BGE_CORE_ASSERT(m_ComponentVersion[handle.m_Index] == handle.m_Generation,
                  "Trying to lookup a component handle that's been deleted.");
  return m_Meshes[m_ComponentIdToIndex[handle.m_Index]];
}

} // namespace bge