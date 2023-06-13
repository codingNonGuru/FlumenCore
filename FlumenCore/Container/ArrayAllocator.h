#pragma once

namespace container
{
    template <class ObjectType>
    class Array;

    template <class ObjectType>
    class ArrayAllocator
    {
        template <class SlotType>
        struct MemorySlot
        {
            SlotType *Objects;
        };

        friend class Array <ObjectType>;

        int objectsPerArray;

        int arraysPerAllocator;

        int objectsPerAllocator;

        int arrayMemoryCapacity;

        int freeArrayIndex;

        int size;

        ObjectType *objects;

        bool *arrayChecks;

        int *arrayIndices;

        MemorySlot <ObjectType> Add()
        {
            auto index = *(arrayIndices + freeArrayIndex);
            ObjectType * arrayStart = objects + index * objectsPerArray;
            *(arrayChecks + index) = true;

            freeArrayIndex--;

            return {arrayStart};
        }

        void Remove(const ObjectType *arrayObjects)
        {
            int objectIndex = arrayObjects - objects;
            int arrayIndex = objectIndex / this->objectsPerArray;

            *(arrayChecks + arrayIndex) = false;
            freeArrayIndex++;
            *(arrayIndices + freeArrayIndex) = arrayIndex;
        }

    public:
        ArrayAllocator() : objects(nullptr), arrayChecks(nullptr), arrayIndices(nullptr) {}

        ArrayAllocator(int arraysPerAllocator, int objectsPerArray)
        {
            this->arraysPerAllocator = arraysPerAllocator;

            this->objectsPerArray = objectsPerArray;
            
            this->objectsPerAllocator = this->arraysPerAllocator * this->objectsPerArray;

            this->arrayMemoryCapacity = this->objectsPerArray * sizeof(ObjectType);

            objects = new ObjectType[this->objectsPerAllocator];

            arrayIndices = malloc(this->arraysPerAllocator * sizeof(int));

            arrayChecks = malloc(this->arraysPerAllocator * sizeof(bool));

            this->freeArrayIndex = this->arraysPerAllocator - 1;

            this->size = 0;

            for(int index = 0; index < this->arraysPerAllocator; ++index)
            {
                *(arrayIndices + index) = index;
            }

            for(auto check = arrayChecks; check != arrayChecks + this->arraysPerAllocator; ++check)
            {
                *check = false;
            }
        }
    };
}