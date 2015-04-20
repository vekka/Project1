
#include <iostream>
#include "assert.h"

class RefCounterBase
{
private:
   mutable int refCount;
public:
   RefCounterBase() : refCount(0) {};
   virtual ~RefCounterBase() {};
   void Grab() const;
   void Release() const;
};

template <class T>
class RefCounterPtr: public RefCounterBase
{
public:

   // RCPtr < T > = new T();
   RefCounterPtr( T *ptr = NULL )
      : internalPtr( ptr )
   {
      if( ptr != NULL ) { ptr->grab(); }
   }

   //copy constructor
   RefCounterPtr( const RefCounterPtr &ptr )
      : internalPtr( ptr.internalPtr )
   {
      if( internalPtr != NULL ) { ptr->Grab(); }
   }

   ~RefCounterPtr()
   {
      if( internalPtr != NULL ) { internalPtr->Release(); }
   }

   // pointer Assignment, x = new T();
   RefCounterPtr &operator=( T* ptr )
   {
      if( ptr != internalPtr )
      {
         if( ptr != NULL ) { ptr->grab(); }
         if( internalPtr != NULL ) { internalPtr->Release(); }
         internalPtr = ptr;
      }
      return (*this);
   }

   // assign this pointer to another 
   RefCounterPtr &operator=( const RefCounterPtr &ptr )
   {
      return (*this) = ptr.internalPtr;
   }

   T *Get() const
   {
      return internalPtr;
   }

   
   T* operator->() const { return internalPtr; }
   T &operator*() const { return *internalPtr;}
   operator T*() const {return internalPtr; }
   operator bool() const { return internalPtr != NULL; }
   bool operator==( const RefCounterPtr &ptr) { internalPtr == ptr.internalPtr; }
   bool operator==( const T *ptr) { return internalPtr ==  ptr; }
private:
   T *internalPtr;
};