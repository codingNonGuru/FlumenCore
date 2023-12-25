#pragma once

#include <iostream>
#include <initializer_list>
#include <functional>

#include "FlumenCore/Container/Container.hpp"
#include "FlumenCore/Container/ArrayAllocator.h"

#include "FlumenCore/Utility/Utility.hpp"

namespace container
{
	template<typename T, typename S>
	concept Comparable = requires(T a, S b)
	{
		a.operator==(b);
	};

	template<class O>
	class Array : public Container
	{
		template<class IteratorType>
        class Iterator
        {
            friend class Array <IteratorType>;

            friend Iterator <IteratorType> begin(const Array <IteratorType> &array);

            friend Iterator <IteratorType> end(const Array <IteratorType> &array);

            const Array <IteratorType> &array;

            IteratorType *element;

            Iterator(const Array <IteratorType> &_array, IteratorType *_element) : 
                array(_array), element(_element)
            {}

        public:
            IteratorType & operator *() {return *element;}

            bool operator !=(const Iterator<IteratorType> &other) {return element != other.element;}

            Iterator<IteratorType> & operator++() 
            {
				if(element != array.GetEnd())
				{
                	element++;
				}

                return *this;
            }

			IteratorType * operator ->()
			{
				return element;
			}

			operator IteratorType *()
			{
				return element;
			}
        };

		O* objects_;

		int capacity_;

		int size_;

		unsigned long memorySize_;

		virtual void* GetData() {return (void*)objects_;}

	public:
		friend Iterator <O> begin(const Array <O> &array) {return {array, array.GetStart()};}

        friend Iterator <O> end(const Array <O> &array) {return {array, array.GetEnd()};}

		struct Memory
		{
			O *Objects;

			int Capacity;
		};

		static const Memory PreallocateMemory(int capacity)
		{
			auto memorySize = capacity * sizeof(O);
			auto objects = (O*)malloc(memorySize);	

			return {objects, capacity};
		}

		Array() : objects_(nullptr), size_(0), capacity_(0), memorySize_(0) {}

		Array(int capacity) : capacity_(capacity), size_(0)
		{
			memorySize_ = capacity_ * sizeof(O);
			objects_ = (O*)malloc(memorySize_);// new O[capacity_];

			//MemoryLog::accrue(memorySize_);
		}

		Array(std::initializer_list<O> list)
		{
			size_ = list.size();
			capacity_ = list.size();
			memorySize_ = list.size() * sizeof(O);
			objects_ = (O*)malloc(memorySize_);

			auto object = objects_;
			for(auto iterator = list.begin(); iterator != list.end(); ++iterator, ++object)
			{
				*object = *iterator;
			}
		}

		Array(ArrayAllocator <O> &allocator)
		{
			auto memorySlot = allocator.Add();

			objects_ = memorySlot.Objects;

			capacity_ = allocator.objectsPerArray;

			Reset();
		}

		void Initialize(int capacity) 
		{
			capacity_ = capacity;
			size_ = 0;
			Destroy();
			memorySize_ = capacity_ * sizeof(O);
			objects_ = (O*)malloc(memorySize_); //new O[capacity_];

			//MemoryLog::accrue(memorySize_);
		}

		void Initialize(ArrayAllocator <O> &allocator)
		{
			auto memorySlot = allocator.Add();

			objects_ = memorySlot.Objects;

			capacity_ = allocator.objectsPerArray;

			Reset();
		}

		void Initialize(int capacity, const Memory memory) 
		{
			capacity_ = capacity;
			size_ = 0;
			memorySize_ = capacity_ * sizeof(O);
			objects_ = memory.Objects; //new O[capacity_];

			//MemoryLog::accrue(memorySize_);
		}

		void Reset()
		{
			size_ = 0;
		}

		bool IsFull()
		{
			return size_ == capacity_;
		}

		bool IsEmpty() const
		{
			return size_ == 0;
		}

		O *const Allocate(int count)
		{
			O *const newObject = (objects_ + size_);
			size_ += count;
			return newObject;
		}

		O *const Allocate()
		{
			if(size_ == capacity_)
				return nullptr;

			O *const newObject = (objects_ + size_);
			size_++;

			*newObject = O();

			return newObject;
		}

		O * Add()
		{
			if(size_ == capacity_)
				return nullptr;

			O *newObject = (objects_ + size_);
			size_++;

			return newObject;
		}

		void Fill()
		{
			size_ = capacity_;
		}

		/*template<class DerivedType>
		O * Add()
		{
			if(size_ == capacity_)
				return nullptr;

			O * newObject = (objects_ + size_);
			//*newObject = DerivedType();
			*(long*)newObject = *(long*)&DerivedType();
			size_++;

			return newObject;
		}*/

		O& operator[] (int index)
		{
			if(index < 0)
				index += capacity_;
			if(index >= capacity_)
				index -= capacity_;

			return *(objects_ + index);
		}

		O* Get(int index) const
		{
			if(index < 0)
				index += capacity_;
			if(index >= capacity_)
				index -= capacity_;

			return objects_ + index;
		}

		template<typename ComparatorType> requires Comparable<O, ComparatorType>
		O* Find(ComparatorType object) const
		{
			for(auto iterator = GetStart(); iterator != GetEnd(); ++iterator)
			{
				if(*iterator == object)
					return iterator;
			}

			return nullptr;
		}

		O* GetRandom()
		{
			if(size_ == 0)
				return nullptr;

			auto index = utility::GetRandom(0, size_ - 1);

			return objects_ + index;
		}

		O* GetStart() const
		{
			return objects_;
		}

		O* GetEnd() const
		{
			return (objects_ + size_);
		}

		O* GetNext(O* object) const
		{
			auto index = object - objects_;
			if(index >= size_ - 1)
				return nullptr;

			return object + 1;
		}

		void Iterate(std::function<void(O*)> function)
		{
			for(auto iterator = GetStart(); iterator != GetEnd(); ++iterator)
			{
				function(iterator);
			}
		}

		void SortDescendantly()
		{
			for(int j = 0; j <= GetSize() / 2; ++j)
			{
				for(int i = 0; i < GetSize() - 1; ++i)
				{
					auto next = Get(i + 1);
					auto current = Get(i);
					if(*next > *current)
					{
						auto swap = *next;
						*next = *current;
						*current = swap;
					}
				}
			}
		}

		void SortAscendantly()
		{
			for(int j = 0; j <= GetSize() / 2; ++j)
			{
				for(int i = 0; i < GetSize() - 1; ++i)
				{
					auto next = Get(i + 1);
					auto current = Get(i);
					if(*next < *current)
					{
						auto swap = *next;
						*next = *current;
						*current = swap;
					}
				}
			}
		}

		void Terminate(ArrayAllocator <O> &allocator)
		{
			allocator.Remove(objects_);

			objects_ = nullptr;

			capacity_ = 0;
			size_ = 0;
		}

		int GetSize() const
		{
			return size_;
		}

		int GetCapacity() const
		{
			return capacity_;
		}

		unsigned long GetMemorySize() const override
		{
			return size_ * sizeof(O);
		}

		unsigned long GetMemoryCapacity() const override
		{
			return capacity_ * sizeof(O);
		}

		void Destroy()
		{
			if(objects_ != nullptr)
				free(objects_);

			objects_ = nullptr;

			//MemoryLog::accrue(-memorySize_);
		}
	};
}
