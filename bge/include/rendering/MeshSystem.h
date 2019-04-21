#pragma once

#include "Material.h"
#include "Mesh.h"

#include "ecs/Entity.h"
#include "logging/Log.h"
#include "math/Mat.h"

#include <unordered_map>

namespace bge
{

struct MeshData
{
  Mesh m_Mesh;
  Mat4f m_Transform;
  Material m_Material;
};

class MeshSystem
{
public:
  MeshSystem();
  void RenderMeshes(const Mat4f& projection, const Mat4f& view);

  void AddComponent(Entity entity, const MeshData& data);
  void DestroyComponent(Entity entity);
  MeshData* LookUpComponent(Entity entity);

private:
  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<MeshData> m_Meshes;
  std::vector<Entity> m_Entities;
};

} // namespace bge