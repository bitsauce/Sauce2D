//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/graphics.h>

const int INT_SIZE = sizeof(GLint);
const int FLOAT_SIZE = sizeof(GLfloat);
const int PTR_SIZE = sizeof(void*);

BEGIN_CGF_NAMESPACE

Shader::Shader(const string &vertexSource, const string &fragmentSource)
{
	// Create vertex and fragment shaders
    m_vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Result variables
	GLint success;

	// Create modified shader code
	string vertexSourceModified = "#version 130\n" + vertexSource;
	string fragmentSourceModified = "#version 130\n" + fragmentSource;

	LOG("Compiling vertex shader...");

	// Compile vertex shader
	const char *data = vertexSourceModified.c_str();
	int len = vertexSourceModified.length();
	glShaderSource(m_vertShaderID, 1, &data, &len);
    glCompileShader(m_vertShaderID);

    // Validate vertex shader
    glGetShaderiv(m_vertShaderID, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		// Get log length
		GLint logLength;
		glGetShaderiv(m_vertShaderID, GL_INFO_LOG_LENGTH, &logLength);

		// Get compilation log
		string compileLog;
		compileLog.resize(logLength);
		glGetShaderInfoLog(m_vertShaderID, logLength, NULL, &compileLog[0]);

		// Throw exception
		THROW(compileLog.c_str());
	}

	LOG("Compiling fragment shader...");

	// Compile fragment shader
	data = fragmentSourceModified.c_str();
	len = fragmentSourceModified.length();
	glShaderSource(m_fragShaderID, 1, &data, &len);
    glCompileShader(m_fragShaderID);

	// Validate fragment shader
	glGetShaderiv(m_fragShaderID, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		// Get log length
		GLint logLength;
		glGetShaderiv(m_fragShaderID, GL_INFO_LOG_LENGTH, &logLength);

		// Get compilation log
		string compileLog;
		compileLog.resize(logLength);
		glGetShaderInfoLog(m_fragShaderID, logLength, NULL, &compileLog[0]);

		// Throw exception
		THROW(compileLog.c_str());
	}

    // Create shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, m_vertShaderID);
    glAttachShader(m_id, m_fragShaderID);

	glBindAttribLocation(m_id, 0, "in_Position");
	glBindAttribLocation(m_id, 1, "in_VertexColor");
	glBindAttribLocation(m_id, 2, "in_TexCoord");
	glBindFragDataLocation(m_id, 0, "out_FragColor");

	link();

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
		uniform->loc = glGetUniformLocation(m_id, name);
		uniform->count = size;

		size_t dataSize = 0;
		switch(type)
		{
			case GL_UNSIGNED_INT_SAMPLER_2D:
			case GL_INT_SAMPLER_2D:
			case GL_SAMPLER_2D:

			case GL_INT:		dataSize = INT_SIZE; break;
			case GL_INT_VEC2:	dataSize = INT_SIZE * 2; break;
			case GL_INT_VEC3:	dataSize = INT_SIZE * 3; break;
			case GL_INT_VEC4:	dataSize = INT_SIZE * 4; break;

			case GL_UNSIGNED_INT:		dataSize = INT_SIZE; break;
			case GL_UNSIGNED_INT_VEC2:	dataSize = INT_SIZE * 2; break;
			case GL_UNSIGNED_INT_VEC3:	dataSize = INT_SIZE * 3; break;
			case GL_UNSIGNED_INT_VEC4:	dataSize = INT_SIZE * 4; break;

			case GL_FLOAT:		dataSize = FLOAT_SIZE; break;
			case GL_FLOAT_VEC2:	dataSize = FLOAT_SIZE * 2; break;
			case GL_FLOAT_VEC3:	dataSize = FLOAT_SIZE * 3; break;
			case GL_FLOAT_VEC4:	dataSize = FLOAT_SIZE * 4; break;
			case GL_FLOAT_MAT4:	dataSize = FLOAT_SIZE * 16; break;
		}
		uniform->data = new char[dataSize * size];

		string strName = name;
		if(strName.length() > 3 && strName.substr(strName.length() - 3) == "[0]")
		{
			strName = strName.substr(0, strName.length() - 3);
		}

		m_uniforms[strName] = uniform;
	}
}

Shader::~Shader()
{
	// Delete shader buffers as they are loaded into the shader program
	glDeleteShader(m_vertShaderID);
	glDeleteShader(m_fragShaderID);
	for(map<string, Uniform*>::iterator itr = m_uniforms.begin(); itr != m_uniforms.end(); ++itr) {
		delete itr->second;
	}
}

void Shader::bindFragLocation(const uint location, const string &name)
{
	glBindFragDataLocation(m_id, location, name.c_str());
}

void Shader::link()
{
	LOG("Linking shader program...");

	// Link program
	glLinkProgram(m_id);

	// Check if link was successful 
	GLint success;
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if(!success)
	{
		// Get log length
		int logLength = 0;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);

		// Get compilation log
		string compileLog;
		compileLog.resize(logLength);
		glGetProgramInfoLog(m_id, logLength, NULL, &compileLog[0]);

		// Throw exception
		THROW(compileLog.c_str());
	}
}

