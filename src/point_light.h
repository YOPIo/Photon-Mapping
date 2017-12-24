#ifndef _POINT_LIGHT_
#define _POINT_LIGHT_
/*
// ---------------------------------------------------------------------------
*/
#include "vec3.h"
#include "ray.h"
#include "random.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class PointLight
{
  /* PointLight constructors */
public:
  PointLight () = default;
  PointLight
  (
   const Vec3& position,
   const Vec3& emission
  ) :
    position_ (position),
    emission_ (emission)
  {}


  /* PointLight destructor */
public:
  virtual ~PointLight () = default;


  /* PointLight public operators*/
public:
  PointLight (const PointLight&  light) = default;
  PointLight (      PointLight&& light) = default;

  auto operator = (const PointLight&  light) -> PointLight& = default;
  auto operator = (      PointLight&& light) -> PointLight& = default;


  /* PointLight public methods */
public:
  auto GeneratePhotonRay (PhotonRay* ray) const -> void
  {
    // Sample a point on the unit sphere
    const Float theta (2.0 * kPi * XorShift::Next01 ());
    const Float phi   (kPi * XorShift::Next01 ());

    // Compute direction
    const Vec3 dir (std::sin (phi) * std::cos (theta),
                    std::sin (phi) * std::sin (theta),
                    std::cos (phi));

    // Store photon ray
    *ray = PhotonRay (position_, dir, emission_);
  }


  /* PointLight private data */
private:
  const Vec3 position_;
  const Vec3 emission_;

}; // class PointLight
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _POINT_LIGHT_
