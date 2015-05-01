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
            U32 count = m_numElements;
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
            AssertFatal(index <= m_numElements, "Vector<T>::insert - out of bounds index.");
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
            insert(index);
            m_array[index] = x;
         }
         
         template <typename T>
         inline void vector<T>::erase(uint32 index)
         {
            AssertFatal(index < m_numElements, "Vector<T>::erase - out of bounds index!");
            destructInPlace(&m_array[index]);
            if (index < (m_numElements - 1))
            {
               dMemmove(&m_array[index], &m_array[index + 1], (m_numElements - index - 1) * sizeof(value_type));
            }
            m_numElements--;
         } 

      
      } // namespace vector
      
   } // namespace containers
   
} // namespace core

#endif