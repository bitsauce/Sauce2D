/**
  *	\brief Defines a sub-region of a Texture.
  */
class TextureRegion
{
public:
	/**
	  * Creates a TextureRegion with texture set to 'null', 
	  * uv0 to (0.0, 0.0) and uv1 to (1.0, 1.0).
	  */
	TextureRegion();
	
	/**
	  * Creates a TextureRegion using texture, and with
	  * uv0 set to (0.0, 0.0) and uv1 set to (1.0, 1.0).
	  */
	TextureRegion(Texture* texture);
	
	/**
	  * Creates a TextureRegion using texture,
	  * uv0 and uv1.
	  */
	TextureRegion(Texture* texture, const Vector2& uv0, const Vector2& uv1);
	
	/**
	  * Creates a TextureRegion using texture, and
	  * uv0 set to (u0, v0) and uv1 set to (u1, v1).
	  */
	TextureRegion(Texture* texture, const float u0, const float v0, const float u1, const float v1);
	
	/**
	  * Change the uv offsets.
	  */
	void setRegion(const Vector2& uv0, const Vector2& uv1) {}

	/**
	  * Change the uv offsets.
	  */
	void setRegion(const float u0, const float v0, const float u1, const float v1) {}

	/**
	  * Get the texture object.
	  */
	Texture *getTexture() const {}
	
	/**
	  * Set the texture object.
	  */
	void setTexture(Texture* texture) const {}

	/**
	  * Start uv coordinates.
	  */
	Vector2 uv0;
	
	/**
	  * End uv coordinates.
	  */
	Vector2 uv1;
};