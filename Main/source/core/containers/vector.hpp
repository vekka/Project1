//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//----------------------------------------------------------------------------- 

#ifndef _VECTOR_HPP_INCLUDED_
#define _VECTOR_HPP_INCLUDED_

#include "../BasicTypes.hpp"

namespace core
{

   namespace containers
   {

      // this code is to be modified to resemble the std vector interface
      // in addition to extra functionality that comes with this code.
      namespace vector
      {     
         template <typename T>
         class vector
         {
         protected:
            uint32 m_numElements;
            uint32 m_numElemAllocated;
            T* m_array; 
        
         public:
            vector(const uint32 initialSize = 0);
            vector(const vector&);
            ~vector();
            void destroy(uint32 start, uint32 end);
            typedef T value_type;
            typedef T& reference;
            typedef const T& const_reference; 
            typedef T* iterator; 
            typedef const T* const_iterator; 
            typedef int32 difference_type; 
            typedef uint32 size_type; 
            //typedef difference_type (QSORT_CALLBACK *compare_func)(const T *a, const T *b); 
            vector<T>& operator=(const vector<T>& p); 
            iterator begin(); 
            const_iterator begin() const; 
            iterator end(); 
            const_iterator end() const; 
            int32 size() const; 
            bool empty() const; 
            bool contains(const T&) const;
            T& front();
            const T& front() const; 
            T& back(); 
            const T& back() const; 
            void push_front(const T&); 
            void push_back(const T&); 
            uint32 push_front_unique(const T&); 
            uint32 push_back_unique(const T&); 
            int32 find_next( const T&, uint32 start = 0 ) const; 
            void pop_front(); 
            void pop_back(); 
            T& operator[](uint32); 
            const T& operator[](uint32) const; 
            T& operator[](int32 i) { return operator[](uint32(i)); } 
            const T& operator[](int32 i ) const { return operator[](uint32(i)); } 
            void reserve(uint32); 
            uint32 capacity() const;
            T* address() const;
            void decrement();
            void erase(uint32 index);
            void insert(uint32 index, const T &x);
            uint32 mem_size() const;
            uint32 set_size(uint32 size);
         };
         
         template <typename T>
         inline vector<T>::vector(const uint32 initialSize) 
         {
            m_array = 0;
            m_numElements = 0;
            m_numElemAllocated = 0;
            if (initialSize)
               reserve(initialSize);
         }            

         template <typename T>
         inline vector<T>::vector(const vector &other)
         {
            m_array = 0;
            resize(other.m_numElements);
            construct(0, other.m_numElements, other.m_array); 
         } 
  
         template <typename T>
         inline void vector<T>::destroy(uint32 start, uint32 end) // destroys from start to end-1 
         { 
            AssertFatal(start <= m_numElements && end <= m_numElements, "Vector<T>::destroy - out of bounds start/end."); 
            while(start < end) 
               destructInPlace(&m_array[start++]); 
         } 
  
         template <typename T>
         inline uint32 vector<T>::mem_size() const
         {
            return capacity() * sizeof(T);
         } 

         template <typename T>
         inline T* vector<T>::address() const
         {
            return m_array;
         }
         
         template <typename T>
         inline uint32 vector<T>::set_size(uint32 size)
         {
            const uint32 oldSize = m_numElements;
            if(size > m_numElements)
            {
               if (size > m_arraySize)
                  resize(size);
               // Set count first so we are in a valid state for construct.
               m_numElements = size;
               construct(oldSize, size);
            } 

            else if (size < m_numElements)
            {
               destroy(size, oldSize);
               m_numElements = size;
            }
            return m_numElements;
         } 

         template <typename T>
         inline void vector<T>::increment()
         {
            if(m_numElements == m_arraySize)
               resize(m_numElements + 1);
            else
               m_numElements++;
            constructInPlace(&m_array[m_numElements - 1]);
         } 

         template <typename T>
         inline void vector<T>::decrement()
         {
            AssertFatal(m_numElements != 0, "Vector<T>::decrement - cannot decrement zero-length vector.");
            m_numElements--;
            destructInPlace(&m_array[m_numElements]);
         } 

