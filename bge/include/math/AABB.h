#pragma once

#include "Mat.h"

namespace bge
{

// ------------------------------------------------------------------------------

/////////////////////////////////////////////////
/// AABB used for intersection testing.
/////////////////////////////////////////////////
class AABB
{
public:
  AABB(Vec3f min, Vec3f max);

  // ------------------------------------------------------------------------------

  AABB Transform(const Mat4f& transform) const;

  // ------------------------------------------------------------------------------

  AABB Expand(const Vec3f& amt) const;

  // ------------------------------------------------------------------------------

  AABB MoveTo(const Vec3f& destination) const;

  // ------------------------------------------------------------------------------

  bool Intersects(const AABB& other) const;
  bool Contains(const Vec3f& point) const;
  bool Contains(const AABB& other) const;

  // ------------------------------------------------------------------------------

  AABB Translate(const Vec3f& amt) const;
  AABB ScaleFromCenter(const Vec3f& amt) const;
  AABB ScaleFromOrigin(const Vec3f& amt) const;

  // ------------------------------------------------------------------------------

  AABB AddPoint(const Vec3f& other) const;
  AABB AddAABB(const AABB& other) const;

  // ------------------------------------------------------------------------------

  Vec3f GetCenter() const;
  Vec3f GetExtents() const;

  // ------------------------------------------------------------------------------

  bool operator==(const AABB& other) const;
  bool operator!=(const AABB& other) const;

  // ------------------------------------------------------------------------------

private:
  Vec3f m_MinExtent;
  Vec3f m_MaxExtent;
};

// ------------------------------------------------------------------------------

} // namespace bge