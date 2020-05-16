#pragma once

#include <functional>

#include "FlumenCore/Container/Array.hpp"
#include "FlumenCore/Container/PoolAllocator.h"

#define BREAK return true;
#define CONTINUE return false;

namespace container
{
	typedef int IndexType;
	
	template<class ObjectType>
	class Pool
	{
		template<class IteratorType>
		class Iterator
		{
			friend class Pool <IteratorType>;

			friend Iterator <IteratorType> begin(Pool <IteratorType> &pool);

			friend Iterator <IteratorType> end(Pool <IteratorType> &pool);

			Pool <IteratorType> &pool;

			IteratorType *element;

			bool *check;

			Iterator(Pool <IteratorType> &_pool, IteratorType *_element) : 
				pool(_pool), element(_element), check(pool.validators)
			{
				if(*check == false)
				{
					auto &iterator = *this;
					iterator++;
				}
			}

			Iterator(Pool <IteratorType> &_pool) : 
				pool(_pool), element(pool.GetEnd()), check(nullptr)
			{}

		public:
			IteratorType & operator *() {return *element;}

			bool operator !=(const Iterator<IteratorType> &other) {return element != other.element;}

			Iterator<IteratorType> & operator++() 
			{
				while(true)
				{
					if(element == pool.GetEnd())
						break;

					element++;
					check++;

					if(*check == true)
						break;
				}

				return *this;
			}
		};

	private:
		ObjectType* objects_;

		IndexType capacity_;

		IndexType size_;

		IndexType* indices_;

		IndexType freeIndex_;

		bool* validators;

		unsigned long memorySize_;

		ObjectType* GetStart();

		ObjectType* GetEnd();

	public:
		friend Iterator <ObjectType> begin(Pool <ObjectType> &pool) {return {pool, pool.objects_};}

        friend Iterator <ObjectType> end(Pool <ObjectType> &pool) {return {pool};}

		Pool();

		Pool(IndexType capacity);

		Pool(PoolAllocator<ObjectType> &allocator);

		void Initialize(IndexType capacity);

		void Initialize(PoolAllocator<ObjectType> &allocator);

		ObjectType * Add();

		template<class DerivedType>
		ObjectType * Add();

		void Remove(ObjectType *const object);

		void Do(std::function <bool (ObjectType &)>);

		IndexType GetSize();

		IndexType GetCapacity();

		ObjectType * GetRandom();

		ObjectType * Find(ObjectType);

		float GetFillPercentage();

		void Reset();

		void Terminate(PoolAllocator<ObjectType> &allocator);
	};

	template<class ObjectType>
	Pool<ObjectType>::Pool()
	{
	}

	template<class ObjectType>
	Pool<ObjectType>::Pool(IndexType capacity) : capacity_(capacity)
	{
		memorySize_ = capacity_ * sizeof(ObjectType);
		objects_ = new ObjectType[capacity_];
		indices_ = new IndexType[capacity_];
		validators = new bool[capacity_];

		Reset();
	}

	template<class ObjectType>
	Pool<ObjectType>::Pool(PoolAllocator <ObjectType> &allocator)
	{
		auto memorySlot = allocator.Add();

		objects_ = memorySlot.Objects;
		indices_ = memorySlot.FreeIndices;
		validators = memorySlot.Checks;

		capacity_ = allocator.objectsPerPool;

		Reset();
	}

	template<class ObjectType>
	void Pool<ObjectType>::Initialize(IndexType capacity)
	{
		capacity_ = capacity;
		memorySize_ = capacity_ * sizeof(ObjectType);
		objects_ = new ObjectType[capacity_];
		indices_ = new IndexType[capacity_];
		validators = new bool[capacity_];
		
		Reset();
	}

	template<class ObjectType>
	void Pool<ObjectType>::Initialize(PoolAllocator <ObjectType> &allocator)
	{
		auto memorySlot = allocator.Add();

		objects_ = memorySlot.Objects;
		indices_ = memorySlot.FreeIndices;
		validators = memorySlot.Checks;

		capacity_ = allocator.objectsPerPool;

		Reset();
	}

	template<class ObjectType>
	ObjectType * Pool<ObjectType>::Add()
	{
		auto index = *(indices_ + freeIndex_);
		ObjectType * newObject = objects_ + index;
		*(validators + index) = true;

		freeIndex_--;
		size_++;
		if(size_ == capacity_ + 1)
		{
			return nullptr;
		}
		return newObject;
	}

	template<class ObjectType>
	template<class DerivedType>
	ObjectType * Pool<ObjectType>::Add()
	{
		auto index = *(indices_ + freeIndex_);
		ObjectType * newObject = (objects_ + index);
		*(validators + index) = true;

		CopyMemory(newObject, &DerivedType(), sizeof(DerivedType));

		freeIndex_--;
		size_++;

		if(size_ == capacity_ + 1)
		{
			return nullptr;
		}
		return newObject;
	}

	template<class ObjectType>
	void Pool<ObjectType>::Remove(ObjectType *const object)
	{
		IndexType index = object - objects_;
		*(validators + index) = false;
		freeIndex_++;
		*(indices_ + freeIndex_) = index;
		size_--;
	}

	template<class ObjectType>
	ObjectType* Pool<ObjectType>::GetEnd()
	{
		return (objects_ + capacity_);
	}

	template<class ObjectType>
	void Pool<ObjectType>::Do(std::function <bool (ObjectType &)> work)
	{
		auto validator = validators;
		for(auto object = objects_; object != objects_ + capacity_; ++object, ++validator)
		{
			if(*validator == false)
				continue;

			if(work(*object) == true)
				break;
		}
	}

	template<class ObjectType>
	IndexType Pool<ObjectType>::GetSize()
	{
		return size_;
	}

	template<class ObjectType>
	IndexType Pool<ObjectType>::GetCapacity()
	{
		return capacity_;
	}

	template<class ObjectType>
	ObjectType * Pool<ObjectType>::GetRandom()
	{
		static auto objects = Array <ObjectType *> (capacity_);
		objects.Reset();

		for(auto &object : *this)
		{
			*objects.Allocate() = &object;
		}

		return *objects.GetRandom();
	}

	template<class ObjectType>
	ObjectType * Pool<ObjectType>::Find(ObjectType comparator)
	{
		for(auto &object : *this)
		{
			if(object == comparator)
				return &object;
		}

		return nullptr;
	}

	template<class ObjectType>
	float Pool<ObjectType>::GetFillPercentage()
	{
		return ((float)size_ / (float)capacity_) * 100.0f;
	}

	template<class ObjectType>
	void Pool<ObjectType>::Reset()
	{
		size_ = 0;
		freeIndex_ = capacity_ - 1;

		for(IndexType index = 0; index < capacity_; ++index)
		{
			*(indices_ + index) = index;
		}

		for(auto validator = validators; validator != validators + capacity_; ++validator)
		{
			*validator = false;
		}
	}

	template<class ObjectType>
	void Pool<ObjectType>::Terminate(PoolAllocator<ObjectType> &allocator)
	{
		allocator.Remove(objects_);

		objects_ = nullptr;
		indices_ = nullptr;
		validators = nullptr;

		capacity_ = 0;
		size_ = 0;
	}
}
