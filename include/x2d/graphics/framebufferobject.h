#ifndef X2D_FRAME_BUFFER_H
#define X2D_FRAME_BUFFER_H

#include "../engine.h"

class XTexture;

class XDAPI XFrameBufferObject
{
public:
	XFrameBufferObject();
	~XFrameBufferObject();

	void bind(shared_ptr<XTexture> texture);
	void unbind();

private:
	GLuint m_id;
	float m_ortho[6];
	int m_viewport[4];
};

#endif // X2D_FRAME_BUFFER_H