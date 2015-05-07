#ifndef _SHADERTYPES_HPP_INCLUDED_
#define _SHADERTYPES_HPP_INCLUDED_

#include "core/BasicTypes.hpp"

#include <glew.h>

namespace oglshader
{

   enum eScalarType
   {
      TYPE_BOOL = GL_BOOL,
      TYPE_BYTE = GL_BYTE,
      TYPE_SHORT = GL_SHORT,
      TYPE_INT = GL_INT,
      TYPE_FLOAT = GL_FLOAT,
      TYPE_DOUBLE = GL_DOUBLE,
      TYPE_HALF_FLOAT = GL_HALF_FLOAT,
      TYPE_UBYTE = GL_UNSIGNED_BYTE,
      TYPE_USHORT = GL_UNSIGNED_SHORT,
      TYPE_UINT = GL_UNSIGNED_INT
   };

   enum eVectorType
   {
      TYPE_FVEC1,
      TYPE_FVEC2 = GL_FLOAT_VEC2,
      TYPE_FVEC3 = GL_FLOAT_VEC3,
      TYPE_FVEC4 = GL_FLOAT_VEC4,
      TYPE_IVEC1 = 1,
      TYPE_IVEC2 = GL_INT_VEC2,
      TYPE_IVEC3 = GL_INT_VEC3,
      TYPE_IVEC4 = GL_INT_VEC4,
      TYPE_UIVEC1 = 2,
      TYPE_UIVEC2 = GL_UNSIGNED_INT_VEC2,
      TYPE_UIVEC3 = GL_UNSIGNED_INT_VEC3,
      TYPE_UIVEC4 = GL_UNSIGNED_INT_VEC4,
      TYPE_DVEC1 = 3,
      TYPE_DVEC2 = GL_DOUBLE_VEC2,
      TYPE_DVEC3 = GL_DOUBLE_VEC3,
      TYPE_DVEC4 = GL_DOUBLE_VEC4,
      TYPE_BVEC1 = 4,
      TYPE_BVEC2 = GL_BOOL_VEC2,
      TYPE_BVEC3 = GL_BOOL_VEC3,
      TYPE_BVEC4 = GL_BOOL_VEC4
   };

   enum eMatrixType
   {
      TYPE_FMAT2 = GL_FLOAT_MAT2,
      TYPE_FMAT3 = GL_FLOAT_MAT3,
      TYPE_FMAT4 = GL_FLOAT_MAT4,
      TYPE_FMAT2x3 = GL_FLOAT_MAT2x3,
      TYPE_FMAT2x4 = GL_FLOAT_MAT2x4,
      TYPE_FMAT3x2 = GL_FLOAT_MAT3x2,
      TYPE_FMAT3x4 = GL_FLOAT_MAT3x4,
      TYPE_FMAT4x2 = GL_FLOAT_MAT4x2,
      TYPE_FMAT4x3 = GL_FLOAT_MAT4x3
   };

   enum eOpaqueType
   {
      TYPE_SAMPLER_1D = GL_SAMPLER_1D,
      TYPE_SAMPLER_2D = GL_SAMPLER_2D,
      TYPE_SAMPLER_3D = GL_SAMPLER_3D,
      TYPE_SAMPLER_1D_ARRAY = GL_SAMPLER_1D_ARRAY,
      TYPE_SAMPLER_2D_ARRAY = GL_SAMPLER_2D_ARRAY,
      TYPE_SAMPLER_CUBE = GL_SAMPLER_CUBE,
      TYPE_SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW,
      TYPE_SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,
      TYPE_SAMPLER_CUBE_SHADOW = GL_SAMPLER_CUBE_SHADOW
   };

   struct TypeAssociation
   {
      const char *str;
      int32 strlen;
      int32 type;
   };

   static struct TypeAssociation types[] =
   {
      // scalars
      { "bool", 4, TYPE_BOOL },
      { "byte", 4, TYPE_BYTE },
      { "short", 5, TYPE_SHORT },
      { "int", 3, TYPE_INT },
      { "float", 5, TYPE_FLOAT },
      { "double", 6, TYPE_DOUBLE },
      { "half", 4, TYPE_HALF_FLOAT }, // NOTE: not an actual GLSL type
      { "ubyte", 5, TYPE_UBYTE },
      { "ushort", 6, TYPE_USHORT },
      { "uint", 4, TYPE_UINT },

      //vectors
      { "vec2", 4, TYPE_FVEC2 },
      { "vec3", 4, TYPE_FVEC3 },
      { "vec4", 4, TYPE_FVEC4 },
      { "ivec2", 5, TYPE_IVEC2 },
      { "ivec3", 5, TYPE_IVEC3 },
      { "ivec4", 5, TYPE_IVEC4 },
      { "uivec2", 6, TYPE_UIVEC2 },
      { "uivec3", 6, TYPE_UIVEC3 },
      { "uivec4", 6, TYPE_UIVEC4 },
      { "bvec2", 5, TYPE_BVEC2 },
      { "bvec3", 5, TYPE_BVEC3 },
      { "bvec4", 5, TYPE_BVEC4 },
      { "dvec2", 5, TYPE_DVEC2 },
      { "dvec3", 5, TYPE_DVEC3 },
      { "dvec4", 5, TYPE_DVEC4 },

      //matrices
      { "mat2", 4, TYPE_FMAT2 },
      { "mat3", 4, TYPE_FMAT3 },
      { "mat4", 4, TYPE_FMAT4 },
      { "mat2x3", 6, TYPE_FMAT2x3 },
      { "mat2x4", 6, TYPE_FMAT2x4 },
      { "mat3x2", 6, TYPE_FMAT3x2 },
      { "mat3x4", 6, TYPE_FMAT3x4 },
      { "mat4x2", 6, TYPE_FMAT4x2 },
      { "mat4x3", 6, TYPE_FMAT4x3 },

      //opaque
      { "sampler1D", 9, TYPE_SAMPLER_1D },
      { "sampler2D", 9, TYPE_SAMPLER_2D },
      { "sampler1DArray", 14, TYPE_SAMPLER_1D_ARRAY },
      { "sampler2DArray", 14, TYPE_SAMPLER_2D_ARRAY },
      { "sampler3D", 9, TYPE_SAMPLER_3D },
      { "samplerCube", 9, TYPE_SAMPLER_CUBE },
      { "samplerCubeShadow", 9, TYPE_SAMPLER_CUBE_SHADOW }
      //{ NULL, 0, 0 },
   };

   int32 TypeSizeof(int type);
   const char *const GetTypeString(int32 type);
   int32 GetNumElements(int32 type);

   //bool IsMatrix(eType type)
   //{
   //	return (type == TYPE_FMAT2 || type == TYPE_FMAT3 || type == TYPE_FMAT4 ||
   //		type == TYPE_FMAT2x3 || type == TYPE_FMAT2x4 || type == TYPE_FMAT3x2 ||
   //		type == TYPE_FMAT3x4 || type == TYPE_FMAT4x2 || type == TYPE_FMAT4x3);
   //}

} // namespace oglshader

#endif