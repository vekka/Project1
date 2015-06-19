#include "quaternion.hpp"

namespace core
{
    namespace math
    {
  		Quaternion<T> Quaternion<T>::Inverse(void)
  		{
  		   float magnitude = x*x + y*y + z*z + w*w;
  		   assert(!core::IsZero(magnitude));
  		   float invMagnitude;
  		   if( core::Equals(magnitude, T(1)) )    // special case: unit quaternion
  		   {
  		      x = -x;
  		      y = -y;
  		      z = -z;
  		   }
  		   else // else scale
  		   {
  		      if( core::IsZero(magnitude) )
  		         invMagnitude = T(1);
  		      else
  		         invMagnitude = T(1) / magnitude;
  		      x *= -invMagnitude;
  		      y *= -invMagnitude;
  		      z *= -invMagnitude;
  		      w *= invMagnitude;
  		   }
  		   return *this;
  		}   
    } // namespace math
} // namespace core
