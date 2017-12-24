#include "camera.h"
#include "core.h"
#include "vec3.h"
#include "surface_intersection_info.h"
#include "sphere.h"
#include "ray.h"
#include "random.h"
#include "point_light.h"
#include "photonmap.h"
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
*/
const std::vector <Sphere> scene =
{
  // Position, radius, emission, reflectance, material type
  Sphere (Vec3 ( 1e5 + 1,         40.8, 81.6),         1e5,  Vec3 (),         Vec3 (0.75, 0.25, 0.25), kMatte), // left
  Sphere (Vec3 (-1e5 + 99,        40.8, 81.6),         1e5,  Vec3 (),         Vec3 (0.25, 0.25, 0.75), kMatte), // right
  Sphere (Vec3 (     50.0,        40.8, 1e5),          1e5,  Vec3 (),         Vec3 (0.75, 0.75, 0.75), kMatte), // back
  Sphere (Vec3 (     50.0,        40.8, -1e5 + 250.0), 1e5,  Vec3 (),         Vec3 (),                 kMatte), // front
  Sphere (Vec3 (     50.0,         1e5, 81.6),         1e5,  Vec3 (),         Vec3 (0.75, 0.75, 0.75), kMatte), // floor
  Sphere (Vec3 (     50.0, -1e5 + 81.6, 81.6),         1e5,  Vec3 (),         Vec3 (0.75, 0.75, 0.75), kMatte), // ceiling
  // Sphere (Vec3 (     65.0,        20.0, 20),           20,   Vec3 (),         Vec3 (0.25, 0.75, 0.25), kMatte), // green
  // Sphere (Vec3 (     27.0,        16.5, 47),           16.5, Vec3 (),         Vec3 (0.99, 0.99, 0.99), kMatte), // mir
  // Sphere (Vec3 (     77.0,        16.5, 78),           16.5, Vec3 (),         Vec3 (0.99, 0.99, 0.99), kMatte), //glass
  // Sphere (Vec3 (     50.0,        90.0, 81.6),         15.0, Vec3 (36,36,36), Vec3 (),                 kMatte), // light
};
const std::vector <PointLight> lights =
{
  // Position, power
  PointLight (Vec3 (50, 60, 70.0), Vec3 (1,1,1))
};
PhotonMap photon_map (kNumPhotons);
Camera camera (Vec3 (50.0, 52.0, 220.0),
               Normalize(Vec3 (0, -0.04, -1.0)),
               Vec3 (0, 1, 0));
