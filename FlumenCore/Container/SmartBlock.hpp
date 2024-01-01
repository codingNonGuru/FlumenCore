#pragma once

#include "Block.hpp"

namespace container
{
    template <typename ObjectType, int Capacity>
    class SmartBlock
    {   
        Block <ObjectType, Capacity> block;

        int size;

    public:
        SmartBlock() : size(0) {}

        int GetSize() const {return size;}

        ObjectType * Get(int index)
        {
            return block.GetStart() + index;
        }

        ObjectType * Find(const ObjectType &object) 
		{
			for(auto iterator = GetFirst(); iterator != GetLast(); ++iterator)
			{
				if(*iterator == object)
					return iterator;
			}

			return nullptr;
		}

		ObjectType * operator [] (int index)
		{
			return block[index];
		}

        ObjectType * GetFirst()  
        {
            return block.GetStart();
        }

        ObjectType * GetLast() 
        {
            return block.GetStart() + size;
        }

        ObjectType * Add()
        {
            if(size == Capacity)
                return nullptr;

            auto object = block.GetStart() + size;
            size++;

            return object;
        }

        void Clear()
        {
            size = 0;
        }
    };
}
