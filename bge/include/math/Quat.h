#pragma once

#include "Mat.h"

namespace bge
{

// ------------------------------------------------------------------------------

/**
 * Quaternion class which is templated to support a float and double quats.
 */
template <typename T> struct Quat
{
  Quat()
  {
    memset(m_Elements, 0, 4 * sizeof(T));
    m_Elements[3] = static_cast<T>(1.0);
  }

  explicit Quat(T val[4]) { memcpy(m_Elements, val, 4 * sizeof(T)); }

  // ------------------------------------------------------------------------------

  Quat(T x, T y, T z, T w)
  {
    m_Elements[0] = x;
    m_Elements[1] = y;
    m_Elements[2] = z;
    m_Elements[3] = w;
  }

  // ------------------------------------------------------------------------------

  explicit Quat(const Vec<T, 3>& eulerAnglesInRadians)
  {
    const Vec<T, 3> c(Cos(eulerAnglesInRadians[0] * static_cast<T>(0.5)),
                      Cos(eulerAnglesInRadians[1] * static_cast<T>(0.5)),
                      Cos(eulerAnglesInRadians[2] * static_cast<T>(0.5)));

    const Vec<T, 3> s(Sin(eulerAnglesInRadians[0] * static_cast<T>(0.5)),
                      Sin(eulerAnglesInRadians[1] * static_cast<T>(0.5)),
                      Sin(eulerAnglesInRadians[2] * static_cast<T>(0.5)));

    m_Elements[0] = s[0] * c[1] * c[2] - c[0] * s[1] * s[2];
    m_Elements[1] = c[0] * s[1] * c[2] + s[0] * c[1] * s[2];
    m_Elements[2] = c[0] * c[1] * s[2] - s[0] * s[1] * c[2];
    m_Elements[3] = c[0] * c[1] * c[2] + s[0] * s[1] * s[2];
  }

  // ------------------------------------------------------------------------------

  explicit Quat(const Mat<T, 4>& mat4)
  {
    T fourXSquaredMinus1 = mat4[0 + 4 * 0] - mat4[1 + 4 * 1] - mat4[2 + 4 * 2];

    T fourYSquaredMinus1 = mat4[1 + 4 * 1] - mat4[0 + 4 * 0] - mat4[2 + 4 * 2];

    T fourZSquaredMinus1 = mat4[2 + 4 * 2] - mat4[0 + 4 * 0] - mat4[1 + 4 * 1];

    T fourWSquaredMinus1 = mat4[0 + 4 * 0] + mat4[1 + 4 * 1] + mat4[2 + 4 * 2];

    int32 biggestIndex = 0;

    T fourBiggestSquaredMinus1 = fourWSquaredMinus1;

    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
    {
      fourBiggestSquaredMinus1 = fourXSquaredMinus1;
      biggestIndex = 1;
    }

    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
    {
      fourBiggestSquaredMinus1 = fourYSquaredMinus1;
      biggestIndex = 2;
    }

    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
    {
      fourBiggestSquaredMinus1 = fourZSquaredMinus1;
      biggestIndex = 3;
    }

    T biggestVal = Sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1.0)) *
                   static_cast<T>(0.5);
    T mult = static_cast<T>(0.25) / biggestVal;

