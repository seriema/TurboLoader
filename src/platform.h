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

typedef std::uint8_t  u8;
typedef std::uint32_t u32;

#endif //PLATFORM_H
