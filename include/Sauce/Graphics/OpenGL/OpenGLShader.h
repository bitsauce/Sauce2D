#pragma once

#include <Sauce/Common.h>
#include <Sauce/Graphics/Texture.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API OpenGLShader : public Shader
{
	friend class OpenGLContext;
	friend class ResourceManager;
public:
	OpenGLShader(const string &vertexSource, const string &fragmentSource, const string &geometrySource);
	~OpenGLShader();

	void bindFragLocation(const uint location, const string &name);

	void link();
	
	// Instance ->
	void setUniform1i(const string &name, const int v0);
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);

	void setUniform1iv(const string &name, const uint count, const int *v);
	void setUniform2iv(const string &name, const uint count, const int *v);
	void setUniform3iv(const string &name, const uint count, const int *v);
	void setUniform4iv(const string &name, const uint count, const int *v);

	void setUniform1ui(const string &name, const uint v0);
	void setUniform2ui(const string &name, const uint v0, const uint v1);
	void setUniform3ui(const string &name, const uint v0, const uint v1, const uint v2);
	void setUniform4ui(const string &name, const uint v0, const uint v1, const uint v2, const uint v3);

	void setUniform1f(const string &name, const float v0);
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform2f(const string &name, const float *v);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
	void setUniform4f(const string &name, const float *v);
	void setUniformMatrix4f(const string &name, const float *v0);
	void setSampler2D(const string &name, shared_ptr<Texture2D> texture);

	void setUniformColor(const string &name, const Color &color);
	void setUniformColorRGB(const string &name, const ColorRGB &color);
	// <- Instance

	void exportAssembly(const string &fileName);

private:
	void compileShader(const string &vertexSource, const string &fragmentSource, const string &geometrySource);

	// Uniform struct
	struct Uniform
	{
		Uniform() :
			type(0),
			loc(0),
			count(0),
			data(0)
		{
		}

		~Uniform()
		{
			delete[] data;
		}

		GLenum type;
		int loc;
		int count;
		void *data;
	};

	GLuint m_id, m_vertShaderID, m_fragShaderID;
	map<string, Uniform*> m_uniforms;

	static string s_glslVersion;
};

template SAUCE_API class shared_ptr<Shader>;

END_SAUCE_NAMESPACE