void Shader::setUniform1i(const string &name, const int v0)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_INT)
		{
			((GLint*) uniform->data)[0] = v0;
		}
		else
		{
			LOG("Uniform '%s' is not type 'int'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform2i(const string &name, const int v0, const int v1)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_INT_VEC2)
		{
			((GLint*) uniform->data)[0] = v0;
			((GLint*) uniform->data)[1] = v1;
		}
		else
		{
			LOG("Uniform '%s' is not type 'ivec2'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform3i(const string &name, const int v0, const int v1, const int v2)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_INT_VEC3)
		{
			((GLint*) uniform->data)[0] = v0;
			((GLint*) uniform->data)[1] = v1;
			((GLint*) uniform->data)[2] = v2;
		}
		else
		{
			LOG("Uniform '%s' is not type 'ivec3'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_INT_VEC4)
		{
			((GLint*) uniform->data)[0] = v0;
			((GLint*) uniform->data)[1] = v1;
			((GLint*) uniform->data)[2] = v2;
			((GLint*) uniform->data)[3] = v3;
		}
		else
		{
			LOG("Uniform '%s' is not type 'ivec4'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform1ui(const string &name, const uint v0)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_UNSIGNED_INT)
		{
			((GLuint*) uniform->data)[0] = v0;
		}
		else
		{
			LOG("Uniform '%s' is not type 'uint'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform2ui(const string &name, const uint v0, const uint v1)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_UNSIGNED_INT_VEC2)
		{
			((GLuint*) uniform->data)[0] = v0;
			((GLuint*) uniform->data)[1] = v1;
		}
		else
		{
			LOG("Uniform '%s' is not type 'uvec2'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform3ui(const string &name, const uint v0, const uint v1, const uint v2)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_UNSIGNED_INT_VEC3)
		{
			((GLuint*) uniform->data)[0] = v0;
			((GLuint*) uniform->data)[1] = v1;
			((GLuint*) uniform->data)[2] = v2;
		}
		else
		{
			LOG("Uniform '%s' is not type 'uvec3'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform4ui(const string &name, const uint v0, const uint v1, const uint v2, const uint v3)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_UNSIGNED_INT_VEC4)
		{
			((GLuint*) uniform->data)[0] = v0;
			((GLuint*) uniform->data)[1] = v1;
			((GLuint*) uniform->data)[2] = v2;
			((GLuint*) uniform->data)[3] = v3;
		}
		else
		{
			LOG("Uniform '%s' is not type 'uvec4'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform1f(const string &name, const float v0)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_FLOAT)
		{
			((GLfloat*) uniform->data)[0] = v0;
		}
		else
		{
			LOG("Uniform '%s' is not type 'float'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform2f(const string &name, const float v0, const float v1)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_FLOAT_VEC2)
		{
			((GLfloat*) uniform->data)[0] = v0;
			((GLfloat*) uniform->data)[1] = v1;
		}
		else
		{
			LOG("Uniform '%s' is not type 'vec2'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform2f(const string & name, const float * v)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_FLOAT_VEC2)
		{
			memcpy(uniform->data, v,  2 * uniform->count * FLOAT_SIZE);
		}
		else
		{
			LOG("Uniform '%s' is not type 'vec2'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform3f(const string &name, const float v0, const float v1, const float v2)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_FLOAT_VEC3)
		{
			((GLfloat*) uniform->data)[0] = v0;
			((GLfloat*) uniform->data)[1] = v1;
			((GLfloat*) uniform->data)[2] = v2;
		}
		else
		{
			LOG("Uniform '%s' is not type 'vec3'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_FLOAT_VEC4)
		{
			((GLfloat*) uniform->data)[0] = v0;
			((GLfloat*) uniform->data)[1] = v1;
			((GLfloat*) uniform->data)[2] = v2;
			((GLfloat*) uniform->data)[3] = v3;
		}
		else
		{
			LOG("Uniform '%s' is not type 'vec4'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setUniformMatrix4f(const string & name, const float * v0)
{
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_FLOAT_MAT4)
		{
			for(int i = 0; i < 16; ++i)
			{
				((GLfloat*) uniform->data)[i] = v0[i];
			}
		}
		else
		{
			LOG("Uniform '%s' is not type 'mat4'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::setSampler2D(const string &name, Texture2D *texture)
{
	// TODO: We should actually store a handle to the texture object to avoid it being destroyed
	map<string, Uniform*>::iterator itr;
	if((itr = m_uniforms.find(name)) != m_uniforms.end())
	{
		Uniform *uniform = itr->second;
		if(uniform->type == GL_SAMPLER_2D ||
			uniform->type == GL_INT_SAMPLER_2D ||
			uniform->type == GL_UNSIGNED_INT_SAMPLER_2D)
		{
			((GLuint*) uniform->data)[0] = texture != 0 ? texture->m_id : 0;
		}
		else
		{
			LOG("Uniform '%s' is not type 'gsampler2D'", name.c_str());
		}
	}
	else
	{
		LOG("Uniform '%s' does not exist.", name.c_str());
	}
}

void Shader::exportAssembly(const string & fileName)
{
	if(!glGetProgramBinary)
	{
		LOG("glGetProgramBinary not supported");
		return;
	}

	// Max file size
	const size_t MAX_SIZE = 1 << 24;

	// Get binary program
	char *binary = new char[MAX_SIZE];
	GLenum format;
	GLint length;
	glGetProgramBinary(m_id, MAX_SIZE, &length, &format, binary);

	// Copy to string
	string content;
	content.resize(length);
	memcpy(&content[0], binary, length);

	// Write to file
	FileSystem::WriteFile(fileName, content);

	// Clean up
	delete[] binary;
}

ShaderPtr Shader::loadResource(const string &name)
{
	FileReader *fileReader;

	fileReader = new FileReader(util::getAbsoluteFilePath(name + ".vert"));
	string vertexSource = fileReader->readAll();
	fileReader->close();
	delete fileReader;

	fileReader = new FileReader(util::getAbsoluteFilePath(name + ".frag"));
	string fragmentSource = fileReader->readAll();
	fileReader->close();
	delete fileReader;

	LOG("Compiling shader program: %s", name.c_str());

	return ShaderPtr(new Shader(vertexSource, fragmentSource));
}

END_CGF_NAMESPACE
