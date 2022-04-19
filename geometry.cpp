#include "geometry.h"

namespace RDMW
{
   namespace Geometry
   {
      /* Generate an icosahedron mesh that fits in a unit sphere. */
      TriangleMesh genIcosahedron()
      {
         TriangleMesh ico;

         const auto t = (1 + sqrt(5.0f)) / 2.0f;

         ico.vertices.push_back(normalize({ -1, +t, 0 }));
         ico.vertices.push_back(normalize({ +1, +t, 0 }));
         ico.vertices.push_back(normalize({ -1, -t, 0 }));
         ico.vertices.push_back(normalize({ +1, -t, 0 }));

         ico.vertices.push_back(normalize({ 0, -1, +t }));
         ico.vertices.push_back(normalize({ 0, +1, +t }));
         ico.vertices.push_back(normalize({ 0, -1, -t }));
         ico.vertices.push_back(normalize({ 0, +1, -t }));

         ico.vertices.push_back(normalize({ +t, 0, -1 }));
         ico.vertices.push_back(normalize({ +t, 0, +1 }));
         ico.vertices.push_back(normalize({ -t, 0, -1 }));
         ico.vertices.push_back(normalize({ -t, 0, +1 }));

         ico.triangles.push_back({ 0, 11, 5 });
         ico.triangles.push_back({ 0, 5, 1 });
         ico.triangles.push_back({ 0, 1, 7 });
         ico.triangles.push_back({ 0, 7, 10 });
         ico.triangles.push_back({ 0, 10, 11 });

         ico.triangles.push_back({ 1, 5, 9 });
         ico.triangles.push_back({ 5, 11, 4 });
         ico.triangles.push_back({ 11, 10, 2 });
         ico.triangles.push_back({ 10, 7, 6 });
         ico.triangles.push_back({ 7, 1, 8 });

         ico.triangles.push_back({ 3, 9, 4 });
         ico.triangles.push_back({ 3, 4, 2 });
         ico.triangles.push_back({ 3, 2, 6 });
         ico.triangles.push_back({ 3, 6, 8 });
         ico.triangles.push_back({ 3, 8, 9 });

         ico.triangles.push_back({ 4, 9, 5 });
         ico.triangles.push_back({ 2, 4, 11 });
         ico.triangles.push_back({ 6, 2, 10 });
         ico.triangles.push_back({ 8, 6, 7 });
         ico.triangles.push_back({ 9, 8, 1 });

         return ico;
      }

      /* Subdivide the triangles of mesh and project vertices onto a unit sphere. Subdivision
         follows a Class 1 Geodesic Polyhedron. Repeat subdivision n times.
            @param mesh The mesh to be subdivided
			@param n Number of times to subdivide*/
      TriangleMesh subdivideAndNormalize(const TriangleMesh& mesh, const unsigned int n)
      {
         if (n == 0)
            return mesh;
         return subdivideAndNormalize(subdivideAndNormalize(mesh), n - 1);
      }

      /* Subdivide the triangles of mesh and project vertices onto a unit sphere. Subdivision
         follows a Class 1 Geodesic Polyhedron.
            @param mesh The mesh to be subdivided*/
      TriangleMesh subdivideAndNormalize(const TriangleMesh& origMesh)
      {
         using Edge = std::array<unsigned int, 2>;

         struct edgeEq
         {
            bool operator()(Edge const& a, Edge const& b) const noexcept
            {
               return (a.at(0) == b.at(0) && a.at(1) == b.at(1)) || (a.at(1) == b.at(0) && a.at(0) == b.at(1));
            }
         };

         struct edgeHash
         {
            std::size_t operator()(Edge const& e) const noexcept
            {
               auto a = e.at(0);
               auto b = e.at(1);
               if (b < a)
               {
                  a = e.at(1);
                  b = e.at(0);
               }

               std::size_t h1 = std::hash<unsigned int>{}(a);
               std::size_t h2 = std::hash<unsigned int>{}(b);
               return h1 ^ (h2 << 1);
            }
         };

         std::unordered_map<Edge, unsigned int, edgeHash, edgeEq> vertIndexMap;

         TriangleMesh newMesh;
         newMesh.vertices = std::move(origMesh.vertices);
         auto vertIndex = (unsigned int)newMesh.vertices.size() - 1;

         auto addVertex = [&vertIndex, &newMesh](const Vector3& v) {
            newMesh.vertices.push_back(normalize(v));
            return ++vertIndex;
         };

         auto getMidpointIndex = [&addVertex, &vertIndexMap, &newMesh](const Edge& e) {
            if (vertIndexMap.count(e))
            {
               return vertIndexMap[e];
            }
            Vector3 v0 = newMesh.vertices[e.at(0)];
            Vector3 v1 = newMesh.vertices[e.at(1)];
            Vector3 mid = { (v0.x + v1.x) / 2.0f, (v0.y + v1.y) / 2.0f, (v0.z + v1.z) / 2.0f };

            auto index = addVertex(mid);
            vertIndexMap[e] = index;

            return index;
         };

         for (const auto& triangle : origMesh.triangles)
         {
            auto mid0 = getMidpointIndex({ triangle.at(0), triangle.at(1) });
            auto mid1 = getMidpointIndex({ triangle.at(1), triangle.at(2) });
            auto mid2 = getMidpointIndex({ triangle.at(2), triangle.at(0) });

            newMesh.triangles.push_back({ triangle.at(0), mid0, mid2 });
            newMesh.triangles.push_back({ triangle.at(1), mid1, mid0 });
            newMesh.triangles.push_back({ triangle.at(2), mid2, mid1 });
            newMesh.triangles.push_back({ mid0, mid1, mid2 });
         }

         return newMesh;
      }

      /* Retrieve the vertices of a mesh in an ordering that can be readily rendered
         using openGL. */
      std::vector<Vector3> genRenderVerts(const TriangleMesh& mesh)
      {
         std::vector<Vector3> verts;
         for (const auto& triangle : mesh.triangles)
         {
            verts.push_back(mesh.vertices[triangle.at(0)]);
            verts.push_back(mesh.vertices[triangle.at(1)]);
            verts.push_back(mesh.vertices[triangle.at(2)]);
         }

         return verts;
      }
   }
}
