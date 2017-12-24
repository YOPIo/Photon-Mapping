#ifndef _CAMERA_H_
#define _CAMERA_H_
/*
// ---------------------------------------------------------------------------
*/
#include "vec3.h"
#include "ray.h"
/*
// ---------------------------------------------------------------------------
*/
class Camera
{
  /* Camera constructors */
public:
  Camera
  (
   const Vec3& position,
   const Vec3& direction,
   const Vec3& up
  ) :
    position_  (position),
    direction_ (direction),
    up_        (up)
  {
    const Float sensor_width  = 30.0 * static_cast <Float> (kWidth)
                                / static_cast <Float> (kHeight);
    const Float sensor_height = 30.0;
    const double sensor_dist  = 40.0;

    screen_x_      = Normalize (Cross(direction, up)) * sensor_width;
    screen_y_      = Normalize (Cross(screen_x_, direction_)) * sensor_height;
    screen_center_ = position_ + direction_ * sensor_dist;
  }


  /* Camera destructor */
public:
  virtual ~Camera () = default;


  /* Camera public operators*/
public:
  Camera (const Camera&  camera) = default;
  Camera (      Camera&& camera) = default;

  auto operator = (const Camera&  camera) -> Camera& = default;
  auto operator = (      Camera&& camera) -> Camera& = default;


  /* Camera public methods */
public:
  auto GenerateRay (uint32_t pixel_x, uint32_t pixel_y) -> Ray
  {
    const Vec3 screen_position
      = screen_center_
      + screen_x_ * ((static_cast <Float> (pixel_x + 0.5))
                     / static_cast <Float> (kWidth)  - 0.5)
      + screen_y_ * ((static_cast <Float> (pixel_y + 0.5))
                     / static_cast <Float> (kHeight) - 0.5);

    const Vec3 dir = Normalize (screen_position - position_);
    return Ray (position_, dir);
  }

  /* Camera private data */
private:
  Vec3 position_;
  Vec3 direction_;
  Vec3 up_;

  Vec3 screen_center_;
  Vec3 screen_x_;
  Vec3 screen_y_;
}; // class Camera
/*
// ---------------------------------------------------------------------------
*/
#endif // _CAMERA_H_
