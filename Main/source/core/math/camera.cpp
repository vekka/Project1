#include "core/math/camera.hpp"

namespace camera
{


   void AbstractCamera::SetupProjection(const float fovy, const float aspectRatio)
   {
      this->aspectRatio = aspectRatio;
      this->fov = fovy;

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

    //void FreeCamera::Walk(const float dt)
    //{
    //   translation += (forward * dt);
    //  isDirty = true;
    //}

   //template <typenamefloat>
    //void FreeCamera::Strafe(const float dt) 
    //{
    //  translation += (right*dt);
    //  isDirty = true;
    //}

   //template <typenamefloat>
    //void FreeCamera::Lift(const float dt) 
    //{
    //  translation += (up*dt);
    //  isDirty = true;
    //}



    void FreeCamera::SetTranslation(const Vector3f &t)
    {
         translation += t; 
    }

    Vector3f FreeCamera::GetTranslation() const
    { 
       return translation; 
    }

    void FreeCamera::SetSpeed(const float speed)
    { 
       this->speed = speed; 
    }

    const float FreeCamera::GetSpeed()
    { 
       return speed; 
    }

   //quaternion multiply is not impl. this function should rotate forward vector by using Quaternion multiplication
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