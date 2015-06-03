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

/** @file scene.h
*  @brief Defines the data structures in which the imported scene is returned.
*/
#ifndef _SCENE_HPP_INCLUDED_
#define _SCENE_HPP_INCLUDED_

//#include "types.h"
//#include "texture.h"
#include "../model/mesh2.hpp"
using mesh2::Mesh;
//#include "light.h"
//#include "camera.h"
//#include "material.h"
//#include "anim.h"
//#include "metadata.h"

namespace scene
{

   /** A node in the imported hierarchy.
   *
   * Each node has name, a parent node (except for the root node),
   * a transformation relative to its parent and possibly several child nodes.
   * Simple file formats don't support hierarchical structures - for these formats
   * the imported scene does consist of only a single root node without children.
   */
   struct Node
   {
      /** The name of the node.
      *
      * The name might be empty (length of zero) but all nodes which
      * need to be referenced by either bones or animations are named.
      * Multiple nodes may have the same name, except for nodes which are referenced
      * by bones (see #aiBone and #Mesh::mBones). Their names *must* be unique.
      *
      * Cameras and lights reference a specific node by name - if there
      * are multiple nodes with this name, they are assigned to each of them.
      * <br>
      * There are no limitations with regard to the characters contained in
      * the name string as it is usually taken directly from the source file.
      *
      * Implementations should be able to handle tokens such as whitespace, tabs,
      * line feeds, quotation marks, ampersands etc.
      *
      * Sometimes assimp introduces new nodes not present in the source file
      * into the hierarchy (usually out of necessity because sometimes the
      * source hierarchy format is simply not compatible). Their names are
      * surrounded by @verbatim <> @endverbatim e.g.
      *  @verbatim<DummyRootNode> @endverbatim.
      */
      std::string m_name;

      /** The transformation relative to the node's parent. */
      Matrix4f m_transformation;

      Node* m_pParentNode; // NULL if root node

      uint32 m_numChildren;

      Node** m_ppChildren; // NULL if no children

      uint32 m_numMeshes;

      uint32* m_ppMeshes; // Each entry is an index into the mesh

      /** Metadata associated with this node or NULL if there is no metadata.
      *  Whether any metadata is generated depends on the source file format. See the
      * @link importer_notes @endlink page for more information on every source file
      * format. Importers that don't document any metadata don't write any.
      */
      //aiMetadata* mMetaData;

      Node()
         // set all members to zero by default
         : m_name("")
         , m_pParentNode(NULL)
         , m_numChildren(0)
         , m_ppChildren(NULL)
         , m_numMeshes(0)
         , m_ppMeshes(NULL)
         //, mMetaData(NULL)
      {
      }

      Node(const std::string &name)
         // set all members to zero by default
         : m_name(name)
         , m_pParentNode(NULL)
         , m_numChildren(0)
         , m_ppChildren(NULL)
         , m_numMeshes(0)
         , m_ppMeshes(NULL)
         //, mMetaData(NULL)
      {
      }

      ~Node()
      {
         // delete all children recursive to make sure we won't crash if the data is invalid ...
         if (m_ppChildren && m_numChildren)
         {
            for (uint32 a = 0; a < m_numChildren; a++)
               delete m_ppChildren[a];
         }
         delete[] m_ppChildren;
         delete[] m_ppMeshes;
         //delete mMetaData;
      }


      /** Searches for a node with a specific name, beginning at this
      *  nodes. Normally you will call this method on the root node
      *  of the scene.
      *
      *  @param name Name to search for
      *  @return NULL or a valid Node if the search was successful.
      */
      inline const Node* FindNode(const std::string &name) const
      {
         return FindNode(name.c_str());
      }

      inline Node* FindNode(const std::string& name)
      {
         return FindNode(name.c_str());
      }

      inline const Node* FindNode(const char *name) const
      {
         if (!strcmp(m_name.c_str(), name) )
            return this;
         for (uint32 i = 0; i < m_numChildren; ++i)
         {
            const Node* const p = m_ppChildren[i]->FindNode(name);
            if (p) {
               return p;
            }
         }

         return NULL;
      }

      inline Node* FindNode(const char *name)
      {
         if (!strcmp(m_name.c_str(), name))
            return this;
         for (uint32 i = 0; i < m_numChildren; ++i)
         {
            Node* const p = m_ppChildren[i]->FindNode(name);
            if (p) {
               return p;
            }
         }

         return NULL;
      }
   };

   enum eSceneFlags
   {
       SCENE_FLAGS_INCOMPLETE = 1,
       SCENE_FLAGS_VALIDATED = 2,
       SCENE_FLAGS_VALIDATION_WARNING = 4,
       SCENE_FLAGS_NON_VERBOSE_FORMAT = 8,
       SCENE_FLAGS_TERRAIN = 16,
   };

   /**
   * Specifies that the scene data structure that was imported is not complete.
   * This flag bypasses some internal validations and allows the import
   * of animation skeletons, material libraries or camera animation paths
   * using Assimp. Most applications won't support such data.
   */

 // #define AI_SCENE_FLAGS_INCOMPLETE	0x1

