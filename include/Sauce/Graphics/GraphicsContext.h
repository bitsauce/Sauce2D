#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/Shader.h>
#include <Sauce/Graphics/BlendState.h>
#include <Sauce/Graphics/TextureRegion.h>

BEGIN_SAUCE_NAMESPACE

class Vertex;
class RenderTarget2D;
class VertexBuffer;
class IndexBuffer;

/**
 * \brief Handles primitive rendering to the screen.
 */
class SAUCE_API GraphicsContext
{
	friend class Game;
	friend class Window;
public:

	// State
	struct State
	{
		State() :
			width(0),
			height(0),
			texture(nullptr),
			shader(nullptr),
			blendState(BlendState::PRESET_ALPHA_BLEND),
			renderTarget(nullptr)
		{
		}

		uint width;
		uint height;
		shared_ptr<Texture2D> texture;
		shared_ptr<Shader> shader;
		BlendState blendState;
		RenderTarget2D *renderTarget;
		stack<Matrix4> transformationMatrixStack;
		Matrix4 projectionMatrix;
	};

	// TODO: For every set*, add a push*/pop* which uses the state stack
	void pushState();
	void popState();

	/**
	 * Graphics capabilites.
	 * For enabling and disabling certain rendering options.
	 */
	enum Capability
	{
		BLEND = GL_BLEND,						///< Back buffer blending
		DEPTH_TEST = GL_DEPTH_TEST,				///< Depth testing
		SCISSOR_TEST = GL_SCISSOR_TEST,			///< Scissor testing
		LINE_SMOOTH = GL_LINE_SMOOTH,			///< Smooth lines
		POLYGON_SMOOTH = GL_POLYGON_SMOOTH,		///< Smooth polygons
		MULTISAMPLE = GL_MULTISAMPLE,			///< Multisample?
		TEXTURE_1D = GL_TEXTURE_1D,				///< 1D textures
		TEXTURE_2D = GL_TEXTURE_2D,				///< 2D textures
		TEXTURE_3D = GL_TEXTURE_3D				///< 3D textures
	};

	/**
	 * Primitive types.
	 * For rendering different kinds of primitives.
	 */
	enum PrimitiveType
	{
		PRIMITIVE_POINTS = GL_POINTS,					///< Points. 1 vertex per primitive.
		PRIMITIVE_LINES = GL_LINES,						///< Lines. 2 vertex per primitive.
		PRIMITIVE_LINE_STRIP = GL_LINE_STRIP,			///< Line strip. 2 vertex for the first primitive, 1 for the next.
		PRIMITIVE_LINE_LOOP = GL_LINE_LOOP,				///< Line loop. n lines
		PRIMITIVE_TRIANGLES = GL_TRIANGLES,				///< Triangles. 3 vertex per primitive.
		PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,	///< Triangle strip. 3 vertex for the first primitive, 1 vertex for the next.
		PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN		///< Triangle fan. 3 vertex for the first primitive, 1 vertex for the next.
	};

	/**
	 * Clearing buffer mask.
	 * Used for selecting how to clear the backbuffer.
	 */
	enum BufferMask
	{
		COLOR_BUFFER = GL_COLOR_BUFFER_BIT,		///< %Color buffer
		DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,		///< Depth buffer
		STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT	///< Stencil buffer
	};

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
	 * Returns true if capability \p cap is enabled
	 */
	bool isEnabled(const Capability cap);

	void setPointSize(const float pointSize);
	void setLineWidth(const float lineWidth);

	/**
	 * Clears the back buffer using \p mask.
	 * \param mask Decides what channels in the back buffer to clear.
	 * \param fillColor Decides what value to clear to.
	 */
	void clear(const uint mask, const Color &fillColor = Color(0, 0, 0, 0));

	void enableScissor(const int x, const int y, const int w, const int h);
	void disableScissor();

	/**
	 * Sets a render target. This means that everything drawn after this function call
	 * will be rendered to the given \p renderTarget instead of the screen.
	 * TODO: Implement a render target stack.
	 * \param renderTarget The target buffer to render to.
	 */
	void setRenderTarget(RenderTarget2D *renderTarget);

	/**
	 * Get current render target.
	 */
	RenderTarget2D *getRenderTarget() const
	{
		return m_stateStack.top().renderTarget;
	}
	
	/**
	 * Sets the model-view matrix. This means that every vertex drawn after this
	 * will be multiplied by \p mat.
	 * \param mat The 4x4 matrix to be multiplied with every vertex.
	 */
	void setTransformationMatrix(const Matrix4 &mat);

	/**
	 * Gets the current model-view matrix
	 */
	Matrix4 getTransformationMatrix() const;

	/**
	 * Pushes a matrix onto the model-view matrix stack.
	 * The matrix is pushed as follows \code stack.push_back(stack.top() * mat) \endcode
	 * \param mat Matrix to push on the stack.
	 */
	void pushMatrix(const Matrix4 &mat);

	/**
	 * Pops the top matrix of the model-view matrix stack.
	 */
	void popMatrix();
	
	/**
	 * Set texture. Every primitive rendered after this will have \p texture
	 * applied to it.
	 * \param texture Texture to apply to the primitives.
	 */
	void setTexture(shared_ptr<Texture2D> texture);

	/**
	 * Gets the current texture.
	 */
	shared_ptr<Texture2D> getTexture() const;

	/**
	 * Set shader. Every vertex and fragment rendered after this will
	 * have the effect of \p shader applied to them.
	 * \param shader Shader to render the primitves with.
	 */
	void setShader(shared_ptr<Shader> shader);

