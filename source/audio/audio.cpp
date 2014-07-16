//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/audio.h>
#include <x2d/console.h>
#include <x2d/filesystem.h>
#include <x2d/debug.h>
#include <x2d/assetloader.h>

AS_REG_SINGLETON(xdAudio, "ScriptAudio")

xdAudio *xdAudio::s_this = 0;

int xdAudio::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectMethod("ScriptAudio", "void set_position(const Vector2 &in)", asMETHOD(xdAudio, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptAudio", "Vector2 get_position() const", asMETHOD(xdAudio, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptAudio", "void set_velocity(const Vector2 &in)", asMETHOD(xdAudio, setVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptAudio", "Vector2 get_velocity() const", asMETHOD(xdAudio, getVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptAudio", "void set_orientation(const Vector3 &in)", asMETHOD(xdAudio, setOrientation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptAudio", "Vector3 get_orientation() const", asMETHOD(xdAudio, getOrientation), asCALL_THISCALL); AS_ASSERT

	return r;
}

AudioSource *xdAudio::CreateSource(AudioBuffer *buffer)
{
	// Check buffer
	if(buffer == 0) {
		LOG("Cannot create a AudioSource using a 'null' buffer");
		return 0;
	}

	// Create a audio source using buffer
	AudioSource *source = s_this->createSource(buffer);
	delete buffer;
	return source;
}

AudioSource *xdAudio::CreateSource(const string &path)
{
	return CreateSource(xdAssetLoader::s_this->loadSound(path));
}