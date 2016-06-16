#ifndef SAUCE_TEXTURE_ATLAS_H
#define SAUCE_TEXTURE_ATLAS_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/Texture.h>
#include <Sauce/Graphics/Pixmap.h>
#include <Sauce/Graphics/TextureRegion.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API TextureAtlas
{
public:
	TextureAtlas();
	TextureAtlas(vector<Resource<Texture2D>> textures, const int border = 1);
	TextureAtlas(vector<Pixmap> &pixmaps, const int border = 1);
	~TextureAtlas();

	void add(Resource<Texture2D> texture);
	void add(const Pixmap &pixmap);

	void update();

	TextureRegion get(const int index) const;
	TextureRegion get(const int index, const Vector2F &uv0, const Vector2F &uv1) const;
	TextureRegion get(const int index, const float u0, const float v0, const float u1, const float v1) const;

	Resource<Texture2D> getTexture() const;
	
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

	Resource<Texture2D> m_texture;
	RectanglePacker m_texturePacker;
	RectanglePacker::Result m_result;
	int m_size;
	bool m_initialized;
	int m_border;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_TEXTURE_ATLAS_H
