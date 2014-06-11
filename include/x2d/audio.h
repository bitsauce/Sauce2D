#ifndef X2D_SOUND_H
#define X2D_SOUND_H

#include <x2d/config.h>
#include <x2d/console.h>

/*********************************************************************
**	Abstract Sound Manager											**
**********************************************************************/
class XDAPI xdAudio
{
public:
	AS_DECL_SINGLETON

	//virtual ~xdAudio();

	// Sfx sources
	/*virtual int createSource()							{ NOT_IMPLEMENTED_RET(createSource, -1) }
	virtual void deleteSource(const int sourceId)		{ NOT_IMPLEMENTED(deleteSource) }

	virtual void playSource(const int sourceId)			{ NOT_IMPLEMENTED(playSource) }
	virtual void stopSource(const int sourceId)			{ NOT_IMPLEMENTED(stopSource) }

	virtual void setSourceBuffer(const int sourceId, const int bufferId)								{ NOT_IMPLEMENTED(setSourceBuffer) }
	virtual void setSourcePosition(const int sourceId, const float x, const float y, const float z)		{ NOT_IMPLEMENTED(setSourcePosition) }
	virtual void setSourceVelocity(const int sourceId, const float x, const float y, const float z)		{ NOT_IMPLEMENTED(setSourceVelocity) }
	virtual void setSourceLooping(const int sourceId, const bool looping)								{ NOT_IMPLEMENTED(setSourceLooping) }
	virtual void setSourceGain(const int sourceId, const float gain)									{ NOT_IMPLEMENTED(setSourceGain) }
	virtual void setSourcePitch(const int sourceId, const float pitch)									{ NOT_IMPLEMENTED(setSourcePitch) }
	virtual void setSourceMinDist(const int sourceId, const float dist)									{ NOT_IMPLEMENTED(setSourceMinDist) }
	virtual void setSourceMaxDist(const int sourceId, const float dist)									{ NOT_IMPLEMENTED(setSourceMaxDist) }

	// Sfx listener
	virtual void setListenerPosition(const float x, const float y, const float z)		{ NOT_IMPLEMENTED(setListenerPosition) }
	virtual void setListenerVelocity(const float x, const float y, const float z)		{ NOT_IMPLEMENTED(setListenerVelocity) }
	virtual void setListenerOrientation(const float x, const float y, const float z)	{ NOT_IMPLEMENTED(setListenerOrientation) }

	// Sfx buffer
	virtual int loadFile(const string &filePath)		{ NOT_IMPLEMENTED_RET(loadFile, -1) }
	virtual void deleteBuffer(const int bufferId)		{ NOT_IMPLEMENTED(deleteBuffer) }*/
};

#endif // X2D_SOUND_H