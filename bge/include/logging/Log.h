#pragma once

#include "core/Common.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace bge
{

class Log
{
public:
  static void Init();

  FORCEINLINE static std::shared_ptr<spdlog::logger>& GetCoreLogger()
  {
    return s_CoreLogger;
  }
  FORCEINLINE static std::shared_ptr<spdlog::logger>& GetClientLogger()
  {
    return s_ClientLogger;
  }

private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace bge

// Core log macros
#define BGE_CORE_TRACE(...) ::bge::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BGE_CORE_INFO(...) ::bge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BGE_CORE_WARN(...) ::bge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BGE_CORE_ERROR(...) ::bge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BGE_CORE_FATAL(...) ::bge::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define BGE_TRACE(...) ::bge::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BGE_INFO(...) ::bge::Log::GetClientLogger()->info(__VA_ARGS__)
#define BGE_WARN(...) ::bge::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BGE_ERROR(...) ::bge::Log::GetClientLogger()->error(__VA_ARGS__)
#define BGE_FATAL(...) ::bge::Log::GetClientLogger()->fatal(__VA_ARGS__)