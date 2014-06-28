//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "glshader.h"
#include "gltexture.h"
#include <x2d/console.h>
#include <x2d/filesystem.h>

GLshader::GLshader(const string &vertFilePath, const string &fragFilePath)
{
	// Create vertex and fragment shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	xdFileReader *fileReader;

	// Read vertex source
	fileReader = xdFileSystem::CreateFileReader(util::getAbsoluteFilePath(vertFilePath));
	string vertSource = fileReader->readAll();
	fileReader->close();
	delete fileReader;
	
	// Read fragment source
	fileReader = xdFileSystem::CreateFileReader(util::getAbsoluteFilePath(fragFilePath));
	string fragSource = fileReader->readAll();
	fileReader->close();
	delete fileReader;

	// Result variables
    int result = 0;
    int logLength;

	LOG("Compiling vertex shader: %s", vertFilePath.c_str());
	
    // Compile vertex shader
	const char *data = vertSource.c_str();
	int len = vertSource.length();
	glShaderSource(vertShader, 1, &data, &len);
    glCompileShader(vertShader);

    // Validate vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    char* compileLog = new char[logLength];
    glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1) {
		LOG("\t%s", compileLog);
	}

    LOG("Compiling fragment shader: %s", fragFilePath.c_str());

    // Compile fragment shader
	data = fragSource.c_str();
	len = fragSource.size();
	glShaderSource(fragShader, 1, &data, &len);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1) {
		LOG("\t%s", compileLog);
	}

    LOG("Linking shader program");

    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    char* programLog = new char[(logLength > 1) ? logLength : 1];
    glGetProgramInfoLog(program, logLength, NULL, programLog);

	// Print program error to console
	if(logLength > 1) {
		LOG("\t%s", compileLog);
	}

	// Delete shader buffers as they are loaded into the shader program
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

	// Delete log buffers
	delete[] compileLog;
	delete[] programLog;

	// Return shader program
	m_id = program;

	// Setup uniform variables
	GLint count;
	glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);
	GLint length, size;
	GLenum type;
	GLchar name[256];
	for(int i = 0; i < count; i++)
	{
		glGetActiveUniform(m_id, i, 256, &length, &size, &type, name);

		if(strncmp(name, "gl_", 3) == 0) // Skip gl_ uniforms
			continue;

		Uniform *uniform = new Uniform;
		uniform->type = type;
		uniform->loc = i;

		int dataSize;
		switch(type)
		{
		case GL_INT: dataSize = INT_SIZE; break;
		case GL_INT_VEC2: dataSize = INT_SIZE*2; break;
		case GL_INT_VEC3: dataSize = INT_SIZE*3; break;
		case GL_INT_VEC4: dataSize = INT_SIZE*4; break;
		case GL_FLOAT: dataSize = FLOAT_SIZE; break;
		case GL_FLOAT_VEC2: dataSize = FLOAT_SIZE*2; break;
		case GL_FLOAT_VEC3: dataSize = FLOAT_SIZE*3; break;
		case GL_FLOAT_VEC4: dataSize = FLOAT_SIZE*4; break;
		case GL_SAMPLER_2D: dataSize = INT_SIZE; break;
		}

		uniform->data = new char[dataSize];
		m_uniforms[name] = uniform;
	}
}

GLshader::~GLshader()
{
	for(map<string, Uniform*>::iterator itr = m_uniforms.begin(); itr != m_uniforms.end(); ++itr)
		delete itr->second;
}

void GLshader::setUniform1i(const string &name, const int v0)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_INT) {
			((GLint*)uniform->data)[0] = v0;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform2i(const string &name, const int v0, const int v1)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_INT_VEC2) {
			((GLint*)uniform->data)[0] = v0;
			((GLint*)uniform->data)[1] = v1;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform3i(const string &name, const int v0, const int v1, const int v2)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_INT_VEC3) {
			((GLint*)uniform->data)[0] = v0;
			((GLint*)uniform->data)[1] = v1;
			((GLint*)uniform->data)[2] = v2;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_INT_VEC4) {
			((GLint*)uniform->data)[0] = v0;
			((GLint*)uniform->data)[1] = v1;
			((GLint*)uniform->data)[2] = v2;
			((GLint*)uniform->data)[3] = v3;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform1f(const string &name, const float v0)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_FLOAT) {
			((GLfloat*)uniform->data)[0] = v0;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform2f(const string &name, const float v0, const float v1)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_FLOAT_VEC2) {
			((GLfloat*)uniform->data)[0] = v0;
			((GLfloat*)uniform->data)[1] = v1;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform3f(const string &name, const float v0, const float v1, const float v2)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_FLOAT_VEC3) {
			((GLfloat*)uniform->data)[0] = v0;
			((GLfloat*)uniform->data)[1] = v1;
			((GLfloat*)uniform->data)[2] = v2;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_FLOAT_VEC4) {
			((GLfloat*)uniform->data)[0] = v0;
			((GLfloat*)uniform->data)[1] = v1;
			((GLfloat*)uniform->data)[2] = v2;
			((GLfloat*)uniform->data)[3] = v3;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}

void GLshader::setSampler2D(const string &name, const Texture *texture)
{
	if(m_uniforms.find(name) != m_uniforms.end())
	{
		Uniform *uniform = m_uniforms[name];
		if(uniform->type == GL_SAMPLER_2D) {
			((GLint*)uniform->data)[0] = ((GLtexture*)texture)->m_id;
		}
	}else{
		LOG("Uniform '%s' does not exist.", name);
	}
}
