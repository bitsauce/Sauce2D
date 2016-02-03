#include <CGF/graphics.h>

BEGIN_CGF_NAMESPACE

class FontLoader
{
public:
	FontLoader(FileReader*, Font *font, const string &);

	virtual int Load() = 0; // Must be implemented by derived class

protected:
	void loadPage(int id, const char *pageFile, string fontFile);
	void SetFontInfo(int outlineThickness);
	void setCommonInfo(short fontHeight, short base, short scaleW, short scaleH, int pages, bool isPacked);
	void addChar(int id, short x, short y, short w, short h, short xoffset, short yoffset, short xadvance, short page, int chnl);
	void AddKerningPair(int first, int second, int amount);

	FileReader *m_file;
	Font *m_font;
	string m_fontFile;

	int m_outlineThickness;
};

class FontLoaderTextFormat : public FontLoader
{
public:
	FontLoaderTextFormat(FileReader*, Font *font, const string &fontFile);

	int Load();

	int SkipWhiteSpace(std::string &str, int start);
	int FindEndOfToken(std::string &str, int start);

	void InterpretInfo(std::string &str, int start);
	void InterpretCommon(std::string &str, int start);
	void InterpretChar(std::string &str, int start);
	void InterpretSpacing(std::string &str, int start);
	void InterpretKerning(std::string &str, int start);
	void InterpretPage(std::string &str, int start, const string &fontFile);
};

class FontLoaderBinaryFormat : public FontLoader
{
public:
	FontLoaderBinaryFormat(FileReader*, Font *font, const string &fontFile);

	int Load();

	void ReadInfoBlock(int size);
	void ReadCommonBlock(int size);
	void ReadPagesBlock(int size);
	void ReadCharsBlock(int size);
	void ReadKerningPairsBlock(int size);
};

//=============================================================================
// Font
//
// This is the Font class that is used to write text with bitmap fonts.
//=============================================================================

Font::Font(ResourceDesc *desc)
{
	m_fontHeight = 0;
	m_base = 0;
	m_scaleW = 0;
	m_scaleH = 0;
	m_scale = 1.0f;
	m_hasOutline = false;
	m_encoding = NONE;
	m_color = Color(255, 255, 255, 255);
	m_depth = 0.0f;

	// Load the font
	FileReader *file = new FileReader(desc->getPath());
	if(file->isOpen())
	{
		// Determine format by reading the first bytes of the file
		char fmt[3];
		file->readBytes(fmt, 3);

		FontLoader *loader = 0;
		if(strcmp(fmt, "BMF") == 0)
		{
			loader = new FontLoaderBinaryFormat(file, this, desc->getPath());
		}
		else
		{
			loader = new FontLoaderTextFormat(file, this, desc->getPath());
		}

		loader->Load();
		delete loader;
	}
}

Font::~Font()
{
	std::map<int, CharDescr*>::iterator it = m_chars.begin();
	while (it != m_chars.end())
	{
		delete it->second;
		it++;
	}
	m_pages.clear();
}

void Font::setTextEncoding(FontTextEncoding encoding)
{
	this->m_encoding = encoding;
}

// Internal
CharDescr *Font::getChar(int id)
{
	map<int, CharDescr*>::iterator it = m_chars.find(id);
	if(it == m_chars.end()) return 0;
	return it->second; 
}

// Internal
float Font::adjustForKerningPairs(int first, int second)
{
	CharDescr *ch = getChar(first);
	if (ch == 0) return 0;
	for (UINT n = 0; n < ch->kerningPairs.size(); n += 2)
	{
		if (ch->kerningPairs[n] == second)
			return ch->kerningPairs[n + 1] * m_scale;
	}

	return 0;
}

float Font::getStringWidth(const string &text, int count)
{
	if (count <= 0) {
		count = getTextLength(text);
	}

	float x = 0;

	for (int n = 0; n < count; )
	{
		int charId = getTextChar(text, n, &n);

		CharDescr *ch = getChar(charId);
		if (ch == 0) ch = &m_defChar;

		x += m_scale * (ch->xAdv);

		if (n < count)
			x += adjustForKerningPairs(charId, getTextChar(text, n));
	}

	return x;
}

