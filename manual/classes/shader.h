/**
  *	\brief Shader object for loading and drawing with shaders
  */
class Shader
{
public:
	void setUniform1i(const string &name, const int v0)
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);
	
	void setUniform1f(const string &name, const float v0)
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
};