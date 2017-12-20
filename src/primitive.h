#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
#include "material.h"
#include "sphere.h"
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class Primitive
{
  /* Primitive constructors */
public:
  Primitive () = delete;
  Primitive
  (
   const std::shared_ptr <Sphere>&   sphere,
   const std::shared_ptr <Material>& material
  ) :
    sphere_   (sphere),
    material_ (material)
  {}


  /* Primitive destructor */
public:
  virtual ~Primitive () = default;


  /* Primitive public operators*/
public:
  Primitive (const Primitive&  primitive) = default;
  Primitive (      Primitive&& primitive) = default;

  auto operator = (const Primitive&  primitive) -> Primitive& = default;
  auto operator = (      Primitive&& primitive) -> Primitive& = default;


  /* Primitive public methods */
public:
  auto GetSphere () const -> const std::weak_ptr <Sphere>
  {
    std::weak_ptr <Sphere> weak (sphere_);
    return std::move (weak);
  }

  auto GetMaterial () const -> const std::weak_ptr <Material>
  {
    std::weak_ptr <Material> weak (material_);
    return std::move (weak);
  }

  auto IsIntersect (const Ray& ray, SurfaceIntersectionInfo* info) const -> bool
  {
    if (sphere_->IsIntersect(ray, info))
    {
      return true;
    }
    return false;
  }


  /* Primitive private data */
private:
  std::shared_ptr <Sphere>   sphere_;
  std::shared_ptr <Material> material_;
}; // class Primitive
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _PRIMITIVE_H_
