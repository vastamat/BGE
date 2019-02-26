#include <bge.h>

class Sandbox : public bge::Application
{
public:
  Sandbox() {}

  ~Sandbox() {}
};

std::unique_ptr<bge::Application> bge::CreateApplication()
{
  return std::make_unique<Sandbox>();
}