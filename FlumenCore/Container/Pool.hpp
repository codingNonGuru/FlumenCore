#pragma once

namespace container
{
	template<class ObjectType, typename IndexType = int>
	class Pool
	{
	private:
		ObjectType* objects_;

		IndexType capacity_;

		IndexType size_;

		IndexType* indices_;

		IndexType freeIndex_;

		unsigned long memorySize_;

	public:
		Pool();

		Pool(IndexType capacity);

		void Initialize(IndexType capacity);

		ObjectType *const Add();

		template<class DerivedType>
		ObjectType *const Add();

		void Remove(ObjectType *const object);

		ObjectType* Get(IndexType index);

		ObjectType* GetStart();

		ObjectType* GetEnd();

		IndexType GetSize();

		IndexType GetCapacity();

		float GetFillPercentage();
	};

	template<class ObjectType, typename IndexType>
	Pool<ObjectType, IndexType>::Pool()
	{
	}

	template<class ObjectType, typename IndexType>
	Pool<ObjectType, IndexType>::Pool(IndexType capacity) : capacity_(capacity)
	{
		memorySize_ = capacity_ * sizeof(ObjectType);
		size_ = 0;
		objects_ = new ObjectType[capacity_];

		indices_ = new IndexType[capacity_];
		freeIndex_ = capacity_ - 1;
		for(IndexType index = 0; index < capacity_; ++index)
		{
			*(indices_ + index) = index;
		}
	}

	template<class ObjectType, typename IndexType>
	void Pool<ObjectType, IndexType>::Initialize(IndexType capacity)
	{
		capacity_ = capacity;
		memorySize_ = capacity_ * sizeof(ObjectType);
		size_ = 0;
		objects_ = new ObjectType[capacity_];

		indices_ = new IndexType[capacity_];
		freeIndex_ = capacity_ - 1;
		for(IndexType index = 0; index < capacity_; ++index)
		{
			*(indices_ + index) = index;
		}
	}

	template<class ObjectType, typename IndexType>
	ObjectType *const Pool<ObjectType, IndexType>::Add()
	{
		ObjectType *const newObject = (objects_ + *(indices_ + freeIndex_));
		freeIndex_--;
		size_++;
		if(size_ == capacity_ + 1)
		{
			return nullptr;
		}
		return newObject;
	}

	template<class ObjectType, typename IndexType>
	template<class DerivedType>
	ObjectType *const Pool<ObjectType, IndexType>::Add()
	{
		ObjectType *const newObject = (objects_ + *(indices_ + freeIndex_));
		*(long*)newObject = *(long*)&DerivedType();
		freeIndex_--;
		size_++;

		if(size_ == capacity_ + 1)
		{
			return nullptr;
		}
		return newObject;
	}

	template<class ObjectType, typename IndexType>
	void Pool<ObjectType, IndexType>::Remove(ObjectType *const object)
	{
		IndexType index = object - objects_;
		freeIndex_++;
		*(indices_ + freeIndex_) = index;
		size_--;
	}

	template<class ObjectType, typename IndexType>
	ObjectType* Pool<ObjectType, IndexType>::Get(IndexType index)
	{
		return objects_ + index;
	}

	template<class ObjectType, typename IndexType>
	ObjectType* Pool<ObjectType, IndexType>::GetStart()
	{
		return objects_;
	}

	template<class ObjectType, typename IndexType>
	ObjectType* Pool<ObjectType, IndexType>::GetEnd()
	{
		return (objects_ + capacity_);
	}

	template<class ObjectType, typename IndexType>
	IndexType Pool<ObjectType, IndexType>::GetSize()
	{
		return size_;
	}

	template<class ObjectType, typename IndexType>
	IndexType Pool<ObjectType, IndexType>::GetCapacity()
	{
		return capacity_;
	}

	template<class ObjectType, typename IndexType>
	float Pool<ObjectType, IndexType>::GetFillPercentage()
	{
		return ((float)size_ / (float)capacity_) * 100.0f;
	}
}
