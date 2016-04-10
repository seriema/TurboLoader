#ifndef __shader__
#define __shader__

#if defined(__APPLE__)
	#include <OpenGL/gl3.h>
#elif defined(_WIN32)
	#include <GL/gl3.h>
#else // RPI
	#include <GLES2/gl2.h>
#endif

#include <string>
#include <map>


class Shader
{
	static std::map<std::string, Shader *> shader_cache;

	const std::string m_name;
	std::map<std::string, GLuint> locations;

	template<typename T> void fail_shader (GLuint shader);// shader fail
	template<typename T> void fail_program ();// program fail
	void compile_shader(const std::string & name, GLenum shader_type);
	void link_program();
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
