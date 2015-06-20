

#include "transformationPipeline.hpp"

namespace pipeline
{

   const Matrix4f& Pipeline::GetWorldTrans()
   {
      Matrix4f scaleTrans, rotateTrans, translationTrans;

      scaleTrans = Matrix4f::IDENTITY;
      rotateTrans = Matrix4f::IDENTITY;
      translationTrans = Matrix4f::IDENTITY;

      scaleTrans.SetScale(m_scale.x, m_scale.y, m_scale.z);
      rotateTrans.SetRotationRadians(m_rotateInfo);
      translationTrans.SetTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);

      m_Wtransformation = translationTrans * rotateTrans * scaleTrans;
      return m_Wtransformation;
   }


   const Matrix4f &Pipeline::InitCameraTransform(const Vector3f& target, const Vector3f& up)
   {

      Vector3f n = target;
      n.Normalize();
      Vector3f u = up;

      u.Normalize();
      u = u.CrossProd(n);
      Vector3f v = n.CrossProd(u);

      m_CameraTransformation(0,0) = u.x;
      m_CameraTransformation(0,1) = u.y;   
      m_CameraTransformation(0,2) = u.z;
      m_CameraTransformation(0,3) = 0.0f;
      m_CameraTransformation(1,0) = v.x;   
      m_CameraTransformation(1,1) = v.y;
      m_CameraTransformation(1,2) = v.z;
      m_CameraTransformation(1,3) = 0.0f;
      m_CameraTransformation(2,0) = n.x;
      m_CameraTransformation(2,1) = n.y;
      m_CameraTransformation(2,2) = n.z;
      m_CameraTransformation(2,3) = 0.0f;
      m_CameraTransformation(3,0) = 0.0f;
      m_CameraTransformation(3,1) = 0.0f;
      m_CameraTransformation(3,2) = 0.0f;
      m_CameraTransformation(3,3) = 1.0f;
      return m_CameraTransformation;
   }

   const Matrix4f& Pipeline::GetViewTrans(const Vector3f &pos, const Vector3f &target, const Vector3f up)
   {
      Matrix4f CameraTranslationTrans, CameraRotateTrans;
      
      CameraTranslationTrans = Matrix4f::IDENTITY;
      CameraRotateTrans = Matrix4f::IDENTITY;
      
      CameraTranslationTrans.SetTranslation(-pos.x, pos.y, pos.z);
      CameraRotateTrans = InitCameraTransform(target, up);

      m_Vtransformation = CameraTranslationTrans*CameraRotateTrans;

      return m_Vtransformation;
   }

}

