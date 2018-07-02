#pragma once

#include <Sauce/Common.h>
#include <Sauce/Graphics/Shader.h>
#include <Sauce/Graphics/BlendState.h>
#include <Sauce/Graphics/TextureRegion.h>

BEGIN_SAUCE_NAMESPACE

class Vertex;
class RenderTarget2D;
class VertexBuffer;
class IndexBuffer;

class SAUCE_API OpenGLContext : public GraphicsContext
{
	friend class Game;
private:
	OpenGLContext(const int major, const int minor);
	~OpenGLContext();

	void setupContext();

	static GLuint s_vao;
	static GLuint s_vbo;
	static GLuint s_ibo;

public:
	/**
	 * Enables the capability \p cap.
	 * \param cap Capability to enable.
	 */
	void enable(const Capability cap);

	/**
	 * Disables the capability \p cap.
	 * \param cap Capability to disable.
	 */
	void disable(const Capability cap);

	/**
	 * Enable scissor testing
	 */
	void enableScissor(const int x, const int y, const int w, const int h);
	
	/**
	 * Disable scissor testing
	 */
	void disableScissor();

	/**
	 * Returns true if capability \p cap is enabled
	 */
	bool isEnabled(const Capability cap);

	/**
	 * Set rendering point size
	 */
	void setPointSize(const float pointSize);
	
	/**
	 * Set rendering line width
	 */
	void setLineWidth(const float lineWidth);
	
	/**
	 * Set the size of the viewport
	 */
	void setViewportSize(const uint w, const uint h);

	/**
	 * Clears the back buffer using \p mask.
	 * \param mask Decides what channels in the back buffer to clear.
	 * \param fillColor Decides what value to clear to.
	 */
	void clear(const uint mask, const Color &fillColor = Color(0, 0, 0, 0));

	void saveScreenshot(string filePath);
	
	/**
	 * Create matricies
	 */
	Matrix4 createOrtographicMatrix(const float left, const float right, const float top, const float bottom, const float n = -1.0f, const float f = 1.0f) const;
	Matrix4 createPerspectiveMatrix(const float fov, const float aspectRatio, const float zNear, const float zFar) const;
	Matrix4 createLookAtMatrix(const Vector3F &position, const Vector3F &fwd) const;
	
	/**
	 * Renders an indexed primitive to the screen.
	 * \param type Types of primitives to render.
	 * \param vertices Array of vertices to render.
	 * \param vertexCount Number of vertices to render.
	 * \param indices Array of indices.
	 * \param indexCount Number of indices.
	 */
	void drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount);

	/**
	 * Renders an indexed primitive to the screen using vertex and index buffers.
	 * \param type Types of primitives to render.
	 * \param vbo Vertex buffer object.
	 * \param ibo Index buffer object.
	 */
	void drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo);

	/**
	 * Renders primitives to the screen.
	 * \param type Types of primitives to render.
	 * \param vertices Array of vertices to render.
	 * \param vertexCount Number of vertices to render.
	 */
	void drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount);

	/**
	 * Renders primitives to the screen.
	 * \param type Types of primitives to render.
	 * \param vbo Vertex buffer object.
	 */
	void drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo);

	Texture2D *createTexture(const Pixmap &pixmap);
	Shader *createShader(const string &vertexSource, const string &fragmentSource, const string &geometrySource);
	RenderTarget2D *createRenderTarget(const uint width, const uint height, const uint targetCount = 1, const PixelFormat &fmt = PixelFormat());

	string getGLSLVersion() const;

private:
	Window *createWindow(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags);

	const int m_majorVersion, m_minorVersion;
};

END_SAUCE_NAMESPACE
