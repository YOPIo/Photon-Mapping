#ifndef _SURFACE_INTERSECTION_INFO_H_
#define _SURFACE_INTERSECTION_INFO_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
#include "ray.h"
#include "vec3.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
struct SurfaceIntersectionInfo
{
  /* SurfaceIntersectionInfo constructors */
  SurfaceIntersectionInfo () :
    position (),
    oriented_normal (),
    outgoing (),
    t (kFloatMax)
  {}


  /* SurfaceIntersectionInfo destructor */
  virtual ~SurfaceIntersectionInfo () = default;


  /* SurfaceIntersectionInfo operators*/
  SurfaceIntersectionInfo (const SurfaceIntersectionInfo&  info) = default;
  SurfaceIntersectionInfo (      SurfaceIntersectionInfo&& info) = default;

  auto operator = (const SurfaceIntersectionInfo&  info)
    -> SurfaceIntersectionInfo& = default;
  auto operator = (      SurfaceIntersectionInfo&& info)
    -> SurfaceIntersectionInfo& = default;


  /* SurfaceIntersectionInfo data */
  Vec3   position;        // Intersected position
  Vec3   oriented_normal; //
  Vec3   outgoing;        // Ray direction
  Float  t;               // Parameter 't' to compute the intersected position
}; // class SurfaceIntersectionInfo
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _SURFACE_INTERSECTION_INFO_H_