/*
// ---------------------------------------------------------------------------
// Save .ppm image
// ---------------------------------------------------------------------------
*/
auto SavePpm (const char* filename, const Vec3* const image) -> void
{
  auto to_int = [] (Float val)
  {
    if (val < 0.0) { val = 0; }
    if (val > 1.0) { val = 1; }
    return static_cast<int> (std::pow (val, 1.0 / 2.2) * 255.0 + 0.5);
  };

  FILE* f = fopen (filename, "wb");
  fprintf(f, "P3\n%d %d\n%d\n", kWidth, kHeight, 255);
  for (int i = 0; i < kWidth * kHeight; i++)
  {
    fprintf(f,"%d %d %d ", to_int (image[i].r),
                           to_int (image[i].g),
                           to_int (image[i].b));
  }
  fclose(f);
}
/*
// ---------------------------------------------------------------------------
*/
auto ReflectAsMatte (const Vec3& normal) -> Vec3
{
  // Trace photon ray from intersected position
  Vec3 tangent, binormal;
  BuildOrthoNormalBasis (normal, &tangent, &binormal);

  const Float phi (XorShift::Next01 () * 2.0 * kPi);
  const Float r2  (XorShift::Next01 ());
  const Float r2s (std::sqrt (r2));

  const Float tx (r2s * std::cos (phi));
  const Float ty (r2s * std::sin (phi));
  const Float tz (sqrt(1.0 - r2));

  return tz * normal + tx * tangent + ty * binormal;
}
/*
// ---------------------------------------------------------------------------
*/
auto ReflectAsMirror (const Vec3& w, const Vec3& normal) -> Vec3
{
  // Perform as mirror reflection
  return 2.0 * (Dot (w, normal)) * normal - w;
}
/*
// ---------------------------------------------------------------------------
*/
auto IsIntersect (const Ray& ray, SurfaceIntersectionInfo* info) -> int
{
  int intersect (-1);
  for (int i = 0; i < scene.size (); ++i)
  {
    SurfaceIntersectionInfo tmp;
    if (scene[i].IsIntersect(ray, &tmp))
    {
      if (tmp.t < info->t)
      {
        *info = tmp;
        intersect = i;
      }
    }
  }
  return intersect;
}
/*
// ---------------------------------------------------------------------------
*/
auto EmitPhoton () -> void
{
  // Generate photon ray to trace from point light
  PhotonRay ray;
  lights[0].GeneratePhotonRay (&ray);

  while (true)
  {
    // Intersection test
    int idx = 0;
    SurfaceIntersectionInfo info;
    if ((idx = IsIntersect(ray, &info)) != -1)
    {
      const Sphere& s (scene[idx]);
      if (s.type_ == MaterialType::kMatte)
      {
        // Photon ray was intersected with matte surface
        // Store photon to photon map
        // photon_map->StorePhotonRayAsPhoton (ray, info);
        float power[3] = {ray.flux[0], ray.flux[1], ray.flux[2]};
        float pos[3]   = {info.position[0], info.position[1], info.position[2]};
        float dir[3]   = {info.outgoing[0], info.outgoing[1], info.outgoing[2]};
        photon_map.store(power, pos, dir);

        // Decide to continue more ray by russian roulette
        const Float p (s.reflectance_.g);
        if (XorShift::Next01 () < p)
        {
          // Continue to trace a photon
          ray = PhotonRay (info.position,
                           ReflectAsMatte(info.oriented_normal),
                           ray.flux);
          continue;
        }
        break;
      }
    }
  }
}
/*
// ---------------------------------------------------------------------------
*/
auto PhotonTrace () -> void
{
  // Emit photons
  for (int i = 0; i < kNumPhotons; ++i)
  {
    EmitPhoton ();
  } // End of for
}
/*
// ---------------------------------------------------------------------------
*/
auto Radiance (const Ray& ray, int depth) -> Vec3
{
  SurfaceIntersectionInfo info;
  int idx (IsIntersect (ray, &info));

  if (idx == -1)
  {
    return Vec3 ();
  }

  // Get sphere
  const Sphere* const s = &(scene.at (idx));
  if (s->type_ == kMatte)
  {
    float estimated_irradiance[3];
    float position[3] = {info.position.x, info.position.y, info.position.z};
    float normal[3]   = {info.oriented_normal.x,
                         info.oriented_normal.y,
                         info.oriented_normal.z};
    photon_map.irradiance_estimate (estimated_irradiance,
                                    position,
                                    normal,
                                    20.0,
                                    100);
    const Vec3 brdf (s->reflectance_ * kInvPi);
    return Vec3 (estimated_irradiance[0],
                 estimated_irradiance[1],
                 estimated_irradiance[2]) * brdf;
  }
}
/*
// ---------------------------------------------------------------------------
*/
auto RayTrace () -> void
{
  // Image buffer
  std::unique_ptr <Vec3 []> img (new Vec3 [kWidth * kHeight]);

  for (int y = 0; y < kHeight; ++y)
  {
    for (int x = 0; x < kWidth; ++x)
    {
      const uint32_t idx ((kHeight - 1 - y) * (kWidth)  + x);
      Ray ray (camera.GenerateRay (x, y));

      auto tmp = Radiance (ray, 0);
      img [idx] = tmp;
    }
  }

  SavePpm("output.ppm", img.get ());
}
/*
// ---------------------------------------------------------------------------
*/
auto Render () -> void
{

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
  // Begin photon tracing
  PhotonTrace ();
  photon_map.balance ();

  //
  RayTrace ();

  return 0;
}

