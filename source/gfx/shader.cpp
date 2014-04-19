#include "shader.h"

AS_REGISTER_REF(Shader)

int Shader::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform1i(const string &in, const int)", asMETHODPR(Shader, setUniform1i, (const string&, const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform2i(const string &in, const int, const int)", asMETHODPR(Shader, setUniform2i, (const string&, const int, const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform3i(const string &in, const int, const int, const int)", asMETHODPR(Shader, setUniform3i, (const string&, const int, const int, const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform4i(const string &in, const int, const int, const int, const int)", asMETHODPR(Shader, setUniform4i, (const string&, const int, const int, const int, const int), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform1f(const string &in, const float)", asMETHODPR(Shader, setUniform1f, (const string&, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform2f(const string &in, const float, const float)", asMETHODPR(Shader, setUniform2f, (const string&, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform3f(const string &in, const float, const float, const float)", asMETHODPR(Shader, setUniform3f, (const string&, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform4f(const string &in, const float, const float, const float, const float)", asMETHODPR(Shader, setUniform4f, (const string&, const float, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	
	//r = scriptEngine->RegisterObjectMethod("Shader", "void setTextureSampler(const string &in, const Texture @texture)", asMETHODPR(Shader, setTextureSampler, (const string&, const Texture*), void), asCALL_THISCALL); AS_ASSERT
	
	return r;
}

Shader::Shader() :
	refCounter(this)
{
}