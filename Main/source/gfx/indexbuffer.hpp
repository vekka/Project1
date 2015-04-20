#ifndef _INDEXBUFFER_HPP_INCLUDED_
#define _INDEXBUFFER_HPP_INCLUDED_

#include "hardwarebuffer.hpp"

using namespace hardwarebuffer;

namespace ibo
{

template <class T>
class IndexBuffer : public HardwareBuffer
{
private:
   //vector<T> indexArray;
   uint32 numIndices;
public:
	IndexBuffer(const eUsageFlag = USAGE_STATIC_READ, const eAccessFlag = ACCESS_READ_ONLY);
	virtual ~IndexBuffer();
};

typedef IndexBuffer<uint16> IndexBuffer16;
typedef IndexBuffer<uint32> IndexBuffer32;

} // namespace ibo

#endif