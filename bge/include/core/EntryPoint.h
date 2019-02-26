#pragma once

#include "Application.h"
#include "logging/Log.h"

extern std::unique_ptr<bge::Application> bge::CreateApplication();

int main(int argc, char** argv)
{
  bge::Log::Init();
  BGE_CORE_WARN("Initialized Log!");
  int a = 5;
  BGE_INFO("Hello! Var={0}", a);

  auto app = bge::CreateApplication();
  app->Run();
}