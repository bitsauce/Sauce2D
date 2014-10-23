array<uint> QUAD_INDICES = { 0,3,1, 1,3,2 };
const int CHUNK_SIZE = 32;
const int TILE_SIZE = 32;

void main()
{
}

void draw()
{
	Batch @batch = @Batch();
	
	VertexFormat fmt;
	fmt.set(VERTEX_POSITION, 2);
	fmt.set(VERTEX_COLOR, 4, UBYTE);
	
	VertexBuffer @vbo = @VertexBuffer(fmt);
	for(int y = 0; y < CHUNK_SIZE; y++)
	{
		for(int x = 0; x < CHUNK_SIZE; x++)
		{
			array<Vertex> vertices = fmt.createVertices(4);
			
			vertices[0].set4f(VERTEX_POSITION, x     * TILE_SIZE + TILE_SIZE * 0.5f, y     * TILE_SIZE - TILE_SIZE * 0.5f);
			vertices[1].set4f(VERTEX_POSITION, (x+1) * TILE_SIZE + TILE_SIZE * 0.5f, y     * TILE_SIZE - TILE_SIZE * 0.5f);
			vertices[2].set4f(VERTEX_POSITION, (x+1) * TILE_SIZE + TILE_SIZE * 0.5f, (y+1) * TILE_SIZE - TILE_SIZE * 0.5f);
			vertices[3].set4f(VERTEX_POSITION, x     * TILE_SIZE + TILE_SIZE * 0.5f, (y+1) * TILE_SIZE - TILE_SIZE * 0.5f);
				
			vertices[0].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
			vertices[1].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
			vertices[2].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
			vertices[3].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
			
			vbo.addVertices(vertices, QUAD_INDICES);
		}
	}
	vbo.makeStatic();
	
	vbo.draw(@batch, null);
	batch.draw();
}

void update()
{
}