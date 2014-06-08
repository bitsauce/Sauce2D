//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "font.h"

AS_REG_REF(Font)

int Font::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Font", asBEHAVE_FACTORY, "Font @f(const string &in)", asFUNCTIONPR(Factory, (const string&), Font*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Font", "float getStringWidth(const string &in)", asMETHOD(Font, getStringWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Font", "float getStringHeight(const string &in)", asMETHOD(Font, getStringHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Font", "void setColor(const Vector4 &in)", asMETHOD(Font, setColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Font", "void draw(Batch @batch, const Vector2 &in, const string &in)", asMETHOD(Font, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

Font::Font(const string &filePathOrFontName) :
	refCounter(this),
	m_color(0.0f)
{
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

void Font::draw(Batch &batch, const Vector2 &pos, const string &str)
{
	Vertex vert;
	vert.color = m_color;
	vector<Vertex> vertices;
	vector<uint> indices;

	// Add space between lines
	float h = m_size/0.63f;
	float yOffset = 0.0f;
	float xOffset = 0.0f;

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

		vert.texCoord.set(c.texCoord0.x, c.texCoord1.y);
		vert.position.set(px, py);
		vertices.push_back(vert);

		vert.texCoord.set(c.texCoord0.x, c.texCoord0.y);
		vert.position.set(px, py + c.size.y);
		vertices.push_back(vert);

		vert.texCoord.set(c.texCoord1.x, c.texCoord0.y);
		vert.position.set(px + c.size.x, py + c.size.y);
		vertices.push_back(vert);

		vert.texCoord.set(c.texCoord1.x, c.texCoord1.y);
		vert.position.set(px + c.size.x, py);
		vertices.push_back(vert);

		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(1);
		
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);

		// Apply advance
		xOffset += c.advance;
	}
	
	batch.setTexture(m_texture);
	batch.addVertices(vertices.data(), vertices.size(), indices.data(), indices.size());
	batch.setTexture(0);
}