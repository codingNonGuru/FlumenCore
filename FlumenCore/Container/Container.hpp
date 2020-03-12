#pragma once

#include <iostream>
#include <string.h>

class MemoryLog
{
public:
	static int64_t usedAmount_;

	static void accrue(int64_t);
};

namespace container
{
	class Container
	{
	public:
		virtual void* GetData() {return nullptr;}

		virtual unsigned long GetMemorySize() {return 0;}

		virtual unsigned long GetMemoryCapacity() {return 0;}
	};

	class Matrix : public Container
	{
	public:
		virtual int GetWidth() const {return 0;}

		virtual int GetHeight() const {return 0;}

		virtual unsigned long GetMemorySize() {return 0;}

		virtual unsigned long GetMemoryCapacity() {return 0;}
	};

	template<class O>
	class LayeredGrid : public Matrix
	{
	private:
		int height_;

		int width_;

		int depth_;

		int objectCount_;

		int layerSize_;

		O* objects_;

		long memorySize_;

	public:
		O* operator() (int x, int y, int z)
		{
			if(x < 0)
				x += width_;
			else if(x >= width_)
				x -= width_;

			if(y < 0)
				y += height_;
			else if(y >= height_)
				y -= height_;

			if(z < 0)
				z += depth_;
			else if(z >= depth_)
				z -= depth_;

			return objects_ + (layerSize_ * z + width_ * y + x);
		}

		O* Get(int x, int y, int z)
		{
			if(x < 0)
				x += width_;
			else if(x >= width_)
				x -= width_;

			if(y < 0)
				y += height_;
			else if(y >= height_)
				y -= height_;

			if(z < 0)
				z += depth_;
			else if(z >= depth_)
				z -= depth_;

			return objects_ + (layerSize_ * z + width_ * y + x);
		}

		O* GetStart()
		{
			return objects_;
		}

		O* GetEnd()
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

		int GetDepth() const
		{
			return depth_;
		}

		void* GetData()
		{
			return objects_;
		}

		unsigned long GetMemorySize()
		{
			return memorySize_;
		}

		unsigned long GetMemoryCapacity()
		{
			return memorySize_;
		}

		int GetTierMemory() const
		{
			return layerSize_ * sizeof(O);
		}

		LayeredGrid()
		{
			objects_ = nullptr;
		}

		LayeredGrid(int width, int height, int depth)
		{
			objectCount_ = height * width * depth;
			objects_ = new O[objectCount_];
			width_ = width;
			height_ = height;
			depth_ = depth;
			memorySize_ = objectCount_ * sizeof(O);
			layerSize_ = height_ * width_;

			MemoryLog::accrue(memorySize_);
		}

		void Initialize(int columnCount, int rowCount, int depth)
		{
			objectCount_ = rowCount * columnCount * depth;
			objects_ = new O[objectCount_];
			width_ = columnCount;
			height_ = rowCount;
			depth_ = depth;
			memorySize_ = objectCount_ * sizeof(O);
			layerSize_ = height_ * width_;

			MemoryLog::accrue(memorySize_);
		}

		void Destroy()
		{
			if(objects_ != nullptr)
				delete[] objects_;

			objects_ = nullptr;

			MemoryLog::accrue(-memorySize_);
		}
	};
}
