#ifndef _PHOTON_MAP_H_
#define _PHOTON_MAP_H_
/*
// ---------------------------------------------------------------------------
*/
#include "ray.h"
#include "surface_intersection_info.h"
#include "bounding_box.h"
#include "vec3.h"
/*
// ---------------------------------------------------------------------------
*/
namespace
{
/*
// ---------------------------------------------------------------------------
*/
struct Photon
{
  /* Photon constructors */
  Photon () = default;
  Photon
  (
   const Vec3& pos,
   const Vec3& pow,
   unsigned char t,
   unsigned char p
  ) :
    position (pos),
    power    (pow),
    theta    (t),
    phi      (p)
  {}


  /* Photon destructor */
  virtual ~Photon () = default;


  /* Photon operators*/
  Photon (const Photon&  photon) = default;
  Photon (      Photon&& photon) = default;

  auto operator = (const Photon&  photon) -> Photon& = default;
  auto operator = (      Photon&& photon) -> Photon& = default;


  /* Photon data */
  Vec3  position; // Where photon was intersected with diffuse surface
  Vec3  power;    // Power of photon
  // char  power[4];
  unsigned char phi, theta;
  short plane;    // Split axis, x = 0, y = 1, z = 2
}; // class Photon
/*
// ---------------------------------------------------------------------------
*/
#define swap(ph,a,b) { Photon* ph2=ph[a]; ph[a]=ph[b]; ph[b]=ph2; }
class PhotonMap
{
  /* PhotonMap constructors */
public:
  PhotonMap () = delete;
  PhotonMap (size_t max_photons) :
    kMaxPhotons         (max_photons),
    num_stored_photons_ (0),
    // Root of kd-tree does not have data
    photons_            (new Photon[max_photons + 1])
  {
    for (int i = 0; i < 256; ++i)
    {
      Float angle (static_cast <Float> (i) * 256.0 * kPi);
      cos_theta[i] = std::cos (angle);
      sin_theta[i] = std::sin (angle);
      cos_phi[i]   = std::cos (2.0 * angle);
      sin_phi[i]   = std::sin (2.0 * angle);
    }
  };


  /* PhotonMap destructor */
public:
  virtual ~PhotonMap () = default;


  /* PhotonMap public operators*/
public:
  PhotonMap (const PhotonMap&  photonmap) = default;
  PhotonMap (      PhotonMap&& photonmap) = default;

  auto operator = (const PhotonMap&  photonmap) -> PhotonMap& = default;
  auto operator = (      PhotonMap&& photonmap) -> PhotonMap& = default;


  /* PhotonMap public methods */
public:
  auto StorePhotonRayAsPhoton
  (
   const PhotonRay& ray,
   const SurfaceIntersectionInfo& info
  )
    -> bool
  {
    if (kMaxPhotons < num_stored_photons_)
    {
      // There is no enough memory space to store photon
      return false;
    }

    // Initialize photon
    Photon* const p = &photons_[++num_stored_photons_];
    p->position = info.position;
    p->power    = ray.flux;

    // Update boundingbox
    bounds_.Append (info.position);

    // Reference to lookup table indices
    int theta (std::acos (ray.direction[2]) * 256.0 / kPi);
    p->theta = static_cast <unsigned char> (theta);
    if (theta > 255)
    {
      p->theta = 255;
    }

    int phi (std::atan2 (ray.direction[1], ray.direction[0])
                * 256.0 / (2.0 * kPi));
    p->phi = static_cast <unsigned char> (phi);
    if (phi > 255)
    {
      p->phi = 255;
    }
    return true;
  }

  auto Balance () -> void
  {
    if (num_stored_photons_ <= 0)
    {
      // Todo: Error processing
      std::cerr << "No photons are stored." << std::endl;
      return ;
    }

    // Allocate two temporary arrays
    Photon **pa1 ((Photon**) malloc (sizeof (Photon*) * (num_stored_photons_ + 1)));
    Photon **pa2 ((Photon**) malloc (sizeof (Photon*) * (num_stored_photons_ + 1)));

    for (int i = 0; i <= num_stored_photons_; ++i)
    {
      pa2[i] = &photons_[i];
      // printf ("%lf, %lf, %lf\n", pa2[i]->position.x, pa2[i]->position.y, pa2[i]->position.z);
      // printf ("%lf, %lf, %lf\n", photons_[i].position.x, photons_[i].position.y, photons_[i].position.z);
    }

    std::cerr << "Begin balacing." << std::endl;
    BalanceSegment(pa1, pa2, 1, 1, num_stored_photons_);
    free(pa2);
    std::cerr << "Balanced doen." << std::endl;

    int d, j = 1, foo = 1;
    Photon foo_photon (photons_[j]);

    for (int i = 1; i <= num_stored_photons_; ++i)
    {
      d = pa1[j] - photons_.get ();
      pa1[j] = nullptr;
      if (d != foo)
      {
        photons_[j] = photons_[d];
      }
      else
      {
        photons_[j] = foo_photon;

        if (i < num_stored_photons_)
        {
          for (; foo <= num_stored_photons_; ++foo)
          {
            if (pa1[foo] != nullptr)
            {
              break;
            }
            foo_photon = photons_[foo];
            j = foo;
          }
          continue;
        }
        j = d;
      }
      free (pa1);
    }
    num_half_stored_photons_ = num_stored_photons_ / 2 - 1;
  }


  /* PhtonMap private methods */
private:
  auto BalanceSegment
  (
   Photon** balanced,
   Photon** original,
   int index,
   int begin,
   int end
  )
    -> void
  {
    // Midian
    int median = 1;
    while (4 * median <= (end - begin + 1))
    {
      median += median;
    }
    if (3 * median <= (end - begin + 1))
    {
      median += median;
      median += begin - 1;
    }
    else
    {
      median = end - median + 1;
    }

    // Find split axis, x = 0, y = 1, z = 2
    // Use longest axis as splitting box
    unsigned axis = 2;
    if ((bounds_.max[0] - bounds_.min[0]) >
        (bounds_.max[1] - bounds_.min[1]) &&
        (bounds_.max[0] - bounds_.min[0]) >
        (bounds_.max[2] - bounds_.min[2]))
    {
      // Split by x axis
      axis = 0;
    }
    else if ((bounds_.max[1] - bounds_.min[1]) >
             (bounds_.max[2] - bounds_.min[2]))
    {
      axis = 1;
    }

    // Partition photon block around the median
    SplitMedian (original, begin, end, median, axis);

    balanced[index] = original[median];
    balanced[index]->plane = axis;

    // Recursively balance the left and right block
    if (median > begin)
    {
      if (begin < median - 1)
      {
        const Float tmp (bounds_.max[axis]);
        bounds_.max[axis] = balanced[index]->position[axis];
        BalanceSegment (balanced, original, 2 * index, begin, median - 1);
        bounds_.max[axis] = tmp;
      }
      else
      {
        balanced[2 * index] = original[begin];
      }
    }

    if (median < end)
    {
      if (median + 1 < end)
      {
        const Float tmp (bounds_.min[axis]);
        bounds_.min[axis] = balanced[index]->position[axis];
        BalanceSegment (balanced, original, 2 * index + 1, median + 1, end);
        bounds_.min[axis] = tmp;
      }
      else
      {
        balanced[2 * index + 1] = original[end];
      }
    }
  }

  auto SplitMedian
  (
   Photon** photon,
   int      begin,
   int      end,
   int      median,
   int      axis
  )
    const -> void
  {
    int left  (begin);
    int right (end);

    while (left < right)
    {
      const float v (photon[right]->position[axis]);
      int i = left - 1;
      int j = right;

      while (true)
      {
        while (photon[++i]->position[axis] < v)
        {}
        while (photon[--j]->position[axis] > v && j > left)
        {}
        if (i >= j)
        {
          break;
        }
        swap (photon, i, j);
      }

      swap (photon, i, right);
      // std::swap (photon[i], photon[right]);
      if (i >= median)
      {
        right = i - 1;
      }
      if (i <= median)
      {
        left = i + 1;
      }
    }
  }



  /* PhotonMap private data */
public:
  const size_t kMaxPhotons;

  size_t num_stored_photons_;
  size_t num_half_stored_photons_;
  std::unique_ptr <Photon []> photons_;

  Float sin_theta[256];
  Float cos_theta[256];
  Float sin_phi[256];
  Float cos_phi[256];

  BoundingBox bounds_;
}; // class PhotonMap
/*
// ---------------------------------------------------------------------------
*/
}  // namespace
/*
// ---------------------------------------------------------------------------
*/
#endif // _PHOTON_MAP_H_
