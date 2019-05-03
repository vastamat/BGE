#pragma once

#include "Quat.h"

namespace bge
{

/////////////////////////////////////////////////
/// Transform class is used to describe a transform matrix with a position,
/// rotation and scale.
/////////////////////////////////////////////////
class Transform
{
public:
  Transform();
  Transform(const Vec3f& translation, const Vec3f& scale,
            const Quatf& rotation);

  void Rotate(const Quatf& amountToRotation);
  void Translate(const Vec3f& translation);
  void Scale(const Vec3f& scalingToAdd);

  void SetRotation(const Quatf& rotation);
  void SetTranslation(const Vec3f& translation);
  void SetScale(const Vec3f& scale);

  const Quatf& GetRotation() const;
  const Vec3f& GetTranslation() const;
  const Vec3f& GetScale() const;

  Vec3f GetRightAxis() const;
  Vec3f GetUpAxis() const;
  Vec3f GetForwardAxis() const;

  Mat4f ToMatrix() const;

private:
  Quatf m_Rotation;
  Vec3f m_Translation;
  Vec3f m_Scale;
};

} // namespace bge