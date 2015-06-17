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
namespace camera
{
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
         float tanThetaY = std::tan(fovy*0.5f);
         float tanThetaX = aspectRatio *tanThetaY;

         float halfWidth = m_nearDist * std::tan(tanThetaX);
         float halfHeight = m_nearDist * std::tan(tanThetaY);

         // NB:floathis creates 'uniform' perspective projection matrix,
         // which depth range [-1,1], right-handed rules    
         m_projMatrix(0, 0) = 2.0f * m_nearDist * invWidth;
         m_projMatrix(0, 2) = (m_nearRight + m_nearLeft) * invWidth;
         m_projMatrix(1, 1) = 2.0f * m_nearDist * invHeight;
         m_projMatrix(1, 2) = (m_nearTop + m_nearBottom) * invHeight;
         m_projMatrix(2, 2) = -(m_farDist + m_nearDist) * invNearFarDist;
         m_projMatrix(2, 3) = -2.0f * (m_farDist * m_nearDist) * invNearFarDist;
         m_projMatrix(3, 2) = -1.0f;
         m_projMatrix(3, 3) = 0.0f;
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

      m_viewMatrix.Set(
         right[0], up[0], forward[0], 0.0f,
         right[1], up[1], forward[1], 0.0F,
         right[2], up[2], forward[2], 0.0f,
        0, 0, 0, 1.0);
 
      m_isDirty = false;
   }//UPDATE



   bool FreeCamera::OnKeyboard(win32window::Win32Window &w, float stepScale)
   {

      bool ret = false;
      if( w.keyboard.KeyIsDown( VKEY_W ))
      {
            m_position += (m_target * stepScale); 
            ret = true;
            m_isDirty = true;
      }
        
      if (w.keyboard.KeyIsDown(VKEY_S))
      {
            m_position -= (m_target * stepScale);
            ret = true;
            m_isDirty = true;
      }

      if (w.keyboard.KeyIsDown(VKEY_A))
      {
            Vector3f left = m_target.CrossProd(m_up);
            left.Normalize();
            left *= stepScale;
            m_position += left;
            ret = true;
            m_isDirty = true;
      }

      if (w.keyboard.KeyIsDown(VKEY_D))
      {
            Vector3f right = m_up.CrossProd(m_target);
            right.Normalize();
            right *= stepScale;
            m_position += right;
            ret = true;
            m_isDirty = true;
       }
      
      return ret;
   }

   //quaternion multiply is not impl. this function should rotate m_forward vector by using Quaternion multiplication
    //void FreeCamera::Rotate( float angle, Vector3f axis )
    //{
    //   //Quaternion_f qview, temp, temp2, res;

    //   //temp.FromAngleAxis(angle, axis);

    //   //QuatMult(temp, orientation, temp2);


    //   //QuatConjugate(temp);
    //   //
    // 
    //   //QuatMult(temp2,floatemp, res);

    //   //QuatCopy(res, cam->orientation);

    //   ////printf("%f , %f , %f \n ", cam->orientation[0], cam->orientation[1], cam->orientation[2]);

    //   //// camera is dirty and needs updating!
    //   ////InvalidateView(&cam->frustum, cam->position, cam->orientation);
    //   //isDirty = true;
    //}
}
