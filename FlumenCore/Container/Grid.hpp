#pragma once

#include "Container.hpp"

namespace container
{
	template<class ObjectType>
	class Grid : public Matrix
	{
	public:
		template<class IteratorType>
        class Iterator
        {
            friend class Grid <IteratorType>;

            friend Iterator <IteratorType> begin(const Grid <IteratorType> &);

            friend Iterator <IteratorType> end(const Grid <IteratorType> &);

            const Grid <IteratorType> &grid;

            IteratorType *element;

        public:
			Iterator(const Grid <IteratorType> &_grid, IteratorType *_element) : 
                grid(_grid), element(_element)
            {}

            IteratorType & operator *() {return *element;}

            bool operator !=(const Iterator <IteratorType> &other) {return element != other.element;}

            Iterator <IteratorType> & operator++() 
            {
				if(element != grid.GetEnd())
				{
                	element++;
				}

                return *this;
            }

			IteratorType * operator ->()
			{
				return element;
			}

			operator IteratorType *()
			{
				return element;
			}
        };

	private:
		int height_;

		int width_;

		int objectCount_;

		ObjectType* objects_;

		unsigned long memorySize_;

		bool isBounded;

	public:
		friend Iterator <ObjectType> begin(const Grid <ObjectType> &grid) {return {grid, grid.GetStart()};}

        friend Iterator <ObjectType> end(const Grid <ObjectType> &grid) {return {grid, grid.GetEnd()};}

		struct Memory
		{
			ObjectType *Objects;

			int Capacity;
		};

		static const Memory PreallocateMemory(int capacity)
		{
			auto objects = (ObjectType *)malloc(capacity * sizeof(ObjectType));

			return {objects, capacity};
		}

		ObjectType* operator() (int x, int y)
		{
			return Get(x, y);
		}

		ObjectType* Get(int x, int y) const
		{
			if(x < 0)
			{
				if(isBounded)
					return nullptr;

				x += width_;
			}
			else if(x >= width_)
			{
				if(isBounded)
					return nullptr;

				x -= width_;
			}

			if(y < 0)
			{
				if(isBounded)
					return nullptr;

				y += height_;
			}
			else if(y >= height_)
			{
				if(isBounded)
					return nullptr;

				y -= height_;
			}

			return objects_ + (width_ * y + x);
		}

		ObjectType* GetStart() const
		{
			return objects_;
		}

		ObjectType* GetEnd() const
		{
			return objects_ + objectCount_;
		}

		int GetWidth() const
		{
			return width_;
		}

		int GetHeight() const
		{
			return height_;
		}

		int GetObjectCount() const
		{
			return width_ * height_;
		}

		void* GetData()
		{
			return (void*)objects_;
		}

		virtual unsigned long GetMemorySize() const override
		{
			return memorySize_;
		}

		virtual unsigned long GetMemoryCapacity() const override
		{
			return memorySize_;
		}

		void TransferData(Grid<ObjectType> &other)
		{
			other.objects_ = objects_;

			other.width_ = width_;
			other.height_ = height_;
			other.memorySize_ = memorySize_;
			other.objectCount_ = objectCount_;

			objects_ = nullptr;
		}

		Grid()
		{
			objectCount_ = 0;
			objects_ = nullptr;

			width_ = 0;
			height_ = 0;

			isBounded = false;
		}

		Grid(int columnCount, int rowCount)
		{
			objectCount_ = rowCount * columnCount;
			objects_ = new ObjectType[objectCount_];

			width_ = columnCount;
			height_ = rowCount;

			memorySize_ = objectCount_ * sizeof(ObjectType);
			//MemoryLog::accrue(memorySize_);

			isBounded = false;
		}

		Grid(Grid<ObjectType> *other)
		{
			other->TransferData(*this);
		}

		void Initialize(int columnCount, int rowCount)
		{
			Destroy();

			objectCount_ = rowCount * columnCount;
			objects_ = new ObjectType[objectCount_];

			width_ = columnCount;
			height_ = rowCount;

			memorySize_ = objectCount_ * sizeof(ObjectType);
			//MemoryLog::accrue(memorySize_);

			isBounded = false;
		}

		void Initialize(int columnCount, int rowCount, const Memory memory)
		{
			objectCount_ = rowCount * columnCount;
			objects_ = memory.Objects;

			width_ = columnCount;
			height_ = rowCount;

			memorySize_ = objectCount_ * sizeof(ObjectType);

			isBounded = false;
		}

		void Bound() 
		{
			isBounded = true;
		}

		void Unbound()
		{
			isBounded = false;
		}

		Grid& operator *=(float factor)
		{
			for(auto cell = GetStart(); cell != GetEnd(); ++cell)
			{
				*cell *= factor;
			}
		}

		~Grid()
		{
			Destroy();
		}

		void Destroy()
		{
			if(objects_ != nullptr)
				delete[] objects_;

			objects_ = nullptr;

			//MemoryLog::accrue(-memorySize_);
		}
	};
}
