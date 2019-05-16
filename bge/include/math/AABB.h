#pragma once

#include "Mat.h"

namespace bge
{

/**
 * Class which represents a bounding box using a min and max exent.
 */
class AABB
{
public:
  AABB(Vec3f min, Vec3f max);

  /**
   * Transform the AABB from local coordinates to world coordinates with a world
   * matrix.
   * @param transform The world matrix
   * @return new AABB whose bounding box is in world space
   */
  AABB Transform(const Mat4f& transform) const;

  /**
   * Expands the bounding box in 3 axes
   * @param amt the amount to expand in each axis (can be negative as well)
   * @return new AABB whose bounding box is modified
   */
  AABB Expand(const Vec3f& amt) const;

  /**
   * Moves the AABB by translating it from the centre
   * @param destination the new destination to go to
   * @return new AABB which is located at the destination
   */
  AABB MoveTo(const Vec3f& destination) const;

  /**
   * Check if this bounding box is intersecting with another
   * @param other The other AABB
   * @return flag whether they are intersecting
   */
  bool Intersects(const AABB& other) const;

  /**
   * Check if a point in 3D space is inside the AABB
   * @param point The 3D point
   * @return flag whether the point is inside the AABB
   */
  bool Contains(const Vec3f& point) const;

  /**
   * Check if another AABB is inside the AABB
   * @param other The other boudning box
   * @return flag whether the other AABB is inside this AABB
   */
  bool Contains(const AABB& other) const;

  /**
   * Translated the AABB by a certain amount
   * @param amt the amount to transform in each axis
   * @return new AABB which is translated
   */
  AABB Translate(const Vec3f& amt) const;

  /**
   * Scales the AABB from its center
   * @param amt the amount to scale in each axis
   * @return new AABB which is scaled
   */
  AABB ScaleFromCenter(const Vec3f& amt) const;

  /**
   * Scales the AABB from its origin
   * @param amt the amount to scale in each axis
   * @return new AABB which is scaled
   */
  AABB ScaleFromOrigin(const Vec3f& amt) const;

  /**
   * Adds a point to the AABB, it only modified the bounding box if the point is
   * outside the AABB.
   * @param other the new point
   * @return new AABB which contains the new point
   */
  AABB AddPoint(const Vec3f& other) const;

  /**
   * Adds an AABB to the AABB, it only modified the bounding box if the AABB has
   * contents outside this AABB
   * @param other the other AABB
   * @return new AABB which contains the other AABB
   */
  AABB AddAABB(const AABB& other) const;

  /**
   * Get the center point of the bounding box
   * @return the center of the AABB
   */
  Vec3f GetCenter() const;
  /**
   * Get the exents in each axis
   * @return the extents amount
   */
  Vec3f GetExtents() const;

  bool operator==(const AABB& other) const;
  bool operator!=(const AABB& other) const;

private:
  Vec3f m_MinExtent; /**< The min extent (left-bottom-back) */
  Vec3f m_MaxExtent; /**< The max extent (right-top-front) */
};

} // namespace bge