float Font::getStringHeight(const string &text)
{
	float height = m_scale * float(m_fontHeight);
	for (char c : text)
	{
		if (c == '\n') {
			height += m_scale * float(m_fontHeight);
		}
	}
	return height;
}

void Font::setHeight(float h)
{
	m_scale = h / float(m_fontHeight);
}

float Font::getHeight() const
{
	return m_scale * float(m_fontHeight);
}

float Font::getBottomOffset() const
{
	return m_scale * (m_base - m_fontHeight);
}

float Font::getTopOffset() const
{
	return m_scale * (m_base - 0);
}

// Internal
// Returns the number of bytes in the string until the null char
int Font::getTextLength(const string &text)
{
	if (m_encoding == UTF16)
	{
		int textLen = 0;
		while (true)
		{
			unsigned int len;
			int r = util::decodeUTF16(&text[textLen], &len);
			if (r > 0)
				textLen += len;
			else if (r < 0)
				textLen++;
			else
				return textLen;
		}
	}

	// Both UTF8 and standard ASCII strings can use strlen
	return text.size();
}

// Internal
int Font::getTextChar(const string &text, int pos, int *nextPos)
{
	int ch;
	unsigned int len;
	if (m_encoding == UTF8)
	{
		ch = util::decodeUTF8(&text[pos], &len);
		if (ch == -1) len = 1;
	}
	else if (m_encoding == UTF16)
	{
		ch = util::decodeUTF16(&text[pos], &len);
		if (ch == -1) len = 2;
	}
	else
	{
		len = 1;
		ch = (unsigned char)text[pos];
	}

	if (nextPos) *nextPos = pos + len;
	return ch;
}

// Internal
int Font::findTextChar(const char *text, int start, int length, int ch)
{
	int pos = start;
	int nextPos;
	int currChar = -1;
	while (pos < length)
	{
		currChar = getTextChar(text, pos, &nextPos);
		if (currChar == ch)
			return pos;
		pos = nextPos;
	}

	return -1;
}

void Font::drawInternal(SpriteBatch *spriteBatch, float x, float y, const string &text, int count, float spacing)
{
	int page = -1;

	Sprite sprite(m_pages[0]);
	sprite.setColor(m_color);
	sprite.setDepth(m_depth);
	for (int n = 0; n < count; )
	{
		int charId = getTextChar(text, n, &n);
		CharDescr *ch = getChar(charId);
		if (ch == 0) ch = &m_defChar;

		// Map the center of the texel to the corners
		// in order to get pixel perfect mapping
		float u = float(ch->srcX) / m_scaleW;
		float v = float(ch->srcY) / m_scaleH;
		float u2 = u + float(ch->srcW) / m_scaleW;
		float v2 = v + float(ch->srcH) / m_scaleH;

		float a = m_scale * float(ch->xAdv);
		float w = m_scale * float(ch->srcW);
		float h = m_scale * float(ch->srcH);
		float ox = m_scale * float(ch->xOff);
		float oy = m_scale * float(ch->yOff);

		sprite.setRegion(TextureRegion(u, v, u2, v2));
		sprite.setPosition(x + ox, y + oy);
		sprite.setSize(w, h);

		if (ch->page != page)
		{
			page = ch->page;
			sprite.setTexture(m_pages[page]);
		}

		spriteBatch->drawSprite(sprite);

		x += a;
		if (charId == ' ')
			x += spacing;

		if (n < count)
			x += adjustForKerningPairs(charId, getTextChar(text, n));
	}
}

void Font::draw(SpriteBatch *spriteBatch, float x, float y, const string &text, FontAlign mode)
{
	// Get character count
	vector<string> lines = util::splitString(text, "\n");
	for (string line : lines)
	{
		int count = getTextLength(line);
		float drawX = x;
		if (mode == FONT_ALIGN_CENTER)
		{
			float w = getStringWidth(line, count);
			drawX -= w * 0.5f;
		}
		else if (mode == FONT_ALIGN_RIGHT)
		{
			float w = getStringWidth(line, count);
			drawX -= w;
		}

		drawInternal(spriteBatch, drawX, y, line, count);
		y += m_scale * float(m_fontHeight);
	}
}

