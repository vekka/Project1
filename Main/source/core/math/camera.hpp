#include "vector3.hpp"
#include "point2.hpp"
#include "matrix4.hpp"
#include "quaternion.hpp"
using core::math::Point2i;
using core::math::Vector3f;
using core::math::Vector4f;
using core::math::Matrix4f;
using core::math::Quaternion_f;

using core::math::Equals;
#undef near
#undef far


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

namespace camera
{

   enum eProjectionType
   {
      FRUSTUM_PERSPECTIVE,
      FRUSTUM_ORTHOGRAPHIC
   };


  //template <typenamefloat>
   class AbstractCamera
   {
   protected:
     float m_yaw, m_pitch, m_roll;
    
     float m_fov;
     float m_aspectRatio;
     float m_nearLeft;
     float m_nearRight;
     float m_nearTop;
     float m_nearBottom;
     float m_nearDist;
     float m_farDist;

      // class Frustum should probably be integrated intofloathis class?
      //It oughtfloato support different projectionsfloato be scalable
   public:
      bool m_isDirty;

      eProjectionType m_projectionType;

      int32 m_windowWidth;
      int32 m_windowHeight;

      // these vectors make up the view matrix. View matrix ~ "the camera"
      Quaternion_f m_orientation;
      Vector3f m_position;
      Vector3f m_target;
      Vector3f m_up;

      Matrix4f m_cameraTranslationMatrix;
      Matrix4f m_cameraRotationMatrix;
      Matrix4f m_viewMatrix; //view matrix
      Matrix4f m_projMatrix; //projection matrix
      //Plane_f planes[6]; //Frustum planes

      bool IsDirty() { return m_isDirty; }
      void SetupProjection(const float fovy, const float aspectRatio);
      void InitProjection(eProjectionType projectionType, float nearLeft, float nearRight,
         float nearTop, float nearBottom, float nearDist, float farDist)
      {
         m_orientation.Set(0.0f, 0.0f, 0.0f, 0.0f);
         m_isDirty = true;
         this->m_projectionType = projectionType;
         this->m_nearLeft = nearLeft;
         this->m_nearRight = nearRight;
         this->m_nearTop = nearTop;
         this->m_nearBottom = nearBottom;
         this->m_nearDist = nearDist;
         this->m_farDist = farDist;
      }

      virtual void Update() = 0;
      
      //virtual void Rotate( float angle, Vector3f axis );
      const Matrix4f &GetViewMatrix() const { return m_viewMatrix; }

      const Matrix4f &GetRotationMatrix() const { return m_cameraRotationMatrix; }
      const Matrix4f &GetTranslationMatrix() const { return m_cameraTranslationMatrix; }
      const Matrix4f &GetProjectionMatrix() const { return m_projMatrix; }

      void SetPosition(const Vector3f &v) { m_position = v; }
      const Vector3f GetPosition() const { return m_position; }
      const Vector3f GetTarget() const { return m_target; }
      const Vector3f GetUp() const { return m_up; }
      void SetFOV(const float fov) {this->m_fov = fov; }
      const float GetFOV() const { return m_fov; }
      const float GetAspectRatio() const { return m_aspectRatio; }
      void CalcFrustumPlanes();
      bool IsPointInFrustum(const Vector3f &point);
      bool IsSphereInFrustum(const Vector3f &center, const float radius);
      bool IsBoxInFrustum(const Vector3f &min, const Vector3f &max);

      void GetFrustumPlanes(Vector4f planes[6]);
      Vector3f farPts[4];
      Vector3f nearPts[4];
   };

  // This class needs both keyboard and mouse support...
   class FreeCamera : public AbstractCamera
   {
   protected:
     float m_speed; //move speed of camera in m/s
     float m_angleH;
     float m_angleV;
     Vector3f m_translation;
     Point2i m_mousePos;
     bool m_onUpperEdge;
     bool m_onLowerEdge;
     bool m_onLeftEdge;
     bool m_onRightEdge;
     const static int32 margin;
     const static float edge_step;

   public:       
      FreeCamera(int32 windowWidth, int32 windowHeight, const Vector3f &pos,
         const Vector3f &target, const Vector3f &up)
      {
         m_isDirty = false;
         m_windowWidth = windowWidth;
         m_windowHeight = windowHeight;
         m_position = pos;

         m_angleH = 0.0f;
         m_angleV = 0.0f;

         m_onUpperEdge = false;
         m_onLowerEdge = false;
         m_onLeftEdge = false;
         m_onRightEdge = false;

         m_target = target;
         m_target.Normalize();

         m_mousePos.x = windowWidth/2;
         m_mousePos.y = windowHeight/2;

         m_up = up;
         m_up.Normalize();

         Init();
      }

      void Update();
      void Init();
      
      void Set(const Vector3f &pos, const Vector3f &target, const Vector3f &up)
      {
         m_position = pos;
         m_target = target;
         m_up = up;
      }

      bool OnKeyboard(int32 k, float stepScale);
      void Rotate(Vector3f &vector, float angle, Vector3f axis);
      void OnMouse(int32 x, int32 y);
      void OnRender();
   };

} // camera
