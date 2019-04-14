#pragma once

#include "Vec.h"

namespace bge
{

// ------------------------------------------------------------------------------

/////////////////////////////////////////////////
/// Matrix class which is templated to support a 3x3 and 4x4 matrix.
/////////////////////////////////////////////////
template <typename T, uint32 Size> class Mat
{
public:
  Mat() { memset(m_Elements, 0, Size * Size * sizeof(T)); }

  // ------------------------------------------------------------------------------

  explicit Mat(T diagonal)
  {
    memset(m_Elements, 0, Size * Size * sizeof(T));

    for (uint32 i = 0; i < Size * Size; i += Size + 1)
    {
      m_Elements[i] = diagonal;
    }
  }

  // ------------------------------------------------------------------------------

  explicit Mat(T* elements)
  {
    memcpy(m_Elements, elements, Size * Size * sizeof(T));
  }

  // ------------------------------------------------------------------------------

  Mat operator+(const Mat& rhs) const
  {
    Mat rtn;

    for (auto i = 0; i < Size * Size; i++)
    {
      rtn[i] = m_Elements[i] + rhs.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Mat operator-(const Mat& rhs) const
  {
    Mat rtn;

    for (auto i = 0; i < Size * Size; i++)
    {
      rtn[i] = m_Elements[i] - rhs.m_Elements[i];
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Mat operator*(const Mat& rhs) const
  {
    Mat rtn;

    for (uint32 row = 0; row < Size; row++)
    {
      for (uint32 col = 0; col < Size; col++)
      {
        T sum = 0.0f;

        for (uint32 e = 0; e < Size; e++)
        {
          sum += m_Elements[e + row * Size] * rhs.m_Elements[col + e * Size];
        }

        rtn.m_Elements[col + row * Size] = sum;
      }
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Mat operator*(T scalar) const
  {
    Mat rtn;

    for (auto i = 0; i < Size * Size; i++)
    {
      rtn[i] = m_Elements[i] * scalar;
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Mat operator/(T scalar) const
  {
    Mat rtn;

    for (auto i = 0; i < Size * Size; i++)
    {
      rtn[i] = m_Elements[i] / scalar;
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Vec<T, Size> operator*(const Vec<T, Size>& vec) const
  {
    Vec<T, Size> rtn;

    for (uint32 row = 0; row < Size; row++)
    {
      for (uint32 col = 0; col < Size; col++)
      {
        rtn[row] += m_Elements[col + row * Size] * vec[col];
      }
    }

    return rtn;
  }

  // ------------------------------------------------------------------------------

  Mat& operator+=(const Mat& rhs)
  {
    *this = *this + rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Mat& operator-=(const Mat& rhs)
  {
    *this = *this - rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Mat& operator*=(const Mat& rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Mat& operator*=(T scalar)
  {
    *this = *this * scalar;
    return *this;
  }

  // ------------------------------------------------------------------------------

  Mat& operator/=(T scalar)
  {
    *this = *this / scalar;
    return *this;
  }

  // ------------------------------------------------------------------------------

  T& operator[](uint32 index)
  {
    assert(index < Size * Size);
    return m_Elements[index];
  }

  // ------------------------------------------------------------------------------

  T operator[](uint32 index) const
  {
    assert(index < Size * Size);
    return m_Elements[index];
  }

  // ------------------------------------------------------------------------------

  bool operator==(const Mat& rhs) const
  {
    bool allElementsAreEqual = true;

    for (auto i = 0; i < Size * Size; i++)
    {
      allElementsAreEqual &=
          Equals(m_Elements[i], rhs.m_Elements[i], c_epsilon<T>);
    }

    return allElementsAreEqual;
  }

  // ------------------------------------------------------------------------------

  bool operator!=(const Mat& rhs) const { return !operator==(rhs); }

  // ------------------------------------------------------------------------------

  friend std::ostream& operator<<(std::ostream& outStream, const Mat& mat)
  {
    outStream << "Mat:\n";

    for (auto row = 0; row < Size; row++)
    {
      outStream << "Row " << row << '\t';
      outStream << "{ ";

      for (auto col = 0; col < Size; col++)
      {
        outStream << mat[col + row * Size] << ", ";
      }

      outStream << '}' << '\n';
    }

    return outStream;
  }

  // ------------------------------------------------------------------------------

private:
  // [row][col] access = col + row * Size
  T m_Elements[Size * Size];
};

// ------------------------------------------------------------------------------

template <typename T> Mat<T, 3> GetTransposedMat(const Mat<T, 3>& mat)
{
  using std::swap;

  Mat<T, 3> rtn(mat);

  swap(rtn.m_Elements[1], rtn.m_Elements[3]);
  swap(rtn.m_Elements[2], rtn.m_Elements[6]);
  swap(rtn.m_Elements[5], rtn.m_Elements[7]);

  return rtn;
}

// ------------------------------------------------------------------------------

template <typename T> Mat<T, 4> GetTransposedMat(const Mat<T, 4>& mat)
{
  using std::swap;

  Mat<T, 4> rtn(mat);

  swap(rtn[1], rtn[4]);
  swap(rtn[2], rtn[8]);
  swap(rtn[6], rtn[9]);
  swap(rtn[3], rtn[12]);
  swap(rtn[7], rtn[13]);
  swap(rtn[11], rtn[14]);

  return rtn;
}

// ------------------------------------------------------------------------------

template <typename T> Mat<T, 4> GenTranslationMat(const Vec<T, 3>& translation)
{
  T matrixData[16] = {
      static_cast<T>(1.0), static_cast<T>(0.0), static_cast<T>(0.0),
      translation[0],      static_cast<T>(0.0), static_cast<T>(1.0),
      static_cast<T>(0.0), translation[1],      static_cast<T>(0.0),
      static_cast<T>(0.0), static_cast<T>(1.0), translation[2],
      static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0),
      static_cast<T>(1.0)};

  return Mat<T, 4>(matrixData);
}

// ------------------------------------------------------------------------------

template <typename T> Mat<T, 4> GenRotationMat(T radians, const Vec<T, 3>& axis)
{
  const T c = Cos(radians);
  const T s = Sin(radians);

  const Vec<T, 3> normalizedAxis = axis.GetNormalized();

  const T omc = static_cast<T>(1.0) - c;

  Mat<T, 4> rtn;

  rtn[0 + 0 * 4] = normalizedAxis[0] * normalizedAxis[0] * omc + c;

  rtn[0 + 1 * 4] =
      normalizedAxis[1] * normalizedAxis[0] * omc + normalizedAxis[2] * s;

  rtn[0 + 2 * 4] =
      normalizedAxis[0] * normalizedAxis[2] * omc - normalizedAxis[1] * s;

  rtn[1 + 0 * 4] =
      normalizedAxis[0] * normalizedAxis[1] * omc - normalizedAxis[2] * s;

  rtn[1 + 1 * 4] = normalizedAxis[1] * normalizedAxis[1] * omc + c;

  rtn[1 + 2 * 4] =
      normalizedAxis[1] * normalizedAxis[2] * omc + normalizedAxis[0] * s;

  rtn[2 + 0 * 4] =
      normalizedAxis[0] * normalizedAxis[2] * omc + normalizedAxis[1] * s;

  rtn[2 + 1 * 4] =
      normalizedAxis[1] * normalizedAxis[2] * omc - normalizedAxis[0] * s;

  rtn[2 + 2 * 4] = normalizedAxis[2] * normalizedAxis[2] * omc + c;

  rtn[3 + 3 * 4] = static_cast<T>(1.0);

  return rtn;
}

// ------------------------------------------------------------------------------

template <typename T> Mat<T, 4> GenScalingMat(const Vec<T, 3>& scale)
{
  T matrixData[16] = {scale[0],
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      scale[1],
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      scale[2],
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(0.0),
                      static_cast<T>(1.0)};

  return Mat<T, 4>(matrixData);
}

// ------------------------------------------------------------------------------

template <typename T>
Mat<T, 4> GenOrthoMat(T left, T rightAxis, T bottom, T top, T near, T far)
{
  Mat<T, 4> rtn;

  rtn[0 + 0 * 4] = static_cast<T>(2.0) / (rightAxis - left);
  rtn[1 + 1 * 4] = static_cast<T>(2.0) / (top - bottom);

  rtn[3 + 0 * 4] = -(rightAxis + left) / (rightAxis - left);
  rtn[3 + 1 * 4] = -(top + bottom) / (top - bottom);

  rtn[2 + 2 * 4] = -static_cast<T>(2.0) / (far - near);
  rtn[3 + 2 * 4] = -(far + near) / (far - near);

  return rtn;
}

// ------------------------------------------------------------------------------

template <typename T>
constexpr Mat<T, 4> GenPerspectiveMat(T FovRadians, T aspectRatio, T near,
                                      T far)
{
  Mat<T, 4> rtn;

  const T tanHalfFoV = Tan(FovRadians * static_cast<T>(0.5));

  rtn[0] = static_cast<T>(1.0) / (aspectRatio * tanHalfFoV);
  rtn[1 + 1 * 4] = static_cast<T>(1.0) / (tanHalfFoV);
  rtn[2 + 3 * 4] = static_cast<T>(-1.0);

  rtn[2 + 2 * 4] = -(far + near) / (far - near);
  rtn[3 + 2 * 4] = -(static_cast<T>(2.0) * far * near) / (far - near);

  return rtn;
}

// ------------------------------------------------------------------------------

template <typename T>
Mat<T, 4> GenLookAtMat(const Vec<T, 3>& camera, const Vec<T, 3>& object,
                       const Vec<T, 3>& up)
{
  Mat<T, 4> rtn(static_cast<T>(1.0));

  const Vec<T, 3> forwardAxis = (object - camera).GetNormalized();
  const Vec<T, 3> rightAxis =
      Cross(forwardAxis, up.GetNormalized()).GetNormalized();
  const Vec<T, 3> upAxis = Cross(rightAxis, forwardAxis);

  rtn[0 + 0 * 4] = rightAxis[0];
  rtn[1 + 0 * 4] = rightAxis[1];
  rtn[2 + 0 * 4] = rightAxis[2];
  rtn[0 + 1 * 4] = upAxis[0];
  rtn[1 + 1 * 4] = upAxis[1];
  rtn[2 + 1 * 4] = upAxis[2];
  rtn[0 + 2 * 4] = -forwardAxis[0];
  rtn[1 + 2 * 4] = -forwardAxis[1];
  rtn[2 + 2 * 4] = -forwardAxis[2];
  rtn[3 + 0 * 4] = -rightAxis.Dot(camera);
  rtn[3 + 1 * 4] = -upAxis.Dot(camera);
  rtn[3 + 2 * 4] = forwardAxis.Dot(camera);

  return rtn;
}

// ------------------------------------------------------------------------------

using Mat3f = Mat<float, 3>;
using Mat3d = Mat<double, 3>;
using Mat4f = Mat<float, 4>;
using Mat4d = Mat<double, 4>;

// ------------------------------------------------------------------------------

} // namespace bge