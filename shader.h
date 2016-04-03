#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif


class Shader {
public:
	void BindAttributeLocation(int index, const std::string &attribute);

	void UseProgram();

	bool Init();

	bool LoadShader(const std::string &fileName, const std::string source, int shaderType);

	void CleanUp();

private:
	int CreateShader(const std::string &source, GLenum shaderType);

	bool TryCompileShader(int shaderId);

	bool LinkShaders();

	std::string ReadFile(const std::string &file);

	void PrintShaderLinkingError(int32_t shaderId);

	// If something went wrong while compiling the shaders, we'll use this function to find the error
	void PrintShaderCompilationErrorInfo(int shaderId);

	// The handle to our shader program
	GLuint shaderProgram;

	// The handles to the individual shader
	GLuint vertexshader, fragmentShader;

	std::vector<int32_t> shaderIds;
};
