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
            if(instance == nullptr)
            {
                instance = static_cast <ObjectType *>(this);
            }
        }

        Singleton(Singleton &) = delete;

    public:
        static ObjectType *Get()
        {
            if(instance == nullptr)
            {
                instance = new ObjectType();
            }

            return instance;
        }
    };

    template <class ObjectType>
    ObjectType *Singleton<ObjectType>::instance = nullptr;
}