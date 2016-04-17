
#include <iostream>
#include <fstream>
#include "shader_program.h"


ShaderProgram::ShaderProgram ()
{
	_programHandle = glCreateProgram();
}


ShaderProgram::~ShaderProgram ()
{
	glDeleteProgram(_programHandle);
}


ShaderProgram::operator GLuint () {
	return _programHandle; }


template<typename T>
void ShaderProgram::fail (GLuint shaderHandle)
{
	GLsizei len;
	glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &len);
	GLchar *msg = new GLchar[len];
	glGetShaderInfoLog(shaderHandle, len, &len, msg);
	delete[] msg;
	throw T(msg);
}


template<typename T>
void ShaderProgram::fail ()
{
	GLsizei len;
	glGetProgramiv(_programHandle, GL_INFO_LOG_LENGTH, &len);
	GLchar *msg = new GLchar[len];
	glGetProgramInfoLog(_programHandle, len, &len, msg);
	delete[] msg;
	throw T(msg);
}


void ShaderProgram::compile (const char* filename, GLenum shaderType)
{
	std::ifstream file(filename);

	if (!file.is_open())
		throw FileOpenError("File not found or could not be opened.");

	std::string str;
	getline(file, str, (char) EOF);
	file.close();
	const GLchar * src = str.c_str();

	GLuint shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, &src, NULL);
	glCompileShader(shaderHandle);

	GLint success;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
		fail<ShaderCompileError>(shaderHandle);

	glAttachShader(_programHandle, shaderHandle);
	glDeleteShader(shaderHandle);
}


GLuint ShaderProgram::link ()
{
	glLinkProgram(_programHandle);
	GLint success;

	glGetProgramiv(_programHandle, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
		fail<ProgramLinkError>();

	glValidateProgram(_programHandle);
	glGetProgramiv(_programHandle, GL_VALIDATE_STATUS, &success);
	if (success != GL_TRUE)
		fail<ProgramValidateError>();

	return _programHandle;
}
