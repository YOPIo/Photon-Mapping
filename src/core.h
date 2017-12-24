#ifndef _CORE_H_
#define _CORE_H_
/*
// ---------------------------------------------------------------------------
*/
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
typedef float Float;
/*
// ---------------------------------------------------------------------------
// Render settings
// ---------------------------------------------------------------------------
*/
static const uint32_t kWidth       = 480;
static const uint32_t kHeight      = 270;
static const uint32_t kSuperSample = 2;
static const uint32_t kSample      = 2;
static const uint32_t kNumPhotons  = 1000000;
/*
// ---------------------------------------------------------------------------
// Global constant variables
// ---------------------------------------------------------------------------
*/
static const Float kPi       = 3.14159265358979323846264338327950288419716939;
static const Float kInvPi    = 0.31830988618379067153776752674502872406891929;
static const Float kFloatMax = std::numeric_limits <Float>::max ();
// static const Float kFloatMin = std::numeric_limits <Float>::min ();
/*
// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------
*/
class  Vec3;
struct Ray;
class  Sphere;
class  SurfaceIntersectionInfo;
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _CORE_H_
