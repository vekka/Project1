#include "camera.hpp"

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

// I added this notice, just in case. Need various sources to know what I am doing...
#include "win32/win32main.hpp"

namespace camera
{
   //void Matrix4f::InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar)
   //{
   //   const float ar = Width / Height;
   //   const float zRange = zNear - zFar;
   //   const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

   //   m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
   //   m[1][0] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
   //   m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-zNear - zFar) / zRange; m[2][3] = 2.0f * zFar*zNear / zRange;
   //   m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
   //}

   void AbstractCamera::SetupProjection(const float fovy, const float aspectRatio)
   {
      this->m_aspectRatio = aspectRatio;
      this->m_fov = fovy;

      m_projMatrix = Matrix4f::IDENTITY;
      float invWidth = 1 / (m_nearRight - m_nearLeft);
      float invHeight = 1 / (m_nearTop - m_nearBottom);
      float invNearFarDist = 1 / (m_farDist - m_nearDist);
      

      if (m_projectionType == FRUSTUM_PERSPECTIVE)
      {
         float zRange = m_nearDist - m_farDist;
         float tanThetaY = std::tan(fovy*0.5f);
         //float tanThetaX = aspectRatio *tanThetaY;

         //float halfWidth = m_nearDist * std::tan(tanThetaX);
         //float halfHeight = m_nearDist * std::tan(tanThetaY);

         //// NB:floathis creates 'uniform' perspective projection matrix,
         //// which depth range [-1,1], right-handed rules    
         //m_projMatrix(0, 0) = 2.0f * m_nearDist * invWidth;
         //m_projMatrix(0, 2) = (m_nearRight + m_nearLeft) * invWidth;
         //m_projMatrix(1, 1) = 2.0f * m_nearDist * invHeight;
         //m_projMatrix(1, 2) = (m_nearTop + m_nearBottom) * invHeight;
         //m_projMatrix(2, 2) = -(m_farDist + m_nearDist) * invNearFarDist;
         //m_projMatrix(2, 3) = -2.0f * (m_farDist * m_nearDist) * invNearFarDist;
         //m_projMatrix(3, 2) = -1.0f;
         //m_projMatrix(3, 3) = 0.0f;


         m_projMatrix(0,0) = 1.0f / (tanThetaY * m_aspectRatio);
         m_projMatrix(0,1) = 0.0f;
         m_projMatrix(0,2) = 0.0f;
         m_projMatrix(0,3) = 0.0;
         m_projMatrix(1,0) = 0.0f;
         m_projMatrix(1,1) = 1.0f / tanThetaY;
         m_projMatrix(1,2) = 0.0f;
         m_projMatrix(1,3) = 0.0;
         m_projMatrix(2,0) = 0.0f;
         m_projMatrix(2,1) = 0.0f;
         m_projMatrix(2,2) = -(m_nearDist - m_farDist) / zRange;
         m_projMatrix(2,3) = (-2.0f * m_farDist*m_nearDist) / zRange;
         m_projMatrix(3,0) = 0.0f;
         m_projMatrix(3,1) = 0.0f;
         m_projMatrix(3,2) = 1.0f;
         m_projMatrix(3,3) = 0.0;
         /*

         0,0   0,1   0,2   0,3
         1,0   1,1   1,2   1,3
         2,0   2,1   2,2   2,3
         3,0   3,1   3,2   3,3
         
         */
      }
      else if (m_projectionType == FRUSTUM_ORTHOGRAPHIC)
      {
         m_projMatrix(0, 0) = 2 * invWidth;
         m_projMatrix(0, 3) = -(m_nearRight + m_nearLeft) * invWidth;
         m_projMatrix(1, 1) = 2 * invHeight;
         m_projMatrix(1, 3) = -(m_nearTop + m_nearBottom) * invHeight;
         m_projMatrix(2, 2) = -2.0f * invNearFarDist;
         m_projMatrix(2, 3) = -(m_farDist + m_nearDist) * invNearFarDist;
        
      }
   }

   void FreeCamera::Update()
   {
      Vector3f forward, right, up;

      forward = m_target - m_position;
      forward.Normalize();

      //bookeepin...
      m_forward = forward;

      //side = forward X up
      right = forward.CrossProd(m_up);
      right.Normalize();

      m_right = right;

      //recalculate up; up = side X forward
      up = right.CrossProd(forward);

      m_up = up;

      m_viewMatrix.SetIdentity();
      m_cameraTranslationMatrix.SetIdentity();

      m_viewMatrix.Set(
         right[0], up[0], forward[0], 0.0f,
         right[1], up[1], forward[1], 0.0F,
         right[2], up[2], forward[2], 0.0f,
        0, 0, 0, 1.0);
 
      m_cameraTranslationMatrix.Set(
         1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         m_position[0], m_position[1], m_position[2], 1
         );
      m_viewMatrix = m_cameraTranslationMatrix;
      m_isDirty = false;
   }//UPDATE



   bool FreeCamera::OnKeyboard(int32 key, float stepScale)
   {
      bool ret = false;

      switch (key)
      {
      case win32keyboard::VKEY_W:
         {
            m_position += (m_target * stepScale);
            ret = true;
            m_isDirty = true;
         }
         break;
      case win32keyboard::VKEY_S:
         {
            m_position -= (m_target * stepScale);
            
            ret = true;
            m_isDirty = true;
         }
         break;

      case win32keyboard::VKEY_A:
         {

            Vector3f left = m_up.CrossProd(m_target);
           
            left.Normalize();
            left *= stepScale;
            m_position += left;
           
            ret = true;
            m_isDirty = true;
         }
         break;

      case win32keyboard::VKEY_D:
         {
            Vector3f right = m_target.CrossProd(m_up);
            right.Normalize();
            right *= stepScale;
            m_position += right;
            ret = true;
            m_isDirty = true;
         }
         break;
      }//SWITCH
      return ret;
   }

    // angle in radians
    void FreeCamera::Rotate( Vector3f &vector, float angle, Vector3f axis )
     {
        const float SinHalfAngle = sinf(angle / 2);
        const float CosHalfAngle = cosf(angle / 2);

        const float Rx = axis.x * SinHalfAngle;
        const float Ry = axis.y * SinHalfAngle;
        const float Rz = axis.z * SinHalfAngle;
        const float Rw = CosHalfAngle;

        Quaternion_f RotationQ(Rx, Ry, Rz, Rw);

        Quaternion_f ConjugateQ = RotationQ.Conjugate();

        Quaternion_f W = RotationQ * vector * ConjugateQ;

        vector.x = W.x;
        vector.y = W.y;
        vector.z = W.z;
        m_isDirty = true;
    }
}
