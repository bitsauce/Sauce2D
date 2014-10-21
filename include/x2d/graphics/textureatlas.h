#ifndef X2D_TEXTURE_ATLAS_H
#define X2D_TEXTURE_ATLAS_H

#include "../engine.h"
#include "texture.h"
#include "pixmap.h"
#include "textureRegion.h"

class XTextureAtlas
{
	AS_DECL_REF
public:
	XTextureAtlas();
	XTextureAtlas(vector<XTexture*> textures, const int border = 1);
	XTextureAtlas(vector<XPixmap> &pixmaps, const int border = 1);
	~XTextureAtlas();

	void add(XTexture *texture);
	void add(const XPixmap &pixmap);

	void update();

	XTextureRegion get(const int index) const;
	XTextureRegion get(const int index, const Vector2 &uv0, const Vector2 &uv1) const;
	XTextureRegion get(const int index, const float u0, const float v0, const float u1, const float v1) const;

	XTexture *getTexture() const;
	
	struct AtlasPage
	{
		AtlasPage(const XPixmap &pixmap, int index) :
			pixmap(pixmap),
			index(index)
		{
		}

		~AtlasPage()
		{
		}

		const XPixmap *getPixmap() const
		{
			return &pixmap;
		}

		int getIndex() const
		{
			return index;
		}

	private:
		XPixmap pixmap;
		int index;
	};

private:
	void init(const vector<XPixmap> &pixmaps);

	XTexture *m_atlas;
	RectanglePacker m_texturePacker;
	RectanglePacker::Result m_result;
	int m_size;
	bool m_initialized;
	int m_border;

	static XTextureAtlas *Factory();
	static XTextureAtlas *Factory(XScriptArray *arr, const int border);
};

#endif // X2D_TEXTURE_ATLAS_H