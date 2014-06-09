#ifndef GFX_BATCH_H
#define GFX_BATCH_H

#include <x2d/math.h>
#include <vector>
using namespace std;

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

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

	void addVertices(Vertex *vertices, int vertcount, uint *indices, int icount);
	void setProjectionMatrix();

	void setShader(class Shader *shader);
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

	struct State {
		State() :
			texture(0)
		{
		}
		
		bool operator<(const State& other) const
		{
			// NOTE: The map is sorted by the order of compares
			TUPLE_CMP(this->texture, other.texture)
			return false;
		}

		Texture *texture;
	} m_state;

	map<State, Buffer> m_buffers;
	Shader *m_shader;

	static Batch *Factory() { return new Batch(); }
};

#endif // GFX_BATCH_H