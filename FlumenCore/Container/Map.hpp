#pragma once

namespace container
{
	template<class Value, class Key>
	class StaticMap
	{
		struct Pair
		{
			Value* Value_;

			Key* Key_;

			Pair() {Key_ = nullptr; Value_ = nullptr;}

			Pair(Value* value, Key* key) {Value_ = value; Key_ = key;}
		};

		Key* keys_;

		Value* values_;

		int size_;

		int capacity_;

	public:
		StaticMap() {}

		StaticMap(int capacity)
		{
			Initialize(capacity);
		}

		void Initialize(int capacity)
		{
			size_ = 0;
			capacity_ = capacity;

			keys_ = new Key[capacity];
			values_ = new Value[capacity];
		}

		int GetSize() const
		{
			return size_;
		}

		Value* Get(Key key)
		{
			int index = 0;
			for(auto currentKey = keys_; currentKey != keys_ + size_; ++currentKey)
			{
				if(*currentKey == key)
				{
					return values_ + index;
				}

				index++;
			}

			return nullptr;
		}

		Pair GetPair(Key key)
		{
			int index = 0;
			for(auto currentKey = keys_; currentKey != keys_ + size_; ++currentKey)
			{
				if(*currentKey == key)
				{
					return Pair(values_ + index, currentKey);
				}

				index++;
			}

			return Pair();
		}

		Value* Add(Key key)
		{
			if(size_ == capacity_)
				return nullptr;

			auto value = values_ + size_;
			*(keys_ + size_) = key;
			size_++;

			return value;
		}

		Value* GetStart()
		{
			return values_;
		}

		Value* GetEnd()
		{
			return values_ + size_;
		}

		Key* GetFirstKey()
		{
			return keys_;
		}

		Key* GetLastKey()
		{
			return keys_ + size_;
		}

		void Destroy()
		{
			if(keys_ != nullptr)
				delete[] keys_;

			keys_ = nullptr;

			if(values_ != nullptr)
				delete[] values_;

			values_ = nullptr;
		}
	};
}
