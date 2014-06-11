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

AS_REG_SINGLETON(xdAudio, "ScriptAudio")

int xdAudio::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	// Register sound functionality
	/*AS_SINGLETON_FUNCTION(X2DSound, "int sfxCreateSource()", createSource, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxDeleteSource(const int)", deleteSource, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxPlaySource(const int)", playSource, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxStopSource(const int)", stopSource, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceBuffer(const int sourceId, const int bufferId)", setSourceBuffer, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourcePosition(const int sourceId, const float x, const float y, const float z)", setSourcePosition, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceVelocity(const int sourceId, const float x, const float y, const float z)", setSourceVelocity, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceLooping(const int sourceId, const bool looping)", setSourceLooping, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceGain(const int sourceId, const float gain)", setSourceGain, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourcePitch(const int sourceId, const float pitch)", setSourcePitch, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceMaxDist(const int sourceId, const float dist)", setSourceMaxDist, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceMinDist(const int sourceId, const float dist)", setSourceMinDist, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetListenerPosition(const float x, const float y, const float z)", setListenerPosition, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetListenerVelocity(const float x, const float y, const float z)", setListenerVelocity, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetListenerOrientation(const float x, const float y, const float z)", setListenerOrientation, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxLoadFile(const string &in filePath)", loadFile, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxDeleteBuffer(const int)", deleteBuffer, sfx)*/

	return r;
}
