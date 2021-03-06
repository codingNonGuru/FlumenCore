#pragma once

#include <string.h>

namespace container
{
	template<int Capacity>
	class String
	{
	private:
		char values_[Capacity];

		int size_;

	public:
		String()
		{
			size_ = 0;
		}

		String(char* values)
		{
			strcpy(values_, values);
			size_ = strlen(values);
		}

		String(const char* values)
		{
			strcpy(values_, values);
			size_ = strlen(values);
		}

		void Add(const char* string)
		{
			Add(string, strlen(string));
		}

		void Add(char* string, int length)
		{
			memcpy(values_ + size_, string, length);
			size_ += length;
			*(values_ + size_) = (char)0;
		}

		void Add(const char* string, int length)
		{
			memcpy(values_ + size_, string, length);
			size_ += length;
			*(values_ + size_) = (char)0;
		}

		String& operator << (const String &string)
		{
			Add(string.values_, string.size_);

			return *this;
		}

		String& operator << (const char* string)
		{
			Add(string, strlen(string));

			return *this;
		}

		String& operator << (int value)
		{
			char valueString[32];
			sprintf(valueString, "%d", value);
			Add(valueString, strlen(valueString));

			return *this;
		}

		String operator *(int count)
		{
			auto copy = String();
			for(int i = 0; i < count; ++i)
			{
				copy << *this;
			}
			return copy;
		}

		char* Get()
		{
			return values_;
		}

		char* GetEnd()
		{
			return values_ + size_;
		}

		int GetSize()
		{
			return size_;
		}

		String GetFirstCharacter()
		{
			char characters[] = {values_[0], 0};
			return characters;
		}

		operator const char*()
		{
			return values_;
		}

		bool operator == (String<Capacity>& other)
		{
			return strcmp(other.Get(), this->Get()) == 0;
		}

		//~String() 
		//{
			//std::cout<<(void*)values_<<"\n";
		//}
	};
}
