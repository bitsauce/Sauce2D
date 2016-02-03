#ifndef CGF_TEXTURE_ATLAS_H
#define CGF_TEXTURE_ATLAS_H

#include <CGF/Common.h>
#include <CGF/Graphics/Texture.h>
#include <CGF/Graphics/Pixmap.h>
#include <CGF/Graphics/TextureRegion.h>

BEGIN_CGF_NAMESPACE

class CGF_API TextureAtlas
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

END_CGF_NAMESPACE

#endif // CGF_TEXTURE_ATLAS_H
