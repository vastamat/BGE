#pragma once

#include "Material.h"
#include "Mesh.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Transform.h"

#include <unordered_map>

namespace bge
{

struct DynamicMeshData
{
  Mesh m_Mesh;
  Material m_Material;
};

class DynamicMeshSystem
{
public:
  void SetEventCallback(const std::function<void(Event&)>& callback);

  void UpdateTransforms(const std::vector<Transform>& transforms);
  void RenderMeshes(const Mat4f& projection, const Mat4f& view);

  void AddComponent(Entity entity, const DynamicMeshData& data);
  void DestroyComponent(Entity entity);
  DynamicMeshData* LookUpComponent(Entity entity);

  void OnEvent(Event& event);

private:
  bool OnEntitiesDestroyed(EntitiesDestroyedEvent& event);

private:
  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<DynamicMeshData> m_Meshes;
  std::vector<Mat4f> m_Transforms;
  std::vector<Entity> m_Entities;
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
