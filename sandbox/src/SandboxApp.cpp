#include <core/Application.h>
#include <logging/Log.h>
#include <math/Transform.h>

#include "CameraControlSystem.h"

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
      bge::Vec3f position(0.0f, -10.0f, -5.0f);
      bge::Vec3f size(10.0f, 2.0f, 10.0f);
      bge::Quatf rotation(bge::Vec3f(bge::ToRadians(90.0f), 0.0f, 0.0f));
      physicsWorld.GetColliderSystem().AddBoxCollider(floorEntity, position,
                                                      rotation, size);
      // physicsWorld.GetColliderSystem().AddSphereCollider(floorEntity,
      // position, 2.0f);
    }

    bge::Entity entity = world.CreateEntity();

    // Add Physics body
    physicsWorld.GetRigidBodySystem().AddBoxBodyComponent(entity, 1.0f, 1.0f,
                                                          1.0f, 1.0f);
    physicsWorld.GetRigidBodySystem().SetBodyPosition(
        entity, bge::Vec3f(0.0f, 0.0f, -5.0f));

    // Add the dynamic mesh
    // bge::Transform transform;
    // transform.Translate(bge::Vec3f(0.0f, 0.0f, -5.0f));
    bge::DynamicMeshData meshCompData;
    meshCompData.m_Mesh = renderWorld.LoadMesh("res/models/cube.obj");
    meshCompData.m_Material.m_Shader =
        renderWorld.LoadShader("res/shaders/basic");
    meshCompData.m_Material.m_Textures.push_back(
        renderWorld.LoadTexture2D("res/textures/bricks.jpg"));

    renderWorld.GetDynamicMeshSystem().AddComponent(entity, meshCompData);

    std::unique_ptr<CameraControlSystem> cameraControlSystem =
        std::make_unique<CameraControlSystem>(cameraId);
    gameWorld.AddGameSystem(std::move(cameraControlSystem));
  }

  ~Sandbox() {}
};

int main(int argc, char** argv)
{
  bge::Log::Init();
  BGE_INFO("Initialized Log!");
  int a = 5;
  BGE_INFO("Hello! Var={0}", a);

  auto app = std::make_unique<Sandbox>();
  app->Run();
}