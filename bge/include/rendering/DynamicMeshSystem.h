#pragma once

#include "Material.h"
#include "Mesh.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Transform.h"

#include <unordered_map>

namespace bge
{

/**
 * Structure which contains cold mesh data
 */
struct DynamicMeshData
{
  Mesh m_Mesh;
  Material m_Material;
};

/**
 * System which handles dynamic meshes
 */
class DynamicMeshSystem
{
public:
  /**
   * Sets the internal event callback variable
   * It's used to broadcast events to the application layer
   * @param callback the function pointer to use to broadcast events
   */
  void SetEventCallback(const std::function<void(Event&)>& callback);

  /**
   * Update the internal transforms of the meshes using an input from outside
   * @param transforms array of transforms to be linearly assigned
   */
  void UpdateTransforms(const std::vector<Transform>& transforms);

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
  void AddComponent(Entity entity, const DynamicMeshData& data);

  /**
   * Remove the existing component instance mapped to an entity
   * @param entity the entity the component is mapped to
   */
  void DestroyComponent(Entity entity);

  /**
   * Retrieve an individual component instance which belongs to an entity
   * @param entity the entity the component is mapped to
   */
  DynamicMeshData* LookUpComponent(Entity entity);

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

private:
  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<DynamicMeshData> m_Meshes;
  std::vector<Mat4f> m_Transforms;
  std::vector<Entity> m_Entities;
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
