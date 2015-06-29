#include "TransPipeline.hpp"

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
      Vector3f forward, _up, right;
   
      forward = target;
      forward.Normalize();


      Vector3f temp = up;
      temp.Normalize();
      right = target.CrossProd(temp);

      _up = right.CrossProd(forward);
      _up.Normalize();

      m_cameraTransformation =
      { 
         right.x, _up.x, forward.x, 0.0f,
         right.y, _up.y, forward.y, 0.0f,
         right.z, _up.z, forward.z, 0.0f,
         0.0f,0.0f,0.0f,1.0f
      };

      
      //m_cameraTransformation(0,0) = u.x;
      //m_cameraTransformation(0,1) = u.y;   
      //m_cameraTransformation(0,2) = u.z;
      //m_cameraTransformation(0,3) = 0.0f;

      //m_cameraTransformation(1,0) = v.x;   
      //m_cameraTransformation(1,1) = v.y;
      //m_cameraTransformation(1,2) = v.z;

      //m_cameraTransformation(1,3) = 0.0f;
      //m_cameraTransformation(2,0) = n.x;
      //m_cameraTransformation(2,1) = n.y;
      //m_cameraTransformation(2,2) = n.z;

      //m_cameraTransformation(2,3) = 0.0f;
      //m_cameraTransformation(3,0) = 0.0f;
      //m_cameraTransformation(3,1) = 0.0f;
      //m_cameraTransformation(3,2) = 0.0f;
      //m_cameraTransformation(3,3) = 1.0f;

      return m_cameraTransformation;
   }

   const Matrix4f &Pipeline::GetViewTrans()
   {
      Matrix4f cameraRotation, cameraTranslation;
      
      cameraTranslation = Matrix4f::IDENTITY;
      cameraRotation = Matrix4f::IDENTITY;
      m_Vtransformation = Matrix4f::IDENTITY;

      cameraTranslation.SetTranslation(m_camera.pos.x, m_camera.pos.y, m_camera.pos.z);
      cameraTranslation = cameraTranslation.Transpose();
      cameraRotation = InitCameraTransform(m_camera.target, m_camera.up);
      m_Vtransformation = cameraTranslation * cameraRotation;

      return m_Vtransformation;
   }


   const Matrix4f &Pipeline::GetWVPTrans()
   {
      Matrix4f scaleTrans, rotateTrans, translationTrans;
      Matrix4f cameraRotation, cameraTranslation;

      scaleTrans = Matrix4f::IDENTITY;
      rotateTrans = Matrix4f::IDENTITY;
      translationTrans = Matrix4f::IDENTITY;
      cameraTranslation = Matrix4f::IDENTITY;
      cameraRotation = Matrix4f::IDENTITY;
      m_Vtransformation = Matrix4f::IDENTITY;
      m_WVPtransformation = Matrix4f::IDENTITY;

      //World transforms
      scaleTrans.SetScale(m_scale.x, m_scale.y, m_scale.z);
      rotateTrans.SetRotationRadians(m_rotateInfo);
      translationTrans.SetTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);

      //camera transforms
      cameraTranslation.SetTranslation(m_camera.pos.x, m_camera.pos.y, m_camera.pos.z);
      cameraTranslation = cameraTranslation.Transpose();
      cameraRotation = InitCameraTransform(m_camera.target, m_camera.up);

      m_projectionMatrix = m_projectionMatrix.Transpose();
      m_WVPtransformation = cameraTranslation * cameraRotation * m_projectionMatrix;// *rotateTrans * translationTrans;
      return m_WVPtransformation;
   }

}

