#pragma once
#include <array>
#include <unordered_map>
#include <vector>

#include "vector_math.h"

namespace RDMW
{
   namespace Geometry
   {
      using TriangleIndices = std::array<unsigned int, 3>;

      struct TriangleMesh
      {
         std::vector<Vector3> vertices;
         std::vector<TriangleIndices> triangles;
      };

      TriangleMesh genIcosahedron();
      TriangleMesh subdivideAndNormalize(const TriangleMesh&);
      TriangleMesh subdivideAndNormalize(const TriangleMesh&, unsigned int);

      std::vector<Vector3> genRenderVerts(const TriangleMesh&);
   }
}
