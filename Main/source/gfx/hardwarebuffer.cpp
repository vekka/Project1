
#include "hardwarebuffer.hpp"

using namespace hardwarebuffer;

void HardwareBuffer::Allocate(const uint32 size)
{
	glBindBuffer(bufferBindingTarget, handle);
	glBufferData(bufferBindingTarget, size, NULL, usageFlag);
	glBindBuffer(bufferBindingTarget, 0);
}

void HardwareBuffer::Bind()
{
	glBindBuffer(bufferBindingTarget, handle);
}

void HardwareBuffer::Unbind()
{
	glBindBuffer(bufferBindingTarget, 0);
}


void HardwareBuffer::WriteBuffer(const float sourceData[], const int32 numElements)
{
	glBindBuffer(bufferBindingTarget, handle);

	glBufferSubData(bufferBindingTarget, offset, numElements*sizeof(float), sourceData);
	offset += numElements*sizeof(float);
	if (format == "PN" || format == "PT" || format == "PC")
	{
	}
	else if (format == "PTT" || format == "PCT")
	{
	}
	else if (format == "PTTT")
	{
	}
	else if (format == "PCTT" || format == "PNTT")
	{
	}
	else if (format == "PNT")
	{
	}
	else if (format == "PCNT")
	{
	}

	glBindBuffer(bufferBindingTarget, 0);
}

void HardwareBuffer::Free()
{
	glDeleteBuffers(1, &handle);
}