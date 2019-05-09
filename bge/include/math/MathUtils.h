#pragma once

#include "core/Common.h"
#include "logging/Log.h"

#include <cmath>
#include <limits>

namespace bge
{

// ------------------------------------------------------------------------------

template <typename T> constexpr T c_pi = 3.1415926535897932;
template <typename T> constexpr T c_twoPi = 6.28318530717959;
template <typename T> constexpr T c_halfPi = 1.57079632679;
template <typename T> constexpr T c_reciprocalPi = 0.31830988618;
template <typename T> constexpr T c_reciprocalTwoPi = 0.159154943091895;
template <typename T> constexpr T c_reciprocalHalfPi = 0.636619772367581;

template <typename T> constexpr T c_eulersNumber = 2.71828182845904523536f;
template <typename T> constexpr T c_log2E = 1.44269504088896;
template <typename T> constexpr T c_radToDeg = 57.2957795f; // 180.0f/PI;
template <typename T> constexpr T c_degToRad = 0.0174532f;  // PI/180.0f;
template <typename T>
constexpr T c_epsilon = std::numeric_limits<T>::epsilon() * 100;

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Floor(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}

template <> FORCEINLINE float Floor(float val) { return floorf(val); }
template <> FORCEINLINE double Floor(double val) { return floor(val); }
template <> FORCEINLINE long double Floor(long double val)
{
  return floorl(val);
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Ceil(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}

template <> FORCEINLINE float Ceil(float val) { return ceilf(val); }
template <> FORCEINLINE double Ceil(double val) { return ceil(val); }
template <> FORCEINLINE long double Ceil(long double val) { return ceill(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Exp(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Exp(float val) { return expf(val); }
template <> FORCEINLINE double Exp(double val) { return exp(val); }
template <> FORCEINLINE long double Exp(long double val) { return expl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T LogOfVal(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float LogOfVal(float val) { return logf(val); }
template <> FORCEINLINE double LogOfVal(double val) { return log(val); }
template <> FORCEINLINE long double LogOfVal(long double val)
{
  return logl(val);
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Logx(T val, T base)
{
  return LogOfVal(val) / LogOfVal(base);
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Log2(T val)
{
  return LogOfVal(val) * c_log2E<T>;
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Fmod(T num, T den)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Fmod(float num, float den)
{
  return fmodf(num, den);
}
template <> FORCEINLINE double Fmod(double num, double den)
{
  return fmod(num, den);
}
template <> FORCEINLINE long double Fmod(long double num, long double den)
{
  return fmodl(num, den);
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Sin(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Sin(float val) { return sinf(val); }
template <> FORCEINLINE double Sin(double val) { return sin(val); }
template <> FORCEINLINE long double Sin(long double val) { return sinl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T ASin(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float ASin(float val) { return asinf(val); }
template <> FORCEINLINE double ASin(double val) { return asin(val); }
template <> FORCEINLINE long double ASin(long double val) { return asinl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Cos(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Cos(float val) { return cosf(val); }
template <> FORCEINLINE double Cos(double val) { return cos(val); }
template <> FORCEINLINE long double Cos(long double val) { return cosl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T ACos(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float ACos(float val) { return acosf(val); }
template <> FORCEINLINE double ACos(double val) { return acos(val); }
template <> FORCEINLINE long double ACos(long double val) { return acosl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Tan(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Tan(float val) { return tanf(val); }
template <> FORCEINLINE double Tan(double val) { return tan(val); }
template <> FORCEINLINE long double Tan(long double val) { return tanl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T ATan(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float ATan(float val) { return atanf(val); }
template <> FORCEINLINE double ATan(double val) { return atan(val); }
template <> FORCEINLINE long double ATan(long double val) { return atanl(val); }

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T ATan2(T y, T x)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float ATan2(float y, float x) { return atan2f(y, x); }
template <> FORCEINLINE double ATan2(double y, double x) { return atan2(y, x); }
template <> FORCEINLINE long double ATan2(long double y, long double x)
{
  return atan2l(y, x);
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Pow(T val, T pow)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Pow(float val, float pow)
{
  return powf(val, pow);
}
template <> FORCEINLINE double Pow(double val, double pow)
{
  return powf(val, pow);
}
template <> FORCEINLINE long double Pow(long double val, long double pow)
{
  return powl(val, pow);
}

// ------------------------------------------------------------------------------

template <typename T> FORCEINLINE T Sqrt(T val)
{
  BGE_CORE_ASSERT(false, "Unsupported type");
}
template <> FORCEINLINE float Sqrt(float val) { return sqrtf(val); }
template <> FORCEINLINE double Sqrt(double val) { return sqrt(val); }
template <> FORCEINLINE long double Sqrt(long double val) { return sqrtl(val); }

// ------------------------------------------------------------------------------

constexpr FORCEINLINE bool IsPowerOfTwo(uint32 val)
{
  return (val != 0u) && ((val & (val - 1u)) == 0u);
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T Abs(const T& val)
{
  return val >= static_cast<T>(0) ? val : -val;
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T Min(const T& val1, const T& val2)
{
  return val1 <= val2 ? val1 : val2;
}

// ------------------------------------------------------------------------------

template <typename T>
constexpr FORCEINLINE T Min3(const T& val1, const T& val2, const T& val3)
{
  return Min(Min(val1, val2), val3);
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T Max(const T& val1, const T& val2)
{
  return val1 >= val2 ? val1 : val2;
}

// ------------------------------------------------------------------------------

template <typename T>
constexpr FORCEINLINE T Max3(const T& val1, const T& val2, const T& val3)
{
  return Max(Max(val1, val2), val3);
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T ToDegrees(const T& val)
{
  return val * c_radToDeg<T>;
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T ToRadians(const T& val)
{
  return val * c_degToRad<T>;
}

// ------------------------------------------------------------------------------

template <typename T, typename U>
constexpr FORCEINLINE T Lerp(const T& src, const T& dst, const U& alpha)
{
  return static_cast<T>((src * (static_cast<U>(1) - alpha)) + dst * alpha);
}

// ------------------------------------------------------------------------------

template <typename T>
constexpr FORCEINLINE T Clamp(const T& val, const T& min, const T& max)
{
  if (val > max)
  {
    return max;
  }
  else if (val > min)
  {
    return val;
  }
  else
  {
    return min;
  }
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T Square(const T& val)
{
  return val * val;
}

// ------------------------------------------------------------------------------

template <typename T> constexpr FORCEINLINE T Cube(const T& val)
{
  return val * val * val;
}

// ------------------------------------------------------------------------------

template <typename T>
constexpr FORCEINLINE bool Equals(const T& val1, const T& val2,
                                  const T& errorMargin)
{
  return Abs(val1 - val2) < errorMargin;
}

// ------------------------------------------------------------------------------

} // namespace bge