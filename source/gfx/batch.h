#ifndef GFX_BATCH_H
#define GFX_BATCH_H

#include <x2d/math.h>
#include <vector>
using namespace std;

// Vertex struct
struct Vertex
{
	Vertex() :
		position(0.0f),
		color(1.0f),
		texCoord(0.0f)
	{
	}
			
	// NOTE TO SELF: DO NOT ALTER ORDER OF DEFINITION
	Vector2 position;
	Vector4 color;
	Vector2 texCoord;
};

class Batch
{
	friend class OpenGL;
public:
	AS_DECL_REF

	Batch();

	//void addVertex(Vertex vertex, int index);
	void addVertices(Vertex *vertices, int vertcount, uint *indices, int icount);
	//void addVertices(const float *vertices, int *indices, int count);
	void setProjectionMatrix();
	void setTexture(class Texture *texture);
	Texture *renderToTexture() {
		/*gfx->bindFrameBuffer(m_frameBuffer);
		gfx->renderBatch(this);
		gfx->bindFrameBuffer(0);*/
		return 0;
	}

	void draw();

private:

	struct Buffer {
		vector<Vertex> vertices;
		vector<uint> indices;
	};

	Texture *m_texture;
	map<Texture*, Buffer> m_buffers;

	static Batch *Factory() { return new Batch(); }
};

#endif // GFX_BATCH_H