         template <typename T>
         inline void vector<T>::increment(uint32 delta)
         {
            uint32 count = m_numElements;
            if ((m_numElements += delta) > m_arraySize)
               resize(m_numElements);
            construct(count, m_numElements);
         }

         template <typename T>
         inline void vector<T>::decrement(uint32 delta)
         {
            AssertFatal(m_numElements != 0, "Vector<T>::decrement - cannot decrement zero-length vector."); 

            const uint32 count = m_numElements;
            // Determine new count after decrement...
            uint32 newCount = m_numElements;
            if (m_numElements > delta)
               newCount -= delta;
            else
               newCount = 0;
            // Destruct removed items...
            destroy(newCount, count);
            // Note new element count.
            m_numElements = newCount;
         }
         
         template <typename T>
         inline void vector<T>::insert(uint32 index)
         {
            assert(index <= m_numElements, "Vector<T>::insert - out of bounds index.");
            if(m_numElements == m_array)
               resize(m_numElements + 1);
            else
               m_numElements++;
            dMemmove(&m_array[index + 1],
                             &m_array[index],
                             (m_numElements - index - 1) * sizeof(value_type));
            constructInPlace(&m_array[index]);
         } 

         template <typename T>
         inline void vector<T>::insert(uint32 index, const T &x)
         {
            assert(index <= m_numElements);
            insert(index);
            m_array[index] = x;
         }
         
         template <typename T>
         inline void vector<T>::erase(uint32 index)
         {
            assert(index < m_numElements && "Vector<T>::erase - out of bounds index!");
            destructInPlace(&m_array[index]);
            if (index < (m_numElements - 1))
            {
               dMemmove(&m_array[index], &m_array[index + 1], (m_numElements - index - 1) * sizeof(value_type));
            }
            m_numElements--;
         }
         
         template<class T>
         inline bool vector<T>::remove(const T& x)
         {
            iterator i = begin();
            while (i != end())
            {
               if (*i == x)
               {
                  erase( i );
                  return true;
               }
         
               i++;
            }
         
            return false;
         }
         
         template<class T>
         inline void vector<T>::erase(uint32 index, uint32 count)
         {
            assert(index < m_numElements && "vector<T>::erase - out of bounds index!");
            assert(count > 0 && "vector<T>::erase - count must be greater than zero!");
            assert(index+count <= m_numElements && "vector<T>::erase - out of bounds count!");
         
            destroy( index, index+count );
         
            dMemmove(   &m_array[index],
                        &m_array[index + count],
                        (m_numElements - index - count) * sizeof(value_type));
         
            m_numElements -= count;
         }
         
         template<class T>
         inline void vector<T>::erase_fast(uint32 index)
         {
            assert(index < m_numElements, "vector<T>::erase_fast - out of bounds index.");
         
            // CAUTION: this operator does NOT maintain list order
            // Copy the last element into the deleted 'hole' and decrement the
            //   size of the vector.
            destructInPlace(&mArray[index]);
            if (index < (mElementCount - 1))
               dMemmove(&m_array[index], &m_array[m_numElements - 1], sizeof(value_type));
            m_numElements--;
         }
         
         template<class T>
         inline bool vector<T>::contains(const T& x) const
         {
         	const_iterator i = begin();
         	while (i != end())
         	{
         		if (*i == x)
         			return true;
         
         		i++;
         	}
         
         	return false;
         }
         
         template<class T>
         inline void vector<T>::fill( const T& value )
         {
            for( uint32 i = 0; i < size(); ++ i )
               m_array[i] = value;
         }
         
         template<class T>
         inline T& vector<T>::first()
         {
            assert(m_numElements != 0 && "vector<T>::first - Error, no first element of a zero sized array!");
            return m_array[0];
         }
         
         template<class T>
         inline const T& vector<T>::first() const
         {
            assert(m_numElements != 0 && "vector<T>::first - Error, no first element of a zero sized array! (const)");
            return m_array[0];
         }
         
         template<class T>
         inline T& vector<T>::last()
         {
            assert(m_numElements != 0 && "vector<T>::last - Error, no last element of a zero sized array!");
            return m_array[m_numElements - 1];
         }
         
