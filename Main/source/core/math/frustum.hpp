#ifndef _FRUSTUM_HPP_INCLUDED_
#define _FRUSTUM_HPP_INCLUDED_

#include "matrix4.hpp"
#include "aabbox.hpp"

namespace core
{

   namespace math
   {

      namespace frustum
      {

         enum eProjectionType
         {
            FRUSTUM_PERSPECTIVE,
            FRUSTUM_ORTHOGRAPHIC
         };

         enum eFrustumPlanes
         {
            FRUSTUM_FAR_PLANE,
            FRUSTUM_NEAR_PLANE,
            FRUSTUM_LEFT_PLANE,
            FRUSTUM_RIGHT_PLANE,
            FRUSTUM_BOTTOM_PLANE,
            FRUSTUM_TOP_PLANE
         };

         class Frustum
         {
         private:
            Matrix4f viewMatrix;
            Matrix4f projMatrix;

            eProjectionType projectionType;
            //When true, points, planes and bounds must be re - calculated before use.
            bool dirty;

            float fovy;
            float aspectRatio;

            float nearLeft;
            float nearRight;
            float nearTop;
            float nearBottom;
            float nearDist;
            float farDist;
         public:
            AABBox_f aabbox;
            Vector3f cameraPos;
            Plane_f planes[6];

            Frustum();
            Frustum(
               eProjectionType,
               float nearLeft, float nearRight,
               float nearTop, float nearBottom,
               float nearDist, float farDist);

            Frustum(const Frustum &other);

            void Update();
            void Transform(const Matrix4f &);
            void Create(const Matrix4f &);

            Matrix4f &GetProjectionMatrix();

            Point3f GetFarLeftTop() const;
            Point3f GetFarLeftBottom() const;
            Point3f GetFarRightTop() const;
            Point3f GetFarRightBottom() const;
            Point3f GetNearLeftTop() const;
            Point3f GetNearLeftBottom() const;
            Point3f GetNearRightTop() const;
            Point3f GetNearRightBottom() const;

            const AABBox_f &GetBoundingBox() const;
            void RecalcBoundingBox() const;
         };

         inline void Frustum::Transform(const Matrix4f &mat4)
         {
         }

         inline void Frustum::Create(const Matrix4f &mat4)
         {
         }

         inline Point3f Frustum::GetFarLeftTop() const
         {
         }

         inline Point3f Frustum::GetFarLeftBottom() const
         {
         }

         inline Point3f Frustum::GetFarRightTop() const
         {
         }

         inline Point3f Frustum::GetFarRightBottom() const
         {
         }

         inline Point3f Frustum::GetNearLeftTop() const
         {
         }

         inline Point3f Frustum::GetNearLeftBottom() const
         {
         }

         inline Point3f Frustum::GetNearRightTop() const
         {
         }

         inline Point3f Frustum::GetNearRightBottom() const
         {
         }

         inline const AABBox_f &Frustum::GetBoundingBox() const
         {
            return aabbox;
         }

         inline void Frustum::RecalcBoundingBox() const
         {
            //aabbox.Reset(cameraPosition);

            //aabbox.AddInternalPoint(GetFarLeftUp());
            //aabbox.AddInternalPoint(GetFarRightUp());
            //aabbox.AddInternalPoint(GetFarLeftDown());
            //aabbox.AddInternalPoint(GetFarRightDown());
         }

      } // namespace frustum

   } // namespace math

} // namespace core

#endif