#pragma once

namespace container
{
    template <class ObjectType>
    class Pool;

    template <class ObjectType>
    class PoolAllocator
    {
        template <class SlotType>
        struct PoolMemorySlot
        {
            SlotType *Objects;

            int *FreeIndices;

            bool *Checks;
        };

        friend class Pool <ObjectType>;

        int objectsPerPool;

        int poolsPerAllocator;

        int objectsPerAllocator;

        int poolMemoryCapacity;

        int freePoolIndex;

        int size;

        ObjectType *objects;

        bool *objectChecks;

        int *objectIndices;

        bool *poolChecks;

        int *poolIndices;

        PoolMemorySlot <ObjectType> Add()
        {
            auto index = *(poolIndices + freePoolIndex);
            ObjectType * poolStart = objects + index * objectsPerPool;
            *(poolChecks + index) = true;

            freePoolIndex--;

            return {poolStart, objectIndices + index * objectsPerPool, objectChecks + index * objectsPerPool};
        }

        void Remove(const ObjectType *poolObjects)
        {
            int objectIndex = poolObjects - objects;
            int poolIndex = objectIndex / this->objectsPerPool;

            *(poolChecks + poolIndex) = false;
            freePoolIndex++;
            *(poolIndices + freePoolIndex) = poolIndex;
        }

    public:
        PoolAllocator() : objects(nullptr), objectIndices(nullptr), objectChecks(nullptr) {}

        PoolAllocator(int poolsPerAllocator, int objectsPerPool)
        {
            this->poolsPerAllocator = poolsPerAllocator;

            this->objectsPerPool = objectsPerPool;
            
            this->objectsPerAllocator = this->poolsPerAllocator * this->objectsPerPool;

            this->poolMemoryCapacity = this->objectsPerPool * sizeof(ObjectType);

            objects = new ObjectType[this->objectsPerAllocator];

            objectIndices = malloc(this->objectsPerAllocator * sizeof(int));

            objectChecks = malloc(this->objectsPerAllocator * sizeof(bool));

            poolIndices = malloc(this->poolsPerAllocator * sizeof(int));

            poolChecks = malloc(this->poolsPerAllocator * sizeof(bool));

            this->freePoolIndex = this->poolsPerAllocator - 1;

            this->size = 0;

            for(int index = 0; index < this->poolsPerAllocator; ++index)
            {
                *(poolIndices + index) = index;
            }

            for(auto check = poolChecks; check != poolChecks + this->poolsPerAllocator; ++check)
            {
                *check = false;
            }
        }
    };
}