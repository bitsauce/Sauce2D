#include "shader.h"

Shader::Shader(const string &vertFilePath, const string &fragFilePath)
{
	// Create vertex and fragment shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shader files to string
	const char *vertShaderBuffer, *fragShaderBuffer;
	long vertBufferLen = 0, fragBufferLen = 0; 
	//AssetLoader::loadAsBytes(vertFilePath.c_str(), &vertShaderBuffer, &vertBufferLen);
	//AssetLoader::loadAsBytes(fragFilePath.c_str(), &fragShaderBuffer, &fragBufferLen);

	// Default vars
    int result = 0;
    int logLength;

    // Compile vertex shader
	iosystem::print("Compiling vertex shader: %s", vertFilePath.c_str());
	glShaderSource(vertShader, 1, &vertShaderBuffer, (int*)&vertBufferLen);
    glCompileShader(vertShader);
	delete[] vertShaderBuffer;

    // Validate vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    char* compileLog = new char[logLength];
    glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1)
		iosystem::print("\tCompile error: %s", compileLog);

    // Compile fragment shader
    iosystem::print("Compiling fragment shader: %s", fragFilePath.c_str());
    glShaderSource(fragShader, 1, &fragShaderBuffer, (int*)&fragBufferLen);
    glCompileShader(fragShader);
	delete[] fragShaderBuffer;

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1)
		iosystem::print("\tCompile error: %s", compileLog);

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
	if(logLength > 1)
		iosystem::print("\tCompile error: %s", compileLog);

	// Delete shader buffers as they are loaded into the shader program
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

	// Delete log buffers
	delete[] compileLog;
	delete[] programLog;

	// Return shader program
	m_id = program;

	// Setup default uniform values
	GLint count;
	glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);
	GLint length, size;
	GLenum type;
	GLchar name[256];
	for(int i = 0; i < count; i++) {
		glGetActiveUniform(m_id, i, 256, &length, &size, &type, name);
		Uniform uniform;
		uniform.type = type;
		uniform.loc = i;
		uniform.data = new char[size];
		m_uniforms[name] = uniform;
	}
}

void Shader::setUniform1i(const string &name, const int v0)
{
	((int*)m_uniforms[name].data)[0] = v0;
}

void Shader::setUniform2i(const string &name, const int v0, const int v1)
{
	((int*)m_uniforms[name].data)[0] = v0;
	((int*)m_uniforms[name].data)[1] = v1;
}

void Shader::setUniform3i(const string &name, const int v0, const int v1, const int v2)
{
	((int*)m_uniforms[name].data)[0] = v0;
	((int*)m_uniforms[name].data)[1] = v1;
	((int*)m_uniforms[name].data)[2] = v2;
}

void Shader::setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3)
{
	((int*)m_uniforms[name].data)[0] = v0;
	((int*)m_uniforms[name].data)[1] = v1;
	((int*)m_uniforms[name].data)[2] = v2;
	((int*)m_uniforms[name].data)[3] = v3;
}

void Shader::setUniform1f(const string &name, const float v0)
{
	((float*)m_uniforms[name].data)[0] = v0;
}

void Shader::setUniform2f(const string &name, const float v0, const float v1)
{
	((float*)m_uniforms[name].data)[0] = v0;
	((float*)m_uniforms[name].data)[1] = v1;
}

void Shader::setUniform3f(const string &name, const float v0, const float v1, const float v2)
{
	((float*)m_uniforms[name].data)[0] = v0;
	((float*)m_uniforms[name].data)[1] = v1;
	((float*)m_uniforms[name].data)[2] = v2;
}

void Shader::setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3)
{
	((float*)m_uniforms[name].data)[0] = v0;
	((float*)m_uniforms[name].data)[1] = v1;
	((float*)m_uniforms[name].data)[2] = v2;
	((float*)m_uniforms[name].data)[3] = v3;
}

void Shader::setUniformTexture(const string &name, const int texId)
{
	((int*)m_uniforms[name].data)[0] = texId;
}
