#ifndef _MATERIAL_H_
#define _MATERIAL_H_
/*
// ---------------------------------------------------------------------------
*/
#include "random.h"
#include "core.h"
#include "vec3.h"
/*
// ---------------------------------------------------------------------------
// Material types
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
enum MaterialType
{
  kMatte,
  kMirror
};
/*
// ---------------------------------------------------------------------------
// Abstruct class
// ---------------------------------------------------------------------------
*/
class Material
{
  /* Material constructors */
public:
  Material () = default;


  /* Material destructor */
public:
  virtual ~Material () = default;


  /* Material public operators*/
public:
  Material (const Material&  material) = default;
  Material (      Material&& material) = default;

  auto operator = (const Material&  material) -> Material& = default;
  auto operator = (      Material&& material) -> Material& = default;


  /* Material public method */
public:
  auto GetType () const -> MaterialType
  {
    return type_;
  }

  /* Material public pure virtual method */
public:
  // Return reflectance
  virtual auto GetReflectance () const -> Vec3 = 0;

  // Sample a ray direction
  virtual auto Sample
  (
   const Vec3&  outgoing,
   const Vec3&  normal,
         Vec3*  incident,
         Float* pdf,
         Vec3*  brdf
  )
  const -> void = 0;


  /* Material private data */
private:
  MaterialType type_;

}; // class Material
/*
// ---------------------------------------------------------------------------
*/
class Matte : public Material
{
  /* Matte constructors */
public:
  Matte () :
    Material     (),
    reflectance_ ()
  {};

  Matte (const Vec3& reflectance) :
    Material     (),
    reflectance_ (reflectance)
  {}


  /* Matte destructor */
public:
  virtual ~Matte () = default;


  /* Matte public operators*/
public:
  Matte (const Matte&  matte) = default;
  Matte (      Matte&& matte) = default;

  auto operator = (const Matte&  matte) -> Matte& = default;
  auto operator = (      Matte&& matte) -> Matte& = default;


  /* Matte public override methods */
public:
  auto GetReflectance () const -> Vec3 override final
  {
    return reflectance_;
  }

  // Sample a ray direction
  auto Sample
  (
   const Vec3&  outgoing,
   const Vec3&  normal,
         Vec3*  incident,
         Float* pdf,
         Vec3*  brdf
  )
  const -> void override final
  {
    // Create orthonormal basis from oriented normal
    Vec3 tangent, binormal;
    BuildOrthoNormalBasis (normal, &tangent, &binormal);

    const Float phi = Random::Sample1d () * 2.0 * kPi;
    const Float r2  = Random::Sample1d ();
    const Float r2s = std::sqrt (r2);

    const Float tx = r2s * std::cos (phi);
    const Float ty = r2s * std::sin (phi);
    const Float tz = sqrt(1.0 - r2);

    *incident = tz * normal + tx * tangent + ty * binormal;
    *pdf      = Dot (normal, *incident) * kInvPi;
    *brdf     = reflectance_ * kInvPi;
  }


  /* Matte private data */
private:
  Vec3 reflectance_;

}; // class Matte
/*
// ---------------------------------------------------------------------------
*/
class Mirror : public Material
{
  /* Mirror constructors */
public:
  Mirror () :
    Material     (),
    reflectance_ ()
  {}

  Mirror (const Vec3& reflectance) :
    Material     (),
    reflectance_ (reflectance)
  {}


  /* Mirror destructor */
public:
  virtual ~Mirror () = default;


  /* Mirror public operators*/
public:
  Mirror (const Mirror&  mirror) = default;
  Mirror (      Mirror&& mirror) = default;

  auto operator = (const Mirror&  mirror) -> Mirror& = default;
  auto operator = (      Mirror&& mirror) -> Mirror& = default;


  /* Mirror public override method */
public:
  auto GetReflectance () const -> Vec3 override final
  {
    return reflectance_;
  }

  auto Sample
  (
   const Vec3&  outgoing,
   const Vec3&  normal,
         Vec3*  incident,
         Float* pdf,
         Vec3*  brdf
  )
  const -> void override final
  {
    // Perform as mirror reflection
    *incident = 2.0 * (Dot (outgoing, normal)) * normal - outgoing;
  }


  /* Mirror private methods */
  Vec3 reflectance_;

}; // class Mirror
/*
// ---------------------------------------------------------------------------
*/
}  // namespace n
/*
// ---------------------------------------------------------------------------
*/
#endif // _MATERIAL_H_
