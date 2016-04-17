#include "shader.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define VERT_SHADER_SUFFIX ".vert"
#define FRAG_SHADER_SUFFIX ".frag"



// TODO: HACK: This is because we haven't figured out how to bundle shaders with CMake yet
const std::string fragmentShaderSource =
#include "debug.frag"
;
const std::string vertexShaderSource =
#include "debug.vert"
;



std::map<std::string, Shader *> Shader::shader_cache;


Shader::Shader(const std::string & name) : m_name(name)
{
	GLuint v = compile_shader(name + VERT_SHADER_SUFFIX, GL_VERTEX_SHADER);
	GLuint f = compile_shader(name + FRAG_SHADER_SUFFIX, GL_FRAGMENT_SHADER);
	link_program(v, f);

	link_uniforms();
	link_attributes();
}


Shader::~Shader()
{
	glDeleteProgram(m_program);
}


void Shader::link_attributes()
{
	GLint total = -1;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &total);
	for(int i=0; i<total; ++i)
	{
		GLsizei len = -1;
		GLint size = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveAttrib(m_program, GLuint(i), sizeof(name)-1, &len, &size, &type, name);
		name[len] = 0;
		locations[name] = glGetAttribLocation(m_program, name);
	}
}


void Shader::link_uniforms()
{
	GLint total = -1;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &total);
	for(int i=0; i<total; ++i)
	{
		GLsizei len = -1;
		GLint size = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(m_program, GLuint(i), sizeof(name)-1, &len, &size, &type, name);
		name[len] = 0;
		locations[name] = glGetUniformLocation(m_program, name);
	}
}


Shader * Shader::create(const std::string & name)
{
	const auto it = shader_cache.find(name);
	if (it != shader_cache.end())
		return it->second;

	shader_cache[name] = new Shader(name);
	return shader_cache[name];
}


template<typename T>
void Shader::fail_shader(GLuint shader)
{
	GLsizei len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	GLchar *msg = new GLchar[len];
	glGetShaderInfoLog(shader, len, &len, msg);
	std::cerr << msg << std::endl;
	delete[] msg;
	throw T(msg);
}


template<typename T>
void Shader::fail_program()
{
	GLsizei len;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
	GLchar *msg = new GLchar[len];
	glGetProgramInfoLog(m_program, len, &len, msg);
	std::cerr << msg << std::endl;
	delete[] msg;
	throw T(msg);
}


GLuint Shader::compile_shader(const std::string & name, GLenum shader_type)
{
	std::cout << "READ SHADER FILE: " << name << std::endl;
//	std::ifstream file(name);

//	if (!file.is_open())
//		throw FileOpenError("File not found or could not be opened.");
//
//	std::string str;
//	getline(file, str, (char) EOF);
//	file.close();
//	const GLchar * src = str.c_str();


	// TODO: HACK: This is because we haven't figured out how to bundle shaders with CMake yet
	const GLchar * src;
	if (name == "debug.vert")
		src = vertexShaderSource.c_str();
	if (name == "debug.frag")
		src = fragmentShaderSource.c_str();



	GLuint shader_handle = glCreateShader(shader_type);
	glShaderSource(shader_handle, 1, &src, NULL);
	glCompileShader(shader_handle);

	GLint success;
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
		fail_shader<ShaderCompileError>(shader_handle);

	return shader_handle;
}


void Shader::link_program(GLuint vert_handle, GLuint frag_handle)
{
	m_program = glCreateProgram();
	glAttachShader(m_program, vert_handle);
	glAttachShader(m_program, frag_handle);
	glLinkProgram(m_program);
	glDeleteShader(vert_handle);
	glDeleteShader(frag_handle);

	GLint success;

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
		fail_program<ProgramLinkError>();

	glValidateProgram(m_program);
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &success);
	if (success != GL_TRUE)
		fail_program<ProgramValidateError>();
}