void Font::drawBox(SpriteBatch *spriteBatch, float x, float y, float width, const string &text, int count, FontAlign mode)
{
	if (count <= 0) {
		count = getTextLength(text);
	}

	float currWidth = 0, wordWidth;
	int lineStart = 0, lineEnd = 0, wordStart = 0, wordEnd = 0;
	int wordCount = 0;

	const char *s = " ";
	float spaceWidth = getStringWidth(s, 1);

	enum { NONE, SOFT, HARD } breakType;
	breakType = NONE;

	for (; lineStart < count;)
	{
		// Determine the extent of the line
		for (;;)
		{
			// Determine the number of characters in the word
			while(wordEnd < count &&
				getTextChar(text, wordEnd) != ' ' &&
				getTextChar(text, wordEnd) != '\n')
			{
				// Advance the cursor to the next character
				getTextChar(text, wordEnd, &wordEnd);
			}

			// Determine the width of the word
			if (wordEnd > wordStart)
			{
				wordCount++;
				wordWidth = getStringWidth(&text[wordStart], wordEnd - wordStart);
			}
			else
			{
				wordWidth = 0;
			}

			// If the word is longer than the width of our box, we need hard line breaks
			if(wordWidth > width)
			{
				// Find out where the word needs to be broken
				int linePos = lineStart + 1;
				while(getStringWidth(&text[lineStart], linePos++ - lineStart) < width);

				// Set line and word
				lineEnd = wordEnd = linePos - 3;

				// Set break type
				breakType = HARD;
				break;
			}
			// Does the word fit on the line? The first word is always accepted.
			else if (wordCount == 1 || currWidth + (wordCount > 1 ? spaceWidth : 0) + wordWidth <= width)
			{
				// Increase the line extent to the end of the word
				lineEnd = wordEnd;
				currWidth += (wordCount > 1 ? spaceWidth : 0) + wordWidth;

				// Did we reach the end of the line?
				if (wordEnd == count || getTextChar(text, wordEnd) == '\n')
				{
					breakType = NONE;

					// Skip the newline character
					if (wordEnd < count)
						// Advance the cursor to the next character
						getTextChar(text, wordEnd, &wordEnd);
					break;
				}

				// Skip the trailing space
				if (wordEnd < count && getTextChar(text, wordEnd) == ' ')
					// Advance the cursor to the next character
					getTextChar(text, wordEnd, &wordEnd);
				// Move to next word
				wordStart = wordEnd;
			}
			else
			{
				breakType = SOFT;

				// Skip the trailing space
				if (wordEnd < count && getTextChar(text, wordEnd) == ' ')
					// Advance the cursor to the next character
					getTextChar(text, wordEnd, &wordEnd);
				break;
			}
		}

		// Write the line
		if (mode == FONT_ALIGN_JUSTIFY)
		{
			float spacing = 0.0f;
			if (breakType == SOFT)
			{
				if (wordCount > 2)
					spacing = (width - currWidth) / (wordCount - 2);
				else
					spacing = (width - currWidth);
			}
			drawInternal(spriteBatch, x, y, &text[lineStart], lineEnd - lineStart, spacing);
		}
		else
		{
			float cx = x;
			if (mode == FONT_ALIGN_RIGHT)
				cx = x + width - currWidth;
			else if (mode == FONT_ALIGN_CENTER)
				cx = x + 0.5f * (width - currWidth);
			drawInternal(spriteBatch, x, y, &text[lineStart], lineEnd - lineStart);
		}

		switch(breakType)
		{
			case NONE:
			{
				// Skip the line break
				if(lineEnd < count && getTextChar(text, lineEnd) == '\n')
				{
					// Advance the cursor to the next character
					getTextChar(text, lineEnd, &lineEnd);
				}
				currWidth = 0;
				wordCount = 0;
			}
			break;

			case SOFT:
			{
				// Skip the trailing space
				if(lineEnd < count && getTextChar(text, lineEnd) == ' ')
				{
					// Advance the cursor to the next character
					getTextChar(text, lineEnd, &lineEnd);
				}

				// We've already counted the first word on the next line
				currWidth = wordWidth;
				wordCount = 1;
			}
			break;

			case HARD:
			{
				// Add a hyphen at the end of the line when we enounter a hard line break
				drawInternal(spriteBatch, x + getStringWidth(&text[lineStart], lineEnd - lineStart), y, "-", 1);

				// Set width and word count to 0 since we're starting anew on the next line
				currWidth = 0;
				wordCount = 0;
				break;
			}
		}

		// Move to next line
		lineStart = lineEnd;
		wordStart = wordEnd;
		y += m_scale * float(m_fontHeight);
	}
}

