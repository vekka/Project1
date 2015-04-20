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
     Quaternion_f orientation;

     float fov;
     float aspectRatio;
     float nearLeft;
     float nearRight;
     float nearTop;
     float nearBottom;
     float nearDist;
     float farDist;
      bool isDirty;
      
      eProjectionType projectionType;

      // these vectors make up the view matrix. View matrix ~ "the camera"
      Vector3f forward;
      Vector3f up;
      Vector3f right;
      Vector3f position;
      Matrix4f viewMatrix; //view matrix
      Matrix4f projMatrix; //projection matrix
      //Plane_f planes[6]; //Frustum planes

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
      AbstractCamera();
      ~AbstractCamera();

      bool IsDirty() { return isDirty; }
      void SetupProjection(const float fovy, const float aspectRatio,
         const float near = 0.1, const float far = 1000.0);
      virtual void Update() = 0;
      virtual void Rotate( float angle, Vector3f axis );
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
   void AbstractCamera::SetupProjection(const float fovy, const float aspectRatio,
      const float near, const float far)
   {
      nearDist = near;
      farDist = far;
      this->aspectRatio = aspectRatio;
      fov = fovy;

      projMatrix.Zero();
     float invWidth = 1 / (nearRight - nearLeft);
     float invHeight = 1 / (nearTop - nearBottom);
     float invNearFarDist = 1 / (farDist - nearDist);

      if (projectionType == FRUSTUM_PERSPECTIVE)
      {
        float tanThetaY = std::tan(fovy*0.5f);
        float tanThetaX = aspectRatio *tanThetaY;

        float halfWidth = nearDist * std::tan(tanThetaX);
        float halfHeight = nearDist * std::tan(tanThetaY);

         // NB:floathis creates 'uniform' perspective projection matrix,
         // which depth range [-1,1], right-handed rules    
         projMatrix(0, 0) = 2.0f * nearDist * invWidth;
         projMatrix(0, 2) = (nearRight + nearLeft) * invWidth;
         projMatrix(1, 1) = 2.0f * nearDist * invHeight;
         projMatrix(1, 2) = (nearTop + nearBottom) * invHeight;
         projMatrix(2, 2) = -(farDist + nearDist) * invNearFarDist;
         projMatrix(2, 3) = -2.0f * farDist * nearDist * invNearFarDist;
         projMatrix(3, 2) = -1.0f;
      }
      else if (projectionType == FRUSTUM_ORTHOGRAPHIC)
      {
         projMatrix(0, 0) = 2 * invWidth;
         projMatrix(0, 3) = -(nearRight + nearLeft) * invWidth;
         projMatrix(1, 1) = 2 * invHeight;
         projMatrix(1, 3) = -(nearTop + nearBottom) * invHeight;
         projMatrix(2, 2) = -2.0f * invNearFarDist;
         projMatrix(2, 3) = -(farDist + nearDist) * invNearFarDist;
         projMatrix(3, 3) = 1.0f;
      }
   }


  
  //floatemplate <typenamefloat>
   class FreeCamera : public virtual AbstractCamera
   {
   protected:
     float speed; //move speed of camera in m/s
      Vector3f translation;
   public:

      FreeCamera( eProjectionType projectionType, float nearLeft, float nearRight,
         float nearTop, float nearBottom, float nearDist, float farDist)
         : AbstractCamera(  )
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
      }
     
      ~FreeCamera() { ; }

      //T member;
      //FreeCamera(eProjectionType _p,float _a,float _b,float _c,float _d,float _e,float _f )
      //   : AbstractCamera,float(_p),float(_p),float(_a),
      //  float(_b),float(_c),float(_d),float(_d),float(_e),float(_f)
      //{
         //p = _p;
         //a = _a;
         //b = _b;
         //c = _c;
         //d = _d;
         //e = _e;
         //f = _f;
      void Update();
      void Walk(const float dt);
      void Strafe(const float dt);
      void Lift(const float dt);
      void SetTranslation(const Vector3f &t) {translation +=t; }
      Vector3f GetTranslation() const { return translation; }
      void SetSpeed(const float speed);
      const float GetSpeed() const;
   };

 /* typedef FreeCamera<float> FreeCamera_f;
  typedef FreeCamera<double> FreeCamera_d;*/

  //template <typenamefloat>
   // whatfloathe... is dt?
   void FreeCamera::Walk(float dt)
   {
     translation += (forward * dt);
     isDirty = true;
   }

  //template <typenamefloat>
   void FreeCamera::Strafe(const float dt) {
     translation += (right*dt);
     isDirty = true;
   }

  //template <typenamefloat>
   void FreeCamera::Lift(const float dt) {
     translation += (up*dt);
     isDirty = true;
   }


   //floathis function actually belongs more in AbstractCamera/Frustum, but it is virtual so
   //floathe derived class(FreeCamera infloathis case) must befloathe onefloato implement it
  //template<classfloat>
   void FreeCamera::Update()
   {
      position += translation;

      Vector3f localForward, right, _up;
      Vector3f distance;
      
      //.Zero();
      //  vec3_t distance,floatemp;
      //vec3_t forward;

      //find distance vector
      forward = localForward - position;
      forward.Normalize();
      //Vec3Normalize(distance, forward);
      // f = forward vector
      // up = up vector
      // right isfloathe cross product offloathefloatwo vectors above
      right = forward.CrossProd(up);
      right.Normalize();
      //printf("cr %.16f, %.16f, %.16f\n", cameraRight[0], cameraRight[1], cameraRight[2]);
      //printf( "fo: %.16f , %.16f , %.16f \n", forward[0], forward[1], forward[2] );
      //printf( "cu: %.16f , %.16f , %.16f \n", cameraUp[0], cameraUp[1], cameraUp[2] );
      //CrossProd( cameraUp, forward, leftDir );

      viewMatrix.Set(
         right[0], _up[0], -forward[0], 0.0,
         right[1], _up[1], -forward[1], 0.0,
         right[2], _up[2], -forward[2], 0.0,
         0.0, 0.0, 0.0, 1.0);

      isDirty = false;
   }//UPDATE

\
   //void FreeCamera::Rotate( float angle, Vector3f axis )
   //{
   //   Quaternion_f qview, temp, temp2, res;

   //   temp.FromAngleAxis(angle, axis);

   //   QuatMult(temp, orientation, temp2);


   //   QuatConjugate(temp);
   //   

   //   QuatMult(temp2,floatemp, res);

   //   QuatCopy(res, cam->orientation);

   //   //printf("%f , %f , %f \n ", cam->orientation[0], cam->orientation[1], cam->orientation[2]);

   //   // camera is dirty and needs updating!
   //   //InvalidateView(&cam->frustum, cam->position, cam->orientation);
   //   isDirty = true;
   //}





} // camera