#ifndef PLATFORM_H
#define PLATFORM_H

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
		float v[ 2 ];
		struct { float x, y; };
	};
	float& operator[](int i) { return v[ i ]; }
};

struct vec3
{
	union
	{
		float v[ 3 ];
		struct { float x, y, z; };
	};
	float& operator[](int i) { return v[ i ]; }
};

#endif //PLATFORM_H
