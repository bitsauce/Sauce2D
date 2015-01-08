//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

#define ATLAS_SIZE 2048

// TDOD: Add a border option to the texture atlas (usefull for fixing font bleeding)

BEGIN_XD_NAMESPACE

TextureAtlas::TextureAtlas() :
	m_border(1),
	m_texture(0)
{
	init(vector<Pixmap>());
}

TextureAtlas::TextureAtlas(vector<Texture2DPtr> textures, const int border) :
	m_border(border),
	m_texture(0)
{
	vector<Pixmap> pixmaps;
	for(vector<Texture2DPtr>::iterator itr = textures.begin(); itr != textures.end(); ++itr)
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
	for(vector<RectanglePacker::Rectangle>::iterator itr = m_result.rectangles.begin(); itr != m_result.rectangles.end(); ++itr) {
		delete (AtlasPage*)(*itr).getData();
	}
	//m_atlas->release();
}

void TextureAtlas::init(const vector<Pixmap> &pixmaps)
{
	// Create a texture for the atlas
	m_texture = Texture2DPtr(new Texture2D(ATLAS_SIZE, ATLAS_SIZE));
	m_texturePacker.setMaxWidth(ATLAS_SIZE);

	// Set as uninitialized
	m_initialized = false;
	m_size = 0;
	
	// Add all pixmaps
	for(vector<Pixmap>::const_iterator itr = pixmaps.begin(); itr != pixmaps.end(); ++itr) {
		add(*itr);
	}

	// Update atlas texture
	update();

	// Set as initialized
	m_initialized = true;
}

void TextureAtlas::add(Texture2D *texture)
{
	add(texture->getPixmap());
	//texture->release();
}

void TextureAtlas::add(const Pixmap &pixmap)
{
	RectanglePacker::Rectangle rect(new AtlasPage(pixmap, m_size++));
	rect.setSize(pixmap.getWidth()+m_border*2, pixmap.getHeight()+m_border*2);
	m_texturePacker.addRect(rect);
	if(m_initialized) {
		update();
	}
}

TextureRegion TextureAtlas::get(const int index) const
{
	return get(index, 0.0f, 0.0f, 1.0f, 1.0f);
}

TextureRegion TextureAtlas::get(const int index, const Vector2 &uv0, const Vector2 &uv1) const
{
	// Validate index
	if(index < 0 || index >= m_size) {
		return TextureRegion(Vector2(0.0f), Vector2(1.0f));
	}

	// TODO: Optimization: The texture regions can be precalculated in update() to save time
	// Get texture region
	const RectanglePacker::Rectangle &rect = m_result.rectangles[index];
	return TextureRegion(
		((rect.x+m_border) + (rect.width-m_border*2)*uv0.x)/ATLAS_SIZE, ((rect.y+m_border) + (rect.height-m_border*2)*uv0.y)/ATLAS_SIZE,
		((rect.x+m_border) + (rect.width-m_border*2)*uv1.x)/ATLAS_SIZE, ((rect.y+m_border) + (rect.height-m_border*2)*uv1.y)/ATLAS_SIZE
		);
}

TextureRegion TextureAtlas::get(const int index, const float u0, const float v0, const float u1, const float v1) const
{
	return get(index, Vector2(u0, v0), Vector2(u1, v1));
}

Texture2DPtr TextureAtlas::getTexture() const
{
	return m_texture;
}

bool sortResult(RectanglePacker::Rectangle r1, RectanglePacker::Rectangle r2)
{
	return ((TextureAtlas::AtlasPage*)r1.getData())->getIndex() < ((TextureAtlas::AtlasPage*)r2.getData())->getIndex();
}

void TextureAtlas::update()
{
	uchar *pixels = new uchar[ATLAS_SIZE*ATLAS_SIZE*4];
	memset(pixels, 0, ATLAS_SIZE*ATLAS_SIZE*4);

	const RectanglePacker::Result result = m_texturePacker.pack();
	for(vector<const RectanglePacker::Rectangle>::const_iterator itr = result.rectangles.begin(); itr != result.rectangles.end(); ++itr)
	{
		const RectanglePacker::Rectangle &rect = (*itr);
		const Pixmap *pixmap = ((AtlasPage*)rect.getData())->getPixmap();
		for(int x = 0; x < pixmap->getWidth(); x++)
		{
			for(int y = 0; y < pixmap->getHeight(); y++)
			{
				int dataPos = ((rect.x + x + m_border) + ((rect.y + y + m_border) * ATLAS_SIZE)) * 4;
				int pagePos = (x + y * pixmap->getWidth()) * 4;
				memcpy(pixels + dataPos, pixmap->getData() + pagePos, 4);
			}
		}
	}
	m_result = result;

	sort(m_result.rectangles.begin(), m_result.rectangles.end(), sortResult);

	m_texture->updatePixmap(Pixmap(ATLAS_SIZE, ATLAS_SIZE, pixels));
}

END_XD_NAMESPACE