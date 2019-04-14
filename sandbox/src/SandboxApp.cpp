#include <core/Application.h>
#include <logging/Log.h>
#include <math/Transform.h>

class Sandbox : public bge::Application
{
public:
  Sandbox()
  {
    auto& world = GetWorld();
    auto* renderWorld = world.GetComponentWorld<bge::RenderWorld>();
    renderWorld->AddCamera(bge::Vec4i32(0, 0, 1280, 720), 60.0f, 0.1f, 100.0f);

    bge::EntityId entity = world.CreateEntity();

    bge::Transform transform;
    transform.Translate(bge::Vec3f(0.0f, 0.0f, -5.0f));
    bge::MeshData meshCompData;
    meshCompData.m_Mesh = renderWorld->LoadMesh("res/models/cube.obj");
    meshCompData.m_Transform = transform.ToMatrix();
    meshCompData.m_Material.m_Shader =
        renderWorld->LoadShader("res/shaders/basic");
    meshCompData.m_Material.m_Textures.push_back(
        renderWorld->LoadTexture2D("res/textures/bricks.jpg"));

    world.AddComponent(entity, meshCompData);
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