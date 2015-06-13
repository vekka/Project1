/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file mesh.h
*  @brief Declares the data structures in which the imported geometry is
returned by ASSIMP: Mesh, Face and Bone data structures.
*/
#ifndef _MESH2_HPP_INCLUDED_
#define _MESH2_HPP_INCLUDED_

#include "../core/BasicTypes.hpp"

//#include "core/string/string.hpp"
//using core::string::String_c;

#include "core/math/matrix4.hpp"
using core::math::Matrix4f;

#include "core/math/vector3.hpp"
using core::math::Vector3f;

#include "gfx/color4f.hpp"
using gfx::color4f::Color4f;

#include <string.h>

   /** @brief A single face in a mesh, referring to multiple vertices.
   *
   * If m_numIndices is 3, we call the face 'triangle', for m_numIndices > 3
   * it's called 'polygon' (hey, that's just a definition!).
   * <br>
   * Mesh::m_primitiveTypes can be queried to quickly examine which types of
   * primitive are actually present in a mesh. The #aiProcess_SortByPType flag
   * executes a special post-processing algorithm which splits meshes with
   * *different* primitive types mixed up (e.g. lines and triangles) in several
   * 'clean' submeshes. Furthermore there is a configuration option (
   * #AI_CONFIG_PP_SBP_REMOVE) to force #aiProcess_SortByPType to remove
   * specific kinds of primitives from the imported scene, completely and forever.
   * In many cases you'll probably want to set this setting to
   * @code
   * PRIMITIVE_TYPE_LINE|PRIMITIVE_TYPE_POINT
   * @endcode
   * Together with the #aiProcess_Triangulate flag you can then be sure that
   * #Face::m_numIndices is always 3.
   * @note Take a look at the @link data Data Structures page @endlink for
   * more information on the layout and winding order of a face.
   */

   namespace mesh2
   {

      // Limits. These values are required to match the settings Assimp was 
      // compiled against. Therfore, do not redefine them unless you build the 
      // library from source using the same definitions.

      /** @def MAX_FACE_INDICES
      *  Maximum number of indices per face (polygon). */

      static const uint32 MAX_FACE_INDICES = 0x7fff;

      /** @def MAX_BONE_WEIGHTS
      *  Maximum number of indices per face (polygon). */

      static const uint32 MAX_BONE_WEIGHTS = 0x7fffffff;

      /** @def MAX_VERTICES
      *  Maximum number of vertices per mesh.  */

      static const uint32 MAX_VERTICES = 0x7fffffff;

      /** @def MAX_FACES
      *  Maximum number of faces per mesh. */

      static const uint32 MAX_FACES = 0x7fffffff;

      /** @def MAX_NUMBER_OF_COLOR_SETS
      *  Supported number of vertex color sets per mesh. */

      static const uint32 MAX_NUMBER_OF_COLOR_SETS = 0x8;

      /** @def MAX_NUMBER_OF_TEXTURECOORDS
      *  Supported number of m_texture coord sets (UV(W) channels) per mesh */

      static const uint32 MAX_NUMBER_OF_TEXTURECOORDS = 0x8;

      struct Face
      {
         // Number of indices defining this face. 
         // The maximum value for this member is #MAX_FACE_INDICES.
         uint32 m_numIndices;

         // Pointer to the indices array. Size of the array is given in numIndices.
         uint32* m_pIndexArray;

         Face() : m_numIndices(0), m_pIndexArray(NULL) { }

         ~Face() { delete[] m_pIndexArray; }

         Face(const Face &other) : m_pIndexArray(NULL) { *this = other; }

         Face& operator=(const Face &other)
         {
            if (&other == this)
               return *this;

            delete[] m_pIndexArray;
            m_numIndices = other.m_numIndices;
            if (m_numIndices) {
               m_pIndexArray = new uint32[m_numIndices];
               ::memcpy(m_pIndexArray, other.m_pIndexArray, m_numIndices * sizeof(uint32));
            }
            else {
               m_pIndexArray = NULL;
            }
            return *this;
         }

         bool operator== (const Face &other) const
         {
            if (m_pIndexArray == other.m_pIndexArray)return true;
            else if (m_pIndexArray && m_numIndices == other.m_numIndices)
            {
               for (uint32 i = 0; i < this->m_numIndices; ++i)
                  if (m_pIndexArray[i] != other.m_pIndexArray[i])
                     return false;
               return true;
            }
            return false;
         }

         bool operator != (const Face &other) const
         {
            return !(*this == other);
         }
      }; // struct Face


      // A single influence of a bone on a vertex.
      struct VertexWeight
      {
         //! Index of the vertex which is influenced by the bone.
         uint32 mVertexId;

         //! The strength of the influence in the range (0...1).
         //! The influence from all bones at one vertex amounts to 1.
         float mWeight;

         //! Default constructor
         VertexWeight() { }

         //! Initialisation from a given index and vertex weight factor
         //! \param pID ID
         //! \param pWeight Vertex weight factor
         VertexWeight(uint32 pID, float pWeight)
            : mVertexId(pID), mWeight(pWeight)
         { /* nothing to do here */
         }
      };


      /** @brief A single bone of a mesh.
      *
      *  A bone has a name by which it can be found in the frame hierarchy and by
      *  which it can be addressed by animations. In addition it has a number of
      *  influences on vertices.
      */
      struct Bone
      {
         //! The name of the bone. 
         std::string m_name;

         //! The number of vertices affected by this bone
         //! The maximum value for this member is #MAX_BONE_WEIGHTS.
         uint32 mNumWeights;

         //! The vertices affected by this bone
         VertexWeight* mWeights;

         //! Matrix that transforms from mesh space to bone space in bind pose
         Matrix4f mOffsetMatrix;

         //! Default constructor
         Bone()
            : mNumWeights(0)
            , mWeights(NULL)
         {
         }

         //! Copy constructor
         Bone(const Bone& other)
            : m_name(other.m_name)
            , mNumWeights(other.mNumWeights)
            , mOffsetMatrix(other.mOffsetMatrix)
         {
            if (other.mWeights && other.mNumWeights)
            {
               mWeights = new VertexWeight[mNumWeights];
               ::memcpy(mWeights, other.mWeights, mNumWeights * sizeof(VertexWeight));
            }
         }

         //! Destructor - deletes the array of vertex weights
         ~Bone()
         {
            delete[] mWeights;
         }
      };

      /** @brief Enumerates the types of geometric primitives supported by Assimp.
      *
      *  @see Face Face data structure
      *  @see aiProcess_SortByPType Per-primitive sorting of meshes
      *  @see aiProcess_Triangulate Automatic triangulation
      *  @see AI_CONFIG_PP_SBP_REMOVE Removal of specific primitive types.
      */
      enum ePrimitiveType
      {
         /** A point primitive.
         *
         * This is just a single vertex in the virtual world,
         * #Face contains just one index for such a primitive.
         */
         PRIMITIVE_TYPE_POINT = 0x1,

         /** A line primitive.
         *
         * This is a line defined through a start and an end position.
         * #Face contains exactly two indices for such a primitive.
         */
         PRIMITIVE_TYPE_LINE = 0x2,

         /** A triangular primitive.
         *
         * A triangle consists of three indices.
         */
         PRIMITIVE_TYPE_TRIANGLE = 0x4,

         /** A higher-level polygon with more than 3 edges.
         *
         * A triangle is a polygon, but polygon in this context means
         * "all polygons that are not triangles". The "Triangulate"-Step
         * is provided for your convenience, it splits all polygons in
         * triangles (which are much easier to handle).
         */
         PRIMITIVE_TYPE_POLYGON = 0x8,
      }; 

      // Get the #ePrimitiveType flag for a specific number of face indices
      inline ePrimitiveType GetPrimitiveTypeFlag(int32 faceIdx)
      {
         return (faceIdx > 3 ? PRIMITIVE_TYPE_POLYGON : (ePrimitiveType)(1u << (faceIdx-1)));
      }

      /** @brief NOT CURRENTLY IN USE. An AnimMesh is an attachment to an #Mesh stores per-vertex
      *  animations for a particular frame.
      *
      *  You may think of an #AnimMesh as a `patch` for the host mesh, which
      *  replaces only certain vertex data streams at a particular time.
      *  Each mesh stores n attached attached meshes (#Mesh::m_ppAnimMeshes).
      *  The actual relationship between the time line and anim meshes is
      *  established by #aiMeshAnim, which references singular mesh attachments
      *  by their ID and binds them to a time offset.
      */
      struct AnimMesh
      {
         /** Replacement for Mesh::m_pVertices. If this array is non-NULL,
         *  it *must* contain m_numVertices entries. The corresponding
         *  array in the host mesh must be non-NULL as well - animation
         *  meshes may neither add or nor remove vertex components (if
         *  a replacement array is NULL and the corresponding source
         *  array is not, the source data is taken instead)*/
         Vector3f* m_pVertices;

         /** Replacement for Mesh::m_pNormals.  */
         Vector3f* m_pNormals;

         /** Replacement for Mesh::m_pTangents. */
         Vector3f* m_pTangents;

         /** Replacement for Mesh::m_pBiTangets. */
         Vector3f* m_pBiTangets;

         /** Replacement for Mesh::m_pColors */
         Color4f* m_pColors[MAX_NUMBER_OF_COLOR_SETS];

         /** Replacement for Mesh::m_pTextureCoords */
         Vector3f* m_pTextureCoords[MAX_NUMBER_OF_TEXTURECOORDS];

         /** The number of vertices in the AnimMesh, and thus the length of all
         * the member arrays.
         *
         * This has always the same value as the m_numVerticesNUMBER property in the
         * corresponding Mesh. It is duplicated here merely to make the length
         * of the member arrays accessible even if the Mesh is not known, e.g.
         * from language bindings.
         */
         uint32 m_numVertices;

         AnimMesh()
            : m_pVertices(NULL),
            m_pNormals(NULL),
            m_pTangents(NULL),
            m_pBiTangets(NULL),
            m_numVertices(0)
         {
            // fixme consider moving this to the ctor initializer list as well
            for (uint32 a = 0; a < MAX_NUMBER_OF_TEXTURECOORDS; a++){
               m_pTextureCoords[a] = NULL;
            }
            for (uint32 a = 0; a < MAX_NUMBER_OF_COLOR_SETS; a++) {
               m_pColors[a] = NULL;
            }
         }

         ~AnimMesh()
         {
            delete[] m_pVertices;
            delete[] m_pNormals;
            delete[] m_pTangents;
            delete[] m_pBiTangets;
            for (uint32 a = 0; a < MAX_NUMBER_OF_TEXTURECOORDS; a++) {
               delete[] m_pTextureCoords[a];
            }
            for (uint32 a = 0; a < MAX_NUMBER_OF_COLOR_SETS; a++) {
               delete[] m_pColors[a];
            }
         }

         /** Check whether the anim mesh overrides the vertex positions
         *  of its host mesh*/
         bool HasPositions() const {
            return m_pVertices != NULL;
         }

         /** Check whether the anim mesh overrides the vertex normals
         *  of its host mesh*/
         bool HasNormals() const {
            return m_pNormals != NULL;
         }

         /** Check whether the anim mesh overrides the vertex tangents
         *  and bitangents of its host mesh. As for Mesh,
         *  tangents and bitangents always go together. */
         bool HasTangentsAndBitangents() const {
            return m_pTangents != NULL;
         }

         /** Check whether the anim mesh overrides a particular
         * set of vertex colors on his host mesh.
         *  @param pIndex 0<index<MAX_NUMBER_OF_COLOR_SETS */
         bool HasVertexColors(uint32 pIndex) const	{
            return pIndex >= MAX_NUMBER_OF_COLOR_SETS ? false : m_pColors[pIndex] != NULL;
         }

         /** Check whether the anim mesh overrides a particular
         * set of m_texture coordinates on his host mesh.
         *  @param pIndex 0<index<MAX_NUMBER_OF_TEXTURECOORDS */
         bool HasTextureCoords(uint32 pIndex) const	{
            return pIndex >= MAX_NUMBER_OF_TEXTURECOORDS ? false : m_pTextureCoords[pIndex] != NULL;
         }
      };

      /** @brief A mesh represents a geometry or model with a single material.
      *
      * It usually consists of a number of vertices and a series of primitives/faces
      * referencing the vertices. In addition there might be a series of bones, each
      * of them addressing a number of vertices with a certain weight. Vertex data
      * is presented in channels with each channel containing a single per-vertex
      * information such as a set of m_texture coords or a normal vector.
      * If a data pointer is non-null, the corresponding data stream is present.
      * From C++-programs you can also use the comfort functions Has*() to
      * test for the presence of various data streams.
      *
      * A Mesh uses only a single material which is referenced by a material ID.
      * @note The mPositions member is usually not optional. However, vertex positions
      * *could* be missing if the #AI_SCENE_FLAGS_INCOMPLETE flag is set in
      * @code
      * Scene::m_flags
      * @endcode
      */
      class Mesh
      {
      public:
         /** Bitwise combination of the members of the #ePrimitiveType enum.
         * This specifies which types of primitives are present in the mesh.
         * The "SortByPrimitiveType"-Step can be used to make sure the
         * output meshes consist of one primitive type each.
         */
         uint32 m_primitiveTypes;

         /** The number of vertices in this mesh.
         * This is also the size of all of the per-vertex data arrays.
         * The maximum value for this member is #MAX_VERTICES.
         */
         uint32 m_numVertices;

         /** The number of primitives (triangles, polygons, lines) in this  mesh.
         * This is also the size of the m_pFaces array.
         * The maximum value for this member is #MAX_FACES.
         */
         uint32 m_numFaces;

         /** Vertex positions.
         * This array is always present in a mesh. The array is
         * m_numVertices in size.
         */
         Vector3f* m_pVertices;

         /** Vertex normals.
         * The array contains normalized vectors, NULL if not present.
         * The array is m_numVertices in size. Normals are undefined for
         * point and line primitives. A mesh consisting of points and
         * lines only may not have normal vectors. Meshes with mixed
         * primitive types (i.e. lines and triangles) may have normals,
         * but the normals for vertices that are only referenced by
         * point or line primitives are undefined and set to QNaN (WARN:
         * qNaN compares to inequal to *everything*, even to qNaN itself.
         * Using code like this to check whether a field is qnan is:
         * @code
         * #define IS_QNAN(f) (f != f)
         * @endcode
         * still dangerous because even 1.f == 1.f could evaluate to false! (
         * remember the subtleties of IEEE754 artithmetics). Use stuff like
         * @c fpclassify instead.
         * @note Normal vectors computed by Assimp are always unit-length.
         * However, this needn't apply for normals that have been taken
         *   directly from the model file.
         */
         Vector3f* m_pNormals;

         /** Vertex tangents.
         * The tangent of a vertex points in the direction of the positive
         * X m_texture axis. The array contains normalized vectors, NULL if
         * not present. The array is m_numVertices in size. A mesh consisting
         * of points and lines only may not have normal vectors. Meshes with
         * mixed primitive types (i.e. lines and triangles) may have
         * normals, but the normals for vertices that are only referenced by
         * point or line primitives are undefined and set to qNaN.  See
         * the #m_pNormals member for a detailled discussion of qNaNs.
         * @note If the mesh contains tangents, it automatically also
         * contains bitangents.
         */
         Vector3f* m_pTangents;

         /** Vertex bitangents.
         * The bitangent of a vertex points in the direction of the positive
         * Y m_texture axis. The array contains normalized vectors, NULL if not
         * present. The array is m_numVertices in size.
         * @note If the mesh contains tangents, it automatically also contains
         * bitangents.
         */
         Vector3f* m_pBiTangets;

         /** Vertex color sets.
         * A mesh may contain 0 to #MAX_NUMBER_OF_COLOR_SETS vertex
         * colors per vertex. NULL if not present. Each array is
         * m_numVertices in size if present.
         */
         Color4f* m_pColors[MAX_NUMBER_OF_COLOR_SETS];

         /** Vertex m_texture coords, also known as UV channels.
         * A mesh may contain 0 to MAX_NUMBER_OF_TEXTURECOORDS per
         * vertex. NULL if not present. The array is m_numVertices in size.
         */
         Vector3f* m_pTextureCoords[MAX_NUMBER_OF_TEXTURECOORDS];

         /** Specifies the number of components for a given UV channel.
         * Up to three channels are supported (UVW, for accessing volume
         * or cube maps). If the value is 2 for a given channel n, the
         * component p.z of m_pTextureCoords[n][p] is set to 0.0f.
         * If the value is 1 for a given channel, p.y is set to 0.0f, too.
         * @note 4D coords are not supported
         */
         uint32 m_numUVComponents[MAX_NUMBER_OF_TEXTURECOORDS];

         /** The faces the mesh is constructed from.
         * Each face refers to a number of vertices by their indices.
         * This array is always present in a mesh, its size is given
         * in m_numFaces. If the #AI_SCENE_FLAGS_NON_VERBOSE_FORMAT
         * is NOT set each face references an unique set of vertices.
         */
         Face* m_pFaces;

         /** The number of bones this mesh contains.
         * Can be 0, in which case the m_ppBones array is NULL.
         */
         uint32 m_numBones;

         /** The bones of this mesh.
         * A bone consists of a name by which it can be found in the
         * frame hierarchy and a set of vertex weights.
         */
         Bone** m_ppBones;

         /** The material used by this mesh.
         * A mesh does use only a single material. If an imported model uses
         * multiple materials, the import splits up the mesh. Use this value
         * as index into the scene's material list.
         */
         uint32 m_materialIndex;

         /** Name of the mesh. Meshes can be named, but this is not a
         *  requirement and leaving this field empty is totally fine.
         *  There are mainly three uses for mesh names:
         *   - some formats name nodes and meshes independently.
         *   - importers tend to split meshes up to meet the
         *      one-material-per-mesh requirement. Assigning
         *      the same (dummy) name to each of the result meshes
         *      aids the caller at recovering the original mesh
         *      partitioning.
         *   - Vertex animations refer to meshes by their names.
         **/
         std::string m_name;


         /** NOT CURRENTLY IN USE. The number of attachment meshes */
         uint32 m_numAnimMeshes;

         /** NOT CURRENTLY IN USE. Attachment meshes for this mesh, for vertex-based animation.
         *  Attachment meshes carry replacement data for some of the
         *  mesh'es vertex components (usually positions, normals). */
         AnimMesh** m_ppAnimMeshes;

         //! Default constructor. Initializes all members to 0
         Mesh()
            : m_primitiveTypes(0)
            , m_numVertices(0)
            , m_numFaces(0)
            , m_pVertices(NULL)
            , m_pNormals(NULL)
            , m_pTangents(NULL)
            , m_pBiTangets(NULL)
            , m_pFaces(NULL)
            , m_numBones(0)
            , m_ppBones(NULL)
            , m_materialIndex(0)
            , m_numAnimMeshes(0)
            , m_ppAnimMeshes(NULL)
         {
            for (uint32 a = 0; a < MAX_NUMBER_OF_TEXTURECOORDS; a++)
            {
               m_numUVComponents[a] = 0;
               m_pTextureCoords[a] = NULL;
            }

            for (uint32 a = 0; a < MAX_NUMBER_OF_COLOR_SETS; a++)
               m_pColors[a] = NULL;
         }

         //! Deletes all storage allocated for the mesh
         ~Mesh()
         {
            delete[] m_pVertices;
            delete[] m_pNormals;
            delete[] m_pTangents;
            delete[] m_pBiTangets;
            for (uint32 a = 0; a < MAX_NUMBER_OF_TEXTURECOORDS; a++) {
               delete[] m_pTextureCoords[a];
            }
            for (uint32 a = 0; a < MAX_NUMBER_OF_COLOR_SETS; a++) {
               delete[] m_pColors[a];
            }

            // DO NOT REMOVE THIS ADDITIONAL CHECK
            if (m_numBones && m_ppBones)	{
               for (uint32 a = 0; a < m_numBones; a++) {
                  delete m_ppBones[a];
               }
               delete[] m_ppBones;
            }

            if (m_numAnimMeshes && m_ppAnimMeshes)	{
               for (uint32 a = 0; a < m_numAnimMeshes; a++) {
                  delete m_ppAnimMeshes[a];
               }
               delete[] m_ppAnimMeshes;
            }

            delete[] m_pFaces;
         }

         //! Check whether the mesh contains positions. Provided no special
         //! scene flags are set, this will always be true 
         bool HasPositions() const
         {
            return m_pVertices != NULL && m_numVertices > 0;
         }

         //! Check whether the mesh contains faces. If no special scene flags
         //! are set this should always return true
         bool HasFaces() const
         {
            return m_pFaces != NULL && m_numFaces > 0;
         }

         //! Check whether the mesh contains normal vectors
         bool HasNormals() const
         {
            return m_pNormals != NULL && m_numVertices > 0;
         }

         //! Check whether the mesh contains tangent and bitangent vectors
         //! It is not possible that it contains tangents and no bitangents
         //! (or the other way round). The existence of one of them
         //! implies that the second is there, too.
         bool HasTangentsAndBitangents() const
         {
            return m_pTangents != NULL && m_pBiTangets != NULL && m_numVertices > 0;
         }

         //! Check whether the mesh contains a vertex color set
         //! \param pIndex Index of the vertex color set
         bool HasVertexColors(uint32 pIndex) const
         {
            if (pIndex >= MAX_NUMBER_OF_COLOR_SETS)
               return false;
            else
               return m_pColors[pIndex] != NULL && m_numVertices > 0;
         }

         //! Check whether the mesh contains a m_texture coordinate set
         //! \param pIndex Index of the m_texture coordinates set
         bool HasTextureCoords(uint32 pIndex) const
         {
            if (pIndex >= MAX_NUMBER_OF_TEXTURECOORDS)
               return false;
            else
               return m_pTextureCoords[pIndex] != NULL && m_numVertices > 0;
         }

         //! Get the number of UV channels the mesh contains
         uint32 GetNumUVChannels() const
         {
            uint32 n = 0;
            while (n < MAX_NUMBER_OF_TEXTURECOORDS && m_pTextureCoords[n])++n;
            return n;
         }

         //! Get the number of vertex color channels the mesh contains
         uint32 GetNumColorChannels() const
         {
            uint32 n = 0;
            while (n < MAX_NUMBER_OF_COLOR_SETS && m_pColors[n])++n;
            return n;
         }

         //! Check whether the mesh contains bones
         inline bool HasBones() const
         {
            return m_ppBones != NULL && m_numBones > 0;
         }
      };

} // namespace mesh2

#endif

