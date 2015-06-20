

#include "transformationPipeline.hpp"

namespace pipeline
{

   const Matrix4f& Pipeline::GetWorldTrans()
   {
     // Matrix4f world = Matrix4f::IDENTITY;
      return this->m_WPtransformation;
   }


}