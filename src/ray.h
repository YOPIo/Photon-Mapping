#ifndef _RAY_H_
#define _RAY_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
#include "vec3.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
struct Ray
{
  /* Ray constructors */
  Ray () = default;
  Ray (const Vec3& ori, const Vec3& dir) :
    origin    (ori),
    direction (dir)
  {}
  virtual ~Ray() = default;


  /* Ray operators */
  Ray (const Ray&  ray) = default;
  Ray (      Ray&& ray) = default;

  auto operator = (const Ray& ray) -> Ray& = default;
  auto operator = (Ray&& ray)      -> Ray& = default;

  auto operator () (Float t) -> Vec3
  {
    return origin + t * direction;
  }

  /* Ray data */
  Vec3 origin;
  Vec3 direction;
};
/*
// ---------------------------------------------------------------------------
*/
struct PhotonRay : public Ray
{
  /* PhotonRay constructors */
  PhotonRay () = default;
  PhotonRay
  (
   const Vec3& ori,
   const Vec3& dir,
   const Vec3& f
  ) :
    Ray      (ori, dir),
    flux     (f)
  {}


  /* PhotonRay destructor */
  virtual ~PhotonRay () = default;


  /* PhotonRay operators*/
  PhotonRay (const PhotonRay&  ray) = default;
  PhotonRay (      PhotonRay&& ray) = default;

  auto operator = (const PhotonRay&  ray) -> PhotonRay& = default;
  auto operator = (      PhotonRay&& ray) -> PhotonRay& = default;


  /* PhotonRay data */
  Vec3 flux;     // Power

}; // class PhotonRay
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _RAY_H_
