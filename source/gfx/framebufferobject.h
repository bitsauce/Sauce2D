#ifndef GFX_FRAME_BUFFER_H
#define GFX_FRAME_BUFFER_H

#include <x2d/config.h>

class Texture;

class XDAPI FrameBufferObject
{
public:

	virtual void bind(Texture *texture) = 0;
	virtual void unbind() = 0;
};

#endif // GFX_FRAME_BUFFER_H