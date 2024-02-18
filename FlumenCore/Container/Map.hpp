#pragma once

#include <initializer_list>

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

	public:
		struct Iterator
		{
			Value Value_;

			Key Key_;

			Iterator() {}

			Iterator(Value value, Key key) {Value_ = value; Key_ = key;}
		};

	private:
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

		StaticMap(std::initializer_list <Iterator> list)
		{
			Initialize(list.size());

			size_ = list.size();

			auto key = keys_;
			auto value = values_;
			for(auto iterator = list.begin(); iterator != list.end(); ++iterator, ++key, ++value)
			{
				*key = iterator->Key_;
				*value = iterator->Value_;
			}
		}

		void Initialize(int capacity)
		{
			size_ = 0;
			capacity_ = capacity;

			keys_ = new Key[capacity];
			values_ = new Value[capacity];
		}

		void Initialize(int capacity, Value defaultValue)
		{
			size_ = 0;
			capacity_ = capacity;

			keys_ = new Key[capacity];
			values_ = new Value[capacity];

			for(auto value = values_; value != values_ + capacity; ++value)
			{
				*value = defaultValue;
			}
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

		Pair FindLowestValue()
		{
			auto lowestValueIndex = 0;

			if(size_ == 1)
			{
				return {values_ + lowestValueIndex, keys_ + lowestValueIndex};
			}

			for(auto i = 1; i < size_; ++i)
			{
				if(*(values_ + i) < *(values_ + lowestValueIndex))
				{
					lowestValueIndex = i;
				}
			}

			return {values_ + lowestValueIndex, keys_ + lowestValueIndex};
		}

		Pair FindHighestValue()
		{
			auto lowestValueIndex = 0;

			if(size_ == 1)
			{
				return {values_ + lowestValueIndex, keys_ + lowestValueIndex};
			}

			for(auto i = 1; i < size_; ++i)
			{
				if(*(values_ + i) > *(values_ + lowestValueIndex))
				{
					lowestValueIndex = i;
				}
			}

			return {values_ + lowestValueIndex, keys_ + lowestValueIndex};
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

		void Reset()
		{
			size_ = 0;
		}
	};
}
