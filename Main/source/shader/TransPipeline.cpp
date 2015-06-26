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

      Vector3f n = target;
      n.Normalize();

      Vector3f u = up;
      u.Normalize();
      u = u.CrossProd(n);

      Vector3f v = n.CrossProd(u);

      m_cameraTransformation =
      { 
         u.x, u.y, u.z, 0.0f,
         v.x, v.y, v.z, 0.0f,
         n.x, n.y, n.z, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
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
      Matrix4f cameraTranslationMatrix, cameraRotateTrans;
      
      cameraTranslationMatrix = Matrix4f::IDENTITY;
      cameraRotateTrans = Matrix4f::IDENTITY;
      
      
      cameraTranslationMatrix.SetTranslation(-m_camera.pos.x, -m_camera.pos.y, -m_camera.pos.z);
       
      cameraRotateTrans = InitCameraTransform(m_camera.target, m_camera.up);
      
      m_Vtransformation = cameraRotateTrans * cameraTranslationMatrix;

      return m_Vtransformation;
   }

}

