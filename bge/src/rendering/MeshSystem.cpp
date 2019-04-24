#include "rendering/MeshSystem.h"

#include "ecs/ComponentTraits.h"
#include "events/ECSEvents.h"

namespace bge
{

MeshSystem::MeshSystem()
    : m_EntityToComponentId()
    , m_Meshes()
    , m_Entities()
    , m_EventCallback()
{
}

void MeshSystem::SetEventCallback(const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;
}

void MeshSystem::RenderMeshes(const Mat4f& projection, const Mat4f& view)
{
  for (const auto& instance : m_Meshes)
  {
    RenderDevice::BindShaderProgram(instance.m_Material.m_Shader);

    RenderDevice::SetUniformMat4(instance.m_Material.m_Shader, "in_Projection",
                                 projection);
    RenderDevice::SetUniformMat4(instance.m_Material.m_Shader, "in_View", view);
    RenderDevice::SetUniformMat4(instance.m_Material.m_Shader, "in_Model",
                                 instance.m_Transform);

    for (size_t i = 0; i < instance.m_Material.m_Textures.size(); i++)
    {
      RenderDevice::BindTexture2D(instance.m_Material.m_Textures[i], i);
    }

    RenderDevice::Draw(instance.m_Mesh.m_VertexArray,
                       instance.m_Mesh.m_IndexBuffer,
                       instance.m_Mesh.m_IndicesCount);

    for (int i = instance.m_Material.m_Textures.size() - 1; i >= 0; i--)
    {
      RenderDevice::UnbindTexture2D(i);
    }
  }
}

void MeshSystem::AddComponent(Entity entity, const MeshData& data)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  m_Entities.push_back(entity);
  m_Meshes.push_back(data);
  m_EntityToComponentId[entity.GetId()] = m_Meshes.size() - 1;
}

void MeshSystem::DestroyComponent(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  uint32 componentIndexToRemove = m_EntityToComponentId[entity.GetId()];
  uint32 lastComponentIndex = m_Meshes.size() - 1;
  Entity lastEntity = m_Entities[lastComponentIndex];

  m_Meshes[componentIndexToRemove] = m_Meshes[lastComponentIndex];
  m_Entities[componentIndexToRemove] = m_Entities[lastComponentIndex];

  m_Meshes.pop_back();
  m_Entities.pop_back();

  m_EntityToComponentId[lastEntity.GetId()] = componentIndexToRemove;
  m_EntityToComponentId.erase(entity.GetId());
}

MeshData* MeshSystem::LookUpComponent(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");
  return &m_Meshes[m_EntityToComponentId[entity.GetId()]];
}

void MeshSystem::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<EntitiesDestroyedEvent>(
      BGE_BIND_EVENT_FN(MeshSystem::OnEntitiesDestroyed));
}

bool MeshSystem::OnEntitiesDestroyed(EntitiesDestroyedEvent& event)
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