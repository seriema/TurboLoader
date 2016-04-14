#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(__APPLE__)
	#include <OpenGL/gl3.h>
#elif defined(_WIN32)
#	include <GL/gl3.h>
#else // RPI
	#include <GLES2/gl2.h>
#endif

#endif //PLATFORM_H
