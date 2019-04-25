#include "rendering/DynamicMeshSystem.h"

#include "math/Quat.h"

namespace bge
{

void DynamicMeshSystem::SetEventCallback(
    const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;
}

void DynamicMeshSystem::UpdateTransforms(
    const std::vector<Transform>& transforms)
{
  BGE_CORE_ASSERT(transforms.size() == m_Meshes.size(),
                  "Uneven amount of physical transforms and graphic instances");

  for (size_t i = 0; i < transforms.size(); i++)
  {
    m_Transforms[i] = transforms[i].ToMatrix();
  }
}

void DynamicMeshSystem::RenderMeshes(const Mat4f& projection, const Mat4f& view)
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

void DynamicMeshSystem::AddComponent(Entity entity, const DynamicMeshData& data)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  m_Entities.push_back(entity);
  m_Meshes.push_back(data);
  m_Transforms.emplace_back(1.0f);
  m_EntityToComponentId[entity.GetId()] = m_Meshes.size() - 1;
}

void DynamicMeshSystem::DestroyComponent(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  uint32 componentIndexToRemove = m_EntityToComponentId[entity.GetId()];
  uint32 lastComponentIndex = m_Meshes.size() - 1;
  Entity lastEntity = m_Entities[lastComponentIndex];

  m_Meshes[componentIndexToRemove] = m_Meshes[lastComponentIndex];
  m_Entities[componentIndexToRemove] = m_Entities[lastComponentIndex];
  m_Transforms[componentIndexToRemove] = m_Transforms[lastComponentIndex];

  m_Meshes.pop_back();
  m_Entities.pop_back();
  m_Transforms.pop_back();

  m_EntityToComponentId[lastEntity.GetId()] = componentIndexToRemove;
  m_EntityToComponentId.erase(entity.GetId());
}

DynamicMeshData* DynamicMeshSystem::LookUpComponent(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");
  return &m_Meshes[m_EntityToComponentId[entity.GetId()]];
}

void DynamicMeshSystem::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<EntitiesDestroyedEvent>(
      BGE_BIND_EVENT_FN(DynamicMeshSystem::OnEntitiesDestroyed));
}

bool DynamicMeshSystem::OnEntitiesDestroyed(EntitiesDestroyedEvent& event)
{
  for (auto&& entity : event.GetEntities())
  {
    for (auto&& existingEntity : m_Entities)
    {
      if (entity == existingEntity)
      {
        DestroyComponent(entity);
      }
    }
  }

  return false;
}

} // namespace bge