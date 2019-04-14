#include "math/AABB.h"

#include "math/MathUtils.h"

namespace bge
{

// ------------------------------------------------------------------------------

AABB::AABB(Vec3f min, Vec3f max)
    : m_MinExtent(min)
    , m_MaxExtent(max)
{
}

// ------------------------------------------------------------------------------

// Transform all 8 points of the AABB and choose new min/max
AABB AABB::Transform(const Mat4f& transform) const
{
  // Get the 8 points from the min and max extents
  // m_MinExtent = bottom left behind
  // m_MaxExtent = top right front

  Vec4f bottomLeftBehind =
      Vec4f(m_MinExtent[0], m_MinExtent[1], m_MinExtent[2], 1.0f);

  Vec4f topRightFront =
      Vec4f(m_MaxExtent[0], m_MaxExtent[1], m_MaxExtent[2], 1.0f);

  Vec4f bottomRightBehind =
      Vec4f(m_MaxExtent[0], m_MinExtent[1], m_MinExtent[2], 1.0f);

  Vec4f bottomRightFront =
      Vec4f(m_MaxExtent[0], m_MinExtent[1], m_MaxExtent[2], 1.0f);

  Vec4f TopRightBehind =
      Vec4f(m_MaxExtent[0], m_MaxExtent[1], m_MinExtent[2], 1.0f);

  Vec4f TopLeftBehind =
      Vec4f(m_MinExtent[0], m_MaxExtent[1], m_MinExtent[2], 1.0f);

  Vec4f TopLeftFront =
      Vec4f(m_MinExtent[0], m_MaxExtent[1], m_MaxExtent[2], 1.0f);

  Vec4f bottomLeftFront =
      Vec4f(m_MinExtent[0], m_MinExtent[1], m_MaxExtent[2], 1.0f);

  // Now transform all points

  bottomLeftBehind = transform * bottomLeftBehind;
  topRightFront = transform * topRightFront;
  bottomRightBehind = transform * bottomRightBehind;
  bottomRightFront = transform * bottomRightFront;
  TopRightBehind = transform * TopRightBehind;
  TopLeftBehind = transform * TopLeftBehind;
  TopLeftFront = transform * TopLeftFront;
  bottomLeftFront = transform * bottomLeftFront;

  // Recalc the min/max points and construct new AABB
  Vec4f min = GetMinValues(
      GetMinValues(
          GetMinValues(
              GetMinValues(
                  GetMinValues(GetMinValues(GetMinValues(bottomLeftBehind,
                                                         topRightFront),
                                            bottomRightBehind),
                               bottomRightFront),
                  TopRightBehind),
              TopLeftBehind),
          TopLeftFront),
      bottomLeftFront);

  Vec4f max = GetMaxValues(
      GetMaxValues(
          GetMaxValues(
              GetMaxValues(
                  GetMaxValues(GetMaxValues(GetMaxValues(bottomLeftBehind,
                                                         topRightFront),
                                            bottomRightBehind),
                               bottomRightFront),
                  TopRightBehind),
              TopLeftBehind),
          TopLeftFront),
      bottomLeftFront);

  return AABB(Vec3f(min[0], min[1], min[2]), Vec3f(max[0], max[1], max[2]));
}

// ------------------------------------------------------------------------------

AABB AABB::Expand(const Vec3f& amt) const
{
  return AABB(m_MinExtent - amt, m_MaxExtent + amt);
}

// ------------------------------------------------------------------------------

AABB AABB::MoveTo(const Vec3f& destination) const
{
  return Translate(destination - GetCenter());
}

// ------------------------------------------------------------------------------

bool AABB::Intersects(const AABB& other) const
{
  return (m_MaxExtent >= other.m_MinExtent) &&
         (m_MinExtent <= other.m_MaxExtent);
}

// ------------------------------------------------------------------------------

bool AABB::Contains(const Vec3f& point) const
{
  return (point >= m_MinExtent) && (point <= m_MaxExtent);
}

// ------------------------------------------------------------------------------

bool AABB::Contains(const AABB& other) const
{
  return (other.m_MinExtent >= m_MinExtent) &&
         (other.m_MaxExtent <= m_MaxExtent);
}

// ------------------------------------------------------------------------------

AABB AABB::Translate(const Vec3f& amt) const
{
  return AABB(m_MinExtent + amt, m_MaxExtent + amt);
}

// ------------------------------------------------------------------------------

AABB AABB::ScaleFromCenter(const Vec3f& amt) const
{
  const Vec3f center = GetCenter();
  const Vec3f scaledExtents = GetExtents() * amt;
  return AABB(center - scaledExtents, center + scaledExtents);
}

// ------------------------------------------------------------------------------

AABB AABB::ScaleFromOrigin(const Vec3f& amt) const
{
  return AABB(m_MinExtent * amt, m_MaxExtent * amt);
}

// ------------------------------------------------------------------------------

AABB AABB::AddPoint(const Vec3f& other) const
{
  return AABB(GetMinValues(m_MinExtent, other),
              GetMaxValues(m_MaxExtent, other));
}

// ------------------------------------------------------------------------------

AABB AABB::AddAABB(const AABB& other) const
{
  return AABB(GetMinValues(m_MinExtent, other.m_MinExtent),
              GetMaxValues(m_MaxExtent, other.m_MaxExtent));
}

// ------------------------------------------------------------------------------

Vec3f AABB::GetCenter() const { return (m_MinExtent + m_MaxExtent) * 0.5f; }

// ------------------------------------------------------------------------------

Vec3f AABB::GetExtents() const { return (m_MaxExtent - m_MinExtent) * 0.5f; }

// ------------------------------------------------------------------------------

bool AABB::operator==(const AABB& other) const
{
  return m_MinExtent == other.m_MinExtent && m_MaxExtent == other.m_MaxExtent;
}

// ------------------------------------------------------------------------------

bool AABB::operator!=(const AABB& other) const
{
  return m_MinExtent != other.m_MinExtent && m_MaxExtent != other.m_MaxExtent;
}

// ------------------------------------------------------------------------------

} // namespace bge