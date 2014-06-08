//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "batch.h"
#include "texture.h"

#include <x2d/graphics.h>

AS_REG_REF(Batch)

int Batch::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Batch", asBEHAVE_FACTORY, "Batch @f()", asFUNCTIONPR(Factory, (), Batch*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Batch", "void draw()", asMETHOD(Batch, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

Batch::Batch() :
	refCounter(this),
	m_texture(0)
{
}

void Batch::setTexture(class Texture *texture)
{
	m_texture = texture;
}

//void Batch::addVertex(Vertex vertex, int index)
//{
//	m_buffers[m_texture].vertices.push_back(vertex);
//	m_buffers[m_texture].indices.push_back(index);
//}

void Batch::addVertices(Vertex *vertices, int vertcount, uint *indices, int icount)
{
	Buffer &buffer = m_buffers[m_texture];
	int ioffset = buffer.vertices.size();
	
	for(int i = 0; i < vertcount; i++) {
		buffer.vertices.push_back(vertices[i]);
	}
	
	for(int i = 0; i < icount; i++) {
		buffer.indices.push_back(indices[i] + ioffset);
	}
}

void Batch::draw()
{
	xdGraphics::s_this->renderBatch(*this);
	m_buffers.clear();
}