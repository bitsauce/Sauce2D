//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "shape.h"

AS_REG_REF(Shape)

int Shape::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f()", asFUNCTIONPR(Factory, (), Shape*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f(const Rect &in)", asFUNCTIONPR(Factory, (const Rect&), Shape*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f(const Vector2 &in, const float, const int)", asFUNCTIONPR(Factory, (const Vector2&, const float, const int), Shape*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Shape", asBEHAVE_FACTORY, "Shape @f(const array<Vector2> &in)", asFUNCTIONPR(Factory, (const Array&), Shape*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Shape", "void setFillColor(const Vector4 &in)", asMETHOD(Shape, setFillColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Shape", "void setFillTexture(Texture @texture)", asMETHOD(Shape, setFillTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Shape", "void draw(Batch @batch)", asMETHOD(Shape, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

Shape::Shape() :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
}

Shape::Shape(const Rect &rect) :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
	Vertex vert;
	vert.color.set(1.0f, 1.0f, 1.0f, 1.0f);

	vert.position.set(rect.getLeft(), rect.getTop());
	vert.texCoord.set(0.0f, 1.0f);
	m_vertices.push_back(vert);

	vert.position.set(rect.getRight(), rect.getTop());
	vert.texCoord.set(1.0f, 1.0f);
	m_vertices.push_back(vert);

	vert.position.set(rect.getRight(), rect.getBottom());
	vert.texCoord.set(1.0f, 0.0f);
	m_vertices.push_back(vert);

	vert.position.set(rect.getLeft(), rect.getBottom());
	vert.texCoord.set(0.0f, 0.0f);
	m_vertices.push_back(vert);
	
	m_indices.push_back(0);
	m_indices.push_back(3);
	m_indices.push_back(1);
	
	m_indices.push_back(1);
	m_indices.push_back(3);
	m_indices.push_back(2);

	m_index += 6;
}

Shape::Shape(const Vector2 &center, const float radius, const int vertCount) :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
	Vertex vert;
	vert.color.set(1.0f, 1.0f, 1.0f, 1.0f);
	vert.position = center;
	vert.texCoord.set(0.5f, 0.5f);
	m_vertices.push_back(vert);

	for(int i = 0; i <= vertCount; i++)
	{
		float r = (2.0f*PI*i)/vertCount;
		vert.position.set(center.x + cos(r)*radius, center.y + sin(r)*radius);
		vert.texCoord.set((1+cos(r))/2.0f, 1-(1+sin(r))/2.0f);
		m_vertices.push_back(vert);

		if(i > 0)
		{
			m_indices.push_back(0);
			m_indices.push_back(i < vertCount ? 1+i : 1);
			m_indices.push_back(i);
		}
	}

	m_index += vertCount * 3;
}

Shape::Shape(const vector<Vertex> &vertices) :
	m_fillColor(1.0f),
	m_fillTexture(0),
	m_penColor(1.0f),
	m_penSize(1.0f),
	m_index(0)
{
	//addVertices(vertices);
}

Shape::~Shape()
{
	if(m_fillTexture) {
		m_fillTexture->release();
	}
}

/*void Shape::addVertex(const Vertex &vertex)
{
	m_vertices.push_back(vertex);
	if(++m_index > 2) {
		m_indices.push_back(m_index-3);
		m_indices.push_back(m_index-2);
		m_indices.push_back(m_index);
	}
}

void Shape::addVertices(const vector<Vertex> &vertices)
{
	//if(m_vertices.size() % 3 == 0) LOG("Something's wrong");

	for(uint i = 0; i < vertices.size(); i++) {
		addVertex(vertices[i]);
	}
}*/

void Shape::setFillColor(const Vector4 &color)
{
	m_fillColor = color;
}

void Shape::setFillTexture(Texture* texture)
{
	if(m_fillTexture) {
		m_fillTexture->release();
	}
	m_fillTexture = texture;
}

void Shape::setPenColor(const Vector4 &color)
{
	m_penColor = color;
}

void Shape::setPenSize(const float size)
{
	m_penSize = size;
}

void Shape::draw(Batch *batch)
{
	if(!validate())
		return;

	if(!batch) {
		LOG("Shape.draw: Can not draw to a 'null' batch");
		return;
	}

	for(uint i = 0; i < m_vertices.size(); i++)
		m_vertices[i].color = m_fillColor;
	
	//batch.setColor(m_fillColor);
	if(m_fillTexture) {
		m_fillTexture->addRef();
		batch->setTexture(m_fillTexture);
	}
	batch->addVertices(m_vertices.data(), m_vertices.size(), m_indices.data(), m_indices.size());
	
	batch->setTexture(0);
	//batch.setColor(Vector4(1.0f));
	batch->release();
}

bool Shape::validate()
{
	return true;
}

Shape Shape::merge(const Shape &shape)
{
	// TODO
	return *this;
}

Shape Shape::subtract(const Shape &shape)
{
	// TODO
	return *this;
}

Shape Shape::intersect(const Shape &shape)
{
	// TODO
	return *this;
}