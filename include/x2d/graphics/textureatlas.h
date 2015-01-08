#ifndef X2D_TEXTURE_ATLAS_H
#define X2D_TEXTURE_ATLAS_H

#include "../engine.h"
#include "texture.h"
#include "pixmap.h"
#include "textureRegion.h"

BEGIN_XD_NAMESPACE

class XDAPI TextureAtlas
{
public:
	TextureAtlas();
	TextureAtlas(vector<Texture2DPtr> textures, const int border = 1);
	TextureAtlas(vector<Pixmap> &pixmaps, const int border = 1);
	~TextureAtlas();

	void add(Texture2D *texture);
	void add(const Pixmap &pixmap);

	void update();

	TextureRegion get(const int index) const;
	TextureRegion get(const int index, const Vector2 &uv0, const Vector2 &uv1) const;
	TextureRegion get(const int index, const float u0, const float v0, const float u1, const float v1) const;

	Texture2DPtr getTexture() const;
	
	struct AtlasPage
	{
		AtlasPage(const Pixmap &pixmap, int index) :
			pixmap(pixmap),
			index(index)
		{
		}

		~AtlasPage()
		{
		}

		const Pixmap *getPixmap() const
		{
			return &pixmap;
		}

		int getIndex() const
		{
			return index;
		}

	private:
		Pixmap pixmap;
		int index;
	};

private:
	void init(const vector<Pixmap> &pixmaps);

	Texture2DPtr m_texture;
	RectanglePacker m_texturePacker;
	RectanglePacker::Result m_result;
	int m_size;
	bool m_initialized;
	int m_border;
};

END_XD_NAMESPACE

#endif // X2D_TEXTURE_ATLAS_H