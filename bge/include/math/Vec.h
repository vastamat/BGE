#pragma once

#include "MathUtils.h"

#include "core/Common.h"
#include "logging/Log.h"

#include <ostream>

namespace bge
{

// ------------------------------------------------------------------------------

/////////////////////////////////////////////////
/// Vector class which is templated to support many different types.
/////////////////////////////////////////////////
template <typename T, uint32 Size> class Vec
{
public:
  Vec() { memset(m_Elements, 0, Size * sizeof(T)); }

  // ------------------------------------------------------------------------------

  explicit Vec(T val)
  {
    for (uint32 i = 0; i < Size; i++)
    {
      m_Elements[i] = val;
    }
  }

  // ------------------------------------------------------------------------------

  Vec(T arg1, T arg2)
  {
    static_assert(Size == 2);
    m_Elements[0] = arg1;
    m_Elements[1] = arg2;
  }

  // ------------------------------------------------------------------------------

  Vec(T arg1, T arg2, T arg3)
  {
    static_assert(Size == 3);
    m_Elements[0] = arg1;
    m_Elements[1] = arg2;
    m_Elements[2] = arg3;
  }

  // ------------------------------------------------------------------------------

  Vec(T arg1, T arg2, T arg3, T arg4)
  {
    static_assert(Size == 4);
    m_Elements[0] = arg1;
    m_Elements[1] = arg2;
    m_Elements[2] = arg3;
    m_Elements[3] = arg4;
  }

  // ------------------------------------------------------------------------------

  Vec operator+(const Vec& rhs) const
  {
    Vec<T, Size> rtn;

    for (uint32 i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] + rhs.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec operator-(const Vec& rhs) const
  {
    Vec<T, Size> rtn;

    for (uint32 i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] - rhs.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec operator*(const Vec& rhs) const
  {
    Vec<T, Size> rtn;

    for (uint32 i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] * rhs.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec operator/(const Vec& rhs) const
  {
    Vec<T, Size> rtn;

    for (auto i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] / rhs.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec operator*(T scalar) const
  {
    Vec<T, Size> rtn;

    for (uint32 i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] * scalar;
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec operator/(T scalar) const
  {
    Vec<T, Size> rtn;

    for (auto i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] / scalar;
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec& operator+=(const Vec& rhs)
  {
    for (uint32 i = 0; i < Size; i++)
    {
      m_Elements[i] += rhs.m_Elements[i];
    }

    return *this;
  }

  // ------------------------------------------------------------------------------

  Vec& operator-=(const Vec& rhs)
  {
    for (auto i = 0; i < Size; i++)
    {
      m_Elements[i] -= rhs.m_Elements[i];
    }
    return *this;
  }

  // ------------------------------------------------------------------------------

  Vec& operator*=(const Vec& rhs)
  {
    for (auto i = 0; i < Size; i++)
    {
      m_Elements[i] *= rhs.m_Elements[i];
    }
    return *this;
  }

  // ------------------------------------------------------------------------------

  Vec& operator/=(const Vec& rhs)
  {
    for (auto i = 0; i < Size; i++)
    {
      m_Elements[i] /= rhs.m_Elements[i];
    }
    return *this;
  }

  // ------------------------------------------------------------------------------

  Vec& operator*=(T scalar)
  {
    for (auto i = 0; i < Size; i++)
    {
      m_Elements[i] *= scalar;
    }
    return *this;
  }

  // ------------------------------------------------------------------------------

  Vec& operator/=(T scalar)
  {
    for (auto i = 0; i < Size; i++)
    {
      m_Elements[i] *= scalar;
    }
    return *this;
  }

  // ------------------------------------------------------------------------------

  bool operator==(const Vec& rhs) const
  {
    bool allElementsAreEqual = true;

    for (uint32 i = 0; i < Size; i++)
    {
      allElementsAreEqual &=
          Equals(m_Elements[i], rhs.m_Elements[i], c_epsilon<T>);
    }

    return allElementsAreEqual;
  }

  // ------------------------------------------------------------------------------

  bool operator!=(const Vec& rhs) const { return !operator==(rhs); }

  // ------------------------------------------------------------------------------

  bool operator>(const Vec& rhs) const
  {
    bool allElementsAreHigher = true;

    for (auto i = 0; i < Size; i++)
    {
      allElementsAreHigher &= m_Elements[i] > rhs.m_Elements[i];
    }

    return allElementsAreHigher;
  }

  // ------------------------------------------------------------------------------

  bool operator>=(const Vec& rhs) const
  {
    bool allElementsAreHigherOrEqual = true;

    for (uint32 i = 0; i < Size; i++)
    {
      allElementsAreHigherOrEqual &= m_Elements[i] >= rhs.m_Elements[i];
    }

    return allElementsAreHigherOrEqual;
  }

  // ------------------------------------------------------------------------------

  bool operator<(const Vec& rhs) const
  {
    bool allElementsAreLower = true;

    for (auto i = 0; i < Size; i++)
    {
      allElementsAreLower &= m_Elements[i] < rhs.m_Elements[i];
    }

    return allElementsAreLower;
  }

  // ------------------------------------------------------------------------------

  bool operator<=(const Vec& rhs) const
  {
    bool allElementsAreLowerOrEqual = true;

    for (uint32 i = 0; i < Size; i++)
    {
      allElementsAreLowerOrEqual &= m_Elements[i] <= rhs.m_Elements[i];
    }

    return allElementsAreLowerOrEqual;
  }

  // ------------------------------------------------------------------------------

  T& operator[](uint32 index)
  {
    assert(index < Size);
    return m_Elements[index];
  }

  // ------------------------------------------------------------------------------

  T operator[](uint32 index) const
  {
    assert(index < Size);
    return m_Elements[index];
  }

  // ------------------------------------------------------------------------------

  T GetMagnitude() const { return Sqrt(GetSquaredMagnitude()); }

  // ------------------------------------------------------------------------------

  T GetSquaredMagnitude() const
  {
    T rtn = static_cast<T>(0.0);

    for (auto i = 0; i < Size; i++)
    {
      rtn += m_Elements[i] * m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec GetNormalized() const
  {
    const T magnitude = GetMagnitude();

    Vec<T, Size> rtn;

    for (auto i = 0; i < Size; i++)
    {
      rtn.m_Elements[i] = m_Elements[i] / magnitude;
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  T Dot(const Vec& other) const noexcept
  {
    T rtn = static_cast<T>(0.0);

    for (auto i = 0; i < Size; i++)
    {
      rtn += m_Elements[i] * other.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  T Distance(const Vec& other) const { return (*this - other).GetMagnitude(); }

  // ------------------------------------------------------------------------------

  T SquaredDistance(const Vec& other) const
  {
    return (*this - other).GetSquaredMagnitude();
  }

  // ------------------------------------------------------------------------------

  friend Vec GetMinValues(const Vec& v1, const Vec& v2)
  {
    Vec rtn;

    for (uint32 i = 0; i < Size; i++)
    {
      rtn[i] = Min(v1[i], v2[i]);
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  friend Vec GetMaxValues(const Vec& v1, const Vec& v2)
  {
    Vec rtn;

    for (uint32 i = 0; i < Size; i++)
    {
      rtn[i] = Max(v1[i], v2[i]);
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  friend Vec operator*(T lhs, const Vec& rhs) { return rhs * lhs; }

  // ------------------------------------------------------------------------------

  friend Vec operator/(T lhs, const Vec& rhs) { return rhs / lhs; }

  // ------------------------------------------------------------------------------

  friend std::ostream& operator<<(std::ostream& outStream, const Vec& vec)
  {
    outStream << "Vec: { ";

    for (auto i = 0; i < Size; i++)
    {
      outStream << vec.m_Elements[i] << ", ";
    }

    outStream << '}';
    return outStream;
  }

  // ------------------------------------------------------------------------------

private:
  T m_Elements[Size];
};

// ------------------------------------------------------------------------------

template <typename T>
Vec<T, 3> Cross(const Vec<T, 3>& first, const Vec<T, 3>& second)
{
  return Vec<T, 3>(first[1] * second[2] - first[2] * second[1],
                   first[2] * second[0] - first[0] * second[2],
                   first[0] * second[1] - first[1] * second[0]);
}

// ------------------------------------------------------------------------------

using Vec2f = Vec<float, 2>;
using Vec2d = Vec<double, 2>;
using Vec2i32 = Vec<int32, 2>;
using Vec2ui32 = Vec<uint32, 2>;

// ------------------------------------------------------------------------------

using Vec3f = Vec<float, 3>;
using Vec3d = Vec<double, 3>;
using Vec3i32 = Vec<int32, 3>;
using Vec3ui32 = Vec<uint32, 3>;

// ------------------------------------------------------------------------------

using Vec4f = Vec<float, 4>;
using Vec4d = Vec<double, 4>;
using Vec4i32 = Vec<int32, 4>;
using Vec4ui32 = Vec<uint32, 4>;

// ------------------------------------------------------------------------------

} // namespace bge