#ifndef _POINTER_HPP_INCLUDED_
#define _POINTER_HPP_INCLUDED_

namespace core
{

   namespace pointer
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

      template <typename T>
      struct ScopeGuard
      {
      private:
         // no copying allowed.
         ScopeGuard();
         ScopeGuard(const ScopeGuard &);
         ScopeGuard &operator = (const ScopeGuard &);

         T* obj;
         bool mdismiss;
      
      public:
         ScopeGuard(T* obj) : obj(obj), mdismiss() {}
         
         ~ScopeGuard() throw()
         {
            if (!mdismiss)
            {
               delete obj;
            }
            obj = NULL;
         }

         T* dismiss()
         {
            mdismiss = true;
            return obj;
         }

         operator T*()
         {
            return obj;
         }

         T* operator->()
         {
            return obj;
         }
      };
   } // namespace pointer

} // namespace core

#endif
