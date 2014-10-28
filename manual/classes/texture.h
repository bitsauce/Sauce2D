/**
  *	\brief A texture object for drawing textures/images to the screen.
  */
class Texture
{
public:
	/**
	  * Creates a texture from the given image file.
	  * \note Currently we only support loading .png files.
	  * \param path Image file path.
	  */
	Texture(const string &path) {}

	/**
	  * Creates an empty texture with the dimentions width x height.
	  */
	Texture(const int width, const int height) {}

	/**
	  * Creates a texture from the given Pixmap.
	  */
	Texture(const Pixmap &pixmap) {}
	
	/**
	  * Creates a copy of another texture.
	  */
	Texture(const Texture *texture) {}

	/**
	  * Enable mipmapping for this texture.
	  */
	void enableMipmaps() {}
	
	/**
	  * Disable mipmapping for this texture.
	  */
	void disableMipmaps() {}
	
	/**
	  * Sets the texture magnify filter.
	  */
	void setFiltering(const TextureFilter filter) {}
	
	/**
	  * Sets the wrapping mode for the texture.
	  */
	void setFiltering(const TextureWrapping filter) {}
	
	/**
	  * Creates and returns a Pixmap copy of the texture.
	  */
	Pixmap getPixmap() const {}

	/**
	  * Replaces the content of texture with Pixmap.
	  */
	void update(const Pixmap &pixmap) {}

	/**
	  * Replaces a section of the texture with Pixmap.
	  * \param x The start x-coordinate
	  * \param y The start y-coordinate
	  * \param pixmap The pixmap to insert at the start position
	  */
	void updateSection(const int x, const int y, const Pixmap &pixmap) {}

	/**
	  * Clears the content of the texture by setting all the texels to rgb(0, 0, 0, 0)
	  */
	void clear() {}

	/**
	  * Get the width of the texture in pixels
	  */
	int getWidth() const {}

	/**
	  * Get the height of the texture in pixels
	  */
	int getHeight() const {}

	/**
	  * Get the size of the texture in pixels
	  */
	Vector2i getSize() const {}
	
	/// Texture filtering modes
	enum TextureFilter
	{
		NEAREST,	///< Nearest neightbour magnifying
		LINEAR		///< Linear magnifying
	};

	/// Texture wrapping modes
	enum TextureWrapping
	{
		CLAMP_TO_BORDER,	///< Clamp outside pixels to border
		CLAMP_TO_EDGE,		///< Clamp outside pixels to edge
		REPEAT,				///< Repeat the texture
		MIRRORED_REPEAT,	///< Mirror and repeat the texture
	};
};