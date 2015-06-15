#ifndef _OGLBUFFER_HPP_INCLUDED_
#define _OGLBUFFER_HPP_INCLUDED_

#include "core/math/Vector3.hpp"
using core::math::Vector3f;

#include "scene/scene.hpp"
using scene::Scene;

#include "core/basicTypes.hpp"
namespace oglbuffer
{

   struct Vertex
   {
      Vector3f m_position;
      Vector3f m_normal;
      Vector3f m_texcoord;

      Vertex(const Vector3f &position, const Vector3f &normal, const Vector3f &texcoord)
      {
         m_position = position;
         m_normal = normal;
         m_texcoord = texcoord;
      }
   };
 
   void generateBufferFromScene(const scene::Scene *sc, GLSLShader &shader, uint32 &vaoID, uint32 &vboIndicesID);

} // namespace oglbuffer

#endif