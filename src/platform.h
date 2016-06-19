#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
	// Windows (windef.h) defines macros we don't want.
	// Reference: http://stackoverflow.com/a/4914108/703921
	#define NOMINMAX
	#define UNICODE
	#define STRICT
#endif

#include <cstdint>
#include <utility>

#include <SDL.h>
#include <glad/glad.h>

typedef std::uint8_t                    u8;
typedef std::uint32_t                   u32;

struct vec2
{
	union
	{
		float raw[ 2 ];
		struct { float x, y; };
	};
	inline float operator[](int i) { return raw[ i ]; }
};

struct vec3
{
	union
	{
		float raw[ 3 ];
		struct { float x, y, z; };
	};
	inline float operator[](int i) { return raw[ i ]; }
};

struct vec4
{
	union
	{
		float raw[ 4 ];
		struct { float w, x, y, z; };
	};
	inline float operator[](int i) { return raw[ i ]; }
};

#endif //PLATFORM_H
