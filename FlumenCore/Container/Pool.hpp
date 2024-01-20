#pragma once

#include <functional>
#include <concepts>

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

			friend Iterator <IteratorType> begin(const Pool <IteratorType> &pool);

			friend Iterator <IteratorType> end(const Pool <IteratorType> &pool);

			const Pool <IteratorType> &pool;

			IteratorType *element;

			bool *check;

			Iterator(const Pool <IteratorType> &_pool, IteratorType *_element) : 
				pool(_pool), element(_element), check(pool.validators)
			{
				if(*check == false)
				{
					auto &iterator = *this;
					iterator++;
				}
			}

			Iterator(const Pool <IteratorType> &_pool) : 
				pool(_pool), element(pool.GetEnd()), check(nullptr)
			{}

		public:
			IteratorType & operator *() {return *element;}

			bool operator !=(const Iterator<IteratorType> &other) {return element != other.element;}

			Iterator& operator++() 
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

			Iterator operator++(int) 
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

			IteratorType * operator ->()
			{
				return element;
			}

			operator IteratorType *()
			{
				return element;
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

		ObjectType* GetEnd() const;

	public:
		friend Iterator <ObjectType> begin(const Pool <ObjectType> &pool) {return {pool, pool.objects_};}

        friend Iterator <ObjectType> end(const Pool <ObjectType> &pool) {return {pool};}

		struct Memory
		{
			ObjectType *Objects;

			bool *Validators;

			IndexType *Indices;

			IndexType Capacity;
		};

		static const Memory PreallocateMemory(IndexType);

		Pool();

		Pool(IndexType capacity);

		Pool(PoolAllocator<ObjectType> &allocator);

		Pool(int, const Memory);

		void Initialize(IndexType capacity);

		void Initialize(PoolAllocator<ObjectType> &allocator);

		void Initialize(int, const Memory);

		ObjectType * Add();

		template<class DerivedType>
		ObjectType * Add();

		void RemoveAt(ObjectType *const object);

		template<typename ComparatorType> requires Comparable<ObjectType, ComparatorType>
		void Remove(ComparatorType);

		void Do(std::function <bool (ObjectType &)>);

		IndexType GetSize() const;

		IndexType GetCapacity() const;

		float GetFillPercentage();

		Iterator <ObjectType> GetStart() const {return {*this, objects_};}

		ObjectType * Get(int);

		template<typename ComparatorType> requires Comparable<ObjectType, ComparatorType>
		ObjectType * Find(ComparatorType);

		template<typename ComparatorType> requires Comparable<ObjectType, ComparatorType>
		ObjectType * Find(ComparatorType) const;

		ObjectType * GetRandom() const;

		int GetIndex(ObjectType *) const;

		bool IsFull() const {return size_ == capacity_;}

		void Reset();

		void Terminate(PoolAllocator<ObjectType> &allocator);
	};

	template<class ObjectType>
	const typename Pool<ObjectType>::Memory Pool<ObjectType>::PreallocateMemory(IndexType capacity)
	{
		auto objects = new ObjectType[capacity];
		auto indices = new IndexType[capacity];
		auto validators = new bool[capacity];

		return {objects, validators, indices, capacity};
	}

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
	Pool<ObjectType>::Pool(int capacity, const Pool<ObjectType>::Memory memory)
	{
		capacity_ = capacity;

		memorySize_ = capacity * sizeof(ObjectType);
		objects_ = memory.Objects;
		indices_ = memory.Indices;
		validators = memory.Validators;

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
	void Pool<ObjectType>::Initialize(int capacity, const Pool<ObjectType>::Memory memory) 
	{
		capacity_ = capacity;

		memorySize_ = capacity * sizeof(ObjectType);
		objects_ = new ObjectType[capacity_];
		indices_ = new IndexType[capacity_];
		validators = new bool[capacity_];

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
	void Pool<ObjectType>::RemoveAt(ObjectType *const object)
	{
		IndexType index = object - objects_;
		*(validators + index) = false;
		freeIndex_++;
		*(indices_ + freeIndex_) = index;
		size_--;
	}

	template<class ObjectType>
	template<typename ComparatorType> requires Comparable<ObjectType, ComparatorType>
	void Pool<ObjectType>::Remove(ComparatorType object)
	{
		auto objectLocation = Find(object);
		if(objectLocation != nullptr)
		{
			RemoveAt(objectLocation);
		}
	}

	template<class ObjectType>
	ObjectType* Pool<ObjectType>::GetEnd() const
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
	IndexType Pool<ObjectType>::GetSize() const
	{
		return size_;
	}

	template<class ObjectType>
	IndexType Pool<ObjectType>::GetCapacity() const
	{
		return capacity_;
	}

	template<class ObjectType>
	ObjectType * Pool<ObjectType>::GetRandom() const
	{
		if(size_ == 0)
		{
			return nullptr;
		}

		static auto objects = Array <ObjectType *> (capacity_);
		objects.Reset();

		for(auto &object : *this)
		{
			*objects.Allocate() = &object;
		}

		return *objects.GetRandom();
	}

	template<class ObjectType>
	ObjectType * Pool<ObjectType>::Get(int index)
	{
		auto counter = 0;
		for(auto &object : *this)
		{
			if(counter == index)
				return &object;

			counter++;
		}

		return nullptr;
	}

	template<class ObjectType>
	template<typename ComparatorType> requires Comparable<ObjectType, ComparatorType>
	ObjectType * Pool<ObjectType>::Find(ComparatorType comparator)
	{
		for(auto &object : *this)
		{
			if(object == comparator)
				return &object;
		}

		return nullptr;
	}

	template<class ObjectType>
	template<typename ComparatorType> requires Comparable<ObjectType, ComparatorType>
	ObjectType * Pool<ObjectType>::Find(ComparatorType comparator) const
	{
		for(auto &object : *this)
		{
			if(object == comparator)
				return &object;
		}

		return nullptr;
	}

	template<class ObjectType>
	int Pool<ObjectType>::GetIndex(ObjectType *object) const
	{
		auto counter = 0;
		for(auto &iterator : *this)
		{
			if(&iterator == object)
				return counter;
			
			counter++;
		}

		return -1;
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
