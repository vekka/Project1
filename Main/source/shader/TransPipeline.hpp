#ifndef _TRANSPIPELINE_HPP_INCLUDED_
#define _TRANSPIPELINE_HPP_INCLUDED_

#include "core/math/quaternion.hpp"
using core::math::Quaternion_f;

#include "core/math/vector3.hpp"
using core::math::Vector3f;


#include "core/math/Matrix4.hpp"
using core::math::Matrix4f;

/*
Copyright 2010 Etay Meiri

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// this interface is inspired by : see above

namespace pipeline
{
   class Pipeline
   {
   public:
      Pipeline()
      {
         m_cameraTransformation = Matrix4f::IDENTITY;
         m_WPtransformation = Matrix4f::IDENTITY;
         m_Vtransformation = Matrix4f::IDENTITY;
         m_WVtransformation = Matrix4f::IDENTITY;

         m_scale = Vector3f(1.0f, 1.0f, 1.0f);
         m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
         m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
      }

      void Scale(float s)
      {
         Scale(s, s, s);
      }

      void Scale(const Vector3f &scale)
      {
         Scale(scale.x, scale.y, scale.z);
      }

      void Scale(float scaleX, float scaleY, float scaleZ)
      {
         m_scale.x = scaleX;
         m_scale.y = scaleY;
         m_scale.z = scaleZ;
      }

      void SetWorldPos(const Vector3f &pos)
      {
         m_worldPos = pos;
      }

      void SetWorldPos(float x, float y, float z)
      {
         m_worldPos.x = x;
         m_worldPos.y = y;
         m_worldPos.z = z;
      }

      void Rotate(float rotateX, float rotateY, float rotateZ)
      {
         m_rotateInfo.x = rotateX;
         m_rotateInfo.y = rotateY;
         m_rotateInfo.z = rotateZ;
      }

      void Rotate(const Vector3f &r)
      {
         Rotate(r.x, r.y, r.z);
      }

      void SetPerspectiveProj(const Matrix4f &p)
      {
         m_projectionMatrix = p;
      }

      void SetCamera(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
      {
         m_camera.pos = pos;
         m_camera.target = target;
         m_camera.up = up;
      }

      const Matrix4f &InitCameraTransform(const Vector3f& target, const Vector3f& up);

      const Matrix4f &GetWPTrans();
      const Matrix4f &GetWVTrans();
      const Matrix4f &GetWVPTrans();
      const Matrix4f &Pipeline::GetViewTrans( );

   private:
      Vector3f m_scale;
      Vector3f m_worldPos;
      Vector3f m_rotateInfo;

      Matrix4f m_projectionMatrix;
      Matrix4f m_cameraTransformation;
    
      Matrix4f m_WPtransformation;
      Matrix4f m_Vtransformation;
      Matrix4f m_WVtransformation;
      Matrix4f m_WVPtransformation;

      struct {
         Vector3f pos;
         Vector3f target;
         Vector3f up;
      } m_camera;

   };

} // namespace pipeline

#endif