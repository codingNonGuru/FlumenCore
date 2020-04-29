#pragma once

namespace container
{
	template <class ObjectType, int Capacity>
	class Block
	{
		ObjectType objects_[Capacity];

	public:
		Block() {}

		ObjectType* Find(const ObjectType &object) 
		{
			for(auto iterator = GetStart(); iterator != GetEnd(); ++iterator)
			{
				if(*iterator == object)
					return iterator;
			}

			return nullptr;
		}

		const ObjectType * Get(int index) const 
		{
			return objects_ + index;
		}

		ObjectType* operator [] (int index)
		{
			return objects_ + index;
		}

		ObjectType* GetStart() 
		{
			return objects_;
		}

		ObjectType* GetEnd() 
		{
			return objects_ + Capacity;
		}
	};
}
