#ifndef _SPHERE_H_
#define _SPHERE_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
#include "ray.h"
#include "vec3.h"
#include "surface_intersection_info.h"
#include "material.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class Sphere
{
  /* Sphere constructors */
public:
  Sphere () = delete;
  Sphere
  (
   const Vec3&  center,
         Float  radius
  ) :
    center_      (center),
    radius_      (radius)
  {}

  /* Sphere destructor */
public:
  virtual ~Sphere () = default;


  /* Sphere public operators*/
public:
  Sphere (const Sphere&  sphere) = default;
  Sphere (      Sphere&& sphere) = default;

  auto operator = (const Sphere&  sphere) -> Sphere& = default;
  auto operator = (      Sphere&& sphere) -> Sphere& = default;


  /* Sphere public methods */
  auto IsIntersect
  (
   const Ray& ray,
   SurfaceIntersectionInfo* info
  )
  -> bool
  {
    // Description
    // If ray and this sphere intersect somewhere, then return true

    const Float kSphereEpsilon = 1e-5;

    const Vec3   tmp (center_ - ray.origin);
    const Float b (Dot (tmp, ray.direction));
    const Float c (b * b - Dot (tmp, tmp) + radius_ * radius_);

    // No intersection
    if (c < 0.0) { return false; }

    const Float sqrt_c (std::sqrt (c));
    const Float t1 (b - sqrt_c);
    const Float t2 (b + sqrt_c);
    // No intersection
    if (t1 < kSphereEpsilon && t2 < kSphereEpsilon) { return false; }

    // Intersected
    // Compute position where ray intersected with sphere, normal
    auto lambda = [&ray] (Float t) -> Vec3
    {
      return ray.origin + ray.direction * t;
    };

    // Initialize surface intersection info
    info->position        = t1 > kSphereEpsilon ? lambda (t1) : lambda (t2);
    info->oriented_normal = Normalize (info->position - center_);
    info->outgoing        = -1.0 * ray.direction;
    info->t               = t1 > kSphereEpsilon ? t1 : t2;

    return true;
  }


  /* Sphere private data */
private:
  const Vec3  center_;
  const Float radius_;
}; // class Sphere
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _SPHERE_H_