    switch (biggestIndex)
    {
      case 0:
        m_Elements[3] = biggestVal;
        m_Elements[0] = (mat4[1 + 4 * 2] - mat4[2 + 4 * 1]) * mult;
        m_Elements[1] = (mat4[2 + 4 * 0] - mat4[0 + 4 * 2]) * mult;
        m_Elements[2] = (mat4[0 + 4 * 1] - mat4[1 + 4 * 0]) * mult;
        break;
      case 1:
        m_Elements[3] = (mat4[1 + 4 * 2] - mat4[2 + 4 * 1]) * mult;
        m_Elements[0] = biggestVal;
        m_Elements[1] = (mat4[0 + 4 * 1] + mat4[1 + 4 * 0]) * mult;
        m_Elements[2] = (mat4[2 + 4 * 0] + mat4[0 + 4 * 2]) * mult;
        break;
      case 2:
        m_Elements[3] = (mat4[2 + 4 * 0] - mat4[0 + 4 * 2]) * mult;
        m_Elements[0] = (mat4[0 + 4 * 1] + mat4[1 + 4 * 0]) * mult;
        m_Elements[1] = biggestVal;
        m_Elements[2] = (mat4[1 + 4 * 2] + mat4[2 + 4 * 1]) * mult;
        break;
      case 3:
        m_Elements[3] = (mat4[0 + 4 * 1] - mat4[1 + 4 * 0]) * mult;
        m_Elements[0] = (mat4[2 + 4 * 0] + mat4[0 + 4 * 2]) * mult;
        m_Elements[1] = (mat4[1 + 4 * 2] + mat4[2 + 4 * 1]) * mult;
        m_Elements[2] = biggestVal;
        break;
    }
  }

  // ------------------------------------------------------------------------------

  static Quat GenRotation(const Vec<T, 3>& origin, const Vec<T, 3>& dest)
  {
    const T cosHalfAngleX2 =
        Sqrt((static_cast<T>(2.0) * (static_cast<T>(1.0) + origin.Dot(dest))));

    const T recipCosHalfAngleX2 = (static_cast<T>(1.0) / cosHalfAngleX2);

    const Vec<T, 3> axisOfRotation = Cross(origin, dest) * recipCosHalfAngleX2;

    const T cosAmountOfRotation = cosHalfAngleX2 * static_cast<T>(0.5);

    return Quat(axisOfRotation[0], axisOfRotation[1], axisOfRotation[2],
                cosAmountOfRotation);
  }

  // ------------------------------------------------------------------------------

  static Quat GenRotation(T radians, const Vec<T, 3>& unitVec)
  {
    const T angle = radians * static_cast<T>(0.5);
    const Vec<T, 3> axisOfRotation = unitVec * Sin(angle);
    const T cosAmountOfRotation = Cos(angle);

    return Quat(axisOfRotation[0], axisOfRotation[1], axisOfRotation[2],
                cosAmountOfRotation);
  }

  // ------------------------------------------------------------------------------

  static Quat GenRotationX(T radians)
  {
    const T angle = radians * static_cast<T>(0.5);
    return Quat(Sin(angle), static_cast<T>(0.0), static_cast<T>(0.0),
                Cos(angle));
  }

  // ------------------------------------------------------------------------------

  static Quat GenRotationY(T radians)
  {
    const T angle = radians * static_cast<T>(0.5);
    return Quat(static_cast<T>(0.0), Sin(angle), static_cast<T>(0.0),
                Cos(angle));
  }

  // ------------------------------------------------------------------------------

  static Quat GenRotationZ(T radians)
  {
    const T angle = radians * static_cast<T>(0.5);
    return Quat(static_cast<T>(0.0), static_cast<T>(0.0), Sin(angle),
                Cos(angle));
  }

  // ------------------------------------------------------------------------------

  static Vec<T, 3> RotateVec(const Quat& quat, const Vec<T, 3>& vec)
  {
    // const Vec<T, 3> axesOfRotation(quat.m_Elements[0], quat.m_Elements[1],
    //                                quat.m_Elements[2]);
    // const Vec<T, 3> uv(Cross(axesOfRotation, vec));
    // const Vec<T, 3> uuv(Cross(axesOfRotation, uv));

    // return vec + ((uv * quat.m_Elements[3]) + uuv) * static_cast<T>(2.0);

    const float tmpX =
        (((quat[3] * vec[0]) + (quat[1] * vec[2])) - (quat[2] * vec[1]));
    const float tmpY =
        (((quat[3] * vec[1]) + (quat[2] * vec[0])) - (quat[0] * vec[2]));
    const float tmpZ =
        (((quat[3] * vec[2]) + (quat[0] * vec[1])) - (quat[1] * vec[0]));
    const float tmpW =
        (((quat[0] * vec[0]) + (quat[1] * vec[1])) + (quat[2] * vec[2]));

    return Vec<T, 3>(
        ((((tmpW * quat[0]) + (tmpX * quat[3])) - (tmpY * quat[2])) +
         (tmpZ * quat[1])),
        ((((tmpW * quat[1]) + (tmpY * quat[3])) - (tmpZ * quat[0])) +
         (tmpX * quat[2])),
        ((((tmpW * quat[2]) + (tmpZ * quat[3])) - (tmpX * quat[1])) +
         (tmpY * quat[0])));
  }

  // ------------------------------------------------------------------------------

  Quat operator+(const Quat& rhs) const
  {
    return Quat(
        m_Elements[0] + rhs.m_Elements[0], m_Elements[1] + rhs.m_Elements[1],
        m_Elements[2] + rhs.m_Elements[2], m_Elements[3] + rhs.m_Elements[3]);
  }

  // ------------------------------------------------------------------------------

  Quat operator-(const Quat& rhs) const
  {
    return Quat(
        m_Elements[0] - rhs.m_Elements[0], m_Elements[1] - rhs.m_Elements[1],
        m_Elements[2] - rhs.m_Elements[2], m_Elements[3] - rhs.m_Elements[3]);
  }

  // ------------------------------------------------------------------------------

  Quat operator*(const Quat& rhs) const
  {
    return Quat((((m_Elements[3] * rhs.m_Elements[0]) +
                  (m_Elements[0] * rhs.m_Elements[3])) +
                 (m_Elements[1] * rhs.m_Elements[2])) -
                    (m_Elements[2] * rhs.m_Elements[1]),

                (((m_Elements[3] * rhs.m_Elements[1]) +
                  (m_Elements[1] * rhs.m_Elements[3])) +
                 (m_Elements[2] * rhs.m_Elements[0])) -
                    (m_Elements[0] * rhs.m_Elements[2]),

                (((m_Elements[3] * rhs.m_Elements[2]) +
                  (m_Elements[2] * rhs.m_Elements[3])) +
                 (m_Elements[0] * rhs.m_Elements[1])) -
                    (m_Elements[1] * rhs.m_Elements[0]),

                (((m_Elements[3] * rhs.m_Elements[3]) -
                  (m_Elements[0] * rhs.m_Elements[0])) -
                 (m_Elements[1] * rhs.m_Elements[1])) -
                    (m_Elements[2] * rhs.m_Elements[2]))
        .GetNormalized();
  }

  // ------------------------------------------------------------------------------

  Quat operator*(T rhs) const
  {
    return Quat(m_Elements[0] * rhs, m_Elements[1] * rhs, m_Elements[2] * rhs,
                m_Elements[3] * rhs);
  }

  // ------------------------------------------------------------------------------

  Quat operator/(T rhs) const
  {
    return Quat(m_Elements[0] / rhs, m_Elements[1] / rhs, m_Elements[2] / rhs,
                m_Elements[3] / rhs);
  }

  // ------------------------------------------------------------------------------

  Quat operator-() const
  {
    return Quat(-m_Elements[0], -m_Elements[1], -m_Elements[2], -m_Elements[3]);
  }

  // ------------------------------------------------------------------------------

  Quat& operator+=(const Quat& rhs)
  {
    *this = *this + rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Quat& operator-=(const Quat& rhs)
  {
    *this = *this - rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Quat& operator*=(const Quat& rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Quat& operator*=(T rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Quat& operator/=(T rhs)
  {
    *this = *this / rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  bool operator==(const Quat& rhs) const
  {
    bool allElementsAreEqual = true;

    for (auto i = 0; i < 4; i++)
    {
      allElementsAreEqual &=
          Equals(m_Elements[i], rhs.m_Elements[i], c_epsilon<T>);
    }

    return allElementsAreEqual;
  }

  // ------------------------------------------------------------------------------

  bool operator!=(const Quat& rhs) const { return !operator==(rhs); }

  // ------------------------------------------------------------------------------

  T& operator[](uint32 index)
  {
    assert(index < 4);
    return m_Elements[index];
  }

  // ------------------------------------------------------------------------------

  T operator[](uint32 index) const
  {
    assert(index < 4);
    return m_Elements[index];
  }

  // ------------------------------------------------------------------------------

  Vec<T, 3> GetAxis() const
  {
    const T temp1 = static_cast<T>(1.0) - m_Elements[3] * m_Elements[3];

    if (Abs(m_Elements[0]) < c_epsilon<T>) // Divide by zero safety check
    {
      // default axis = z
      return Vec<T, 3>(static_cast<T>(0.0), static_cast<T>(0.0),
                       static_cast<T>(1.0));
    }

    const T temp2 = Sqrt(static_cast<T>(1.0) / temp1);
    return Vec<T, 3>(m_Elements[0] * temp2, m_Elements[1] * temp2,
                     m_Elements[2] * temp2);
  }

  // ------------------------------------------------------------------------------

  T GetPitchRad() const
  {
    return ATan2(static_cast<T>(2.0) * (m_Elements[1] * m_Elements[2] +
                                        m_Elements[3] * m_Elements[0]),
                 m_Elements[3] * m_Elements[3] - m_Elements[0] * m_Elements[0] -
                     m_Elements[1] * m_Elements[1] +
                     m_Elements[2] * m_Elements[2]);
  }

  // ------------------------------------------------------------------------------

  T GetYawRad() const
  {
    return ASin(Clamp(static_cast<T>(-2.0) * (m_Elements[0] * m_Elements[2] -
                                              m_Elements[3] * m_Elements[1]),
                      static_cast<T>(-1.0), static_cast<T>(1.0)));
  }

  // ------------------------------------------------------------------------------

  T GetRollRad() const
  {
    return ATan2(static_cast<T>(2.0) * (m_Elements[0] * m_Elements[1] +
                                        m_Elements[3] * m_Elements[2]),
                 m_Elements[3] * m_Elements[3] + m_Elements[0] * m_Elements[0] -
                     m_Elements[1] * m_Elements[1] -
                     m_Elements[2] * m_Elements[2]);
  }

  // ------------------------------------------------------------------------------

  Vec<T, 3> ToEulerAngles() const
  {
    return Vec<T, 3>(GetPitchRad(), GetYawRad(), GetRollRad());
  }

  // ------------------------------------------------------------------------------

  Mat<T, 4> ToMat4() const
  {
    Mat<T, 4> rtn;

    Quat normalized = GetNormalized();

    T xx = normalized.m_Elements[0] * normalized.m_Elements[0];
    T yy = normalized.m_Elements[1] * normalized.m_Elements[1];
    T zz = normalized.m_Elements[2] * normalized.m_Elements[2];
    T ww = normalized.m_Elements[3] * normalized.m_Elements[3];

    rtn[0] = (xx - yy - zz + ww);
    rtn[5] = (-xx + yy - zz + ww);
    rtn[10] = (-xx - yy + zz + ww);

    T tmp1 = normalized.m_Elements[0] * normalized.m_Elements[1];
    T tmp2 = normalized.m_Elements[2] * normalized.m_Elements[3];
    rtn[4] = static_cast<T>(2.0) * (tmp1 + tmp2);
    rtn[1] = static_cast<T>(2.0) * (tmp1 - tmp2);

    tmp1 = normalized.m_Elements[0] * normalized.m_Elements[2];
    tmp2 = normalized.m_Elements[1] * normalized.m_Elements[3];
    rtn[8] = static_cast<T>(2.0) * (tmp1 - tmp2);
    rtn[2] = static_cast<T>(2.0) * (tmp1 + tmp2);

    tmp1 = normalized.m_Elements[1] * normalized.m_Elements[2];
    tmp2 = normalized.m_Elements[0] * normalized.m_Elements[3];
    rtn[1 + 2 * 4] = static_cast<T>(2.0) * (tmp1 + tmp2);
    rtn[2 + 1 * 4] = static_cast<T>(2.0) * (tmp1 - tmp2);
    rtn[3 + 3 * 4] = static_cast<T>(1.0);

    return rtn;
  }

  // ------------------------------------------------------------------------------

  T Dot(const Quat& other) const
  {
    return m_Elements[0] * other.m_Elements[0] +
           m_Elements[1] * other.m_Elements[1] +
           m_Elements[2] * other.m_Elements[2] +
           m_Elements[3] * other.m_Elements[3];
  }

  // ------------------------------------------------------------------------------

  T GetMagnitude() const { return Sqrt(GetSquaredMagnitude()); }

  // ------------------------------------------------------------------------------

  T GetSquaredMagnitude() const
  {
    return m_Elements[0] * m_Elements[0] + m_Elements[1] * m_Elements[1] +
           m_Elements[2] * m_Elements[2] + m_Elements[3] * m_Elements[3];
  }

  // ------------------------------------------------------------------------------

  Quat GetNormalized() const
  {
    const T inverseMagnitude =
        Sqrt(static_cast<T>(1.0) / GetSquaredMagnitude());
    return (*this * inverseMagnitude);
  }

  // ------------------------------------------------------------------------------

  Quat GetConjugated() const
  {
    return Quat(-m_Elements[0], -m_Elements[1], -m_Elements[2], m_Elements[3]);
  }

  // ------------------------------------------------------------------------------

  Quat GetInversed() const
  {
    const Quat conjugated = GetConjugated();
    const T sqrLen = GetSquaredMagnitude();
    return Quat(conjugated / sqrLen);
  }

  // ------------------------------------------------------------------------------

  friend std::ostream& operator<<(std::ostream& outStream, const Quat& quat)
  {
    outStream << "Quat: { ";

    for (auto i = 0; i < 4; i++)
    {
      outStream << quat.m_Elements[i] << ", ";
    }

    outStream << '}';
    return outStream;
  }

  // ------------------------------------------------------------------------------

  // [x, y, z, w]
  // [xyz = axes of rotation, w = cosine of amount of rotation]
  T m_Elements[4];
};

// ------------------------------------------------------------------------------

using Quatf = Quat<float>;
using Quatd = Quat<double>;

} // namespace bge