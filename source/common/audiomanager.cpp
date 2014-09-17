//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/audio.h>

AS_REG_SINGLETON(XAudioManager)

XAudioManager *XAudioManager::s_this = 0;

int XAudioManager::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectMethod("XAudioManager", "void set_position(const Vector2 &in)", asMETHOD(XAudioManager, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioManager", "Vector2 get_position() const", asMETHOD(XAudioManager, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioManager", "void set_velocity(const Vector2 &in)", asMETHOD(XAudioManager, setVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioManager", "Vector2 get_velocity() const", asMETHOD(XAudioManager, getVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioManager", "void set_orientation(const Vector3 &in)", asMETHOD(XAudioManager, setOrientation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAudioManager", "Vector3 get_orientation() const", asMETHOD(XAudioManager, getOrientation), asCALL_THISCALL); AS_ASSERT

	return r;
}

XAudioSource *XAudioManager::CreateSource(XAudioBuffer *buffer)
{
	// Check buffer
	if(buffer == 0)
	{
		LOG("Cannot create a AudioSource using a 'null' buffer");
		return 0;
	}

	// Create a audio source using buffer
	XAudioSource *source = s_this->createSource(buffer);
	delete buffer;
	return source;
}

XAudioSource *XAudioManager::CreateSource(const string &path)
{
	return CreateSource(XAssetManager::LoadSound(path));
}