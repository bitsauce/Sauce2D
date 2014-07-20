#ifndef GRAPHICS_TEXTURE_ATLAS_H
#define GRAPHICS_TEXTURE_ATLAS_H

#include <x2d/math.h>
#include <x2d/graphics.h>
#include <x2d/base.h>

class TextureAtlas
{
	AS_DECL_REF
public:
	TextureAtlas();
	TextureAtlas(vector<Texture*> textures);
	TextureAtlas(vector<Pixmap> &pixmaps);
	~TextureAtlas();

	void add(Texture *texture);
	void add(const Pixmap &pixmap);

	void update();

	TextureRegion get(const int index) const;
	TextureRegion get(const int index, const Vector2 &uv0, const Vector2 &uv1) const;
	TextureRegion get(const int index, const float u0, const float v0, const float u1, const float v1) const;

	Texture *getTexture() const;
	
	struct AtlasPage
	{
		AtlasPage(const Pixmap &pixmap, int index) :
			index(index)
		{
			int size = pixmap.getWidth()*pixmap.getHeight()*4;
			data = new float[size];
			memcpy((void*)data, pixmap.getData(), size*sizeof(float));
		}

		~AtlasPage()
		{
			delete[] data;
		}

		const float *getData() const
		{
			return data;
		}

		int getIndex() const
		{
			return index;
		}

	private:
		const float *data;
		int index;
	};

private:
	void init(const vector<Pixmap> &pixmaps);

	Texture *m_atlas;
	RectanglePacker m_texturePacker;
	RectanglePacker::Result m_result;
	int m_size;
	bool m_initialized;

	static TextureAtlas *Factory();
	static TextureAtlas *Factory(Array *arr);
};

#endif // GRAPHICS_TEXTURE_ATLAS_H