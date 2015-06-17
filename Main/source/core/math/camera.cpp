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

using win32keyboard::VKEY_W;
using win32keyboard::VKEY_S;
using win32keyboard::VKEY_A;
using win32keyboard::VKEY_D;

namespace camera
{
   void AbstractCamera::SetupProjection(const float fovy, const float aspectRatio)
   {
      this->m_aspectRatio = aspectRatio;
      this->m_fov = fovy;

      m_projMatrix.Zero();
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
         m_projMatrix(2, 3) = -2.0f * m_farDist * m_nearDist * invNearFarDist;
         m_projMatrix(3, 2) = -1.0f;
      }
      else if (m_projectionType == FRUSTUM_ORTHOGRAPHIC)
      {
         m_projMatrix(0, 0) = 2 * invWidth;
         m_projMatrix(0, 3) = -(m_nearRight + m_nearLeft) * invWidth;
         m_projMatrix(1, 1) = 2 * invHeight;
         m_projMatrix(1, 3) = -(m_nearTop + m_nearBottom) * invHeight;
         m_projMatrix(2, 2) = -2.0f * invNearFarDist;
         m_projMatrix(2, 3) = -(m_farDist + m_nearDist) * invNearFarDist;
         m_projMatrix(3, 3) = 1.0f;
      }
   }

   void FreeCamera::Update()
   {
     
      //find distance vector
      m_forward = m_target - m_position;
      m_forward.Normalize();
     
      m_right = m_forward.CrossProd(m_up);
      m_right.Normalize();

      m_up = m_right.CrossProd(m_forward);
      m_up.Normalize();

      m_viewMatrix.Set(
         m_right[0], m_up[0], m_forward[0], m_position[0],
         m_right[1], m_up[1], m_forward[1], m_position[1],
         m_right[2], m_up[2], m_forward[2], m_position[2],
        0, 0, 0, 1.0);
 
      m_isDirty = false;
   }//UPDATE



   bool FreeCamera::OnKeyboard(int32 key, float stepScale)
   {

      bool ret = false;
      switch (key)
      {
         case VKEY_W:
         {
        
            m_position += (m_target * stepScale);
    
            ret = true;
            m_isDirty = true;          
         }
         break;
         case VKEY_S:
         {
            m_position -= (m_target * stepScale);
            ret = true;
            m_isDirty = true;
         }
         break;
         case VKEY_A:
         {
            Vector3f left = m_target.CrossProd(m_up);
            left.Normalize();
            left *= stepScale;
            m_position += left;
            ret = true;
            m_isDirty = true;
         }
         break;
         case VKEY_D:
         {
            Vector3f right = m_up.CrossProd(m_target);
            right.Normalize();
            right *= stepScale;
            m_position += right;
            ret = true;
            m_isDirty = true;
         }
         break;
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
