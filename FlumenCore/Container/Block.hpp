#pragma once

namespace container
{
	template <class ObjectType, int Capacity>
	class Block
	{
		ObjectType objects_[Capacity];

	public:
		Block() {}

		ObjectType* Find(int index) 
		{
			return objects_ + index;
		}

		ObjectType* operator [] (int index)
		{
			return objects_ + index;
		}

		ObjectType* GetStart() const
		{
			return objects_;
		}

		ObjectType* GetEnd() const
		{
			return objects_ + Capacity;
		}
	};
}