         template<class T>
         inline const T& vector<T>::last() const
         {
            assert(m_numElements != 0 && "vector<T>::last - Error, no last element of a zero sized array! (const)");
            return m_array[mElementCount - 1];
         }
         
         template<class T>
         inline void vector<T>::clear()
         {
            destroy(0, m_numElements);
            mElementCount = 0;
         }
         
         template<class T>
         inline void vector<T>::compact()
         {
            resize(m_numElements);
         }
         
         typedef S32 (QSORT_CALLBACK *qsort_compare_func)(const void *, const void *);
         
         template<class T>
         inline void vector<T>::sort(compare_func f)
         {
            qsort(address(), size(), sizeof(T), (qsort_compare_func) f);
         }
         
         //-----------------------------------------------------------------------------
         
         template<class T>
         inline vector<T> &vector<T>::operator=(const vector<T>& other)
         {
            if(m_numElements > other.mElementCount)
            {
               destroy(other.m_numElements, m_numElements);
            }
            
            uint32 count = getMin( m_numElements, other.m_numElements );
            uint32 i;
            for( i=0; i < count; i++ )
            {
               m_array[i] = other.m_array[i];
            }
            
            resize( other.m_numElements );
            
            if( i < other.m_numElements )
            {
               construct(i, other.m_numElements, other.m_array);
            }
            return *this;
         }
         
         template<class T>
         inline typename vector<T>::iterator vector<T>::begin()
         {
            return m_array;
         }
         
         template<class T>
         inline typename vector<T>::const_iterator vector<T>::begin() const
         {
            return m_array;
         }
         
         template<class T> 
         inline typename vector<T>::iterator vector<T>::end()
         {
            return m_array + m_numElements;
         }
         
         template<class T>
         inline typename vector<T>::const_iterator vector<T>::end() const
         {
            return m_array + m_numElements;
         }
         
         template<class T>
         inline int32 vector<T>::size() const
         {
            return (int32)m_numElements;
         }
         
         template<class T>
         inline bool vector<T>::empty() const
         {
            return (m_numElements == 0);
         }
         
         template<class T>
         inline void vector<T>::insert(iterator p, const T &x)
         {
            uint32 index = (uint32)(p - m_array);
            insert(index);
            m_array[index] = x;
         }
         
         template<class T>
         inline void vector<T>::erase(iterator q)
         {
            erase(uint32(q - m_array));
         }
         
         template<class T>
         inline void Vector<T>::erase_fast(iterator q)
         {
            erase_fast(uint32(q - m_array));
         }
         
         template<class T> inline T& vector<T>::front()
         {
            return *begin();
         }
         
         template<class T>
         inline const T &vector<T>::front() const
         {
            return *begin();
         }
         
         template<class T>
         inline T& vector<T>::back()
         {
            AssertFatal(mElementCount != 0, "Vector<T>::back - cannot access last element of zero-length vector.");
            return *(end()-1);
         }
         
         template<class T> inline const T& Vector<T>::back() const
         {
            AssertFatal(mElementCount != 0, "Vector<T>::back - cannot access last element of zero-length vector.");
            return *(end()-1);
         }
         
         template<class T> inline void Vector<T>::push_front(const T& x)
         {
            insert(0);
            mArray[0] = x;
         }
         
         template<class T> inline void Vector<T>::push_back(const T& x)
         {
            increment();
            mArray[mElementCount - 1] = x;
         }
         
         template<class T> inline U32 Vector<T>::push_front_unique(const T& x)
         {
            S32 index = find_next(x);
         
            if (index == -1)
            {
               index = 0;
         
               insert(index);
               mArray[index] = x;
            }
         
            return index;
         }
         
         template<class T> inline U32 Vector<T>::push_back_unique(const T& x)
         {
            S32 index = find_next(x);
         
            if (index == -1)
            {
               increment();
         
               index = mElementCount - 1;
               mArray[index] = x;
            }
         
            return index;
         }
         
         template<class T> inline S32 Vector<T>::find_next( const T& x, U32 start ) const
         {
            S32 index = -1;
         
            if (start < mElementCount)
            {
               for (U32 i = start; i < mElementCount; i++)
               {
                  if (mArray[i] == x)
                  {
                     index = i;
                     break;
                  }
               }
            }
         
            return index;
         }
         
