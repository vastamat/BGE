#pragma once

#include "ComponentTraits.h"
#include "EntityManager.h"
#include "GameWorld.h"

#include "events/Event.h"
#include "physics/PhysicsWorld.h"
#include "rendering/RenderWorld.h"

namespace bge
{

/**
 * The main worlds which encompasses all the sub-worlds that represent
 * individual modules of the framework.
 */
class World
{
public:
  World();

  /**
   * Initialize the world state after all modules have been initialized
   */
  void Init();

  /**
   * Sets the internal event callback variable
   * It's used to broadcast events to the application layer
   * @param callback the function pointer to use to broadcast events
   */
  void SetEventCallback(const std::function<void(Event&)>& callback);

  /**
   * Updates the game state by updating the sub-worlds in order and passing
   * relevant data through the worlds
   * @param deltaSeconds the time passed since last update.
   * Always a fixed 0.040 seconds (25 FPS)
   */
  void Update(float deltaTime);

  /**
   * Renders the world by calling the RenderWorld
   * @param interpolation shows how close the app is to the next game state
   * update which can be used to interpolate game state
   */
  void Render(float interpolation);

  /**
   * Create a new entity
   * @return the entity
   */
  Entity CreateEntity();

  /**
   * Destroy an existing entity
   * @param id the entity to destroy
   */
  void DestroyEntity(Entity entity);

  /**
   * Calls the OnEvent function of all sub-worlds
   * @param event the broadcast event
   */
  void OnEvent(Event& event);

  FORCEINLINE RenderWorld& GetRenderWorld() { return m_RenderWorld; }
  FORCEINLINE GameWorld& GetGameWorld() { return m_GameWorld; }
  FORCEINLINE PhysicsWorld& GetPhysicsWorld() { return m_PhysicsWorld; }

private:
  EntityManager m_EntityManager; /**< manager of entities */

  RenderWorld m_RenderWorld;   /**< the rendering sub-world */
  PhysicsWorld m_PhysicsWorld; /**< the physics sub-world */
  GameWorld m_GameWorld;       /**< the gameplay logic sub-world */

  std::vector<Entity> m_DestroyedEntities; /**< list of destroyed entities */

  std::function<void(Event&)> m_EventCallback; /**< event broadcast ptr */
};

} // namespace bge
