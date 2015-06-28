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


   const Matrix4f &Pipeline::InitCameraTransform(const Vector3f &position,const Vector3f& target, const Vector3f& up)
   {
      Vector3f forward, _up, right;
   
      forward = target - position;
      forward.Normalize();

      right = forward.CrossProd(up);
      right.Normalize();

      _up = right.CrossProd(forward);
      _up.Normalize();

    
      Vector3f temp_right = right;
      Vector3f temp_up = _up;
      Vector3f temp_forward = forward;

      m_cameraTransformation =
      { 
         right.x, _up.x, forward.x, 0.0f,
         right.y, _up.y, forward.y, 0.0f,
         right.z, _up.z, forward.z, 0.0f,
         0.0f,0.0f,0.0f,1.0f //-temp_right.DotProd(position), -temp_up.DotProd(position), -temp_forward.DotProd(position), 1.0f
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

      cameraTranslation.SetTranslation(m_camera.pos);
     
      cameraRotation = InitCameraTransform(m_camera.pos, m_camera.target, m_camera.up);


      m_Vtransformation = cameraRotation*cameraTranslation;

      return m_Vtransformation;
   }

}