//=============================================================================
// FontLoader
//
// This is the base class for all loader classes. This is the only class
// that has access to and knows how to set the Font members.
//=============================================================================

FontLoader::FontLoader(FileReader *f, Font *font, const string &fontFile)
{
	this->m_file = f;
	this->m_font = font;
	this->m_fontFile = fontFile;

	m_outlineThickness = 0;
}

void FontLoader::loadPage(int id, const char *pageFile, string fontFile)
{
	// Load the texture from the same directory as the font descriptor file

	// Find the directory
	for (size_t n = 0; (n = fontFile.find('/', n)) != string::npos; ) fontFile.replace(n, 1, "\\");
	size_t i = fontFile.rfind('\\');
	if (i != string::npos)
		fontFile = fontFile.substr(0, i + 1);
	else
		fontFile = "";

	// Load the font textures
	fontFile += pageFile;

	m_font->m_pages[id] = Resource<Texture2D>(new Texture2D(Pixmap(fontFile)));
}

void FontLoader::SetFontInfo(int outlineThickness)
{
	m_outlineThickness = outlineThickness;
}

void FontLoader::setCommonInfo(short fontHeight, short base, short scaleW, short scaleH, int pages, bool isPacked)
{
	m_font->m_fontHeight = fontHeight;
	m_font->m_base = base;
	m_font->m_scaleW = scaleW;
	m_font->m_scaleH = scaleH;
	m_font->m_pages.resize(pages);
	for (int n = 0; n < pages; n++)
		m_font->m_pages[n] = 0;

	if (isPacked && m_outlineThickness)
		m_font->m_hasOutline = true;
}

void FontLoader::addChar(int id, short x, short y, short w, short h, short xoffset, short yoffset, short xadvance, short page, int chnl)
{
	// Convert to a 4 element vector
	// TODO: Does this depend on hardware? It probably does
	if (chnl == 1) chnl = 0x00010000;  // Blue channel
	else if (chnl == 2) chnl = 0x00000100;  // Green channel
	else if (chnl == 4) chnl = 0x00000001;  // Red channel
	else if (chnl == 8) chnl = 0x01000000;  // Alpha channel
	else chnl = 0;

	if (id >= 0)
	{
		CharDescr *ch = new CharDescr;
		ch->srcX = x;
		ch->srcY = y;
		ch->srcW = w;
		ch->srcH = h;
		ch->xOff = xoffset;
		ch->yOff = yoffset;
		ch->xAdv = xadvance;
		ch->page = page;
		ch->chnl = chnl;

		m_font->m_chars.insert(std::map<int, CharDescr*>::value_type(id, ch));
	}
	else if (id == -1)
	{
		m_font->m_defChar.srcX = x;
		m_font->m_defChar.srcY = y;
		m_font->m_defChar.srcW = w;
		m_font->m_defChar.srcH = h;
		m_font->m_defChar.xOff = xoffset;
		m_font->m_defChar.yOff = yoffset;
		m_font->m_defChar.xAdv = xadvance;
		m_font->m_defChar.page = page;
		m_font->m_defChar.chnl = chnl;
	}
}

void FontLoader::AddKerningPair(int first, int second, int amount)
{
	if (first >= 0 && first < 256 && m_font->m_chars[first])
	{
		m_font->m_chars[first]->kerningPairs.push_back(second);
		m_font->m_chars[first]->kerningPairs.push_back(amount);
	}
}

//=============================================================================
// FontLoaderTextFormat
//
// This class implements the logic for loading a BMFont file in text format
//=============================================================================

FontLoaderTextFormat::FontLoaderTextFormat(FileReader *f, Font *font, const string &fontFile) : FontLoader(f, font, fontFile)
{
}

