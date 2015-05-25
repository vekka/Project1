#ifndef _SCOPEDPTR_HPP_INCLUDED_
#define _SCOPEDPTR_HPP_INCLUDED_

namespace core
{

   namespace memory
   {
      template <class T>
      class ScopedPtr
      {
      private:
         T* ptr;
      public:
         ScopedPtr() : ptr(0) { }
         ScopedPtr(T* _ptr) : ptr(_ptr) { }
         ~ScopedPtr() { delete ptr; }
         T* Get() const { return ptr; }
         operator T*() { return ptr; }
         T* operator->() { return ptr; }
         
         void reset(T* t = 0)
         {
            delete ptr;
            ptr = t;
         }

         void Swap(ScopedPtr &other)
         {
            std::swap(ptr, other.ptr);
         }

      };

      template<class T>
      inline void swap(ScopedPtr<T> &a, ScopedPtr<T> &b)
      {
         a.swap(b);
      }

   } // namespace memory

} // namespace core

#endif
