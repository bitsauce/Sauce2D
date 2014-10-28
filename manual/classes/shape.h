/**
  *	\brief Usefull for drawing simple shapes to the screen.
  */
class Shape
{
public:
	/**
	  * Creates a rectangle shape
	  */
	Shape(const Rect &rect);

	/**
	  * Creates a circle shape
	  */
	Shape(const Vector2 &center, const float radius, const int vertexCount = -1);

	/**
	  * Sets the fill color of the shape.
	  */
	void setFillColor(const Vector4 &color);

	/**
	  * Sets the fill texture.
	  */
	void setFillTexture(const Vector4 &color);

	/**
	  * Draws this shape in its current state to the batch.
	  */
	void draw(Batch *batch);
};