int FontLoaderTextFormat::Load()
{
	string line;

	while (!m_file->isEOF())
	{
		// Read until line feed (or EOF)
		line = "";
		line.reserve(256);
		while (!m_file->isEOF())
		{
			char ch;
			if (m_file->readBytes(&ch, 1))
			{
				if (ch != '\n')
					line += ch;
				else
					break;
			}
		}

		// Skip white spaces
		int pos = SkipWhiteSpace(line, 0);

		// Read token
		int pos2 = FindEndOfToken(line, pos);
		string token = line.substr(pos, pos2 - pos);

		// Interpret line
		if (token == "info")
			InterpretInfo(line, pos2);
		else if (token == "common")
			InterpretCommon(line, pos2);
		else if (token == "char")
			InterpretChar(line, pos2);
		else if (token == "kerning")
			InterpretKerning(line, pos2);
		else if (token == "page")
			InterpretPage(line, pos2, m_fontFile);
	}

	m_file->close();
	delete m_file;
	m_file = 0;

	// Success
	return 0;
}

int FontLoaderTextFormat::SkipWhiteSpace(string &str, int start)
{
	UINT n = start;
	while (n < str.size())
	{
		char ch = str[n];
		if (ch != ' ' &&
			ch != '\t' &&
			ch != '\r' &&
			ch != '\n')
			break;

		++n;
	}

	return n;
}

int FontLoaderTextFormat::FindEndOfToken(string &str, int start)
{
	UINT n = start;
	if (str[n] == '"')
	{
		n++;
		while (n < str.size())
		{
			char ch = str[n];
			if (ch == '"')
			{
				// Include the last quote char in the token
				++n;
				break;
			}
			++n;
		}
	}
	else
	{
		while (n < str.size())
		{
			char ch = str[n];
			if (ch == ' ' ||
				ch == '\t' ||
				ch == '\r' ||
				ch == '\n' ||
				ch == '=')
				break;

			++n;
		}
	}

	return n;
}

void FontLoaderTextFormat::InterpretKerning(string &str, int start)
{
	// Read the attributes
	int first = 0;
	int second = 0;
	int amount = 0;

	int pos, pos2 = start;
	while (true)
	{
		pos = SkipWhiteSpace(str, pos2);
		pos2 = FindEndOfToken(str, pos);

		string token = str.substr(pos, pos2 - pos);

		pos = SkipWhiteSpace(str, pos2);
		if ((uint)pos == str.size() || str[pos] != '=') break;

		pos = SkipWhiteSpace(str, pos + 1);
		pos2 = FindEndOfToken(str, pos);

		string value = str.substr(pos, pos2 - pos);

		if (token == "first")
			first = strtol(value.c_str(), 0, 10);
		else if (token == "second")
			second = strtol(value.c_str(), 0, 10);
		else if (token == "amount")
			amount = strtol(value.c_str(), 0, 10);

		if ((uint)pos == str.size()) break;
	}

	// Store the attributes
	AddKerningPair(first, second, amount);
}

void FontLoaderTextFormat::InterpretChar(string &str, int start)
{
	// Read all attributes
	int id = 0;
	short x = 0;
	short y = 0;
	short width = 0;
	short height = 0;
	short xoffset = 0;
	short yoffset = 0;
	short xadvance = 0;
	short page = 0;
	int chnl = 0;

	int pos, pos2 = start;
	while (true)
	{
		pos = SkipWhiteSpace(str, pos2);
		pos2 = FindEndOfToken(str, pos);

		string token = str.substr(pos, pos2 - pos);

		pos = SkipWhiteSpace(str, pos2);
		if ((uint)pos == str.size() || str[pos] != '=') break;

		pos = SkipWhiteSpace(str, pos + 1);
		pos2 = FindEndOfToken(str, pos);

		string value = str.substr(pos, pos2 - pos);

		if (token == "id")
			id = strtol(value.c_str(), 0, 10);
		else if (token == "x")
			x = (short)strtol(value.c_str(), 0, 10);
		else if (token == "y")
			y = (short)strtol(value.c_str(), 0, 10);
		else if (token == "width")
			width = (short)strtol(value.c_str(), 0, 10);
		else if (token == "height")
			height = (short)strtol(value.c_str(), 0, 10);
		else if (token == "xoffset")
			xoffset = (short)strtol(value.c_str(), 0, 10);
		else if (token == "yoffset")
			yoffset = (short)strtol(value.c_str(), 0, 10);
		else if (token == "xadvance")
			xadvance = (short)strtol(value.c_str(), 0, 10);
		else if (token == "page")
			page = (short)strtol(value.c_str(), 0, 10);
		else if (token == "chnl")
			chnl = strtol(value.c_str(), 0, 10);

		if ((uint)pos == str.size()) break;
	}

	// Store the attributes
	addChar(id, x, y, width, height, xoffset, yoffset, xadvance, page, chnl);
}

