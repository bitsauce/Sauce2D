#ifndef X2D_SHADER_H
#define X2D_SHADER_H

#include "../engine.h"
#include "texture.h"

BEGIN_XD_NAMESPACE

class Shader;
typedef shared_ptr<Shader> ShaderPtr;

class XDAPI Shader
{
	friend class GraphicsContext;
public:
	Shader(const string &vertexSource, const string &fragmentSource);
	~Shader();
	
	void setUniform1i(const string &name, const int v0);
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);
	void setUniform1f(const string &name, const float v0);
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
	void setUniformMatrix4f(const string &name, const float *v0);
	void setSampler2D(const string &name, Texture2DPtr texture);
	
	static ShaderPtr loadResource(const string &name);

private:
	
	// Uniform struct
	struct Uniform
	{
		Uniform() :
			type(0),
			loc(0),
			data(0)
		{
		}

		~Uniform()
		{
			delete[] data;
		}

		GLenum type;
		int loc;
		void *data;
	};

	GLint m_id;
	map<string, Uniform*> m_uniforms;
};

template XDAPI class shared_ptr<Shader>;

END_XD_NAMESPACE

#endif // X2D_SHADER_H