#pragma once

#include "Material.h"
#include "Mesh.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Mat.h"

#include <unordered_map>

namespace bge
{

struct StaticMeshData
{
  Mesh m_Mesh;
  Mat4f m_Transform;
  Material m_Material;
};

class StaticMeshSystem
{
public:
  StaticMeshSystem();

  void SetEventCallback(const std::function<void(Event&)>& callback);

  void RenderMeshes(const Mat4f& projection, const Mat4f& view);

  void AddComponent(Entity entity, const StaticMeshData& data);
  void DestroyComponent(Entity entity);
  StaticMeshData* LookUpComponent(Entity entity);

  void OnEvent(Event& event);

private:
  bool OnEntitiesDestroyed(EntitiesDestroyedEvent& event);

  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<StaticMeshData> m_Meshes;
  std::vector<Entity> m_Entities;
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge