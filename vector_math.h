#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

namespace RDMW
{
   namespace Geometry
   {
      struct Vector3
      {
         union {
            struct
            {
               float x, y, z;
            };
            float v[3];
         };
      };

      inline Vector3 normalize(const Vector3& v)
      {
         auto length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
         return { v.x / length, v.y / length, v.z / length };
      }

      inline Vector3 azElToVec3(const float az, const float el)
      {
         Vector3 v;
         auto deg2rad = (float)(M_PI / 180);

         v.x = cos(az * deg2rad) * cos(el * deg2rad);
         v.y = sin(az * deg2rad) * cos(el * deg2rad);
         v.z = sin(el * deg2rad);

         return v;
      }

      inline std::pair<float, float> vec3ToAzEl(const Vector3& v)
      {
         auto rad2deg = (float)(180.0 / M_PI);

         auto magGroundPlane = sqrt(v.x * v.x + v.y * v.y);
         auto az = atan2(v.y, v.x) * rad2deg;
         auto el = atan2(v.z, magGroundPlane) * rad2deg;

         return { az, el };
      }

      inline float dot(const Vector3& w, const Vector3& v) { return w.x * v.x + w.y * v.y + w.z * v.z; }

      inline float angleBetweenVec3(const Vector3& w, const Vector3& v) { return acos(dot(w, v)) * (180 / M_PI); }
   }
}
