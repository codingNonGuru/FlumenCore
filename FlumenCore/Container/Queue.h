#pragma once

namespace container
{
    template <typename ElementType>
    class Queue
    {
        ElementType *elements;

        int capacity;

        ElementType *firstElement;

        ElementType *lastElement;

        int size;

    public:
        Queue(int newCapacity) : capacity(newCapacity), size(0)
        {
            elements = malloc(sizeof(ElementType) * capacity);

            firstElement = elements;

            lastElement = firstElement;
        }

        ElementType *Grow()
        {
            if(size == capacity)
                return nullptr;

            auto element = lastElement;

            lastElement++;
            if(lastElement == elements + capacity)
            {
                lastElement = elements;
            }

            size++;
            
            return element;
        }

        ElementType *Pop()
        {
            if(size == 0)
                return nullptr;

            auto element = firstElement;

            firstElement++;
            if(firstElement == elements + capacity)
            {
                firstElement = elements;
            }

            size--;

            return element;
        }

        bool IsEmpty() const 
        {
            return size == 0;
        }

        bool IsFull() const 
        {
            return size == capacity;
        }
    };
}