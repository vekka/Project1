#include "frustum.hpp"

namespace core
{

   namespace math
   {

      namespace frustum
      {

         Frustum::Frustum()
         {
         }

         Frustum::Frustum(
            eProjectionType projectionType,
            float nearLeft, float nearRight,
            float nearTop, float nearBottom,
            float nearDist, float farDist)
         {
            assert(projectionType == FRUSTUM_PERSPECTIVE || projectionType == FRUSTUM_ORTHOGRAPHIC);
            assert(!Equals(nearLeft, nearRight));
            assert(!Equals(nearTop, nearBottom));
            assert(!Equals(farDist, nearDist));

            this->projectionType = projectionType;
            this->nearLeft = nearLeft;
            this->nearRight = nearRight;
            this->nearTop = nearTop;
            this->nearBottom = nearBottom;
            this->nearDist = nearDist;
            this->farDist = farDist;
         }

         Frustum::Frustum(const Frustum &other)
         {
            cameraPos = other.cameraPos;
            aabbox = other.aabbox;
            viewMatrix = other.viewMatrix;
            projMatrix = other.projMatrix;

            for (int32 i = 0; i < 6; i++)
               planes[i] = other.planes[i];
         }

         void Frustum::Update()
         {
            projMatrix.Zero();
            float invWidth = 1 / (nearRight - nearLeft);
            float invHeight = 1 / (nearTop - nearBottom);
            float invNearFarDist = 1 / (farDist - nearDist);

            if (projectionType == FRUSTUM_PERSPECTIVE)
            {
               float tanThetaY = std::tan(fovy*0.5f);
               float tanThetaX = aspectRatio * tanThetaY;

               float halfWidth = nearDist * std::tan(tanThetaX);
               float halfHeight = nearDist * std::tan(tanThetaY);

               // NB: This creates 'uniform' perspective projection matrix,
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

         Matrix4f &Frustum::GetProjectionMatrix()
         {
            Update();

            return projMatrix;
         }

      } // namespace frustum

   } // namespace math

} // namespace core