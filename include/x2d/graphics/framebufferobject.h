#ifndef X2D_FRAME_BUFFER_H
#define X2D_FRAME_BUFFER_H

#include "../engine.h"

class XTexture;

class XDAPI XFrameBufferObject
{
public:

	virtual void bind(XTexture *texture) = 0;
	virtual void unbind() = 0;
};

#endif // X2D_FRAME_BUFFER_H