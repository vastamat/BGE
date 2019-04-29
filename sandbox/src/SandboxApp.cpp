#include <core/Application.h>
#include <logging/Log.h>
#include <math/Transform.h>
#include <util/RandomNumberGenerator.h>

#include "BallControlSystem.h"
#include "CameraControlSystem.h"

bge::Entity AddBall(bge::World& world, bge::PhysicsWorld& physicsWorld,
                    bge::RenderWorld& renderWorld, float mass = 1.0f)
{
  bge::RandomNumberGenerator rng;
  bge::Entity entity = world.CreateEntity();

  float x = rng.GenRandReal(-15.0f, 15.0f);
  float y = rng.GenRandReal(-15.0f, 15.0f);
  float z = rng.GenRandReal(-15.0f, 15.0f);

  // Add Physics body
  physicsWorld.GetRigidBodySystem().AddSphereBodyComponent(entity, mass, mass);
  physicsWorld.GetRigidBodySystem().SetBodyPosition(entity,
                                                    bge::Vec3f(x, y, z));

  // Add the dynamic mesh
  // bge::Transform transform;
  // transform.Translate(bge::Vec3f(0.0f, 0.0f, -5.0f));
  bge::DynamicMeshData meshCompData;
  meshCompData.m_Mesh = renderWorld.LoadMesh("res/models/sphere.obj");
  meshCompData.m_Material.m_Shader =
      renderWorld.LoadShader("res/shaders/basic");
  meshCompData.m_Material.m_Textures.push_back(
      renderWorld.LoadTexture2D("res/textures/bricks.jpg"));

  renderWorld.GetDynamicMeshSystem().AddComponent(entity, meshCompData);

  return entity;
}

class Sandbox : public bge::Application
{
public:
  Sandbox()
  {
    auto& world = GetWorld();
    bge::RenderWorld& renderWorld = world.GetRenderWorld();
    bge::GameWorld& gameWorld = world.GetGameWorld();
    bge::PhysicsWorld& physicsWorld = world.GetPhysicsWorld();

    uint32 cameraId = renderWorld.AddCamera(bge::Vec4i32(0, 0, 1280, 720),
                                            60.0f, 0.1f, 100.0f);

    {
      // Floor
      bge::Entity floorEntity = world.CreateEntity();
      bge::Vec3f position(0.0f, -20.0f, -5.0f);
      bge::Vec3f size(20.0f, 1.0f, 20.0f);
      bge::Quatf rotation;
      physicsWorld.GetColliderSystem().AddBoxCollider(floorEntity, position,
                                                      rotation, size);
      // physicsWorld.GetColliderSystem().AddSphereCollider(floorEntity,
      // position, 2.0f);
    }

    {
      // Ceiling
      bge::Entity ceiling = world.CreateEntity();
      bge::Vec3f position(0.0f, 20.0f, -5.0f);
      bge::Vec3f size(20.0f, 1.0f, 20.0f);
      bge::Quatf rotation;
      physicsWorld.GetColliderSystem().AddBoxCollider(ceiling, position,
                                                      rotation, size);
    }

    {
      // left-side wall
      bge::Entity wall = world.CreateEntity();
      bge::Vec3f position(-21.f, 0.0f, -5.0f);
      bge::Vec3f size(19.f, 1.0f, 20.0f);
      bge::Quatf rotation(bge::Vec3f(0.0f, 0.0f, bge::ToRadians(90.0f)));
      physicsWorld.GetColliderSystem().AddBoxCollider(wall, position, rotation,
                                                      size);
    }

    {
      // right-side wall
      bge::Entity wall = world.CreateEntity();
      bge::Vec3f position(21.0f, 0.0f, -5.0f);
      bge::Vec3f size(19.0f, 1.0f, 20.0f);
      bge::Quatf rotation(bge::Vec3f(0.0f, 0.0f, bge::ToRadians(90.0f)));
      physicsWorld.GetColliderSystem().AddBoxCollider(wall, position, rotation,
                                                      size);
    }

    {
      // back-side wall
      bge::Entity wall = world.CreateEntity();
      bge::Vec3f position(0.0f, 0.0f, 16.0f);
      bge::Vec3f size(20.0f, 1.0f, 19.0f);
      bge::Quatf rotation(bge::Vec3f(bge::ToRadians(90.0f), 0.0f, 0.0f));
      physicsWorld.GetColliderSystem().AddBoxCollider(wall, position, rotation,
                                                      size);
    }

    {
      // forward-side wall
      bge::Entity wall = world.CreateEntity();
      bge::Vec3f position(0.0f, 0.0f, -26.0f);
      bge::Vec3f size(20.0f, 1.0f, 19.0f);
      bge::Quatf rotation(bge::Vec3f(bge::ToRadians(90.0f), 0.0f, 0.0f));
      physicsWorld.GetColliderSystem().AddBoxCollider(wall, position, rotation,
                                                      size);
    }

    for (size_t i = 0; i < 200; i++)
    {
      AddBall(world, physicsWorld, renderWorld);
    }

    bge::Entity entity = AddBall(world, physicsWorld, renderWorld);

    std::unique_ptr<CameraControlSystem> cameraControlSystem =
        std::make_unique<CameraControlSystem>(cameraId);
    std::unique_ptr<BallControlSystem> ballControlSystem =
        std::make_unique<BallControlSystem>(entity, 10.0f);
    gameWorld.AddGameSystem(std::move(cameraControlSystem));
    gameWorld.AddGameSystem(std::move(ballControlSystem));
  }

  ~Sandbox() {}
};

int main(int argc, char** argv)
{
  bge::Log::Init();
  BGE_INFO("Initialized Log!");

  auto app = std::make_unique<Sandbox>();
  app->Run();
}