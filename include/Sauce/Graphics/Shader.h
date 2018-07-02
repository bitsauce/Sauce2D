#ifndef SAUCE_SHADER_H
#define SAUCE_SHADER_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/Texture.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API Shader
{
	friend class GraphicsContext;
	friend class ResourceManager;
public:
	Shader();
	virtual ~Shader();

	virtual void bindFragLocation(const uint location, const string &name) = 0;

	virtual void link() = 0;
	
	virtual void setUniform1i(const string &name, const int v0) = 0;
	virtual void setUniform2i(const string &name, const int v0, const int v1) = 0;
	virtual void setUniform3i(const string &name, const int v0, const int v1, const int v2) = 0;
	virtual void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3) = 0;

	virtual void setUniform1iv(const string &name, const uint count, const int *v) = 0;
	virtual void setUniform2iv(const string &name, const uint count, const int *v) = 0;
	virtual void setUniform3iv(const string &name, const uint count, const int *v) = 0;
	virtual void setUniform4iv(const string &name, const uint count, const int *v) = 0;

	virtual void setUniform1ui(const string &name, const uint v0) = 0;
	virtual void setUniform2ui(const string &name, const uint v0, const uint v1) = 0;
	virtual void setUniform3ui(const string &name, const uint v0, const uint v1, const uint v2) = 0;
	virtual void setUniform4ui(const string &name, const uint v0, const uint v1, const uint v2, const uint v3) = 0;

	virtual void setUniform1f(const string &name, const float v0) = 0;
	virtual void setUniform2f(const string &name, const float v0, const float v1) = 0;
	virtual void setUniform2f(const string &name, const float *v) = 0;
	virtual void setUniform3f(const string &name, const float v0, const float v1, const float v2) = 0;
	virtual void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3) = 0;
	virtual void setUniform4f(const string &name, const float *v) = 0;
	virtual void setUniformMatrix4f(const string &name, const float *v0) = 0;
	virtual void setSampler2D(const string &name, shared_ptr<Texture2D> texture) = 0;

	virtual void setUniformColor(const string &name, const Color &color) = 0;
	virtual void setUniformColorRGB(const string &name, const ColorRGB &color) = 0;
};

template SAUCE_API class shared_ptr<Shader>;

class ShaderResourceDesc : public ResourceDesc
{
public:
	ShaderResourceDesc(const string &name, const string &vertexFilePath, const string &fragmentFilePath, const string &geometryFilePath) :
		ResourceDesc(RESOURCE_TYPE_TEXTURE, name),
		m_vertexFilePath(vertexFilePath),
		m_fragmentFilePath(fragmentFilePath),
		m_geometryFilePath(geometryFilePath)
	{
	}

	string getVertexFilePath() const
	{
		return m_vertexFilePath;
	}

	string getFragmentFilePath() const
	{
		return m_fragmentFilePath;
	}

	string getGeometryFilePath() const
	{
		return m_geometryFilePath;
	}

	void *create() const;

private:
	const string m_vertexFilePath;
	const string m_fragmentFilePath;
	const string m_geometryFilePath;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_SHADER_H
