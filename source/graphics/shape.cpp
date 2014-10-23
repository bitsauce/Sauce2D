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

AS_REG_REF(XShape, "Shape")

int XShape::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f()", asFUNCTIONPR(Factory, (), XShape*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f(const Rect &in)", asFUNCTIONPR(Factory, (const Rect&), XShape*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f(const Vector2 &in, const float, const int)", asFUNCTIONPR(Factory, (const Vector2&, const float, const int), XShape*), asCALL_CDECL); AS_ASSERT
	//r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f(const array<Vector2> &in)", asFUNCTIONPR(Factory, (const XScriptArray&), XShape*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Shape", "void setFillColor(const Vector4 &in)", asMETHOD(XShape, setFillColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shape", "void setFillTexture(Texture @texture)", asMETHOD(XShape, setFillTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Shape", "void draw(Batch @batch)", asMETHOD(XShape, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

XShape::XShape() :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
}

XShape::XShape(const Rect &rect) :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
	m_vertices = new XVertex[4]; 

	m_vertices[0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	m_vertices[1].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	m_vertices[2].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	m_vertices[3].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	
	m_vertices[0].set4f(VERTEX_POSITION, rect.getLeft(), rect.getTop());
	m_vertices[1].set4f(VERTEX_POSITION, rect.getRight(), rect.getTop());
	m_vertices[2].set4f(VERTEX_POSITION, rect.getRight(), rect.getBottom());
	m_vertices[3].set4f(VERTEX_POSITION, rect.getLeft(), rect.getBottom());
	
	m_vertices[0].set4f(VERTEX_TEX_COORD, 0.0f, 1.0f);
	m_vertices[1].set4f(VERTEX_TEX_COORD, 1.0f, 1.0f);
	m_vertices[2].set4f(VERTEX_TEX_COORD, 1.0f, 0.0f);
	m_vertices[3].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
	
	m_indices.push_back(0);
	m_indices.push_back(3);
	m_indices.push_back(1);
	
	m_indices.push_back(1);
	m_indices.push_back(3);
	m_indices.push_back(2);

	m_index += 6;
	m_vertCount = 4;
}

XShape::XShape(const Vector2 &center, const float radius, const int vertCount) :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
	m_vertices = new XVertex[vertCount];
	
	m_vertices[0].set4f(VERTEX_POSITION, center.x, center.y);
	m_vertices[0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	m_vertices[0].set4f(VERTEX_TEX_COORD, 0.5f, 0.5f);

	for(int i = 0; i < vertCount; i++)
	{
		float r = (2.0f*PI*i)/(vertCount-1);
		m_vertices[i].set4f(VERTEX_POSITION, center.x + cos(r)*radius, center.y + sin(r)*radius);
		m_vertices[i].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
		m_vertices[i].set4f(VERTEX_TEX_COORD, (1 + cos(r))/2.0f, 1.0f - (1 + sin(r))/2.0f);

		if(i > 0)
		{
			m_indices.push_back(0);
			m_indices.push_back(i < vertCount ? i+1 : 1);
			m_indices.push_back(i);
		}
	}

	m_index += vertCount * 3;
	m_vertCount = vertCount;
}

XShape::XShape(const XVertex *vertices, const int vertCount) :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
	// TODO: implement
	//memcpy(m_vertices, vertices, vertCount * sizeof(XVertex));
	//m_index = vertCount;
}

XShape::~XShape()
{
	if(m_fillTexture) {
		m_fillTexture->release();
	}
	delete[] m_vertices;
}

/*void XShape::addVertex(const Vertex &vertex)
{
	m_vertices.push_back(vertex);
	if(++m_index > 2) {
		m_indices.push_back(m_index-3);
		m_indices.push_back(m_index-2);
		m_indices.push_back(m_index);
	}
}

void XShape::addVertices(const vector<Vertex> &vertices)
{
	//if(m_vertices.size() % 3 == 0) LOG("Somethings wrong");

	for(uint i = 0; i < vertices.size(); i++) {
		addVertex(vertices[i]);
	}
}*/

void XShape::setFillColor(const Vector4 &color)
{
	m_fillColor = color;
}

void XShape::setFillTexture(XTexture* texture)
{
	if(m_fillTexture) {
		m_fillTexture->release();
	}
	m_fillTexture = texture;
}

void XShape::setPenColor(const Vector4 &color)
{
	m_penColor = color;
}

void XShape::setPenSize(const float size)
{
	m_penSize = size;
}

void XShape::draw(XBatch *batch)
{
	if(!validate())
		return;

	if(!batch)
	{
		LOG("void XShape::draw(): Can not draw to 'null' batch");
		return;
	}

	// Set vertex colors
	for(uint i = 0; i < m_vertCount; i++)
	{
		m_vertices[i].set4ub(VERTEX_COLOR, m_fillColor.x*255, m_fillColor.y*255, m_fillColor.z*255, m_fillColor.w*255);
	}
	
	if(m_fillTexture)
	{
		m_fillTexture->addRef();
	}
	batch->setTexture(m_fillTexture);
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
	batch->addVertices(m_vertices, m_vertCount, m_indices.data(), m_indices.size());
	batch->release();
}

bool XShape::validate()
{
	return true;
}

XShape XShape::merge(const XShape &XShape)
{
	// TODO
	return *this;
}

XShape XShape::subtract(const XShape &XShape)
{
	// TODO
	return *this;
}

XShape XShape::intersect(const XShape &XShape)
{
	// TODO
	return *this;
}

static XShape *Factory(const XScriptArray& arr)
{
	return 0;
}