#ifndef _HARDWAREBUFFER_HPP_INCLUDED_
#define _HARDWAREBUFFER_HPP_INCLUDED_

#include "core/BasicTypes.hpp"
#include "../opengl/ogldriver.hpp"

namespace hardwarebuffer
{

   enum eUsageFlag
   {
      USAGE_STREAM_DRAW = GL_STREAM_DRAW,
      USAGE_STREAM_READ = GL_STREAM_READ,
      USAGE_STREAM_COPY = GL_STREAM_COPY,
      USAGE_STATIC_DRAW = GL_STATIC_DRAW,
      USAGE_STATIC_READ = GL_STATIC_READ,
      USAGE_STATIC_COPY = GL_STATIC_COPY,
      USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
      USAGE_DYNAMIC_READ = GL_DYNAMIC_READ,
      USAGE_DYNAMIC_COPY = GL_DYNAMIC_COPY
   };

   enum eAccessFlag
   {
      ACCESS_READ_ONLY = GL_READ_ONLY,
      ACCESS_WRITE_ONLY = GL_WRITE_ONLY,
      ACCESS_READWRITE = GL_READ_WRITE
   };

   enum eBufferBindingTarget
   {
      BBTARGET_ARRAY_BUFFER = GL_ARRAY_BUFFER,
      BBTARGET_ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
      BBTARGET_COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
      BBTARGET_COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
      BBTARGET_DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
      BBTARGET_DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
      BBTARGET_PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
      BBTARGET_PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
      BBTARGET_ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
      BBTARGET_QUERY_BUFFER = GL_QUERY_BUFFER,
      BBTARGET_SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
      BBTARGET_TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
      BBTARGET_TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
      BBTARGET_UNIFORM_BUFFER = GL_UNIFORM_BUFFER
   };

   class HardwareBuffer
   {
   protected:
      uint32 handle;
      int32 offset;
      eUsageFlag usageFlag;
      eAccessFlag accessFlag;
      std::string format;
      eBufferBindingTarget bufferBindingTarget;
   public:

      void Allocate(const uint32 size);
      // write to buffer according to the format alignments
      void WriteBuffer(const float sourceData[], const int32 numElements);
      void Bind();
      void Unbind();

      void Free();
   };

} // namespace hardwarebuffer

#endif