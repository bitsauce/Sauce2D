//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
// Inspired by: http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/

#include "font.h"
#include <x2d/console.h>
#include <x2d/graphics.h>
#include "pixmap.h"
#include "sprite.h"

// Credits to the FreeType library
// FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftsnames.h>

AS_REG_REF(Font)

int Font::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Font", asBEHAVE_FACTORY, "Font @f(const string &in, const int)", asFUNCTIONPR(Factory, (const string&, const uint), Font*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Font", "float getStringWidth(const string &in)", asMETHOD(Font, getStringWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Font", "float getStringHeight(const string &in)", asMETHOD(Font, getStringHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Font", "void setColor(const Vector4 &in)", asMETHOD(Font, setColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Font", "void draw(Batch @batch, const Vector2 &in, const string &in)", asMETHOD(Font, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

bool getFontFile(LPCTSTR fontName, string &fileName)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hkey) == ERROR_SUCCESS)
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
			// Get name of font
			string valstr = value;
			valstr = valstr.substr(0, valstr.find_first_of('(')-1);

			// Check if the value was found
			if(fontName == valstr)
			{
				// Set filename
				char *winDir = new char[MAX_PATH];
				GetWindowsDirectory(winDir, MAX_PATH);
				fileName = string(winDir) + "\\Fonts\\" + string(reinterpret_cast<char*>(data));

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

Font::Font(const string &filePathOrFontName, const uint size) :
	m_color(1.0f)
{
	// Check if we can find the file in the local directories
	string filePath;
	if(util::fileExists(filePathOrFontName))
	{
		// Set file
		filePath = filePathOrFontName;
	}else{
		// Loop throught the registry to find the file by font name
		if(!getFontFile(filePathOrFontName.c_str(), filePath)) {
			ERR("Font '%s' not found!", filePath.c_str());
			return;
		}
	}

	load(filePath, size);
}

Font::~Font()
{
	if(m_texture) {
		m_texture->release();
	}
}

void Font::load(const string &filePath, const uint size)
{
	// Create and initialize a FreeType library
	FT_Library library;
	if(FT_Init_FreeType(&library))
		assert("FT_Init_FreeType failed");

	// Load the font information from file
	FT_Face face;
	if(FT_New_Face(library, filePath.c_str(), 0, &face) != 0)
		assert("FT_New_Face failed (there is probably a problem with your font file)");

	// Generate texture and resize chars
	m_chars.resize(128);
	m_size = size;

	// Get font name
	//m_fontName = face->family_name + string(" ") + util::intToStr(size);

	// Set character size
	FT_Set_Char_Size(face, size << 6, size << 6, 96, 96);

	// Load bitmap data for each of the character of the font
	RectPacker packer;
	for(uchar ch = 0; ch < 128; ch++)
	{
		// Load the glyph for our character
		if(FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
			assert("FT_Load_Glyph failed");

		// Move The Face's Glyph Into A Glyph Object.
		FT_Glyph glyph;
		if(FT_Get_Glyph(face->glyph, &glyph))
			assert("FT_Get_Glyph failed");

		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;

		// This reference will make accessing the bitmap easier
		FT_Bitmap &bitmap = bitmapGlyph->bitmap;

		// Get dimentions of glyph
		int width = next_p2(bitmap.width);
		int height = next_p2(bitmap.rows);

		// Allocate memory for the texture data
		uchar* charData = new uchar[2*width*height + 1]; // Create a 16-bit buffer
		for(uint y = 0; y < height; y++)
		{ 
			for(uint x = 0; x < width; x++)
			{
				charData[2*(x + y*width)] = 255;
				charData[2*(x + y*width) + 1] = (x >= bitmap.width || bitmap.rows-y-1 >= bitmap.rows) ? 0 : bitmap.buffer[x + (bitmap.rows-y-1)*bitmap.width];// = 0;
			}
		}

		charData[2*width*height] = ch;

		// Get width of our font texture
		RectPacker::Rect rect(charData);
		rect.setSize(width, height);
		packer.addRect(rect);

		// Store advance
		m_chars[ch].advance = face->glyph->advance.x >> 6;
		m_chars[ch].pos.set(0, (-bitmapGlyph->top+(bitmap.rows-height)));

		// Finish the display list
		FT_Done_Glyph(glyph);
	}

	// Pack rects
	RectPacker::Result result = packer.pack();

	// Create font texture
	uchar *dataPtr = new uchar[result.canvas.x*result.canvas.y*2];
	memset(dataPtr, 0, result.canvas.x*result.canvas.y*2);
	for(uchar i = 0; i < 128; i++)
	{
		// Fill glyph data
		RectPacker::Rect rect = result.rectList[i];
		uchar *rectData = (uchar*)rect.getData();
		uchar ch = rectData[2*rect.getArea()];
		int dataPos = 0, rectPos = 0;
		for(int y = 0; y < rect.getHeight(); y++)
		{
			for(int x = 0; x < rect.getWidth(); x++)
			{
				dataPos = 2 * (((rect.getY()+y) * result.canvas.x) + (rect.getX()+x));
				rectPos = 2 * (y*rect.getWidth()+x);
				dataPtr[dataPos] = rectData[rectPos];
				dataPtr[dataPos+1] = rectData[rectPos+1];
			}
		}
		delete[] rectData;

		// Set char tex coord
		m_chars[ch].size = rect.getSize();
		m_chars[ch].texCoord1.set((float)rect.getRight()/(float)result.canvas.x, (float)rect.getBottom()/(float)result.canvas.y);// = rect.getSize()/result.canvas;
		m_chars[ch].texCoord0.set((float)rect.getLeft()/(float)result.canvas.x, (float)rect.getTop()/(float)result.canvas.y);
	}

	// Set texture param
	Vector4 *pixels = new Vector4[result.canvas.x*result.canvas.y];
	for(int i = 0; i < result.canvas.x*result.canvas.y*2; i += 2)
	{
		float v = dataPtr[i]/255.0f;
		float a = dataPtr[i+1]/255.0f;
		pixels[i/2].set(v, v, v, a);
	}

	m_texture = xdGraphics::CreateTexture(Pixmap(result.canvas.x, result.canvas.y, pixels));
	m_texture->setFiltering(xdLinear);

	delete[] pixels;

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	delete[] dataPtr;
}

float Font::getStringWidth(const string &str)
{
	float width = 0.0f;
	for(uint i = 0; i < str.size(); i++) {
		width += m_chars[str[i]].size.x;
	}
	return width;
}

float Font::getStringHeight(const string &str)
{
	float height = 0.0f;
	for(uint i = 0; i < str.size(); i++) {
		if(str[i] == '\n')
			height += m_size/0.63f;
	}
	return height;
}

void Font::setColor(const Vector4 &color)
{
	m_color = color;
}

void Font::draw(Batch *batch, const Vector2 &pos, const string &str)
{
	// Add space between lines
	float h = m_size/0.63f;
	float yOffset = 0.0f;
	float xOffset = 0.0f;

	m_texture->addRef();
	batch->setTexture(m_texture);

	// Split string lines
	string line;
	for(uint i = 0; i < str.size(); i++)
	{
		// Check for new line
		if(str[i] == '\n')
		{
			yOffset += h;
			xOffset = 0.0f;
			continue;
		}

		// Get char
		Char c = m_chars[str[i]];

		// Draw char
		float px = pos.x + xOffset + c.pos.x;
		float py = pos.y + yOffset + c.pos.y + m_size;
		
		Vertex vertices[4];
		
		vertices[0].position.set(px, py);
		vertices[0].color = m_color;
		vertices[0].texCoord.set(c.texCoord0.x, c.texCoord1.y);
		
		vertices[1].position.set(px + c.size.x, py);
		vertices[1].color = m_color;
		vertices[1].texCoord.set(c.texCoord1.x, c.texCoord1.y);
		
		vertices[2].position.set(px + c.size.x, py + c.size.y);
		vertices[2].color = m_color;
		vertices[2].texCoord.set(c.texCoord1.x, c.texCoord0.y);
		
		vertices[3].position.set(px, py + c.size.y);
		vertices[3].color = m_color;
		vertices[3].texCoord.set(c.texCoord0.x, c.texCoord0.y);
		
		batch->addVertices(vertices, 4, QUAD_INDICES, 6);

		// Apply advance
		xOffset += c.advance;
	}
	batch->setTexture(0);
	batch->release();
}