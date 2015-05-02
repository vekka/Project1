#ifndef _ALLOCATOR_HPP_INCLUDED_
#define _ALLOCATOR_HPP_INCLUDED_

#include <new>

#include "core/BasicTypes.hpp"

template <class T>
class Allocator
{
protected:
	virtual void* InternalNew(size_t cnt)
	{
		return operator new(cnt);
	}

	virtual void InternalDelete(void* ptr)
	{
		operator delete(ptr);
	}
public:
   ~Allocator() {}

	T* Allocate( const size_t count )
	{
		return (T*)InternalNew( count * sizeof(T) );
	}

   void Free( T* ptr )
	{
		InternalDelete( ptr );
	}

	void AddElem( const T* ptr, const T &elem )
	{
		new ( (void*)ptr ) T(elem);
	}

   void RemoveElem( const T* ptr )
	{
		ptr->~T();
	}
};

template <class T, class TAlloc>
class PoolAllocator
{
 protected:
	virtual void* InternalNew(size_t cnt)
	{
	}

	virtual void InternalDelete(void* ptr)
	{
	}
public:
   ~PoolAllocator() {}

	T* Allocate( const size_t count )
	{
	}

   void Free( T* ptr )
	{
	}

	void AddElem( const T* ptr, const T &elem )
	{
	}

   void RemoveElem( const T* ptr )
	{
	}
};

#endif
