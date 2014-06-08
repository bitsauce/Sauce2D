#ifndef GFX_SHADER_H
#define GFX_SHADER_H

#include <x2d/config.h>
#include <x2d/base.h>

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

class XDAPI Shader
{
public:
	AS_DECL_REF

	Shader();
	
	virtual void setUniform1i(const string &name, const int v0) = 0;
	virtual void setUniform2i(const string &name, const int v0, const int v1) = 0;
	virtual void setUniform3i(const string &name, const int v0, const int v1, const int v2) = 0;
	virtual void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3) = 0;
	virtual void setUniform1f(const string &name, const float v0) = 0;
	virtual void setUniform2f(const string &name, const float v0, const float v1) = 0;
	virtual void setUniform3f(const string &name, const float v0, const float v1, const float v2) = 0;
	virtual void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3) = 0;
	virtual void setUniformTexture(const string &name, const int texId) = 0;
};

#endif // GFX_SHADER_H