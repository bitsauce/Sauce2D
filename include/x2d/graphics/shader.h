#ifndef X2D_SHADER_H
#define X2D_SHADER_H

#include "../engine.h"

class XTexture;

class XDAPI XShader
{
public:
	virtual ~XShader() {}
	
	virtual void setUniform1i(const string &name, const int v0) = 0;
	virtual void setUniform2i(const string &name, const int v0, const int v1) = 0;
	virtual void setUniform3i(const string &name, const int v0, const int v1, const int v2) = 0;
	virtual void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3) = 0;
	virtual void setUniform1f(const string &name, const float v0) = 0;
	virtual void setUniform2f(const string &name, const float v0, const float v1) = 0;
	virtual void setUniform3f(const string &name, const float v0, const float v1, const float v2) = 0;
	virtual void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3) = 0;
	virtual void setSampler2D(const string &name, XTexture *texture) = 0;
};

#endif // X2D_SHADER_H