   /**
   * This flag is set by the validation postprocess-step (aiPostProcess_ValidateDS)
   * if the validation is successful. In a validated scene you can be sure that
   * any cross references in the data structure (e.g. vertex indices) are valid.
   */
//#define AI_SCENE_FLAGS_VALIDATED	0x2

   /**
   * This flag is set by the validation postprocess-step (aiPostProcess_ValidateDS)
   * if the validation is successful but some issues have been found.
   * This can for example mean that a texture that does not exist is referenced
   * by a material or that the bone weights for a vertex don't sum to 1.0 ... .
   * In most cases you should still be able to use the import. This flag could
   * be useful for applications which don't capture Assimp's log output.
   */
//#define AI_SCENE_FLAGS_VALIDATION_WARNING  	0x4

   /**
   * This flag is currently only set by the aiProcess_JoinIdenticalVertices step.
   * It indicates that the vertices of the output meshes aren't in the internal
   * verbose format anymore. In the verbose format all vertices are unique,
   * no vertex is ever referenced by more than one face.
   */
//#define AI_SCENE_FLAGS_NON_VERBOSE_FORMAT  	0x8

   /**
   * Denotes pure height-map terrain data. Pure terrains usually consist of quads,
   * sometimes triangles, in a regular grid. The x,y coordinates of all vertex
   * positions refer to the x,y coordinates on the terrain height map, the z-axis
   * stores the elevation at a specific point.
   *
   * TER (Terragen) and HMP (3D Game Studio) are height map formats.
   * @note Assimp is probably not the best choice for loading *huge* terrains -
   * fully triangulated data takes extremely much free store and should be avoided
   * as long as possible (typically you'll do the triangulation when you actually
   * need to render it).
   */
// #define AI_SCENE_FLAGS_TERRAIN 0x10

   /** The root structure of the imported data.
   *
   *  Everything that was imported from the given file can be accessed from here.
   *  Objects of this class are generally maintained and owned by Assimp, not
   *  by the caller. You shouldn't want to instance it, nor should you ever try to
   *  delete a given scene on your own.
   */

   struct Scene
   {
      eSceneFlags m_flags; // Most applications will want to reject all scenes with the AI_SCENE_FLAGS_INCOMPLETE


      /*
      * There will always be at least the root node if the import
      * was successful (and no special flags have been set).
      * Presence of further nodes depends on the format and content
      * of the imported file.
      */
      Node* m_pRootNode;

      uint32 m_numMeshes;

      /** The array of meshes.
      *
      * Use the indices given in the Node structure to access
      * this array. The array is m_numMeshes in size. If the
      * AI_SCENE_FLAGS_INCOMPLETE flag is not set there will always
      * be at least ONE material.
      */
      Mesh** m_ppMeshes;

      uint32 m_numMaterials;

      /** The array of materials.
      *
      * Use the index given in each Mesh structure to access this
      * array. The array is m_numMaterials in size. If the
      * AI_SCENE_FLAGS_INCOMPLETE flag is not set there will always
      * be at least ONE material.
      */
      material::Material** m_ppMaterials;

      uint32 m_numAnimations;

      /*
      * All animations imported from the given file are listed here.
      * The array is m_numAnimations in size.
      */
      //Animation** m_ppAnimations;

      /** The number of textures embedded into the file */
      uint32 m_numTextures;

      /** The array of embedded textures.
      *
      * Not many file formats embed their textures into the file.
      * An example is Quake's MDL format (which is also used by
      * some GameStudio versions)
      */
      //Texture** m_ppTextures;

      uint32 m_numLights; // light sources are fully optional, in most cases this is 0

      /** The array of light sources.
      *
      * All light sources imported from the given file are
      * listed here. The array is m_numLights in size.
      */
      //Light** m_ppLights;


      /** The number of cameras in the scene. Cameras
      * are fully optional, in most cases this attribute will be 0
      */
      uint32 m_numCameras; // cameras are fully optional, in most cases this is 0

      /*
      * All cameras imported from the given file are listed here.
      * The array is m_numCameras in size. The first camera in the
      * array (if existing) is the default camera view into
      * the scene.
      */
       //Camera** m_ppCameras;

      Scene();
      ~Scene();

      // Unless no special scene flags are set this will always be true.
      inline bool HasMeshes() const
      {
         return m_ppMeshes != NULL && m_numMeshes > 0;
      }

      // Unless no special scene flags are set this will always be true.
      inline bool HasMaterials() const
      {
         return false;
         //return m_ppMaterials != NULL && m_numMaterials > 0;
      }

      inline bool HasLights() const
      {
         return false;
         //return m_ppLights != NULL && m_numLights > 0;
      }

      inline bool HasTextures() const
      {
         return false;
         //return m_ppTextures != NULL && m_numTextures > 0;
      }

      inline bool HasCameras() const
      {
         return false;
         //return m_ppCameras != NULL && m_numCameras > 0;
      }

      inline bool HasAnimations() const
      {
         return false;
         //return m_ppAnimations != NULL && m_numAnimations > 0;
      }

      /**  Internal data, do not touch */
      void* m_pPrivate;
   };

} // namespace scene

#endif
