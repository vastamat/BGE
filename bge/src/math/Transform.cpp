#include "math/Transform.h"

namespace bge
{

// ------------------------------------------------------------------------------

Transform::Transform()
    : m_Rotation()
    , m_Translation()
    , m_Scale(1.0f)
{
}

// ------------------------------------------------------------------------------

void Transform::Rotate(const Quatf& amountToRotation)
{
  m_Rotation = amountToRotation * m_Rotation;
}

// ------------------------------------------------------------------------------

void Transform::Translate(const Vec3f& translation)
{
  m_Translation += translation;
}

// ------------------------------------------------------------------------------

void Transform::Scale(const Vec3f& scalingToAdd) { m_Scale += scalingToAdd; }

// ------------------------------------------------------------------------------

void Transform::SetRotation(const Quatf& rotation) { m_Rotation = rotation; }

// ------------------------------------------------------------------------------

void Transform::SetTranslation(const Vec3f& translation)
{
  m_Translation = translation;
}

// ------------------------------------------------------------------------------

void Transform::SetScale(const Vec3f& scale) { m_Scale = scale; }

// ------------------------------------------------------------------------------

const Quatf& Transform::GetRotation() const { return m_Rotation; }

// ------------------------------------------------------------------------------

const Vec3f& Transform::GetTranslation() const { return m_Translation; }

// ------------------------------------------------------------------------------

const Vec3f& Transform::GetScale() const { return m_Scale; }

// ------------------------------------------------------------------------------

Vec3f Transform::GetRightAxis() const
{
  return Quatf::RotateVec(m_Rotation, Vec3f(1.0f, 0.0f, 0.0f));
}

// ------------------------------------------------------------------------------

Vec3f Transform::GetUpAxis() const
{
  return Quatf::RotateVec(m_Rotation, Vec3f(0.0f, 1.0f, 0.0f));
}

// ------------------------------------------------------------------------------

Vec3f Transform::GetForwardAxis() const
{
  return Quatf::RotateVec(m_Rotation, Vec3f(0.0f, 0.0f, -1.0f));
}

// ------------------------------------------------------------------------------

Mat4f Transform::ToMatrix() const
{
  return GenTranslationMat(m_Translation) * m_Rotation.ToMat4() *
         GenScalingMat(m_Scale);
}

// ------------------------------------------------------------------------------

} // namespace bge