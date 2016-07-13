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
	TextureAtlas(const int width = 2048, const int height = 2048, const int border = 1);
	~TextureAtlas();

	void add(const string &key, Resource<Texture2D> texture);
	void add(const string &key, const Pixmap &pixmap);

	TextureRegion get(const string &key) const;
	TextureRegion get(const string &key, const Vector2F &uv0, const Vector2F &uv1) const;
	TextureRegion get(const string &key, const float u0, const float v0, const float u1, const float v1) const
	{
		return get(key, Vector2F(u0, v0), Vector2F(u1, v1));
	}

	Resource<Texture2D> getTexture() const
	{
		return m_texture;
	}

	void create();
	
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
	// Atlas texture
	Resource<Texture2D> m_texture;

	// Atlas properties
	int m_width, m_height;
	int m_border;

	// Rectangle packer result
	RectanglePacker m_rectanglePacker;
	RectanglePacker::Result m_result;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_TEXTURE_ATLAS_H