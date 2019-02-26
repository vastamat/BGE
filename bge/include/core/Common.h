#pragma once

#include <cstdint>

#if defined BGE_PLATFORM_WINDOWS
#define BGE_DEBUG_BREAK                                                        \
  {                                                                            \
    __debugbreak();                                                            \
  }
#ifdef BGE_BUILD_SHARED
#define BGE_API __declspec(dllexport)
#else
#define BGE_API __declspec(dllimport)
#endif
#elif defined BGE_PLATFORM_UNIX
#include <signal.h>
#define BGE_DEBUG_BREAK                                                        \
  {                                                                            \
    raise(SIGTRAP);                                                            \
  }
#ifdef BGE_BUILD_SHARED
#define BGE_API __attribute__((visibility("default")))
#else
#define BGE_API
#endif
#elif defined BGE_PLATFORM_APPLE
#error Apple is not supported.
#else
#error Unknown platform!
#endif

#ifdef BGE_DEBUG
#define BGE_ENABLE_ASSERTS
#endif

#ifdef BGE_ENABLE_ASSERTS
#define BGE_ASSERT(x, ...)                                                     \
  {                                                                            \
    if (!(x))                                                                  \
    {                                                                          \
      BGE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                         \
      BGE_DEBUG_BREAK;                                                         \
    }                                                                          \
  }
#define BGE_CORE_ASSERT(x, ...)                                                \
  {                                                                            \
    if (!(x))                                                                  \
    {                                                                          \
      BGE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                    \
      BGE_DEBUG_BREAK;                                                         \
    }                                                                          \
  }
#else
#define BGE_ASSERT(x, ...)
#define BGE_CORE_ASSERT(x, ...)
#endif

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using intptr = std::intptr_t;
using uintptr = std::uintptr_t;

// Detect compiler
#if defined(__clang__)
#define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC
#elif defined(_MSC_VER)
#define COMPILER_MSVC
#else
#define COMPILER_OTHER
#endif

#ifdef COMPILER_MSVC
#define FORCEINLINE __forceinline
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#define FORCEINLINE inline __attribute__((always_inline))
#else
#define FORCEINLINE inline
#endif

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

#define BGE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define DELETE_COPY_AND_ASSIGN(T)                                              \
  T(const T& other) = delete;                                                  \
  void operator=(const T& other) = delete;
