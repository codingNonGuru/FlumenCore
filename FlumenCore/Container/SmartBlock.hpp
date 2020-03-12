#pragma once

#include "Block.hpp"

namespace container
{
    template <class ObjectType, int Capacity>
    class SmartBlock
    {   
        Block <ObjectType, Capacity> block;

        int size;

    public:
        SmartBlock() {}

        ObjectType * GetFirst() const 
        {
            return block.GetStart();
        }

        ObjectType * GetLast() const 
        {
            return block.GetStart() + size;
        }

        ObjectType * Allocate()
        {
            if(size == Capacity)
                return nullptr;

            auto object = block.GetStart() + size;
            size++;

            return object;
        }
    };
}
