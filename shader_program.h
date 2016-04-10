#ifndef shader_program
#define shader_program

#if defined(__APPLE__)
	#include <OpenGL/gl3.h>
#elif defined(_WIN32)
	#include <GL/gl3.h>
#else // RPI
	#include <GLES2/gl2.h>
#endif

#include <stdexcept>
#include <string>
#include <vector>

struct ShaderError : public std::logic_error {
	ShaderError (const std::string& msg) : std::logic_error(msg) {}; };


struct FileOpenError : ShaderError {
	FileOpenError (const std::string& msg) : ShaderError(msg) {}; };


struct ShaderCompileError : ShaderError {
	ShaderCompileError (const std::string& msg) : ShaderError(msg) {}; };


struct ProgramLinkError : ShaderError {
	ProgramLinkError (const std::string& msg) : ShaderError(msg) {}; };


struct ProgramValidateError : ShaderError {
	ProgramValidateError (const std::string& msg) : ShaderError(msg) {}; };



class ShaderProgram
{
protected:
	GLuint _programHandle;

	template<typename T> void fail (GLuint shaderHandle);// shader fail
	template<typename T> void fail ();// program fail

public:
	ShaderProgram ();
	virtual ~ShaderProgram ();
	operator GLuint ();

	void compile (const char *filename, GLenum shaderType);
	GLuint link ();
};

#endif
