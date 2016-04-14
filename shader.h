#ifndef __shader__
#define __shader__

#include "platform.h"

#include <stdexcept>
#include <string>
#include <map>


class Shader
{
	static std::map<std::string, Shader *> shader_cache;

	const std::string m_name;
	std::map<std::string, GLuint> locations;

	template<typename T> void fail_shader (GLuint shader);// shader fail
	template<typename T> void fail_program ();// program fail
	GLuint compile_shader(const std::string & name, GLenum shader_type);
	void link_program(GLuint vert_handle, GLuint frag_handle);
	void link_attributes();
	void link_uniforms();

	Shader(const std::string & name);

	GLuint vao;

public:
	GLuint m_program;
	virtual ~Shader();

	static Shader * create(const std::string & name);

	void bind() const {
		glUseProgram(m_program); }

	void unbind() const {
		glUseProgram(0); }

	GLuint operator[](const char * param_name) {
		return locations[param_name]; }

	GLuint operator[](std::string & param_name) {
		return locations[param_name]; }

	struct ShaderError : public std::logic_error {
		ShaderError (const std::string & msg) : std::logic_error(msg) {}; };


	struct FileOpenError : ShaderError {
		FileOpenError (const std::string & msg) : ShaderError(msg) {}; };


	struct ShaderCompileError : ShaderError {
		ShaderCompileError (const std::string & msg) : ShaderError(msg) {}; };


	struct ProgramLinkError : ShaderError {
		ProgramLinkError (const std::string & msg) : ShaderError(msg) {}; };


	struct ProgramValidateError : ShaderError {
		ProgramValidateError (const std::string & msg) : ShaderError(msg) {}; };
};


#endif
