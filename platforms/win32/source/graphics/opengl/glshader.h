#ifndef WIN_GLSHADER_H
#define WIN_GLSHADER_H

#include "glgraphics.h"

class GLshader : public XShader
{
	friend class OpenGL;
	friend class GLtexture;
public:
	GLshader(const string &vertFilePath, const string &fragFilePath);
	~GLshader();
	
	void setUniform1i(const string &name, const int v0);
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);
	void setUniform1f(const string &name, const float v0);
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
	void setSampler2D(const string &name, XTexture *texture);

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

#endif // WIN_GLSHADER_H