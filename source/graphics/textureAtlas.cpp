//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

#define ATLAS_SIZE 2048

BEGIN_SAUCE_NAMESPACE

TextureAtlas::TextureAtlas() :
	m_border(1),
	m_texture(0)
{
	init(vector<Pixmap>());
}

TextureAtlas::TextureAtlas(vector<Resource<Texture2D>> textures, const int border) :
	m_border(border),
	m_texture(0)
{
	vector<Pixmap> pixmaps;
	for(vector<Resource<Texture2D>>::iterator itr = textures.begin(); itr != textures.end(); ++itr)
	{
		if(*itr)
		{
			pixmaps.push_back((*itr)->getPixmap());
		}
	}

	init(pixmaps);
}

TextureAtlas::TextureAtlas(vector<Pixmap> &pixmaps, const int border) :
	m_border(border),
	m_texture(0)
{
	init(pixmaps);
}

TextureAtlas::~TextureAtlas()
{
	for(vector<RectanglePacker::Rect>::iterator itr = m_result.rectangles.begin(); itr != m_result.rectangles.end(); ++itr)
	{
		delete (AtlasPage*) (*itr).getData();
	}
}

void TextureAtlas::init(const vector<Pixmap> &pixmaps)
{
	// Create a texture for the atlas
	m_texture = Resource<Texture2D>(new Texture2D(ATLAS_SIZE, ATLAS_SIZE));
	m_texturePacker.setMaxWidth(ATLAS_SIZE);

	// Set as uninitialized
	m_initialized = false;
	m_size = 0;

	// Add all pixmaps
	for(vector<Pixmap>::const_iterator itr = pixmaps.begin(); itr != pixmaps.end(); ++itr)
	{
		add(*itr);
	}

	// Update atlas texture
	update();

	// Set as initialized
	m_initialized = true;
}

void TextureAtlas::add(Resource<Texture2D> texture)
{
	add(texture->getPixmap());
}

void TextureAtlas::add(const Pixmap &pixmap)
{
	m_texturePacker.addRect(RectanglePacker::Rect(pixmap.getWidth() + m_border * 2, pixmap.getHeight() + m_border * 2, new AtlasPage(pixmap, m_size++)));
	if(m_initialized)
	{
		update();
	}
}

TextureRegion TextureAtlas::get(const int index) const
{
	return get(index, 0.0f, 0.0f, 1.0f, 1.0f);
}

TextureRegion TextureAtlas::get(const int index, const Vector2F &uv0, const Vector2F &uv1) const
{
	// Validate index
	if(index < 0 || index >= m_size)
	{
		return TextureRegion(Vector2F(0.0f), Vector2F(1.0f));
	}

	// TODO: Optimization: The texture regions can be precalculated in update() to save time
	// Get texture region
	const RectanglePacker::Rect &rect = m_result.rectangles[index];
	return TextureRegion(
		((rect.getX() + m_border) + (rect.getWidth() - m_border * 2) * uv0.x) / ATLAS_SIZE, ((rect.getY() + m_border) + (rect.getHeight() - m_border * 2) * uv0.y) / ATLAS_SIZE,
		((rect.getX() + m_border) + (rect.getWidth() - m_border * 2) * uv1.x) / ATLAS_SIZE, ((rect.getY() + m_border) + (rect.getHeight() - m_border * 2) * uv1.y) / ATLAS_SIZE
		);
}

TextureRegion TextureAtlas::get(const int index, const float u0, const float v0, const float u1, const float v1) const
{
	return get(index, Vector2F(u0, v0), Vector2F(u1, v1));
}

Resource<Texture2D> TextureAtlas::getTexture() const
{
	return m_texture;
}

bool sortResult(RectanglePacker::Rect r1, RectanglePacker::Rect r2)
{
	return ((TextureAtlas::AtlasPage*)r1.getData())->getIndex() < ((TextureAtlas::AtlasPage*)r2.getData())->getIndex();
}

void TextureAtlas::update()
{
	uchar *pixels = new uchar[ATLAS_SIZE * ATLAS_SIZE * 4];
	memset(pixels, 0, ATLAS_SIZE * ATLAS_SIZE * 4);

	const RectanglePacker::Result result = m_texturePacker.pack();
	for(vector<RectanglePacker::Rect>::const_iterator itr = result.rectangles.begin(); itr != result.rectangles.end(); ++itr)
	{
		const RectanglePacker::Rect &rect = (*itr);
		const Pixmap *pixmap = ((AtlasPage*) rect.getData())->getPixmap();
		for(uint x = 0; x < pixmap->getWidth(); x++)
		{
			for(uint y = 0; y < pixmap->getHeight(); y++)
			{
				int dataPos = ((rect.getX() + x + m_border) + ((rect.getY() + y + m_border) * ATLAS_SIZE)) * 4;
				int pagePos = (x + y * pixmap->getWidth()) * 4;
				memcpy(pixels + dataPos, pixmap->getData() + pagePos, 4);
			}
		}
	}
	m_result = result;

	sort(m_result.rectangles.begin(), m_result.rectangles.end(), sortResult);

	m_texture->updatePixmap(Pixmap(ATLAS_SIZE, ATLAS_SIZE, pixels));
}

END_SAUCE_NAMESPACE
