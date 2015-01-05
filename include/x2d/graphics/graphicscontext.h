#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "../engine.h"
#include "shader.h"
#include "blendState.h"

namespace xd {

class RenderTarget2D;
class Vertex;
class VertexBuffer;
class IndexBuffer;

class XDAPI GraphicsContext
{
public:
	GraphicsContext();

	// Capabilities
	enum Capability
	{
		BLEND = GL_BLEND,
		ALPHA_TEST = GL_ALPHA_TEST,
		DEPTH_TEST = GL_DEPTH_TEST,
		SCISSOR_TEST = GL_SCISSOR_TEST,
		LINE_SMOOTH = GL_LINE_SMOOTH,
		POINT_SMOOTH = GL_POINT_SMOOTH,
		POLYGON_SMOOTH = GL_POLYGON_SMOOTH,
		MULTISAMPLE = GL_MULTISAMPLE,
		TEXTURE_1D = GL_TEXTURE_1D,
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_3D = GL_TEXTURE_3D
	};

	// Primitive type
	enum PrimitiveType
	{
		PRIMITIVE_POINTS = GL_POINTS,
		PRIMITIVE_LINES = GL_LINES,
		PRIMITIVE_TRIANGLES = GL_TRIANGLES,
		PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN
	};

	// Buffer mask
	enum BufferMask
	{
		COLOR_BUFFER = GL_COLOR_BUFFER_BIT,
		DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,
		ACCUM_BUFFER = GL_ACCUM_BUFFER_BIT,
		STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT
	};

	// Toggle capabilities
	void enable(const Capability cap);
	void disable(const Capability cap);
	
	// Clear buffers
	void clear(const uint mask, const Color &fillColor = Color(0, 0, 0, 0));

	// Render target
	void setRenderTarget(RenderTarget2D *renderTarget);
	RenderTarget2D *getRenderTarget() const { return m_renderTarget; }
	
	// Projection matrix
	void setProjectionMatrix(const Matrix4 &projmat);
	Matrix4 getProjectionMatrix() const;

	// Matrix stack
	void pushMatrix(const Matrix4 &mat);
	void popMatrix();
	
	// Texture
	void setTexture(const Texture2DPtr texture);
	Texture2DPtr getTexture() const;

	// Shader
	void setShader(const ShaderPtr shader);
	ShaderPtr getShader() const;

	// Blend state
	void setBlendState(const BlendState &blendState);
	BlendState getBlendState();

	// Copy to texture
	void saveScreenshot(string path);

	// Resize viewport
	void resizeViewport(const uint w, const uint h);

	// Viewport size
	uint getWidth() const { return m_width; }
	uint getHeight() const { return m_height; }

	// Draw primitives
	void drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount);
	void drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo);
	//void drawInstancedPrimitives(const PrimitiveType type);
	void drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount);
	void drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo);

	// Draw shapes
	void drawRectangle(const Rect &rect, const Color &c = Color(255));
	void drawRectangle(const Vector2 &pos, const Vector2 &size, const Color &c = Color(255));
	void drawRectangle(const float x, const float y, const float width, const float height, const Color &c = Color(255));
	void drawCircle(const Vector2 &center, const float r, const uint segments, const Color &c = Color(255)); // TODO: Implement
	void drawCircle(const float x, const float y, const float r, const uint segments, const Color &c = Color(255));

private:
	void setupContext();

	uint m_width;
	uint m_height;
	Texture2DPtr m_texture;
	ShaderPtr m_shader;
	BlendState m_blendState;
	RenderTarget2D *m_renderTarget;
	stack<Matrix4> m_matrixStack;
};

}

#endif // GRAPHICS_CONTEXT_H