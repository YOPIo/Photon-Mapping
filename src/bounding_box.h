#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_
/*
// ---------------------------------------------------------------------------
*/
#include "vec3.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class BoundingBox
{
  /* BoundingBox constructors */
public:
  BoundingBox () :
    min (Vec3 ( 1e8,  1e8,  1e8)),
    max (Vec3 (-1e8, -1e8, -1e8))
  {};


  /* BoundingBox destructor */
public:
  virtual ~BoundingBox () = default;


  /* BoundingBox public operators*/
public:
  BoundingBox (const BoundingBox&  bounds) = default;
  BoundingBox (      BoundingBox&& bounds) = default;

  auto operator = (const BoundingBox&  bounds) -> BoundingBox& = default;
  auto operator = (      BoundingBox&& bounds) -> BoundingBox& = default;


  /* BoundingBox public methods */
public:
  auto Append (const Vec3& v) -> void
  {
    // Update bounding box for each axis
    for (int i = 0; i < 3; ++i)
    {
      if (v[i] < min[i])
      {
        min[i] = v[i];
      }
      if (v[i] > max[i])
      {
        max[i] = v[i];
      }
    }
  }

  /* BoundingBox public data*/
public:
  Vec3 min;
  Vec3 max;
}; // class BoundingBox
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _BOUNDING_BOX_H_
