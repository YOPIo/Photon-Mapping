#ifndef _RANDOM_H_
#define _RANDOM_H_
/*
// ---------------------------------------------------------------------------
*/
#include "core.h"
#include <random>
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
class XorShift
{
  /* Xorshift public constructors */
 public:
  XorShift () = default;


  /* Xorshift public destructor */
 public:
  virtual ~XorShift() = default;


  /* Xorshift public operators */
 public:
  XorShift (const XorShift&  rnd) = default;
  XorShift (      XorShift&& rnd) = default;

  auto operator = (const XorShift&  rnd) -> XorShift& = default;
  auto operator = (      XorShift&& rnd) -> XorShift& = default;


  /* Xorshift public methods */
 public:
  // Reset the seed
  // Give:
  //   - seed
  auto SetSeed (std::uint_fast32_t seed) -> void
  {
    x_ = seed << 13;
    y_ = (seed >> 9) ^ (x_ << 6);
    z_ = y_ >> 7;
    w_ = seed;
  }

  // Generate the random number in [0, 1)
  // Return:
  //   - Random number in [0, 1)
  static auto Next01 () -> float
  {
    return static_cast<float> (Next ()) / std::numeric_limits<uint_fast32_t>::max ();
  }

  // Generate the random number in [0, 2^32)
  // Return:
  //   - Random number in [0, 2^32)
  static auto Next () -> std::uint_fast32_t
  {

    std::uint_fast32_t t = (x_ ^ (x_ << 11));
    x_ = y_;
    y_ = z_;
    z_ = w_;
    return w_ = (w_ ^ (w_ >> 19)) ^ (t ^ (t >> 8));
  }


 private:
  static std::uint_fast32_t x_;
  static std::uint_fast32_t y_;
  static std::uint_fast32_t z_;
  static std::uint_fast32_t w_;
};
/*
// ---------------------------------------------------------------------------
*/
std::uint_fast32_t XorShift::x_ = 123456789;
std::uint_fast32_t XorShift::y_ = 362436069;
std::uint_fast32_t XorShift::z_ = 521288629;
std::uint_fast32_t XorShift::w_ = 88675123;
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _RANDOM_H_
