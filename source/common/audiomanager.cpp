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

XAudioManager *XAudioManager::s_this = 0;

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