#ifndef _SCENE_H_
#define _SCENE_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
#include "ray.h"
#include "point_light.h"
#include "sphere.h"
#include "material.h"
#include "primitive.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class Scene
{
  /* Scene constructors */
public:
  Scene () = default;
  Scene
  (
   const std::vector <Primitive>&  primitives,
   const std::vector <PointLight>& lights
  ) :
    primitives_ (primitives),
    lights_     (lights)
  {}


  /* Scene destructor */
public:
  virtual ~Scene () = default;


  /* Scene public operators*/
public:
  Scene (const Scene&  scene) = default;
  Scene (      Scene&& scene) = default;

  auto operator = (const Scene&  scene) -> Scene& = default;
  auto operator = (      Scene&& scene) -> Scene& = default;


  /* Scene public method */
public:
  auto IsIntersect (const Ray& ray, SurfaceIntersectionInfo* info) const -> bool
  {
    bool intersect = false;

    // Intersection test
    for (const auto& p : primitives_)
    {
      SurfaceIntersectionInfo tmp;
      if (p.IsIntersect (ray, &tmp))
      {
        if (tmp.t < info->t)
        {
          *info          = tmp;
          info->sphere   = p.GetSphere ();
          info->material = p.GetMaterial ();
          intersect = true;
        }
      }
    }
    return intersect;
  }

  auto EmitPhotonRay (PhotonRay* ray) const -> void
  {
    // TODO: Choose a light in randomly if there are some lights in the scene
    lights_[0].GeneratePhotonRay (ray);
  }


  /* Scene private data */
private:
  std::vector <Primitive>  primitives_;
  std::vector <PointLight> lights_;

}; // class Scene
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _SCENE_H_
