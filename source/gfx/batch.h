#ifndef GFX_BATCH_H
#define GFX_BATCH_H

class Batch
{
public:
	Batch();

	void addVertex();

private:

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
		vec4 color;
		Vector2 texCoord;
	};

	vector<Vertex> m_vertices;
};

#endif // GFX_BATCH_H