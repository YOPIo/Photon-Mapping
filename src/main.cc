#include "core.h"
#include "vec3.h"
#include "surface_intersection_info.h"
#include "scene.h"
#include "sphere.h"
#include "ray.h"
#include "random.h"
#include "point_light.h"
#include "material.h"
#include "primitive.h"
#ifdef _OPENMP
#include <omp.h>
#endif
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
// Save .ppm image
// ---------------------------------------------------------------------------
*/
auto SavePpm (const char* filename, const Vec3* image) -> void
{
  auto to_int = [] (Float val)
  {
    if (val < 0.0) { val = 0; }
    if (val > 1.0) { val = 1; }
    return static_cast<int> (std::pow (val, 1 / 2.2) * 255 + 0.5);
  };

  FILE* f = fopen (filename, "wb");
  fprintf(f, "P3\n%d %d\n%d\n", kWidth, kHeight, 255);
  for (int i = 0; i < kWidth * kHeight; i++)
  {
    fprintf(f,"%d %d %d ", to_int (image[i].x),
                           to_int (image[i].y),
                           to_int (image[i].z));
  }
  fclose(f);
}
/*
// ---------------------------------------------------------------------------
*/
auto MatteReflect (const Vec3& normal) -> Vec3
{
  // Trace photon ray from intersected position
  Vec3 tangent, binormal;
  BuildOrthoNormalBasis (normal, &tangent, &binormal);

  const Float phi (Random::Sample1d () * 2.0 * kPi);
  const Float r2  (Random::Sample1d ());
  const Float r2s (std::sqrt (r2));

  const Float tx (r2s * std::cos (phi));
  const Float ty (r2s * std::sin (phi));
  const Float tz (sqrt(1.0 - r2));

  return tz * normal + tx * tangent + ty * binormal;
}
/*
// ---------------------------------------------------------------------------
*/
auto MirrorReflect (const Vec3& w, const Vec3& normal) -> Vec3
{
  // Perform as mirror reflection
  return 2.0 * (Dot (w, normal)) * normal - w;
}

/*
// ---------------------------------------------------------------------------
*/
auto EmitPhoton
(
 const Scene&     scene,
       PhotonMap* photon_map
)
-> void
{
  // Emit photon ray from light in the scene
  PhotonRay ray;
  scene.EmitPhotonRay (&ray);

  while (1)
  {
    // Intersection test
    SurfaceIntersectionInfo info;
    if (scene.IsIntersect (ray, &info))
    {
      // Get material
      const std::shared_ptr <Material> material (info.material.lock ());

      // Get material type
      MaterialType type (material->GetType ());

      if (type == MaterialType::kMatte)
      {
        // Ray intersected with matte surface
        // Photon is going to be stored to photon map
        photon_map->StorePhotonRayAsPhoton (PhotonRay (info.position,
                                                       info.outgoing,
                                                       ray.flux));

        // Decide to reflect by russian roullet
        const Float p (material->GetReflectance ().g);
        const float s (Random::Sample1d ());
        if (s < p)
        {
          ray = PhotonRay (info.position,
                           MatteReflect (info.oriented_normal),
                           ray.flux);
          continue;
        }

        break;
      }
      if (type == MaterialType::kMirror)
      {
        // Ray intersected with mirror surface
        // Ray is going to be reflected as mirror reflection
        // Before ray reflected, it is necessary to evaluated to trace next ray
        // or not by russian roullet.

        // Apply russian roullet
        const Float p (material->GetReflectance ().g);
        const Float s (Random::Sample1d ());
        if (s < p)
        {
          // Photon reflects at the intersected position
          ray = PhotonRay (info.position,
                           MirrorReflect (info.outgoing, info.oriented_normal),
                           ray.flux);
          continue;
        }
        break;
      }
      break;
    }
  } // End of while
}
/*
// ---------------------------------------------------------------------------
*/
auto PhotonTracing (const Scene& scene, PhotonMap* photon_map) -> void
{
  // Emit photons
  for (int i = 0; i < kNumPhotons; ++i)
  {
    EmitPhoton (scene, photon_map);
  } // End of for
}
/*
// ---------------------------------------------------------------------------
*/
auto Render () -> void
{
  // Write here!
}
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
int main (int argc, char *argv[])
{
  // Scene construction
  std::vector <Primitive> primitives =
  {
    Primitive (std::make_shared <Sphere> (Vec3 ( 1e5 + 1,   40.8,       81.6 ), 1e5  ), std::make_shared <Matte> (Vec3 ( 0.99, 0.01, 0.01 ))),
    Primitive (std::make_shared <Sphere> (Vec3 (-1e5 + 99,  40.8,       81.6 ), 1e5  ), std::make_shared <Matte> (Vec3 ( 0.01, 0.01, 0.99 ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 50,        40.8,        1e5 ), 1e5  ), std::make_shared <Matte> (Vec3 ( 0.75, 0.75, 0.75 ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 50,        40.8, -1e5 + 170 ), 1e5  ), std::make_shared <Matte> (Vec3 ( 0.0,  0.0,  0.0  ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 50,         1e5,       81.6 ), 1e5  ), std::make_shared <Matte> (Vec3 ( 0.75, 0.75, 0.75 ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 50, -1e5 + 81.6,       81.6 ), 1e5  ), std::make_shared <Matte> (Vec3 ( 0.75, 0.75, 0.75 ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 27,        16.5,         47 ), 16.5 ), std::make_shared <Matte> (Vec3 ( 0.25, 0.85, 0.25 ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 73,        16.5,         88 ), 16.5 ), std::make_shared <Matte> (Vec3 ( 0.99, 0.99, 0.99 ))),
    Primitive (std::make_shared <Sphere> (Vec3 ( 50,         8.5,         60 ), 8.5  ), std::make_shared <Matte> (Vec3 ( 0.75, 0.75, 0.75 ))),
  };
  std::vector <PointLight> lights = {PointLight (Vec3 (50, 60, 85), Vec3 (500, 500, 500))};

  const Scene scene (primitives, lights);
  PhotonMap photon_map (kNumPhotons);

  // Begin photon tracing
  PhotonTracing (scene, &photon_map);

  photon_map.Balance ();


  return 0;
}

