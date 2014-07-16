//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/graphics.h>
#include <x2d/graphics/shader.h>

AS_REG_REF(Shader)

int Shader::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Shader", asBEHAVE_FACTORY, "Shader @f(const string &in, const string &in)", asFUNCTIONPR(xdGraphics::CreateShader, (const string&, const string&), Shader*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform1i(const string &in, const int)", asMETHODPR(Shader, setUniform1i, (const string&, const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform2i(const string &in, const int, const int)", asMETHODPR(Shader, setUniform2i, (const string&, const int, const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform3i(const string &in, const int, const int, const int)", asMETHODPR(Shader, setUniform3i, (const string&, const int, const int, const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform4i(const string &in, const int, const int, const int, const int)", asMETHODPR(Shader, setUniform4i, (const string&, const int, const int, const int, const int), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform1f(const string &in, const float)", asMETHODPR(Shader, setUniform1f, (const string&, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform2f(const string &in, const float, const float)", asMETHODPR(Shader, setUniform2f, (const string&, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform3f(const string &in, const float, const float, const float)", asMETHODPR(Shader, setUniform3f, (const string&, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shader", "void setUniform4f(const string &in, const float, const float, const float, const float)", asMETHODPR(Shader, setUniform4f, (const string&, const float, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Shader", "void setSampler2D(const string &in, Texture @texture)", asMETHODPR(Shader, setSampler2D, (const string&, Texture*), void), asCALL_THISCALL); AS_ASSERT
	
	return r;
}
