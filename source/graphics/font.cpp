//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
// Inspired by: http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/

#include <x2d/engine.h>
#include <x2d/graphics.h>

// Credits to the FreeType library
// FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftsnames.h>

AS_REG_REF(XFont)

int XFont::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("XFont", asBEHAVE_FACTORY, "XFont @f(const string &in, const int)", asFUNCTIONPR(Factory, (string&, const uint), XFont*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XFont", "float getStringWidth(const string &in)", asMETHOD(XFont, getStringWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFont", "float getStringHeight(const string &in)", asMETHOD(XFont, getStringHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFont", "void setColor(const Vector4 &in)", asMETHOD(XFont, setColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFont", "void draw(XBatch @batch, const Vector2 &in, const string &in)", asMETHOD(XFont, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

bool getXFontFile(string &XFontName)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\XFonts", 0, KEY_READ, &hkey) == ERROR_SUCCESS)
	{
		// Get maximum buffer sizes
		ulong maxValue;
		ulong maxData;
		RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &maxValue, &maxData, NULL, NULL);
		maxValue++;
		maxData++;

		// Enumerate registry
		int i = 0;
		char *value = new char[maxValue];
		ulong valsize = maxValue;
		uchar *data = new uchar[maxData];
		ulong datasize = maxData;
		ulong datatype = REG_SZ;
		while(RegEnumValue(hkey, i++, value, &valsize, 0, &datatype, data, &datasize) == ERROR_SUCCESS)
		{
			// Get name of XFont
			string valstr = value;
			valstr = valstr.substr(0, valstr.find_first_of('(')-1);

			// Check if the value was found
			if(XFontName == valstr)
			{
				// Set filename
				char *winDir = new char[MAX_PATH];
				GetWindowsDirectory(winDir, MAX_PATH);
				XFontName = string(winDir) + "\\XFonts\\" + string(reinterpret_cast<char*>(data));

				// Clean up
				delete[] value;
				delete[] data;
				delete[] winDir;
				return true;
			}

			// Reset variables
			ZeroMemory(value, maxValue);
			ZeroMemory(data, maxData);
			valsize = maxValue;
			datasize = maxData;
		}

		// Clean up
		delete[] value;
		delete[] data;
	}
	return false;
}

// Finds next power of two
inline int next_p2(int a)
{
	int rval = 2;
	while(rval < a) rval <<= 1;
	return rval;
}

XFont::XFont(const string &path, const uint size) :
	m_color(1.0f),
	m_atlas(0),
	m_size(0),
	m_lineSize(0),
	m_Msize(0)
{
	load(path, size);
}

XFont::~XFont()
{
	if(m_atlas) {
		m_atlas->release();
	}
}

void XFont::load(const string &filePath, const uint size)
{
	// Create and initialize a FreeType library
	FT_Library library;
	FT_Error error;
	if((error = FT_Init_FreeType(&library)) != 0)
	{
		LOG("XFont::load - Failed to initialize FreeType 2 (error code: %i)", error);
		return;
	}

	// Load the XFont information from file
	FT_Face face;
	if((error = FT_New_Face(library, filePath.c_str(), 0, &face)) != 0)
	{
		LOG("XFont::load - Failed load XFont data (error code: %i)", error);
		return;
	}

	// Setup XFont characters
	m_metrics.resize(128);
	m_size = size;

	// Set characterset size
	//if(error = FT_Set_Char_Size(face, size << 6, size << 6, 96, 96))
	if((error = FT_Set_Pixel_Sizes(face, size, size)) != 0)
	{
		LOG("XFont::load - Failed to set pixel size (error code: %i)", error);
		return;
	}

	// Load bitmap data for each of the character of the XFont
	vector<XPixmap> pixmaps;
	for(uchar ch = 0; ch < 128; ch++)
	{
		// Load the glyph for our character
		if((error = FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) != 0)
		{
			LOG("XFont::load - Failed to load glyph '%c' (error code: %i)", ch, error);
			return;
		}

		// Render glyph to bitmap
		if((error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) != 0)
		{
			LOG("XFont::load - Failed to get glyph '%c' (error code: %i)", ch, error);
			return;
		}

		// Get bitmap object
		FT_Bitmap &bitmap = face->glyph->bitmap;

		// Get dimentions of glyph
		uint width = bitmap.width;
		uint height = bitmap.rows;

		// Allocate memory for texture data
		XPixmap pixmap(width, height);
		for(uint y = 0; y < height; y++)
		{ 
			for(uint x = 0; x < width; x++)
			{
				uchar c = bitmap.buffer[x + (height-y-1)*width];
				pixmap.setColor(x, y, Vector4(1.0f, 1.0f, 1.0f, c/255.0f));
			}
		}
		pixmaps.push_back(pixmap);

		// Store metrics
		CharMetrics &metrics = m_metrics[ch];
		metrics.bearing.set(
			face->glyph->metrics.horiBearingX >> 6,
			face->glyph->metrics.horiBearingY >> 6
			);
		metrics.size.set(
			face->glyph->metrics.width >> 6,
			face->glyph->metrics.height >> 6
			);
		metrics.advance.set(
			face->glyph->advance.x >> 6,
			metrics.bearing.y - metrics.size.y
			);
	}
	m_lineSize = face->height >> 6;
	m_Msize = m_metrics['M'].size.y;

	// Create XFont atlas
	m_atlas = new XTextureAtlas(pixmaps);

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

float XFont::getStringWidth(const string &str)
{
	float width = 0.0f;
	for(uint i = 0; i < str.size(); i++)
	{
		uchar ch = str[i];
		if(!isValidChar(ch))
			continue;
		width += m_metrics[ch].advance.x;
	}
	return width;
}

float XFont::getStringHeight(const string &str)
{
	float height = (float)m_lineSize;
	for(uint i = 0; i < str.size(); i++)
	{
		if(str[i] == '\n')
			height += m_lineSize;
	}
	return height;
}

void XFont::setColor(const Vector4 &color)
{
	m_color = color;
}

void XFont::draw(XBatch *batch, const Vector2 &pos, const string &str)
{
	// Get current position
	Vector2 currentPos = pos;

	// Setup batch
	batch->setTexture(m_atlas->getTexture());
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);

	// Draw string
	XVertex vertices[4];
	for(uint i = 0; i < str.size(); i++)
	{
		// Check for new line
		if(str[i] == '\n')
		{
			currentPos.x = pos.x;
			currentPos.y += m_lineSize;
			continue;
		}

		// Get char index
		uchar ch = str[i];
		if(!isValidChar(ch)) {
			continue; // Cannot draw this char (yet)
		}
		
		// Get char object
		const CharMetrics &metrics = m_metrics[ch];

		// Apply char bearing
		currentPos.x += metrics.bearing.x;
		
		// Draw char
		XTextureRegion region = m_atlas->get(ch);

		vertices[0].position.set(currentPos.x, currentPos.y + (m_Msize - metrics.size.y) - metrics.advance.y);
		vertices[0].color = m_color;
		vertices[0].texCoord.set(region.uv0.x, region.uv1.y);

		vertices[1].position.set(currentPos.x + metrics.size.x, currentPos.y + (m_Msize - metrics.size.y) - metrics.advance.y);
		vertices[1].color = m_color;
		vertices[1].texCoord.set(region.uv1.x, region.uv1.y);
		
		vertices[2].position.set(currentPos.x + metrics.size.x, currentPos.y + (m_Msize - metrics.size.y) - metrics.advance.y + metrics.size.y);
		vertices[2].color = m_color;
		vertices[2].texCoord.set(region.uv1.x, region.uv0.y);
		
		vertices[3].position.set(currentPos.x, currentPos.y + (m_Msize - metrics.size.y) - metrics.advance.y + metrics.size.y);
		vertices[3].color = m_color;
		vertices[3].texCoord.set(region.uv0.x, region.uv0.y);
		
		batch->addVertices(vertices, 4, QUAD_INDICES, 6);

		// Apply advance
		currentPos.x += metrics.advance.x - metrics.bearing.x;
	}
	batch->release();
}

XFont *XFont::Factory(string &XFontName, const uint size)
{
	// Check if we can find the XFont in the local directories
	util::toAbsoluteFilePath(XFontName);
	if(!util::fileExists(XFontName))
	{
		// Loop throught the registry to find the file by XFont name
		if(!getXFontFile(XFontName))
		{
			LOG("XFont '%s' not found!", XFontName);
			return 0;
		}
	}
	return new XFont(XFontName, size);
}