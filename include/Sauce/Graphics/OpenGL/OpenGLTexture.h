#pragma once

#include <Sauce/Common.h>
#include <Sauce/Graphics/Pixmap.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API OpenGLTexture2D : public Texture2D
{
	friend class RenderTarget2D;
	friend class GraphicsContext;
public:
	OpenGLTexture2D(const Pixmap &pixmap);
	~OpenGLTexture2D();

	// Pixmap (texture data)
	Pixmap getPixmap() const;
	void updatePixmap(const Pixmap &pixmap);
	void updatePixmap(const uint x, const uint y, const Pixmap &pixmap);
	void clear();

	GLint getID() const { return m_id; }

protected:
	void initialize(const Pixmap &pixmap);
	void updateFiltering();

	GLuint m_id;
};

template class SAUCE_API shared_ptr<Texture2D>;

END_SAUCE_NAMESPACE
