#pragma once

#include "Material.h"
#include "Mesh.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Mat.h"

#include <unordered_map>

namespace bge
{

/**
 * Structure which contains cold mesh data
 */
struct StaticMeshData
{
  Mesh m_Mesh;
  Mat4f m_Transform;
  Material m_Material;
};

/**
 * System which handles static meshes
 */
class StaticMeshSystem
{
public:
  StaticMeshSystem();

  /**
   * Sets the internal event callback variable
   * It's used to broadcast events to the application layer
   * @param callback the function pointer to use to broadcast events
   */
  void SetEventCallback(const std::function<void(Event&)>& callback);

  /**
   * Renders the existing meshes in the system from the POV of the input camera
   * @param projection the camera's projection matrix
   * @param view the camera's view matrix
   */
  void RenderMeshes(const Mat4f& projection, const Mat4f& view);

  /**
   * Allocates a new component instance mapped to the passed entity
   * @param entity the entity to map to the new data
   * @param data the data to use for the new allocation
   */
  void AddComponent(Entity entity, const StaticMeshData& data);

  /**
   * Remove the existing component instance mapped to an entity
   * @param entity the entity the component is mapped to
   */
  void DestroyComponent(Entity entity);

  /**
   * Retrieve an individual component instance which belongs to an entity
   * @param entity the entity the component is mapped to
   */
  StaticMeshData* LookUpComponent(Entity entity);

  /**
   * Event handler function
   * @param event the propagated event
   */
  void OnEvent(Event& event);

private:
  /**
   * Handler of destroyed entities event
   * @param event the propagated event
   */
  bool OnEntitiesDestroyed(EntitiesDestroyedEvent& event);

  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<StaticMeshData> m_Meshes;
  std::vector<Entity> m_Entities;
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge