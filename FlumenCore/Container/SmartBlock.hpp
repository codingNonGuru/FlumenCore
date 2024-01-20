#pragma once

#include "Block.hpp"

namespace container
{
    template <typename ObjectType, int Capacity>
    class SmartBlock
    {   
        class Iterator
		{
			friend class SmartBlock <ObjectType, Capacity>;

			friend const Iterator begin(const SmartBlock <ObjectType, Capacity> &);

			friend const Iterator end(const SmartBlock <ObjectType, Capacity> &);

			const SmartBlock <ObjectType, Capacity> &block;

			ObjectType const *element;

			Iterator(const SmartBlock <ObjectType, Capacity> &_block, ObjectType const *_element) : 
				block(_block), element(_element) {}

			Iterator(const SmartBlock <ObjectType, Capacity> &_block) : 
				block(_block), element(block.GetLast()) {}

		public:
			const ObjectType & operator *() {return *element;}

			bool operator !=(const Iterator &other) {return element != other.element;}

			Iterator &operator++() 
			{
				if(element != block.GetLast())
				{
					element++;
				}

				return *this;
			}

			ObjectType * operator ->()
			{
				return element;
			}

			operator ObjectType *()
			{
				return element;
			}
		};

        Block <ObjectType, Capacity> block;

        int size;

    public:
        friend const Iterator begin(const SmartBlock <ObjectType, Capacity> &array) 
        {
            const Iterator iterator(array, array.GetFirst());
            return iterator;
        }

        friend const Iterator end(const SmartBlock <ObjectType, Capacity> &array) 
        {
            const Iterator iterator(array, array.GetLast());
            return iterator;
        }

        SmartBlock() : size(0) {}

        int GetSize() const {return size;}

		ObjectType *Get(int index)
        {
            return block.GetStart() + index;
        }

        ObjectType const *Get(int index) const
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

		ObjectType *operator [] (int index)
		{
			return block[index];
		}

        ObjectType *GetFirst()  
        {
            return block.GetStart();
        }

        ObjectType *GetLast() 
        {
            return block.GetStart() + size;
        }

		ObjectType const *GetFirst() const 
        {
            return block.GetStart();
        }

        ObjectType const *GetLast() const
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
