#pragma once

#include "Container.hpp"

namespace container
{
	template<class ObjectType>
	class Grid : public Matrix
	{
	private:
		int height_;

		int width_;

		int objectCount_;

		ObjectType* objects_;

		unsigned long memorySize_;

	public:
		ObjectType* operator() (int x, int y)
		{
			if(x < 0)
				x += width_;
			else if(x >= width_)
				x -= width_;

			if(y < 0)
				y += height_;
			else if(y >= height_)
				y -= height_;

			return objects_ + (width_ * y + x);
		}

		ObjectType* Get(int x, int y)
		{
			if(x < 0)
				x = width_ - 1;
			else if(x >= width_)
				x = 0;

			if(y < 0)
				y = height_ - 1;
			else if(y >= height_)
				y = 0;

			return objects_ + (width_ * y + x);
		}

		ObjectType* GetStart()
		{
			return objects_;
		}

		ObjectType* GetEnd()
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

		void* GetData()
		{
			return (void*)objects_;
		}

		virtual unsigned long GetMemorySize()
		{
			return memorySize_;
		}

		virtual unsigned long GetMemoryCapacity()
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
		}

		Grid(int columnCount, int rowCount)
		{
			objectCount_ = rowCount * columnCount;
			objects_ = new ObjectType[objectCount_];

			width_ = columnCount;
			height_ = rowCount;

			memorySize_ = objectCount_ * sizeof(ObjectType);
			//MemoryLog::accrue(memorySize_);
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
		}

		operator *=(float factor)
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
