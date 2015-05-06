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
            void erase_fast(iterator q);
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
            assert(start <= m_numElements && end <= m_numElements && "vector<T>::destroy - out of bounds start/end."); 
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
            assert(m_numElements != 0 && "vector<T>::decrement - cannot decrement zero-length vector.");
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
            assert(m_numElements != 0 && "vector<T>::decrement - cannot decrement zero-length vector."); 

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
            assert(index <= m_numElements && "vector<T>::insert - out of bounds index.");
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
         
         template<typename T>
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
         
         template<typename T>
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
         
         template<typename T>
         inline void vector<T>::erase_fast(uint32 index)
         {
            assert(index < m_numElements && "vector<T>::erase_fast - out of bounds index.");
         
            // CAUTION: this operator does NOT maintain list order
            // Copy the last element into the deleted 'hole' and decrement the
            //   size of the vector.
            destructInPlace(&m_array[index]);
            if (index < (mElementCount - 1))
               dMemmove(&m_array[index], &m_array[m_numElements - 1], sizeof(value_type));
            m_numElements--;
         }
         
         template<typename T>
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
         
         template<typename T>
         inline void vector<T>::fill( const T& value )
         {
            for( uint32 i = 0; i < size(); ++ i )
               m_array[i] = value;
         }
         
         template<typename T>
         inline T& vector<T>::first()
         {
            assert(m_numElements != 0 && "vector<T>::first - Error, no first element of a zero sized array!");
            return m_array[0];
         }
         
         template<typename T>
         inline const T& vector<T>::first() const
         {
            assert(m_numElements != 0 && "vector<T>::first - Error, no first element of a zero sized array! (const)");
            return m_array[0];
         }
         
         template<typename T>
         inline T& vector<T>::last()
         {
            assert(m_numElements != 0 && "vector<T>::last - Error, no last element of a zero sized array!");
            return m_array[m_numElements - 1];
         }
         
         template<typename T>
         inline const T& vector<T>::last() const
         {
            assert(m_numElements != 0 && "vector<T>::last - Error, no last element of a zero sized array! (const)");
            return m_array[m_numElements - 1];
         }
         
         template<typename T>
         inline void vector<T>::clear()
         {
            destroy(0, m_numElements);
            m_numElements = 0;
         }
         
         template<typename T>
         inline void vector<T>::compact()
         {
            resize(m_numElements);
         }
         
         //typedef uint32 (QSORT_CALLBACK *qsort_compare_func)(const void *, const void *);
         
         template<typename T>
         inline void vector<T>::sort(compare_func f)
         {
            qsort(address(), size(), sizeof(T), (qsort_compare_func) f);
         }
         
         template<typename T>
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
         
         template<typename T>
         inline typename vector<T>::iterator vector<T>::begin()
         {
            return m_array;
         }
         
         template<typename T>
         inline typename vector<T>::const_iterator vector<T>::begin() const
         {
            return m_array;
         }
         
         template<typename T> 
         inline typename vector<T>::iterator vector<T>::end()
         {
            return m_array + m_numElements;
         }
         
         template<typename T>
         inline typename vector<T>::const_iterator vector<T>::end() const
         {
            return m_array + m_numElements;
         }
         
         template<typename T>
         inline int32 vector<T>::size() const
         {
            return (int32)m_numElements;
         }
         
         template<typename T>
         inline bool vector<T>::empty() const
         {
            return (m_numElements == 0);
         }
         
         template<typename T>
         inline void vector<T>::insert(iterator p, const T &x)
         {
            uint32 index = (uint32)(p - m_array);
            insert(index);
            m_array[index] = x;
         }
         
         template<typename T>
         inline void vector<T>::erase(iterator q)
         {
            erase(uint32(q - m_array));
         }
         
         template<typename T>
         inline void vector<T>::erase_fast(iterator q)
         {
            erase_fast(uint32(q - m_array));
         }
         
         template<typename T>
         inline T& vector<T>::front()
         {
            return *begin();
         }
         
         template<typename T>
         inline const T &vector<T>::front() const
         {
            return *begin();
         }
         
         template<typename T>
         inline T& vector<T>::back()
         {
            assert(m_numElements != 0 && "vector<T>::back - cannot access last element of zero-length vector.");
            return *(end()-1);
         }
         
         template<typename T>
         inline const T& vector<T>::back() const
         {
            assert(m_numElements != 0 && "vector<T>::back - cannot access last element of zero-length vector.");
            return *(end()-1);
         }
         
         template<typename T>
         inline void vector<T>::push_front(const T& x)
         {
            insert(0);
            m_array[0] = x;
         }
         
         template<typename T>
         inline void vector<T>::push_back(const T& x)
         {
            increment();
            m_array[m_numElements - 1] = x;
         }
         
         template<typename T>
         inline uint32 vector<T>::push_front_unique(const T& x)
         {
            int32 index = find_next(x);
         
            if (index == -1)
            {
               index = 0;
         
               insert(index);
               m_array[index] = x;
            }
         
            return index;
         }
         
         template<typename T>
         inline uint32 vector<T>::push_back_unique(const T& x)
         {
            int32 index = find_next(x);
         
            if (index == -1)
            {
               increment();
         
               index = m_numElements - 1;
               m_array[index] = x;
            }
         
            return index;
         }
         
         template<typename T>
         inline int32 vector<T>::find_next( const T& x, uint32 start ) const
         {
            int32 index = -1;
         
            if (start < m_numElements)
            {
               for (uint32 i = start; i < m_numElements; i++)
               {
                  if (m_array[i] == x)
                  {
                     index = i;
                     break;
                  }
               }
            }
         
            return index;
         }
         
         template<typename T>
         inline void vector<T>::pop_front()
         {
            assert(m_numElements != 0 && "Vector<T>::pop_front - cannot pop the front of a zero-length vector.");
            erase(uint32(0));
         }
         
         template<typename T>
         inline void vector<T>::pop_back()
         {
            assert(m_numElements != 0 && "vector<T>::pop_back - cannot pop the back of a zero-length vector.");
            decrement();
         }
         
         template<typename T> 
         inline T& vector<T>::operator[](U32 index)
         {
            assert(index < m_numElements && "vector<T>::operator[] - out of bounds array access!");
            return m_array[index];
         }
         
         template<typename T>
         inline const T& vector<T>::operator[](uint32 index) const
         {
            assert(index < m_numElements && "vector<T>::operator[] - out of bounds array access!");
            return m_array[index];
         }
         
         template<typename T>
         inline void vector<T>::reserve(uint32 size)
         {
            if (size <= mArraySize)
               return;
         
            const uint32 ec = m_numElements;
            if (resize(size))
               mElementCount = ec;
         }
         
         template<typename T> inline
         uint32 vector<T>::capacity() const
         {
             return m_numElemAllocated;
         }
         
         template<typename T>
         inline void vector<T>::set(void * addr, uint32 sz)
         {
            if ( !addr )
               sz = 0;
         
            set_size( sz );
         
            if ( addr && sz > 0 )
               dMemcpy(address(),addr,sz*sizeof(T));
         }
         
         template<typename T>
         inline bool vector<T>::resize(uint32 ecount)
         {
         #ifdef hsfhaf_DEBUG_GUARD
            return VectorResize(&m_numElemAllocated, &m_numElements, (void**) &m_array, ecount, sizeof(T),
                                mFileAssociation, mLineAssociation);
         #else
            return VectorResize(&m_numElemAllocated, &m_numElements, (void**) &m_array, ecount, sizeof(T));
         #endif
         }
         
         template<typename T>
         inline void vector<T>::merge(const vector &other)
         {
            if ( !other.size() )
               return;
         
            const uint32 oldSize = m_numElements;
            const uint32 newSize = oldSize + other.size();
            if ( newSize > m_numElemAllocated )
               resize( newSize );
         
            T *dest = m_array + oldSize;
            const T *src = other.m_array;
            while ( dest < m_array + newSize )
               constructInPlace( dest++, src++ );
         
            m_numElements = newSize;
         }
         
         template<typename T>
         inline void vector<T>::merge(const T *addr, uint32 count)
         {
            const uint32 oldSize = m_numElements;
            const uint32 newSize = oldSize + count;
            if ( newSize > mArraySize )
               resize( newSize );
         
            T *dest = m_array + oldSize;
            while ( dest < m_array + newSize )
               constructInPlace( dest++, addr++ );
         
            m_numElements = newSize;
         }
         
         template<typename T>
         inline void vector<T>::reverse()
         {
            for (uint32 i = 0, j = size();  (i != j) && (i != --j);  ++i)
               std::swap( m_array[i],  m_array[j] );
         }
      
      } // namespace vector
      
   } // namespace containers
   
} // namespace core

#endif