void FontLoaderTextFormat::InterpretCommon(string &str, int start)
{
	short fontHeight = 0;
	short base = 0;
	short scaleW = 0;
	short scaleH = 0;
	int pages = 0;
	int packed = 0;

	// Read all attributes
	int pos, pos2 = start;
	while (true)
	{
		pos = SkipWhiteSpace(str, pos2);
		pos2 = FindEndOfToken(str, pos);

		string token = str.substr(pos, pos2 - pos);

		pos = SkipWhiteSpace(str, pos2);
		if ((uint)pos == str.size() || str[pos] != '=') break;

		pos = SkipWhiteSpace(str, pos + 1);
		pos2 = FindEndOfToken(str, pos);

		string value = str.substr(pos, pos2 - pos);

		if (token == "lineHeight")
			fontHeight = (short)strtol(value.c_str(), 0, 10);
		else if (token == "base")
			base = (short)strtol(value.c_str(), 0, 10);
		else if (token == "scaleW")
			scaleW = (short)strtol(value.c_str(), 0, 10);
		else if (token == "scaleH")
			scaleH = (short)strtol(value.c_str(), 0, 10);
		else if (token == "pages")
			pages = strtol(value.c_str(), 0, 10);
		else if (token == "packed")
			packed = strtol(value.c_str(), 0, 10);

		if ((uint)pos == str.size()) break;
	}

	setCommonInfo(fontHeight, base, scaleW, scaleH, pages, packed ? true : false);
}

void FontLoaderTextFormat::InterpretInfo(string &str, int start)
{
	int outlineThickness = 0;

	// Read all attributes
	int pos, pos2 = start;
	while (true)
	{
		pos = SkipWhiteSpace(str, pos2);
		pos2 = FindEndOfToken(str, pos);

		string token = str.substr(pos, pos2 - pos);

		pos = SkipWhiteSpace(str, pos2);
		if ((uint)pos == str.size() || str[pos] != '=') break;

		pos = SkipWhiteSpace(str, pos + 1);
		pos2 = FindEndOfToken(str, pos);

		string value = str.substr(pos, pos2 - pos);

		if (token == "outline")
			outlineThickness = (short)strtol(value.c_str(), 0, 10);

		if ((uint)pos == str.size()) break;
	}

	SetFontInfo(outlineThickness);
}

void FontLoaderTextFormat::InterpretPage(string &str, int start, const string &fontFile)
{
	int id = 0;
	string file;

	// Read all attributes
	int pos, pos2 = start;
	while (true)
	{
		pos = SkipWhiteSpace(str, pos2);
		pos2 = FindEndOfToken(str, pos);

		string token = str.substr(pos, pos2 - pos);

		pos = SkipWhiteSpace(str, pos2);
		if ((uint)pos == str.size() || str[pos] != '=') break;

		pos = SkipWhiteSpace(str, pos + 1);
		pos2 = FindEndOfToken(str, pos);

		string value = str.substr(pos, pos2 - pos);

		if (token == "id")
			id = strtol(value.c_str(), 0, 10);
		else if (token == "file")
			file = value.substr(1, value.length() - 2);

		if ((uint)pos == str.size()) break;
	}

	loadPage(id, file.c_str(), fontFile);
}

//=============================================================================
// FontLoaderBinaryFormat
//
// This class implements the logic for loading a BMFont file in binary format
//=============================================================================

FontLoaderBinaryFormat::FontLoaderBinaryFormat(FileReader *f, Font *font, const string &fontFile) : FontLoader(f, font, fontFile)
{
}

