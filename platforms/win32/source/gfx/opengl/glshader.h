#ifndef GFX_GLSHADER_H
#define GFX_GLSHADER_H

#include "glgraphics.h"
#include <x2d/util.h>

#include "../source/gfx/shader.h"

class GLshader : public Shader
{
public:
	GLshader(const string &vertFilePath, const string &fragFilePath);
	
	void setUniform1i(const string &name, const int v0);
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);
	void setUniform1f(const string &name, const float v0);
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
	void setUniformTexture(const string &name, const int texId);

protected:

	void load(const string &vertFilePath, const string &fragFilePath);

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

		bool operator<(const Uniform& other) const
		{
			TUPLE_CMP(this->type, other.type)
			TUPLE_CMP(this->loc, other.loc)
			TUPLE_CMP(this->data, other.data)
			return false;
		}

		GLenum type;
		int loc;
		void *data;
	};

	GLint m_id;
	map<string, Uniform> m_uniforms;
};

#endif // GFX_GLSHADER_H