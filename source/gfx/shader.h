#ifndef GFX_SHADER_H
#define GFX_SHADER_H

#include "gfx.h"

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

class Shader
{
public:
	Shader(const string &vertFilePath, const string &fragFilePath);
	
	void setUniform1i(const string &name, const int v0);
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);
	void setUniform1f(const string &name, const float v0);
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
	void setUniformTexture(const string &name, const int texId);

private:

	// Virtual part
	virtual void load(const string &vertFilePath, const string &fragFilePath) = 0;
	
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

#endif // GFX_SHADER_H