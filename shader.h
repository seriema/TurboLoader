#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif



class Shader
{
public:
	std::string ReadFile(const char* file);

	void BindAttributeLocation(int index, const std::string &attribute);

	void UseProgram();

	bool Init();

	bool LoadVertexShader(const std::string &filename);

	bool LoadFragmentShader(const std::string &filename);

	bool LinkShaders();

	void PrintShaderLinkingError(int32_t shaderId);

	// If something went wrong whil compiling the shaders, we'll use this function to find the error
	void PrintShaderCompilationErrorInfo(int32_t shaderId);

	void CleanUp();

	// The handle to our shader program
	GLuint shaderProgram;

	// The handles to the induvidual shader
	GLuint vertexshader, fragmentShader;

};
