#include <x2d/audio.h>

AS_REG_REF(AudioSource)

int AudioSource::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("AudioSource", asBEHAVE_FACTORY, "AudioSource @f(const string &in)", asFUNCTIONPR(xdAudio::CreateSource, (const string&), AudioSource*), asCALL_CDECL); AS_ASSERT
		
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void play()", asMETHOD(AudioSource, play), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void stop()", asMETHOD(AudioSource, stop), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void playOnce()", asMETHOD(AudioSource, playOnce), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("AudioSource", "bool isPlaying() const", asMETHOD(AudioSource, isPlaying), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("AudioSource", "void set_position(const Vector2 &in)", asMETHOD(AudioSource, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "Vector2 get_position() const", asMETHOD(AudioSource, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void set_velocity(const Vector2 &in)", asMETHOD(AudioSource, setVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "Vector2 get_velocity() const", asMETHOD(AudioSource, getVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void set_looping(const bool)", asMETHOD(AudioSource, setLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "bool get_looping() const", asMETHOD(AudioSource, getLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void set_gain(const float)", asMETHOD(AudioSource, setGain), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "float get_gain() const", asMETHOD(AudioSource, getGain), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "void set_pitch(const float)", asMETHOD(AudioSource, setPitch), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("AudioSource", "float get_pitch() const", asMETHOD(AudioSource, getPitch), asCALL_THISCALL); AS_ASSERT

	return r;
}