         template<class T> inline void Vector<T>::pop_front()
         {
            AssertFatal(mElementCount != 0, "Vector<T>::pop_front - cannot pop the front of a zero-length vector.");
            erase(U32(0));
         }
         
         template<class T> inline void Vector<T>::pop_back()
         {
            AssertFatal(mElementCount != 0, "Vector<T>::pop_back - cannot pop the back of a zero-length vector.");
            decrement();
         }
         
         template<class T> inline T& Vector<T>::operator[](U32 index)
         {
            AssertFatal(index < mElementCount, "Vector<T>::operator[] - out of bounds array access!");
            return mArray[index];
         }
         
         template<class T> inline const T& Vector<T>::operator[](U32 index) const
         {
            AssertFatal(index < mElementCount, "Vector<T>::operator[] - out of bounds array access!");
            return mArray[index];
         }
         
         template<class T> inline void Vector<T>::reserve(U32 size)
         {
            if (size <= mArraySize)
               return;
         
            const U32 ec = mElementCount;
            if (resize(size))
               mElementCount = ec;
         }
         
         template<class T> inline U32 Vector<T>::capacity() const
         {
             return mArraySize;
         }
         
         template<class T> inline void Vector<T>::set(void * addr, U32 sz)
         {
            if ( !addr )
               sz = 0;
         
            setSize( sz );
         
            if ( addr && sz > 0 )
               dMemcpy(address(),addr,sz*sizeof(T));
         }
         
         //-----------------------------------------------------------------------------
         
         template<class T> inline bool Vector<T>::resize(U32 ecount)
         {
         #ifdef TORQUE_DEBUG_GUARD
            return VectorResize(&mArraySize, &mElementCount, (void**) &mArray, ecount, sizeof(T),
                                mFileAssociation, mLineAssociation);
         #else
            return VectorResize(&mArraySize, &mElementCount, (void**) &mArray, ecount, sizeof(T));
         #endif
         }
         
         template<class T> inline void Vector<T>::merge( const Vector &p )
         {
            if ( !p.size() )
               return;
         
            const U32 oldSize = mElementCount;
            const U32 newSize = oldSize + p.size();
            if ( newSize > mArraySize )
               resize( newSize );
         
            T *dest = mArray + oldSize;
            const T *src = p.mArray;
            while ( dest < mArray + newSize )
               constructInPlace( dest++, src++ );
         
            mElementCount = newSize;
         }
         
         template<class T> inline void Vector<T>::merge( const T *addr, U32 count )
         {
            const U32 oldSize = mElementCount;
            const U32 newSize = oldSize + count;
            if ( newSize > mArraySize )
               resize( newSize );
         
            T *dest = mArray + oldSize;
            while ( dest < mArray + newSize )
               constructInPlace( dest++, addr++ );
         
            mElementCount = newSize;
         }
         
         template<class T> inline void Vector<T>::reverse()
         {
            for (U32 i = 0, j = size();  (i != j) && (i != --j);  ++i)
               std::swap( mArray[ i ],  mArray[ j ] );
         }
         
         //-----------------------------------------------------------------------------
         /// Template for vectors of pointers.
         template <class T>
         class VectorPtr : public Vector<void *>
         {
            /// @deprecated Disallowed.
            VectorPtr(const VectorPtr&);  // Disallowed
         
           public:
            VectorPtr();
            VectorPtr(const char* fileName, const U32 lineNum);
         
            /// @name STL interface
            /// @{
         
            typedef T        value_type;
            typedef T&       reference;
            typedef const T& const_reference;
         
            typedef T*       iterator;
            typedef const T* const_iterator;
            typedef U32      difference_type;
            typedef U32      size_type;
         
            iterator       begin();
            const_iterator begin() const;
            iterator       end();
            const_iterator end() const;
         
            void insert(iterator,const T&);
            void insert(S32 idx) { Parent::insert(idx); }
            void erase(iterator);
         
            T&       front();
            const T& front() const;
            T&       back();
            const T& back() const;
            void     push_front(const T&);
            void     push_back(const T&);
         
            T&       operator[](U32);
            const T& operator[](U32) const;
         
            /// @}
         
            /// @name Extended interface
            /// @{
         