int FontLoaderBinaryFormat::Load()
{
	// Read and validate the tag. It should be 66, 77, 70, 2, 
	// or 'BMF' and 2 where the number is the file version.
	char magicString[4];
	m_file->readBytes(magicString, 4);
	if (strcmp(magicString, "BMF\003") != 0)
	{
		LOG("Unrecognized format for '%s'", m_fontFile);
		
		m_file->close();
		delete m_file;
		m_file = 0;

		return -1;
	}

	// Read each block
	char blockType;
	int blockSize;
	while (m_file->readBytes(&blockType, 1))
	{
		// Read the blockSize
		m_file->readBytes((char*)&blockSize, 4);

		switch (blockType)
		{
		case 1: // info
			ReadInfoBlock(blockSize);
			break;
		case 2: // common
			ReadCommonBlock(blockSize);
			break;
		case 3: // pages
			ReadPagesBlock(blockSize);
			break;
		case 4: // chars
			ReadCharsBlock(blockSize);
			break;
		case 5: // kerning pairs
			ReadKerningPairsBlock(blockSize);
			break;
		default:
			LOG("Unexpected block type (%d)", blockType);

			m_file->close();
			delete m_file;
			m_file = 0;

			return -1;
		}
	}

	m_file->close();
	delete m_file;
	m_file = 0;

	// Success
	return 0;
}

void FontLoaderBinaryFormat::ReadInfoBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
	struct infoBlock
	{
		WORD fontSize;
		BYTE reserved : 4;
		BYTE bold : 1;
		BYTE italic : 1;
		BYTE unicode : 1;
		BYTE smooth : 1;
		BYTE charSet;
		WORD stretchH;
		BYTE aa;
		BYTE paddingUp;
		BYTE paddingRight;
		BYTE paddingDown;
		BYTE paddingLeft;
		BYTE spacingHoriz;
		BYTE spacingVert;
		BYTE outline;         // Added with version 2
		char fontName[1];
	};
#pragma pack(pop)

	infoBlock blk;
	m_file->readBytes((char*)&blk, size);

	// We're only interested in the outline thickness
	SetFontInfo(blk.outline);
}

void FontLoaderBinaryFormat::ReadCommonBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
	struct commonBlock
	{
		WORD lineHeight;
		WORD base;
		WORD scaleW;
		WORD scaleH;
		WORD pages;
		BYTE packed : 1;
		BYTE reserved : 7;
		BYTE alphaChnl;
		BYTE redChnl;
		BYTE greenChnl;
		BYTE blueChnl;
	};
#pragma pack(pop)

	commonBlock blk;
	m_file->readBytes((char*)&blk, size);

	setCommonInfo(blk.lineHeight, blk.base, blk.scaleW, blk.scaleH, blk.pages, blk.packed ? true : false);
}

void FontLoaderBinaryFormat::ReadPagesBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
	struct pagesBlock
	{
		char pageNames[1];
	};
#pragma pack(pop)

	pagesBlock blk;
	m_file->readBytes((char*)&blk, size);

	for (int id = 0, pos = 0; pos < size; id++)
	{
		loadPage(id, &blk.pageNames[pos], m_fontFile);
		pos += 1 + (int)strlen(&blk.pageNames[pos]);
	}
}

void FontLoaderBinaryFormat::ReadCharsBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
	struct charsBlock
	{
		struct charInfo
		{
			DWORD id;
			WORD  x;
			WORD  y;
			WORD  width;
			WORD  height;
			short xoffset;
			short yoffset;
			short xadvance;
			BYTE  page;
			BYTE  chnl;
		} chars[1];
	};
#pragma pack(pop)

	charsBlock blk;
	m_file->readBytes((char*)&blk, size);

	for (int n = 0; int(n*sizeof(charsBlock::charInfo)) < size; n++)
	{
		addChar(blk.chars[n].id,
			blk.chars[n].x,
			blk.chars[n].y,
			blk.chars[n].width,
			blk.chars[n].height,
			blk.chars[n].xoffset,
			blk.chars[n].yoffset,
			blk.chars[n].xadvance,
			blk.chars[n].page,
			blk.chars[n].chnl);
	}
}

void FontLoaderBinaryFormat::ReadKerningPairsBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
	struct kerningPairsBlock
	{
		struct kerningPair
		{
			DWORD first;
			DWORD second;
			short amount;
		} kerningPairs[1];
	};
#pragma pack(pop)

	kerningPairsBlock blk;
	m_file->readBytes((char*)&blk, size);

	for (int n = 0; int(n*sizeof(kerningPairsBlock::kerningPair)) < size; n++)
	{
		AddKerningPair(blk.kerningPairs[n].first,
			blk.kerningPairs[n].second,
			blk.kerningPairs[n].amount);
	}
}

END_CGF_NAMESPACE
