#pragma once

namespace core
{
    template <class ObjectType>
    class Singleton
    {
        static ObjectType *instance;

    protected:
        Singleton()
        {
            instance = this;
        }

        Singleton(Singleton &) = delete;

    public:
        static ObjectType * Get()
        {
            if(instance == nullptr)
            {
                return new ObjectType();
            }

            return instance;
        }
    };

    template <class ObjectType>
    ObjectType * Singleton<ObjectType>::instance = nullptr;
}