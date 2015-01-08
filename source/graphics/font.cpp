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

BEGIN_XD_NAMESPACE

bool getFontFile(string &fontName)
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
			// Get name of Font
			string valstr = value;
			valstr = valstr.substr(0, valstr.find_first_of('(')-1);

			// Check if the value was found
			if(fontName == valstr)
			{
				// Set filename
				char *winDir = new char[MAX_PATH];
				GetWindowsDirectory(winDir, MAX_PATH);
				fontName = string(winDir) + "\\Fonts\\" + string(reinterpret_cast<char*>(data));

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

Font::Font(const string &filePath, const uint size) :
	m_color(0, 0, 0, 255),
	m_atlas(0),
	m_size(0),
	m_lineSize(0),
	m_Msize(0)
{
	// Load font from file
	//
	// Create and initialize a FreeType library
	FT_Library library;
	FT_Error error;
	if((error = FT_Init_FreeType(&library)) != 0)
	{
		LOG("Font::load - Failed to initialize FreeType 2 (error code: %i)", error);
		return;
	}

	// Load font information from file
	FT_Face face;
	if((error = FT_New_Face(library, filePath.c_str(), 0, &face)) != 0)
	{
		LOG("Font::load - Failed load font data (error code: %i)", error);
		return;
	}

	// Setup font characters
	m_metrics.resize(128);
	m_size = size;

	// Set characterset size
	//if(error = FT_Set_Char_Size(face, size << 6, size << 6, 96, 96))
	if((error = FT_Set_Pixel_Sizes(face, size, size)) != 0)
	{
		LOG("Font::load - Failed to set pixel size (error code: %i)", error);
		return;
	}

	// Load bitmap data for each of the character of the font
	vector<Pixmap> pixmaps; int maxY = 0; int minY = 0;
	for(uchar ch = 0; ch < 128; ch++)
	{
		// Load the glyph for our character
		if((error = FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) != 0)
		{
			LOG("Font::load - Failed to load glyph '%c' (error code: %i)", ch, error);
			return;
		}

		// Render glyph to bitmap
		if((error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) != 0)
		{
			LOG("Font::load - Failed to get glyph '%c' (error code: %i)", ch, error);
			return;
		}

		// Get bitmap object
		FT_Bitmap &bitmap = face->glyph->bitmap;

		// Get dimentions of glyph
		uint width = bitmap.width;
		uint height = bitmap.rows;

		// Allocate memory for texture data
		Pixmap pixmap(width, height);
		for(uint y = 0; y < height; y++)
		{ 
			for(uint x = 0; x < width; x++)
			{
				uchar c = bitmap.buffer[x + (height-y-1)*width];
				pixmap.setColor(x, y, Color(255, 255, 255, c));
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

		if(metrics.bearing.y > maxY) maxY = metrics.bearing.y;
		if(metrics.bearing.y - metrics.size.y < minY) minY = metrics.bearing.y - metrics.size.y;
	}
	m_lineSize = maxY - minY;
	m_Msize = m_metrics['M'].size.y;

	// Create Font atlas
	m_atlas = new xd::TextureAtlas(pixmaps);

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

Font::~Font()
{
	delete m_atlas;
}

FontPtr Font::loadResource(const string &name)
{
	// Get name and size
	uint size = util::strToInt(name.substr(name.find_last_of(' ') + 1));
	string filePath = name.substr(0, name.find_last_of(' '));

	// Check if we can find the font in the local directories
	util::toAbsoluteFilePath(filePath);
	if(!util::fileExists(filePath))
	{
		// Loop throught the registry to find the file by font name
		
		if(!getFontFile(filePath))
		{
			LOG("Font '%s' not found!", name);
			return FontPtr(0);
		}
	}

	return FontPtr(new Font(filePath, size));
}

float Font::getStringWidth(const string &str)
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

float Font::getStringHeight(const string &str)
{
	float height = (float)m_lineSize;
	for(uint i = 0; i < str.size(); i++)
	{
		if(str[i] == '\n')
			height += m_lineSize;
	}
	return height;
}

void Font::setColor(const Color &color)
{
	m_color = color;
}

void Font::draw(SpriteBatch *batch, const Vector2 &pos, const string &str) const
{
	// Get current position
	Vector2 currentPos = pos;

	// Draw string
	Sprite sprite(m_atlas->getTexture());
	sprite.setColor(m_color);
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
		sprite.setRegion(m_atlas->get(ch));
		sprite.setPosition(currentPos.x, currentPos.y + (m_Msize - metrics.size.y - metrics.advance.y));
		sprite.setSize(metrics.size.x, metrics.size.y);
		batch->drawSprite(sprite);

		// Apply advance
		currentPos.x += metrics.advance.x - metrics.bearing.x;
	}
}

xd::Texture2DPtr Font::renderToTexture(GraphicsContext &graphicsContext, const string &text, const uint padding)
{
	// Setup render target
	RenderTarget2D renderTarget((uint)ceil(getStringWidth(text)) + padding, (uint)ceil(getStringHeight(text)) + padding, 1);
	graphicsContext.setRenderTarget(&renderTarget);
	graphicsContext.disable(GraphicsContext::BLEND);

	SpriteBatch spriteBatch(graphicsContext);
	spriteBatch.begin();
	draw(&spriteBatch, Vector2(padding*0.5f, padding*0.5f), text);
	spriteBatch.end();

	graphicsContext.setRenderTarget(nullptr);

	return renderTarget.getTexture();
}

END_XD_NAMESPACE