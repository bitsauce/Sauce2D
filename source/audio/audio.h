#ifndef X2D_SOUND_H
#define X2D_SOUND_H

#include "common/config.h"

#include "source.h"
#include "buffer.h"

/*********************************************************************
**	Abstract Audio Manager											**
**********************************************************************/
class XDAPI XAudioManager
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON
		
	// Listener position
	virtual void setPosition(const Vector2&)	{ NOT_IMPLEMENTED(setPosition) }
	virtual Vector2 getPosition() const			{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }

	// Listener velocity
	virtual void setVelocity(const Vector2&)	{ NOT_IMPLEMENTED(setVelocity) }
	virtual Vector2 getVelocity() const			{ NOT_IMPLEMENTED_RET(getVelocity, Vector2(0.0f)) }

	// Listener orientation
	virtual void setOrientation(const Vector3&) { NOT_IMPLEMENTED(setOrientation) }
	virtual Vector3 getOrientation() const		{ NOT_IMPLEMENTED_RET(getOrientation, Vector3(0.0f)) }
	
	// Global factories
	static XAudioSource *CreateSource(XAudioBuffer *buffer);
	static XAudioSource *CreateSource(const string &path);

private:
	// XAudioManager instance
	static XAudioManager *s_this;

	// Virtual factory functions
	virtual XAudioSource *createSource(XAudioBuffer *buffer) = 0;
};

#endif // X2D_SOUND_H