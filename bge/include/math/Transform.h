#pragma once

#include "Quat.h"

namespace bge
{

/**
 * Transform class is used to describe a transform matrix with a position,
 * rotation and scale.
 */
class Transform
{
public:
  Transform();
  Transform(const Vec3f& translation, const Vec3f& scale,
            const Quatf& rotation);

  /**
   * Rotates the orientation by set amount
   * @param amountToRotation rotation to append
   */
  void Rotate(const Quatf& amountToRotation);

  /**
   * Translates the translation by set amount
   * @param translation amount to translate
   */
  void Translate(const Vec3f& translation);

  /**
   * Scales the scale by set amount
   * @param scalingToAdd amount to add to the scale
   */
  void Scale(const Vec3f& scalingToAdd);

  /**
   * Sets the rotation to set value
   * @param rotation new rotation value
   */
  void SetRotation(const Quatf& rotation);

  /**
   * Sets the translation to set value
   * @param translation new translation value
   */
  void SetTranslation(const Vec3f& translation);

  /**
   * Sets the scale to set value
   * @param scale new scale value
   */
  void SetScale(const Vec3f& scale);

  // Getters
  const Quatf& GetRotation() const;
  const Vec3f& GetTranslation() const;
  const Vec3f& GetScale() const;

  Vec3f GetRightAxis() const;
  Vec3f GetUpAxis() const;
  Vec3f GetForwardAxis() const;

  /**
   * Calculates the matrix representation of the transform
   * @return the 4x4 matrix which matches this transform.
   */
  Mat4f ToMatrix() const;

private:
  Quatf m_Rotation;    /**< The orientation of the transform */
  Vec3f m_Translation; /**< The translation of the transform */
  Vec3f m_Scale;       /**< The scale of the transform */
};

} // namespace bge