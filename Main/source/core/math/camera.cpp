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

const int32 camera::FreeCamera::margin = 10;
const float camera::FreeCamera::edge_step = 0.5f;
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


         m_projMatrix = Matrix4f::ZERO;

         m_projMatrix(0,0) = 1.0f / (tanThetaY * m_aspectRatio);
         m_projMatrix(1,1) = 1.0f / tanThetaY;
         m_projMatrix(2,2) = -(m_nearDist - m_farDist) / zRange;
         m_projMatrix(2,3) = (-2.0f * m_farDist*m_nearDist) / zRange;
         m_projMatrix(3,2) = 1.0f;
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

   void FreeCamera::Init()
   {
      Vector3f Htarget(m_target.x, 0.0f, m_target.z);
      Htarget.Normalize();

      Vector3f HTarget(m_target.x, 0.0, m_target.z);
      HTarget.Normalize();

      if (HTarget.z >= 0.0f)
      {
         if (HTarget.x >= 0.0f)
         {
            m_angleH = 360.0f - core::math::RadToDeg(asin(HTarget.z));
         }
         else
         {
            m_angleH = 180.0f + core::math::RadToDeg(asin(HTarget.z));
         }
      }
      else
      {
         if (HTarget.x >= 0.0f)
         {
            m_angleH = core::math::RadToDeg(asin(-HTarget.z));
         }
         else
         {
            m_angleH = 90.0f + core::math::RadToDeg(asin(-HTarget.z));
         }
      }

      m_angleV = -core::math::RadToDeg(asin(m_target.y));

      m_onUpperEdge = false;
      m_onLowerEdge = false;
      m_onLeftEdge = false;
      m_onRightEdge = false;
      m_mousePos.x = m_windowWidth / 2;
      m_mousePos.y = m_windowHeight / 2;

   }

   void FreeCamera::Update()
   {
      const Vector3f vAxis(0.0f, 1.0f, 0.0f);

      Vector3f view(1.0f, 0.0f, 0.0f);
      Rotate(view, m_angleH, vAxis);
      view.Normalize();


      Vector3f hAxis = vAxis.CrossProd(view);
      hAxis.Normalize();
      Rotate(view, m_angleV, hAxis);


      m_target = view;
      m_target.Normalize();

      m_up = m_target.CrossProd(hAxis);
      m_up.Normalize();
      //Vector3f forward, right, up;
      //      
      //forward = m_target - m_position;
      //forward.Normalize();

      ////side = forward X up
      //right = forward.CrossProd(m_up);
      //right.Normalize();

      ////recalculate up; up = side X forward
      //up = right.CrossProd(forward);

      //m_up = up;

      m_isDirty = false;
   }//UPDATE

   bool FreeCamera::OnKeyboard(int32 key, float stepScale)
   {
      bool ret = false;
      switch (key)
      {
      case win32keyboard::VKEY_W:
         {
            Vector3f temp = m_target;
            temp *= stepScale;
            m_position += temp;
            //std::cout << m_position << std::endl;
            ret = true;
            m_isDirty = true;
         }
         break;
      case win32keyboard::VKEY_S:
         {
            Vector3f temp = m_target;
            temp *= stepScale;
            m_position -= temp;
            
            ret = true;
            m_isDirty = true;
         }
         break;

      case win32keyboard::VKEY_A:
         {

            Vector3f left = m_target.CrossProd(m_up);
           
            left.Normalize();
            left *= stepScale;
            m_position += left;
           
            ret = true;
            m_isDirty = true;
         }
         break;

      case win32keyboard::VKEY_D:
         {
    
            Vector3f right = m_up.CrossProd(m_target);
            right.Normalize();
            right *= stepScale;
            m_position += right;
            ret = true;
            m_isDirty = true;
         }
         break;
      } //SWITCH
      
      return ret;
   }

    // angle in radians
    void FreeCamera::Rotate( Vector3f &vector, float angle, Vector3f axis )
     {
        const float sinHalfAngle = sinf(angle / 2);
        const float cosHalfAngle = cosf(angle / 2);

        const float rx = axis.x * sinHalfAngle;
        const float ry = axis.y * sinHalfAngle;
        const float rz = axis.z * sinHalfAngle;
        const float rw = cosHalfAngle;

        Quaternion_f RotationQ(rx, ry, rz, rw);

        Quaternion_f ConjugateQ = RotationQ.Conjugate();

        //I am uncertain if it is possible to convert a vector to quaternion like this...
        Quaternion_f vectorAsQuat(vector.x, vector.y, vector.z, 0);
        Quaternion_f w = RotationQ * vectorAsQuat *ConjugateQ;

        vector.x = w.x;
        vector.y = w.y;
        vector.z = w.z;
        m_isDirty = true;
    }

    void FreeCamera::OnMouse(int32 x, int32 y)
    {
       const int32 deltaX = x - m_mousePos.x;
       const int32 deltaY = y - m_mousePos.y;

       m_mousePos.x = x;
       m_mousePos.y = y;

       m_angleH += (float)deltaX / 200.0f;
       m_angleV += (float)deltaY / 200.0f;

       if (deltaX == 0) {
          if (x <= margin) {
             //    m_AngleH -= 1.0f;
             m_onLeftEdge = true;
          }
          else if (x >= (m_windowWidth - margin)) {
             //    m_AngleH += 1.0f;
             m_onRightEdge = true;
          }
       }
       else {
          m_onLeftEdge = false;
          m_onRightEdge = false;
       }

       if (deltaY == 0) {
          if (y <= margin) {
             m_onUpperEdge = true;
          }
          else if (y >= (m_windowHeight - margin)) {
             m_onLowerEdge = true;
          }
       }
       else {
          m_onUpperEdge = false;
          m_onLowerEdge = false;
       }

          Update();
    }

    void FreeCamera::OnRender()
    {
       bool ShouldUpdate = false;

       if (m_onLeftEdge) {
          m_angleH -= edge_step;
          m_isDirty = true;
       }
       else if (m_onRightEdge) {
          m_angleH += edge_step;
          m_isDirty = true;
       }

       if (m_onUpperEdge) {
          if (m_angleV > -90.0f) {
             m_angleV -= edge_step;
             m_isDirty = true;
          }
       }
       else if (m_onLowerEdge) {
          if (m_angleV < 90.0f) {
             m_angleV += edge_step;
             m_isDirty = true;
          }
       }

       if (m_isDirty) {
          Update();
       }
    }

}
