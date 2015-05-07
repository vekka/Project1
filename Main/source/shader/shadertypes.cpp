#include "shadertypes.hpp"

namespace oglshader
{

   const char *const GetTypeString(int32 type)
   {
      switch (type)
      {
         //scalar types
      case TYPE_BOOL: return "bool";
      case TYPE_BYTE: return "byte";
      case TYPE_SHORT: return "short";
      case TYPE_INT: return "int";
      case TYPE_FLOAT: return "float";
      case TYPE_DOUBLE: return "double";
      case TYPE_HALF_FLOAT: return "half";
      case TYPE_UBYTE: return "ubyte";
      case TYPE_USHORT: return "ushort";
      case TYPE_UINT: return "uint";

         //vector types
      case TYPE_FVEC2: return "vec2";
      case TYPE_FVEC3: return "vec3";
      case TYPE_FVEC4: return "vec4";
      case TYPE_IVEC2: return "ivec2";
      case TYPE_UIVEC2: return "uivec2";
      case TYPE_IVEC3: return "ivec3";
      case TYPE_UIVEC3: return "uivec3";
      case TYPE_IVEC4: return "ivec4";
      case TYPE_UIVEC4: return "uivec4";
      case TYPE_BVEC2: return "bvec2";
      case TYPE_BVEC3: return "bvec3";
      case TYPE_BVEC4: return "bvec4";
      case TYPE_DVEC2: return "dvec2";
      case TYPE_DVEC3: return "dvec3";
      case TYPE_DVEC4: return "dvec4";

         //matrix types
      case TYPE_FMAT2: return "mat2";
      case TYPE_FMAT3: return "mat3";
      case TYPE_FMAT4: return "mat4";
      case TYPE_FMAT2x3: return "mat2x3";
      case TYPE_FMAT2x4: return "mat2x4";
      case TYPE_FMAT3x2: return "mat3x2";
      case TYPE_FMAT3x4: return "mat3x4";
      case TYPE_FMAT4x2: return "mat4x2";
      case TYPE_FMAT4x3: return "mat4x3";

         //opaque types
      case TYPE_SAMPLER_1D: return "sampler1D";
      case TYPE_SAMPLER_2D: return "sampler2D";
      case TYPE_SAMPLER_3D: return "sampler3D";
      case TYPE_SAMPLER_1D_ARRAY: return "sampler1DArray";
      case TYPE_SAMPLER_2D_ARRAY: return "sampler2DArray";

      default:
         return NULL;
      }
   }

   int32 TypeSizeof(int type)
   {
      switch (type)
      {
         //scalar types
      case TYPE_BOOL: return sizeof(bool);
      case TYPE_BYTE: return sizeof(int8);
      case TYPE_SHORT: return sizeof(int16);
      case TYPE_INT: return sizeof(int32);
      case TYPE_FLOAT: return sizeof(float);
      case TYPE_DOUBLE: return sizeof(double);
      case TYPE_HALF_FLOAT: return (sizeof(float) / 2);
      case TYPE_UBYTE: return sizeof(uint8);
      case TYPE_USHORT: return sizeof(uint16);
      case TYPE_UINT: return sizeof(uint32);

         //vector types
      case TYPE_FVEC2: return sizeof(float) * 2;
      case TYPE_FVEC3: return sizeof(float) * 3;
      case TYPE_FVEC4: return sizeof(float) * 4;
      case TYPE_IVEC2:
      case TYPE_UIVEC2:
         return sizeof(int) * 2;
      case TYPE_IVEC3:
      case TYPE_UIVEC3:
         return sizeof(int) * 3;
      case TYPE_IVEC4:
      case TYPE_UIVEC4:
         return sizeof(int) * 4;
      case TYPE_BVEC2: return sizeof(bool) * 2;
      case TYPE_BVEC3: return sizeof(bool) * 3;
      case TYPE_BVEC4: return sizeof(bool) * 4;
      case TYPE_DVEC2: return sizeof(double) * 2;
      case TYPE_DVEC3: return sizeof(double) * 3;
      case TYPE_DVEC4: return sizeof(double) * 4;

         //matrix types
      case TYPE_FMAT2: return sizeof(float) * 2 * 2;
      case TYPE_FMAT3: return sizeof(float) * 3 * 3;
      case TYPE_FMAT4: return sizeof(float) * 4 * 4;
      case TYPE_FMAT2x3: return sizeof(float) * 2 * 3;
      case TYPE_FMAT2x4: return sizeof(float) * 2 * 4;
      case TYPE_FMAT3x2: return sizeof(float) * 3 * 2;
      case TYPE_FMAT3x4: return sizeof(float) * 3 * 4;
      case TYPE_FMAT4x2: return sizeof(float) * 4 * 2;
      case TYPE_FMAT4x3: return sizeof(float) * 4 * 3;

      default:
         return 0;
      }
   }

   int32 GetNumElements(int32 type)
   {
      switch (type)
      {
      case TYPE_FVEC2:
      case TYPE_IVEC2:
      case TYPE_UIVEC2:
      case TYPE_DVEC2:
      case TYPE_BVEC2:
         return 2;
      case TYPE_FVEC3:
      case TYPE_IVEC3:
      case TYPE_UIVEC3:
      case TYPE_DVEC3:
      case TYPE_BVEC3:
         return 3;
      case TYPE_FVEC4:
      case TYPE_IVEC4:
      case TYPE_UIVEC4:
      case TYPE_DVEC4:
      case TYPE_BVEC4:
         return 4;
      }
      return 1;
   }

} // namespace oglshader