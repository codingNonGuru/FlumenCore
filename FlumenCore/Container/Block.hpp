#pragma once

namespace container
{
	template <class ObjectType, int Capacity>
	class Block
	{
		class Iterator
		{
			friend class Block <ObjectType, Capacity>;

			friend const Iterator begin(const Block <ObjectType, Capacity> &);

			friend const Iterator end(const Block <ObjectType, Capacity> &);

			const Block <ObjectType, Capacity> &block;

			ObjectType const *element;

			Iterator(const Block <ObjectType, Capacity> &_block, ObjectType const *_element) : 
				block(_block), element(_element) {}

		public:
			const ObjectType & operator *() {return *element;}

			bool operator !=(const Iterator &other) {return element != other.element;}

			Iterator &operator++() 
			{
				if(element != &block.objects_[Capacity])
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

		ObjectType objects_[Capacity];

	public:
		friend const Iterator begin(const Block <ObjectType, Capacity> &array) 
		{
			const Iterator iterator(array, array.GetStart());
			return iterator;
		}

        friend const Iterator end(const Block <ObjectType, Capacity> &array) 
		{
			const Iterator iterator (array, array.GetEnd());
			return iterator;
		}

		Block() {}

		Block(ObjectType object)
		{
			for(auto iterator = GetStart(); iterator != GetEnd(); ++iterator)
			{
				*iterator = object;
			}
		}

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

		ObjectType const *GetStart() const
		{
			return &objects_[0];
		}

		ObjectType const *GetEnd() const
		{
			return &objects_[Capacity];
		}
	};
}
