//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/graphics.h>

BEGIN_CGF_NAMESPACE

Shape::Shape() :
	m_fillColor(255),
	m_fillTexture(0),
	m_penColor(255),
	m_penSize(1.0f),
	m_index(0)
{
}

Shape::Shape(const Rect<float> &rect) :
	m_fillColor(255),
	m_fillTexture(0),
	m_penColor(255),
	m_penSize(1.0f),
	m_index(0)
{
	m_vertices = new Vertex[4]; 

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

Shape::Shape(const Vector2F &center, const float radius, const int vertCount) :
	m_fillColor(255),
	m_fillTexture(0),
	m_penColor(255),
	m_penSize(1.0f),
	m_index(0)
{
	m_vertices = new Vertex[vertCount];
	
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

Shape::Shape(const Vertex *vertices, const int vertCount) :
	m_fillColor(255),
	m_fillTexture(0),
	m_penColor(255),
	m_penSize(1.0f),
	m_index(0)
{
	// TODO: implement
	//memcpy(m_vertices, vertices, vertCount * sizeof(Vertex));
	//m_index = vertCount;
}

Shape::~Shape()
{
	//if(m_fillTexture) {
	//	m_fillTexture->release();
	//}
	delete[] m_vertices;
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
	//if(m_vertices.size() % 3 == 0) LOG("Somethings wrong");

	for(uint i = 0; i < vertices.size(); i++) {
		addVertex(vertices[i]);
	}
}*/

void Shape::setFillColor(const Color &color)
{
	m_fillColor = color;
}

void Shape::setFillTexture(Texture2D *texture)
{
	m_fillTexture = texture;
}

void Shape::setPenColor(const Color &color)
{
	m_penColor = color;
}

void Shape::setPenSize(const float size)
{
	m_penSize = size;
}

/*void Shape::draw(Batch *batch)
{
	if(!validate())
		return;

	if(!batch)
	{
		LOG("void Shape::draw(): Can not draw to 'null' batch");
		return;
	}

	// Set vertex colors
	for(int i = 0; i < m_vertCount; i++)
	{
		m_vertices[i].set4ub(VERTEX_COLOR, m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);
	}

	batch->setTexture(m_fillTexture);
	batch->setPrimitive(Batch::PRIMITIVE_TRIANGLES);
	batch->addVertices(m_vertices, m_vertCount, m_indices.data(), m_indices.size());
	//batch->release();
}*/

bool Shape::validate()
{
	return true;
}

Shape Shape::merge(const Shape &Shape)
{
	// TODO
	return *this;
}

Shape Shape::subtract(const Shape &Shape)
{
	// TODO
	return *this;
}

Shape Shape::intersect(const Shape &Shape)
{
	// TODO
	return *this;
}

END_CGF_NAMESPACE
