#include <Sauce/Common.h>
#include <Sauce/Graphics.h>

BEGIN_SAUCE_NAMESPACE

TextureAtlas::TextureAtlas(const int width, const int height, const int border) :
	m_border(border),
	m_texture(0),
	m_width(width),
	m_height(height)
{
	// Create a texture for the atlas
	m_texture = shared_ptr<Texture2D>(new Texture2D(width, height));
	m_rectanglePacker.setMaxWidth(width);
}

TextureAtlas::~TextureAtlas()
{
	if(m_result.valid)
	{
		for(map<string, RectanglePacker::Entry>::iterator itr = m_result.rectangles.begin(); itr != m_result.rectangles.end(); ++itr)
		{
			delete (Pixmap*) itr->second.getData();
		}
	}
}

void TextureAtlas::add(const string &key, Resource<Texture2D> texture)
{
	add(key, texture->getPixmap());
}

void TextureAtlas::add(const string & key, const Pixmap & pixmap)
{
	m_rectanglePacker.addRectangle(key, pixmap.getWidth() + m_border * 2, pixmap.getHeight() + m_border * 2, new Pixmap(pixmap));
}

TextureRegion TextureAtlas::get(const string & key) const
{
	return get(key, 0.0f, 0.0f, 1.0f, 1.0f);
}

TextureRegion TextureAtlas::get(const string &key, const Vector2F & uv0, const Vector2F & uv1) const
{
	// Validate input
	if(!m_result.valid) return TextureRegion();
	if(m_result.rectangles.find(key) == m_result.rectangles.end()) return TextureRegion();

	// TODO: Optimization: The texture regions can be precalculated in update() to save time
	// Get texture region
	const RectanglePacker::Entry &rect = m_result.rectangles.at(key);
	return TextureRegion(
		((rect.getX() + m_border) + (rect.getWidth() - m_border * 2) * uv0.x) / m_width, ((rect.getY() + m_border) + (rect.getHeight() - m_border * 2) * uv0.y) / m_height,
		((rect.getX() + m_border) + (rect.getWidth() - m_border * 2) * uv1.x) / m_width, ((rect.getY() + m_border) + (rect.getHeight() - m_border * 2) * uv1.y) / m_height
		);

}

void TextureAtlas::create()
{
	uchar *pixels = new uchar[m_width * m_height * 4];
	memset(pixels, 0, m_width * m_height * 4);

	const RectanglePacker::Result result = m_rectanglePacker.pack();
	for(map<string, RectanglePacker::Entry>::const_iterator itr = result.rectangles.begin(); itr != result.rectangles.end(); ++itr)
	{
		const RectanglePacker::Entry &rect = itr->second;
		const Pixmap *pixmap = (Pixmap*) rect.getData();
		for(uint x = 0; x < pixmap->getWidth(); x++)
		{
			for(uint y = 0; y < pixmap->getHeight(); y++)
			{
				int dataPos = ((rect.getX() + x + m_border) + ((rect.getY() + y + m_border) * m_width)) * 4;
				int pagePos = (x + y * pixmap->getWidth()) * 4;
				memcpy(pixels + dataPos, pixmap->getData() + pagePos, 4);
			}
		}
	}
	m_result = result;
	m_texture->updatePixmap(Pixmap(m_width, m_height, pixels));
}

END_SAUCE_NAMESPACE
