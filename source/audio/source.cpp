//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/audio.h>

AS_REG_REF(XAudioSource)

int XAudioSource::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("XAudioSource", asBEHAVE_FACTORY, "XAudioSource @f(const string &in)", asFUNCTIONPR(XAudioManager::CreateSource, (const string&), XAudioSource*), asCALL_CDECL); AS_ASSERT
		
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void play()", asMETHOD(XAudioSource, play), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void stop()", asMETHOD(XAudioSource, stop), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void playOnce()", asMETHOD(XAudioSource, playOnce), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "bool isPlaying() const", asMETHOD(XAudioSource, isPlaying), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void set_position(const Vector2 &in)", asMETHOD(XAudioSource, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "Vector2 get_position() const", asMETHOD(XAudioSource, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void set_velocity(const Vector2 &in)", asMETHOD(XAudioSource, setVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "Vector2 get_velocity() const", asMETHOD(XAudioSource, getVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void set_looping(const bool)", asMETHOD(XAudioSource, setLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "bool get_looping() const", asMETHOD(XAudioSource, getLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void set_gain(const float)", asMETHOD(XAudioSource, setGain), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "float get_gain() const", asMETHOD(XAudioSource, getGain), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "void set_pitch(const float)", asMETHOD(XAudioSource, setPitch), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioSource", "float get_pitch() const", asMETHOD(XAudioSource, getPitch), asCALL_THISCALL); AS_ASSERT

	return r;
}