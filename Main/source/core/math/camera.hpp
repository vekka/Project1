#include "vector3.hpp"
#include "vector2.hpp"
#include "matrix4.hpp"
#include "quaternion.hpp"
using core::math::Vector2i;
using core::math::Vector3f;
using core::math::Vector4f;
using core::math::Matrix4f;
using core::math::Quaternion_f;

using core::math::Equals;
#undef near
#undef far



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
      Vector3f m_forward;
      Vector3f m_right;
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

      const Matrix4f &GetProjectionMatrix() const { return m_projMatrix; }

      void SetPosition(const Vector3f &v) { m_position = v; }
      const Vector3f GetPosition() const { return m_position; }
      void SetFOV(const float fov) {this->m_fov = fov; }
      const float GetFOV() const { return m_fov; }
      const float GetAspectRatio() const { return m_aspectRatio; }


      void CalcFrustumPlanes();
      bool IsPointInFrustum(const Vector3f &point);
      bool IsSphereInFrustum(const Vector3f &center, const float
         radius);
      bool IsBoxInFrustum(const Vector3f &min, const Vector3f &max);

      void GetFrustumPlanes(Vector4f planes[6]);
      Vector3f farPts[4];
      Vector3f nearPts[4];
   };

  //floatemplate <typenamefloat>
   class FreeCamera : public AbstractCamera
   {
   protected:
     float m_speed; //move speed of camera in m/s
     float m_angleH;
     float m_angleV;
     Vector3f m_translation;
     Vector2i m_mousePos;
     bool m_onUpperEdge;
     bool m_onLowerEdge;
     bool m_onLeftEdge;
     bool m_onRightEdge;
   public: 
         // I want this to be synced with git repo!.

      FreeCamera(int32 windowWidth, int32 windowHeight, const Vector3f &pos,
         const Vector3f &target, const Vector3f &up)
      {
         m_windowWidth = windowWidth;
         m_windowHeight = windowHeight;
         m_position = pos;

         m_target = target;
         m_target.Normalize();

         m_up = up;
         m_up.Normalize();

         m_cameraTranslationMatrix.Set(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            m_position[0], m_position[1], m_position[2], 1
            );
         m_viewMatrix.Set(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
            );
      }
      void Update();


      void Set(const Vector3f &pos, const Vector3f &target, const Vector3f &up)
      {
         m_position = pos;
         m_target = target;
         m_up = up;
      }

      bool OnKeyboard(int32 k, float stepScale);
      void Rotate(Vector3f &vector, float angle, Vector3f axis)

   };

} // camera