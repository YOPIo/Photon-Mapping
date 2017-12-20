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
class Random
{
  /* Random constructors */
public:
  Random () = default;


  /* Random destructor */
public:
  virtual ~Random () = default;


  /* Random public operators*/
public:
  Random (const Random&  random) = default;
  Random (      Random&& random) = default;

  auto operator = (const Random&  random) -> Random& = default;
  auto operator = (      Random&& random) -> Random& = default;

  static auto Sample1d () -> Float
  {
    return distribution_ (engine_);
  }

  static auto Sample2d () -> std::pair <Float, Float>
  {
    const Float s1 (distribution_ (engine_));
    const Float s2 (distribution_ (engine_));
    return std::make_pair (s1, s2);
  }

  static auto Sample3d () -> std::tuple <Float, Float, Float>
  {
    const Float s1 (distribution_ (engine_));
    const Float s2 (distribution_ (engine_));
    const Float s3 (distribution_ (engine_));
    return std::make_tuple (s1, s2, s3);
  }


  /* Random private operators */
private:
  static int a;
  static std::random_device rng_;
  static std::mt19937_64    engine_;
  static std::uniform_real_distribution <> distribution_;

}; // class Random
/*
// ---------------------------------------------------------------------------
*/
std::random_device Random::rng_;
std::mt19937_64    Random::engine_ (rng_ ());
std::uniform_real_distribution <> Random::distribution_ (0.0, 1.0);
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _RANDOM_H_