            typedef Vector<void*> Parent;
            T&       first();
            T&       last();
            const T& first() const;
            const T& last() const;
            void erase_fast(U32);
            void erase_fast(iterator);
         
            /// @}
         };
         
         
         //-----------------------------------------------------------------------------
         template<class T> inline VectorPtr<T>::VectorPtr()
         {
            //
         }
         
         template<class T> inline VectorPtr<T>::VectorPtr(const char* fileName,
                                                          const U32   lineNum)
            : Vector<void*>(fileName, lineNum)
         {
            //
         }
         
         template<class T> inline T& VectorPtr<T>::first()
         {
            return (T&)Parent::first();
         }
         
         template<class T> inline const T& VectorPtr<T>::first() const
         {
            return (const T)Parent::first();
         }
         
         template<class T> inline T& VectorPtr<T>::last()
         {
            return (T&)Parent::last();
         }
         
         template<class T> inline const T& VectorPtr<T>::last() const
         {
            return (const T&)Parent::last();
         }
         
         template<class T> inline typename VectorPtr<T>::iterator VectorPtr<T>::begin()
         {
            return (iterator)Parent::begin();
         }
         
         template<class T> inline typename VectorPtr<T>::const_iterator VectorPtr<T>::begin() const
         {
            return (const_iterator)Parent::begin();
         }
         
         template<class T> inline typename VectorPtr<T>::iterator VectorPtr<T>::end()
         {
            return (iterator)Parent::end();
         }
         
         template<class T> inline typename VectorPtr<T>::const_iterator VectorPtr<T>::end() const
         {
            return (const_iterator)Parent::end();
         }
         
         template<class T> inline void VectorPtr<T>::insert(iterator i,const T& x)
         {
            Parent::insert( (Parent::iterator)i, (Parent::reference)x );
         }
         
         template<class T> inline void VectorPtr<T>::erase(iterator i)
         {
            Parent::erase( (Parent::iterator)i );
         }
         
         template<class T> inline void VectorPtr<T>::erase_fast(U32 index)
         {
            AssertFatal(index < mElementCount, "VectorPtr<T>::erase_fast - out of bounds index." );
         
            // CAUTION: this operator does not maintain list order
            // Copy the last element into the deleted 'hole' and decrement the
            //   size of the vector.
            // Assert: index >= 0 && index < mElementCount
            if (index < (mElementCount - 1))
               mArray[index] = mArray[mElementCount - 1];
            decrement();
         }
         
         template<class T> inline void VectorPtr<T>::erase_fast(iterator i)
         {
            erase_fast(U32(i - iterator(mArray)));
         }
         
         template<class T> inline T& VectorPtr<T>::front()
         {
            return *begin();
         }
         
         template<class T> inline const T& VectorPtr<T>::front() const
         {
            return *begin();
         }
         
         template<class T> inline T& VectorPtr<T>::back()
         {
            AssertFatal(mElementCount != 0, "Vector<T>::back - cannot access last element of zero-length vector.");
            return *(end()-1);
         }
         
         template<class T> inline const T& VectorPtr<T>::back() const
         {
            AssertFatal(mElementCount != 0, "Vector<T>::back - cannot access last element of zero-length vector.");
            return *(end()-1);
         }
         
         template<class T> inline void VectorPtr<T>::push_front(const T& x)
         {
            Parent::push_front((Parent::const_reference)x);
         }
         
         template<class T> inline void VectorPtr<T>::push_back(const T& x)
         {
            Parent::push_back((Parent::const_reference)x);
         }
         
         template<class T> inline T& VectorPtr<T>::operator[](U32 index)
         {
            return (T&)Parent::operator[](index);
         }
         
         template<class T> inline const T& VectorPtr<T>::operator[](U32 index) const
         {
            return (const T&)Parent::operator[](index);
         }
         
         //------------------------------------------------------------------------------
         
         template <class T> class VectorSet : public Vector<T>
         {
         public:
            void insert(T dat)
            {
               if(find(this->begin(), this->end(), dat) == this->end())
                  push_back(dat);
            }
         };
         
         #endif //_TVECTOR_H_
      
      } // namespace vector
      
   } // namespace containers
   
} // namespace core

#endif
