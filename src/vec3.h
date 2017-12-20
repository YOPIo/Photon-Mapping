#ifndef _VEC3_H_
#define _VEC3_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class Vec3
{
  /* Vec3 constructors */
public:
  Vec3 () = default;
  Vec3 (Float _x, Float _y, Float _z) :
    x (_x), y (_y), z (_z)
  {}

  /* Vec3 destructor */
public:
  virtual ~Vec3 () = default;;


  /* Vec3 public operators*/
public:
  Vec3 (const Vec3&  vec3) = default;
  Vec3 (      Vec3&& vec3) = default;

  auto operator = (const Vec3&  vec3) -> Vec3& = default;
  auto operator = (      Vec3&& vec3) -> Vec3& = default;

  auto operator [] (size_t idx) -> Float&
  {
    if (idx == 0) { return x; }
    if (idx == 1) { return y; }
    return z;
  }

  auto operator [] (size_t idx) const -> Float
  {
    if (idx == 0) { return x; }
    if (idx == 1) { return y; }
    return z;
  }


  /* Vec3 public methods */
  auto SquaredLength () const -> Float
  {
    return std::sqrt (this->Length ());
  }

  auto Length () const -> Float
  {
    return x * x + y * y + z * z;
  }


  /* Vec3f public data */
  union
  {
    struct { Float x, y, z; };
    struct { Float r, g, b; };
  };
}; // class Vec3
/*
// ---------------------------------------------------------------------------
*/
auto operator + (const Vec3& v0, const Vec3& v1) -> Vec3
{
  return Vec3 (v0.x + v1.x, v0.y + v1.y, v1.z + v1.z);
}
/*
// ---------------------------------------------------------------------------
*/
auto operator - (const Vec3& v0, const Vec3& v1) -> Vec3
{
  return Vec3 (v0.x - v1.x, v0.y - v1.y, v0.z - v1.z);
}
/*
// ---------------------------------------------------------------------------
*/
auto operator * (const Vec3& v0, const Vec3& v1) -> Vec3
{
  return Vec3 (v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
}
/*
// ---------------------------------------------------------------------------
*/
auto operator * (const Vec3& v, Float t) -> Vec3
{
  return Vec3 (v.x * t, v.y * t, v.z * t);
}
/*
// ---------------------------------------------------------------------------
*/
auto operator * (Float t, const Vec3& v) -> Vec3
{
  return Vec3 (v.x * t, v.y * t, v.z * t);
}
/*
// ---------------------------------------------------------------------------
*/
auto Normalize (const Vec3& v) -> Vec3
{
  const Float inv (1.0 / v.SquaredLength ());
  return v * inv;
}
/*
// ---------------------------------------------------------------------------
*/
auto Dot (const Vec3& v0, const Vec3& v1) -> Float
{
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}
/*
// ---------------------------------------------------------------------------
*/
auto Cross (const Vec3& v0, const Vec3& v1) -> Vec3
{
  return Vec3 (v0.y * v1.z - v0.z * v1.y,
               v0.z * v1.x - v0.x * v1.z,
               v0.x * v1.y - v0.y * v1.x);
}
/*
// ---------------------------------------------------------------------------
*/
auto BuildOrthoNormalBasis
(
 const Vec3& normal,
       Vec3* tangent,
       Vec3* binormal
)
-> void
{
  if(normal.z < 0.0)
  {
    const Float a (1.0f / (1.0f - normal.z));
    const Float b (normal.x * normal.y * a);
    *tangent  =
        Normalize (Vec3 (1.0f - normal.x * normal.x * a, -b, normal.x));
    *binormal =
        Normalize (Vec3 (b, normal.y * normal.y * a - 1.0f, -normal.y));
  }
  else
  {
    const Float a = 1.0f / (1.0f + normal.z);
    const Float b = -normal.x * normal.y * a;
    *tangent  = Vec3 (1.0f - normal.x * normal.x * a, b, -normal.x);
    *binormal = Vec3 (b, 1.0f - normal.y * normal.y * a, -normal.y);
  }
  *tangent  = *tangent  + Vec3 (0, 0, 0);
  *binormal = *binormal + Vec3 (0, 0, 0);
}
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _VEC3_H_
