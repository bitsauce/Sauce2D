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

AS_REG_REF(XTextureAtlas, "TextureAtlas")

int XTextureAtlas::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("TextureAtlas", asBEHAVE_FACTORY, "TextureAtlas @f()", asFUNCTIONPR(Factory, (), XTextureAtlas*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureAtlas", asBEHAVE_FACTORY, "TextureAtlas @f(array<Texture@>@)", asFUNCTIONPR(Factory, (XScriptArray*), XTextureAtlas*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureAtlas", asBEHAVE_FACTORY, "TextureAtlas @f(array<Pixmap>@)", asFUNCTIONPR(Factory, (XScriptArray*), XTextureAtlas*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("TextureAtlas", "TextureRegion get(const int) const", asMETHODPR(XTextureAtlas, get, (const int) const, XTextureRegion), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureAtlas", "TextureRegion get(const int, const Vector2 &in, const Vector2 &in) const", asMETHODPR(XTextureAtlas, get, (const int, const Vector2&, const Vector2&) const, XTextureRegion), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureAtlas", "TextureRegion get(const int, const float, const float, const float, const float) const", asMETHODPR(XTextureAtlas, get, (const int, const float, const float, const float, const float) const, XTextureRegion), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("TextureAtlas", "Texture @getTexture() const", asMETHOD(XTextureAtlas, getTexture), asCALL_THISCALL); AS_ASSERT
	
	return r;
}

XTextureAtlas::XTextureAtlas()
{
	init(vector<XPixmap>());
}

XTextureAtlas::XTextureAtlas(vector<XTexture*> textures)
{
	vector<XPixmap> pixmaps;
	for(vector<XTexture*>::iterator itr = textures.begin(); itr != textures.end(); ++itr) {
		if(*itr) {
			pixmaps.push_back((*itr)->getPixmap());
		}
	}

	init(pixmaps);
}

XTextureAtlas::XTextureAtlas(vector<XPixmap> &pixmaps)
{
	init(pixmaps);
}

XTextureAtlas::~XTextureAtlas()
{
	for(vector<RectanglePacker::Rectangle>::iterator itr = m_result.rectangles.begin(); itr != m_result.rectangles.end(); ++itr) {
		delete (AtlasPage*)(*itr).getData();
	}
	m_atlas->release();
}

void XTextureAtlas::init(const vector<XPixmap> &pixmaps)
{
	// Create a texture for the atlas
	m_atlas = XGraphics::CreateTexture(ATLAS_SIZE, ATLAS_SIZE);

	// Set as uninitialized
	m_initialized = false;
	m_border = 1;
	m_size = 0;
	
	// Add all pixmaps
	for(vector<XPixmap>::const_iterator itr = pixmaps.begin(); itr != pixmaps.end(); ++itr) {
		add(*itr);
	}

	// Update atlas texture
	update();

	// Set as initialized
	m_initialized = true;
}

void XTextureAtlas::add(XTexture *texture)
{
	add(texture->getPixmap());
	texture->release();
}

void XTextureAtlas::add(const XPixmap &pixmap)
{
	RectanglePacker::Rectangle rect(new AtlasPage(pixmap, m_size++));
	rect.setSize(pixmap.getWidth()+m_border*2, pixmap.getHeight()+m_border*2);
	m_texturePacker.addRect(rect);
	if(m_initialized) {
		update();
	}
}

XTextureRegion XTextureAtlas::get(const int index) const
{
	return get(index, 0.0f, 0.0f, 1.0f, 1.0f);
}

XTextureRegion XTextureAtlas::get(const int index, const Vector2 &uv0, const Vector2 &uv1) const
{
	// Validate index
	if(index < 0 || index >= m_size) {
		return XTextureRegion(0, Vector2(0.0f), Vector2(1.0f));
	}

	// Get texture region
	const RectanglePacker::Rectangle &rect = m_result.rectangles[index];
	m_atlas->addRef();
	return XTextureRegion(m_atlas,
		(rect.x + rect.width*uv0.x)/ATLAS_SIZE, (rect.y + rect.height*uv0.y)/ATLAS_SIZE,
		(rect.x + rect.width*uv1.x)/ATLAS_SIZE, (rect.y + rect.height*uv1.y)/ATLAS_SIZE
		);
}

XTextureRegion XTextureAtlas::get(const int index, const float u0, const float v0, const float u1, const float v1) const
{
	return get(index, Vector2(u0, v0), Vector2(u1, v1));
}

XTexture *XTextureAtlas::getTexture() const
{
	m_atlas->addRef();
	return m_atlas;
}

bool sortResult(RectanglePacker::Rectangle r1, RectanglePacker::Rectangle r2)
{
	return ((XTextureAtlas::AtlasPage*)r1.getData())->getIndex() < ((XTextureAtlas::AtlasPage*)r2.getData())->getIndex();
}

void XTextureAtlas::update()
{
	float *pixels = new float[ATLAS_SIZE*ATLAS_SIZE*4];
	memset(pixels, 0, ATLAS_SIZE*ATLAS_SIZE*4);

	const RectanglePacker::Result result = m_texturePacker.pack();
	for(vector<const RectanglePacker::Rectangle>::const_iterator itr = result.rectangles.begin(); itr != result.rectangles.end(); ++itr)
	{
		const RectanglePacker::Rectangle &rect = (*itr);
		const XPixmap *pixmap = ((AtlasPage*)rect.getData())->getPixmap();
		for(int x = 0; x < pixmap->getWidth(); x++)
		{
			for(int y = 0; y < pixmap->getHeight(); y++)
			{
				int dataPos = ((rect.x + x + m_border) + ((rect.y + y + m_border) * ATLAS_SIZE)) * 4;
				int pagePos = (x + y * pixmap->getWidth()) * 4;
				memcpy(&pixels[dataPos], &pixmap->getData()[pagePos], sizeof(float) * 4);
			}
		}
	}
	m_result = result;

	sort(m_result.rectangles.begin(), m_result.rectangles.end(), sortResult);

	m_atlas->updatePixmap(XPixmap(ATLAS_SIZE, ATLAS_SIZE, (Vector4*)pixels));
}

XTextureAtlas *XTextureAtlas::Factory()
{
	return new XTextureAtlas();
}

XTextureAtlas *XTextureAtlas::Factory(XScriptArray *arr)
{
	XTextureAtlas *atlas = 0;
	if(arr->GetElementTypeName() == "Texture")
	{
		vector<XTexture*> textures;
		for(uint i = 0; i < arr->GetSize(); i++) {
			textures.push_back(*(XTexture**)arr->At(i));
		}
		atlas = new XTextureAtlas(textures);
	}else if(arr->GetElementTypeName() == "Pixmap")
	{
		vector<XPixmap> pixmaps;
		for(uint i = 0; i < arr->GetSize(); i++) {
			pixmaps.push_back(*(XPixmap*)arr->At(i));
		}
		atlas = new XTextureAtlas(pixmaps);
	}

	arr->Release();
	return atlas;
}