	/**
	 * Returns the current shader.
	 */
	shared_ptr<Shader> getShader() const;

	/**
	 * Set blend state. Every pixel rendered after this will use a 
	 * formula defined by \p blendState to blend new pixels with the back buffer.
	 * \param blendState The blend state to render with.
	 */
	void setBlendState(const BlendState &blendState);

	/**
	 * Returns the current blend state.
	 */
	BlendState getBlendState();

	/**
	 * Resizes the viewport (the area of the screen rendered to).
	 * \param w Width of the viewport in pixels
	 * \param h Height of the viewport in pixels
	 */
	void resizeViewport(const uint w, const uint h, const bool flipY = false);

	Matrix4 createOrtographicMatrix(const float left, const float right, const float top, const float bottom, const float n = -1.0f, const float f = 1.0f) const;
	Matrix4 createPerspectiveMatrix(const float left, const float right, const float top, const float bottom, const float n = -1.0f, const float f = 1.0f) const;

	/**
	 * Set projection matrix
	 */
	void setProjectionMatrix(const Matrix4 matrix);

	/**
	* Saves a screen shot of the back buffer to \p path as a PNG file.
	* \param path Screen shot destination path
	*/
	void saveScreenshot(string path);

	/**
	 * Returns the width of the viewport.
	 */
	uint getWidth() const
	{
		return m_stateStack.top().width;
	}

	/**
	 * Returns the height of the viewport.
	 */
	uint getHeight() const
	{
		return m_stateStack.top().height;
	}

	/**
	* Returns the size of the viewport.
	*/
	Vector2I getSize() const
	{
		return Vector2I(m_stateStack.top().width, m_stateStack.top().height);
	}

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

	/**
	 * Renders a rectangle.
	 * \param rect Rectangle to render.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const Rect<float> &rect, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a rectangle.
	 * \param pos Top-left corner of the rectangle.
	 * \param size Size of the rectangle.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const Vector2F &pos, const Vector2F &size, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a rectangle.
	 * \param x Left x position of the rectangle.
	 * \param y Top y position of the rectangle.
	 * \param width Width of the rectangle.
	 * \param height Height of the rectangle.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const float x, const float y, const float width, const float height, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	* Renders a rectangle outline.
	* \param rect Rectangle to render.
	* \param color %Color of the rectangle.
	* \param textureRegion Texture region of the rectangle.
	*/
	void drawRectangleOutline(const Rect<float> &rect, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	* Renders a rectangle outline.
	* \param pos Top-left corner of the rectangle.
	* \param size Size of the rectangle.
	* \param color %Color of the rectangle.
	* \param textureRegion Texture region of the rectangle.
	*/
	void drawRectangleOutline(const Vector2F &pos, const Vector2F &size, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	* Renders a rectangle outline.
	* \param x Left x position of the rectangle.
	* \param y Top y position of the rectangle.
	* \param width Width of the rectangle.
	* \param height Height of the rectangle.
	* \param color %Color of the rectangle.
	* \param textureRegion Texture region of the rectangle.
	*/
	void drawRectangleOutline(const float x, const float y, const float width, const float height, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a circle.
	 * \param pos Center of the circle.
	 * \param radius Radius of the circle.
	 * \param segments Number of triangle segments to divide the circle into.
	 * \param color %Color of the circle.
	 */
	void drawCircleGradient(const Vector2F &pos, const float radius, const uint segments, const Color &center = Color::White, const Color &outer = Color::White);
	void drawCircle(const Vector2F &pos, const float radius, const uint segments, const Color &color = Color::White);

	/**
	 * Renders a circle.
	 * \param x Center x position of the circle.
	 * \param y Center y position of the circle.
	 * \param radius Radius of the circle.
	 * \param segments Number of triangle segments to divide the circle into.
	 * \param color %Color of the circle.
	 */
	void drawCircleGradient(const float x, const float y, const float radius, const uint segments, const Color &center = Color::White, const Color &outer = Color::White);
	void drawCircle(const float x, const float y, const float radius, const uint segments, const Color &color = Color::White);

	/**
	 * Draws an arrow from a starting point to an ending point.
	 * \param x0 Starting x-coordinate
	 * \param y0 Starting y-coordinate
	 * \param x1 Ending x-coordinate
	 * \param y1 Ending y-coordinate
	 * \param color %Color of the arrow
	 */
	void drawArrow(const float x0, const float y0, const float x1, const float y1, const Color &color = Color::White);
	void drawArrow(const Vector2F p0, const Vector2F p1, const Color &color = Color::White) { drawArrow(p0.x, p0.y, p1.x, p1.y, color); }

	SDL_GLContext getSDLHandle() const
	{
		return m_context;
	}

	Vertex *getVertices(const uint vertexCount);

private:
	GraphicsContext(Window *window);
	~GraphicsContext();
	void setupContext();

	SDL_GLContext m_context;
	Window *m_window;

	stack<State> m_stateStack;
	State *m_currentState;
	RenderTarget2D *m_boundRenderTarget;

	vector<Vertex> m_vertices; // Vertices for when needed

	static shared_ptr<Shader> s_defaultShader;
	static shared_ptr<Texture2D> s_defaultTexture;
	static GLuint s_vao;
	static GLuint s_vbo;
	static GLuint s_ibo;
};

END_SAUCE_NAMESPACE

#endif // GRAPHICS_CONTEXT_H