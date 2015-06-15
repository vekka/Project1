#include "vector3.hpp"
#include "matrix4.hpp"
#include "quaternion.hpp"
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
     float yaw, pitch, roll;
    
     float fov;
     float aspectRatio;
     float nearLeft;
     float nearRight;
     float nearTop;
     float nearBottom;
     float nearDist;
     float farDist;

      // class Frustum should probably be integrated intofloathis class?
      //It oughtfloato support different projectionsfloato be scalable
   public:

      // a rather big contstructor...:S
      //AbstractCamera::AbstractCamera(
      //   const eProjectionType projectionType,
      //   const float nearLeft, const float nearRight,
      //   const float nearTop, const float nearBottom,
      //   const float nearDist, const float farDist)
      //{
      //   assert(projectionType == FRUSTUM_PERSPECTIVE || projectionType == FRUSTUM_ORTHOGRAPHIC);
      //   assert(!Equals(nearLeft, nearRight));
      //   assert(!Equals(nearTop, nearBottom));
      //   assert(!Equals(farDist, nearDist));

      //   orientation.Set(0.0f, 0.0f, 0.0f, 0.0f);
      //  isDirty = true;
      //  this->projectionType = projectionType;
      //  this->nearLeft = nearLeft;
      //  this->nearRight = nearRight;
      //  this->nearTop = nearTop;
      //  this->nearBottom = nearBottom;
      //  this->nearDist = nearDist;
      //  this->farDist = farDist;
      //}
      //AbstractCamera();
      //~AbstractCamera();

      bool isDirty;

      eProjectionType projectionType;

      // these vectors make up the view matrix. View matrix ~ "the camera"
      Quaternion_f orientation;
      Vector3f forward;
      Vector3f up;
      Vector3f right;
      Vector3f position;
      Matrix4f viewMatrix; //view matrix
      Matrix4f projMatrix; //projection matrix
      //Plane_f planes[6]; //Frustum planes

      bool IsDirty() { return isDirty; }
      void SetupProjection(const float fovy, const float aspectRatio);
      virtual void Update() = 0;
      
      //virtual void Rotate( float angle, Vector3f axis );
      const Matrix4f &GetViewMatrix() const { return viewMatrix; }

      const Matrix4f &GetProjectionMatrix() const { return projMatrix; }

      void SetPosition(const Vector3f &v) { position = v; }
      const Vector3f GetPosition() const { return position; }
      void SetFOV(const float fov) {this->fov = fov; }
      const float GetFOV() const { return fov; }
      const float GetAspectRatio() const { return aspectRatio; }


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
     float speed; //move speed of camera in m/s
     Vector3f translation;
   public: 
         // I want this to be synced with git repo!.

      FreeCamera( eProjectionType projectionType, float nearLeft, float nearRight,
         float nearTop, float nearBottom, float nearDist, float farDist )
      {
         orientation.Set(0.0f, 0.0f, 0.0f, 0.0f);
         isDirty = true;
         this->projectionType = projectionType;
         this->nearLeft = nearLeft;
         this->nearRight = nearRight;
         this->nearTop = nearTop;
         this->nearBottom = nearBottom;
         this->nearDist = nearDist;
         this->farDist = farDist;

         translation = Vector3f(0.0f, 0.0f, 0.0f);
         position = Vector3f(0.0f, 0.0f, 0.0f);
         forward = Vector3f(0.0f, 0.0f, -1.0f);
         up = Vector3f(0.0f, 1.0f, 0.0f);
         right = Vector3f(1.0f, 0.0f, 0.0f);

         viewMatrix = Matrix4f::IDENTITY;
      }
      void Update();
      void Rotate(float angle, Vector3f axis);

      void Walk(const float dt);
      void Strafe(const float dt);
      void Lift(const float dt);


      void SetSpeed(const float speed);
      const float GetSpeed();

      void SetTranslation(const Vector3f &t);
      Vector3f GetTranslation() const;
   };

} // camera