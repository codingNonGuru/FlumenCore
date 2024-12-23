#pragma once

#define KILOBYTE 1024

#define MEGABYTE 1048576

#include <glm/glm.hpp>

typedef float Float;
typedef glm::vec2 Float2;
typedef glm::vec3 Float3;
typedef glm::vec4 Float4;

typedef int Integer;
typedef glm::ivec2 Integer2;
typedef glm::ivec3 Integer3;
typedef glm::ivec4 Integer4;

typedef unsigned char Byte;

struct Byte3
{
	Byte r_, g_, b_;

	Byte3() {}

	Byte3(Byte r, Byte g, Byte b) : r_(r), g_(g), b_(b) {}
};

struct Byte4
{
	Byte r_, g_, b_, a_;

	Byte4() {}

	Byte4(Byte r, Byte g, Byte b, Byte a) : r_(r), g_(g), b_(b), a_(a) {}
};

typedef float Weight;
typedef Float2 Range;
typedef glm::vec2 Position2;
typedef glm::vec3 Position3;
typedef float Rotation;
typedef glm::vec2 Direction2;
typedef glm::vec3 Direction3;
typedef float Scale;
typedef glm::vec2 Scale2;
typedef glm::ivec2 Size;
typedef glm::mat4 Matrix;
typedef int DrawOrder;

struct Opacity
{
	float Value;

	Opacity() {}

	explicit Opacity(float value) : Value(value) {}

	//Opacity(bool) = delete;

	//Opacity(const char * const) = delete;

	operator float() {return Value;}

	float &Get() {return Value;}

	float operator *(const float &value) const {return Value * value;}

	Opacity operator *(const Opacity &opacity) const {return Opacity(Value * opacity.Value);}

	Opacity &operator=(const float &value) {Value = value; return *this;}
};

typedef unsigned int Index;
typedef unsigned int Length;

#include "Container/Container.hpp"
#include "Container/Map.hpp"
//#include "Container/List.hpp"
#include "Container/String.hpp"
#include "Container/Array.hpp"
#include "Container/Pool.hpp"
#include "Container/Grid.hpp"
#include "Container/Block.hpp"
#include "Container/SmartBlock.hpp"

typedef container::String<16> ShortWord;
typedef container::String<32> Word;
typedef container::String<64> LongWord;
typedef container::String<256> Phrase;

typedef container::String<64> FileName;
typedef container::String<256> FilePath;

typedef container::Container Container;
template <class Type>
using Array = container::Array<Type>;
template <class Type>
using Grid = container::Grid<Type>;
template <class Type>
using Pool = container::Pool<Type>;
template <class ValueType, class KeyType = Word>
using Map = container::StaticMap <ValueType, KeyType>;

#define ITERATE(grid) for(auto cell = grid.GetStart(); cell != grid.GetEnd(); ++cell)

#include "Delegate/FastDelegate.hpp"

typedef fastdelegate::FastDelegate0<> Callback;

void* AllocateMemory(unsigned int);

void* CopyMemory(void *, const void *, size_t);

int CompareMemory(const void *, const void *, size_t);

int CompareStrings(const char *, const char *);

Length GetStringLength(const char *);

const char* FindString(const char*, const char*);

const char* FindLastOccurrence(const char*, char);

#define DEBUG_OPENGL DebugGl(__FILE__, __LINE__);

void DebugGl(const char*, int);

static constexpr auto PI = 3.1415926f;

static constexpr auto TWO_PI = PI * 2.0f;

static constexpr auto HALF_PI = PI * 0.5f;

struct Rectangle
{
	Integer2 Position;

	Integer2 Size;

	bool IsInside(Integer2 location) const
	{
		if(location.x < Position.x)
			return false;

		if(location.x > Position.x + Size.x)
			return false;

		if(location.y < Position.y)
			return false;

		if(location.y > Position.y + Size.y)
			return false;

		return true;
	}
};
