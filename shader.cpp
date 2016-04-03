#include "shader.h"

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif

#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

const std::string fragSource =
#include "shader.frag"
;
const std::string vertSource =
#include "shader.vert"
;
const std::string geomSource =
#include "shader.geom"
;

std::string Shader::ReadFile(const std::string &file) {
	// Open file
	std::ifstream t(file);

	if (!t.is_open()) {
		return "";
	}

	// Read file into buffer
	std::stringstream buffer;
	buffer << t.rdbuf();

	// Make a std::string and fill it with the contents of buffer
	std::string fileContent = buffer.str();

	return fileContent;
}

void Shader::BindAttributeLocation(int index, const std::string &attribute) {
	// Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
	// Attribute locations must be setup before calling glLinkProgram
	glBindAttribLocation(shaderProgram, index, attribute.c_str());
}

void Shader::UseProgram() {
	// Load the shader into the rendering pipeline
	glUseProgram(shaderProgram);
}

bool Shader::Init() {
	// Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
	shaderProgram = glCreateProgram();

	// Bind the location of our attributes
	BindAttributeLocation(0, "in_Position");
	BindAttributeLocation(1, "in_Color");

	if (!LoadShader("geometry", geomSource, GL_GEOMETRY_SHADER))
		return false;

	if (!LoadShader("vertex", vertSource, GL_VERTEX_SHADER))
		return false;

	if (!LoadShader("fragment", fragSource, GL_FRAGMENT_SHADER))
		return false;

	// All shaders have been create, now we must put them together into one large object
	return LinkShaders();
}

bool Shader::LoadShader(const std::string &fileName, const std::string source, int shaderType) {
	std::cout << "Loading Shader : " << fileName << std::endl;

	int shaderId = CreateShader(source, shaderType);

	if (TryCompileShader(shaderId)) {
		glAttachShader(shaderProgram, shaderId);
		shaderIds.push_back(shaderId);

		return true;
	}

	return false;
}

bool Shader::LinkShaders() {
	// Link. At this point, our shaders will be inspected/optimized and the binary code generated
	// The binary code will then be uploaded to the GPU
	glLinkProgram(shaderProgram);

	// Verify that the linking succeeded
	int isLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *) &isLinked);

	if (isLinked == false)
		PrintShaderLinkingError(shaderProgram);

	return isLinked != 0;
}

void Shader::PrintShaderLinkingError(int32_t shaderId) {
	std::cout << "=======================================\n";
	std::cout << "Shader linking failed : " << std::endl;

	// Find length of shader info log
	int maxLength;
	glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	std::cout << "Info Length : " << maxLength << std::endl;

	// Get shader info log
	char *shaderProgramInfoLog = new char[maxLength];
	glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

	std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

	/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
	/* In this simple program, we'll just leave */
	delete shaderProgramInfoLog;
	return;
}

// If something went wrong whil compiling the shaders, we'll use this function to find the error
void Shader::PrintShaderCompilationErrorInfo(int shaderId) {
	std::cout << "=======================================\n";
	std::cout << "Shader compilation failed : " << std::endl;

	// Find length of shader info log
	int maxLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	// Get shader info log
	char *shaderInfoLog = new char[maxLength];
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog);

	// Print shader info log
	std::cout << "\tError info : " << shaderInfoLog << std::endl;

	std::cout << "=======================================\n\n";
	delete shaderInfoLog;
}

void Shader::CleanUp() {
	/* Cleanup all the things we bound and allocated */
	glUseProgram(0);
	glDetachShader(shaderProgram, vertexshader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteProgram(shaderProgram);


	glDeleteShader(vertexshader);
	glDeleteShader(fragmentShader);
}

int Shader::CreateShader(const std::string &source, GLenum shaderType) {
	// Read file as std::string
	std::string str = source; // We'll skip ReadFile() call as we're currently including the source through C++11 magic

	// c_str() gives us a const char*, but we need a non-const one
	char *src = const_cast<char *>( str.c_str());
	int32_t size = str.length();

	// Create an empty vertex shader handle
	int shaderId = glCreateShader(shaderType);

	// Send the vertex shader source code to OpenGL
	glShaderSource(shaderId, 1, &src, &size);

	return shaderId;
}

bool Shader::TryCompileShader(int shaderId) {
	// Compile the vertex shader
	glCompileShader(shaderId);

	// Ask OpenGL if the shaders was compiled
	int wasCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &wasCompiled);

	// Return false if compilation failed
	if (wasCompiled == 0) {
		PrintShaderCompilationErrorInfo(shaderId);
		return false;
	}

	return true;
}



