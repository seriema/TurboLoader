#ifndef shader_program
#define shader_program

#include "platform.h"

#include <stdexcept>

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

	template<typename T>
	static void fail (GLuint shaderHandle);// shader fail
	template<typename T> void fail () const;// program fail

public:
	ShaderProgram ();
	virtual ~ShaderProgram ();
	operator GLuint () const;

	void compile (const char *filename, GLenum shaderType) const;
	GLuint link ();
};

#endif
