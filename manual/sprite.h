/**
  *	\brief A general purpose Sprite class.
  * Usefull for drawing sprites to the screen.
  */
class Sprite : public RefType
{
public:
	/**
	  * Creates a Sprite object.
	  * \param region The texture region for the sprite to use when drawing.
	  */
	Sprite(TextureRegion @region);
	
	/**
	  * Creates a Sprite object.
	  * \param texture The texture which the sprite will use when drawing.
	  */
	Sprite(Texture @texture);
	
	/**
	  * Sets the position of the Sprite
	  * \param position New sprite position.
	  */
	void setPosition(const Vector2 &position);
	
	/**
	  * Sets the size of the Sprite
	  * \param size New sprite size.
	  */
	void setSize(const Vector2 &size);
	
	/**
	  * Sets the origin of the Sprite.
	  * The sprite will rotate around this point
	  * (relative to the sprite itself).
	  * \param origin Relative origion position.
	  */
	void setOrigin(const Vector2 &origin);
	
	/**
	  * Sets the rotation of the sprite. The sprite will rotate around it's origin (see setOrigin).
	  * \param rotation New sprite rotation.
	  */
	void setRotation(const float rotation);

	/**
	  * Sets the TextureRegion of which to draw this sprite with.
	  * \param region New texture region.
	  */
	void setRegion(TextureRegion @region);
	
	/**
	  * Gets the axis-aligned bounding box (AABB).
	  * This box will not be rotated and will contain the entire Sprite.
	  * \return Returns a axis-aligned bounding box as a Shape-object.
	  */
	Shape@ getAABB() const;
	
	/**
	  * Gets the current position of the sprite.
	  * \return The current position of the sprite.
	  */
	Vector2 getPosition() const;
	
	/**
	  * Gets the current size of the sprite.
	  * \return The current size of the sprite.
	  */
	Vector2 getSize() const;
	
	/**
	  * Gets the sprite-relative origin of the sprite.
	  * \return The sprite-relative origin of the sprite.
	  */
	Vector2 getOrigin() const;
	
	/**
	  * Gets the world-relative center of the sprite.
	  * \return The world-relative center of the sprite.
	  */
	Vector2 getCenter() const;
	
	/**
	  * Applies move to the position.
	  * \param move The vector to apply to the position.
	  */
	void move(const Vector2 &move);
	
	/**
	  * Applies a rotation to the sprite.
	  * \param angle The angle which to apply to the sprite.
	  */
	void rotate(const float angle);
	
	/**
	  * Applies a scale to the sprite.
	  * \param scl The scale which to apply to the sprite.
	  */
	void scale(const float scl);
	
	/**
	  * Draws the sprite to the given batch. The sprite will be
	  * drawn in it's current state.
	  * \param batch The batch which to draw this sprite to.
	  */
	void draw(Batch @batch);
};