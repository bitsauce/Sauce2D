#ifndef X2D_FRAME_BUFFER_H
#define X2D_FRAME_BUFFER_H

#include "../engine.h"
#include "texture.h"

namespace xd
{

class XDAPI RenderTarget2D
{
public:
	RenderTarget2D();
	~RenderTarget2D();

	void bind(Texture2DPtr texture);
	void unbind();

private:
	GLuint m_id;
	float m_ortho[6];
	int m_viewport[4];
};

}

#endif // X2D_FRAME_BUFFER_H