/**
  *	\brief The Graphics class is responsible for performing fundemental rendering operations.
  */
class Graphics
{
public:
	/**
	  *	Draws text on-screen using the current active font. The text is drawn from below the y and to the rigth. See gfxSetFont() to set the current font
	  *	\param x The top-left x-coordinate
	  *	\param y The top-left y-coordinate
	  *	\param text The text which is to be drawn
	  */
	void gfxDrawText(const float x, const float y, const string &text) {}
	
	/**
	  * This function loads a font file located the given path, or using the given name
	  * \note Using the font name may perform slowly
	  * \param fontPathOrName Font file path or font name.
	  * \param fontSize The size (pt) of the loading font.
	  * \return Returns the ID of the loaded font
	  */
	int gfxLoadFont(const string &fontPathOrName, const uint fontSize) {}

	/**
	  * Set the current font used when drawing text
	  * \param fontId The ID of a loaded font. Load fonts using gfxLoadFont()
	  */
	void gfxSetFont(const uint fontId) {}

	/**
	  * Returns the width the string in pixels, using the current font. See gfxSetFont() to set the current font
	  * \param str The string to calculate the width of
	  */
	float gfxStringWidth(const string &str) {}

	/**
	  * Returns the height of a string in pixels, using the current font. See gfxSetFont() to set the current font
	  * \param str The string to calculate the height of
	  */
	float gfxStringHeight(const string &str) {}

	/**
	  * Set the current polygon drawing mode
	  * \param type This defines how the graphics engine should interpret the vertices given for making a polygon.
	  */
	void gfxBeginDraw(const uint type) {}

	/**
	  * Adds a vertex to the current polygon. 
	  * \note The behaviour of this function will depend on the current polygon drawing mode. This is set using gfxBeginDraw()
	  * \param x The x-coordinate of the vertex
	  * \param y The y-coordinate of the vertex
	  * \param z Optional z-coordinate of the vertex
	  */
	void gfxAddVertex(const float x, const float y, const float z = 0.0f) {}

	/**
	  * Reset the current polygon drawing mode.
	  * \note It is not needed to call this function if you are drawing another polygon of the same type immedeately after this
	  */
	void gfxEndDraw() {}
	
	/**
	  * Applies clipping for all drawing being done outside the defined rectangle.
	  * \param x The x-coordinate of the clipping rectancle
	  * \param y The y-coordinate of the clipping rectancle
	  * \param width The width of the clipping rectangle
	  * \param height The height of the clipping rectangle
	  */
	void gfxBeginClip(const float x, const float y, const float width, const float height) {}

	/**
	  * Ends the current draw clipping
	  */
	void gfxEndClip() {}
	
	/**
	  * Enables geometry wireframe drawing
	  */
	void gfxEnableWireframe() {}
	
	/**
	  * Disables geometry wireframe drawing
	  */
	void gfxDisableWireframe() {}
	
	/**
	  * Sets the image which should be applied to the next drawn polygon
	  * \note Be aware that if this is not reset, it will affect all the next drawn polygons
	  * \param imageId The ID of the loaded image. Use gfxLoadImage() to load images
	  */
	void gfxSetDrawImage(const uint imageId) {}
	
	/**
	  * Sets the image offset for the next drawn vertex.
	  * \note The values are given in percentage of the image size
	  * \param x The image x-offset [0-1]
	  * \param y The image y-offset [0-1]
	  */
	void gfxSetImageCoord(const float x, const float y){} 
	
	/**
	  * Sets the color which should be applied to the next drawn polygon. This will blend with gfxSetDrawImage()
	  * \note Be aware that if this is not reset it will affect all the next drawn polygons.
	  * \param color The integer encoded color value. See rgb() for encoding colors
	  */
	void gfxSetDrawColor(const uint color) {}

	/**
	  * Sets the current blending formula. (more info to come)
	  * \param src The source blend factor.
	  * \param dst The destination blend factor.
	  */
	void setBlendFunc(const uint src, const uint dst) {}
	
	/**
	  * Sets the current blending formula with separate blending factors for the alpha values. (more info to come)
	  * \param srcColor The source color blend factor.
	  * \param dstColor The destination color blend factor.
	  * \param srcAlpha The source alpha blend factor.
	  * \param dstAlpha The destination alpha blend factor.
	  */
	void setBlendFuncExt(const uint srcColor, const uint dstColor, const uint srcAlpha, const uint dstAlpha) {}
	
	/**
	  * This function applies a transformation to the drawing matrix.
	  * \param px The transformation x-offset
	  * \param py The transformation y-offset
	  * \param pz The transformation z-offset
	  * \param ang The transformation angle
	  * \param rx The x-axis rotation factor
	  * \param ry The y-axis rotation factor
	  * \param rz The z-axis rotation factor
	  * \param sx The x-wise scale factor
	  * \param sy The y-wise scale factor
	  * \param sz The z-wise scale factor
	  */
	void gfxPushTransform(const float px, const float py, const float pz,
		const float ang, const float rx, const float ry, const float rz,
		const float sx, const float sy, const float sz) {}
	
	/**
	  * Pops (removes) the transformation on the top of the stacks. This is the transformation last done by gfxPushTransform()
	  */
	void gfxPopTransform() {}

	/** 
	  *	Creates a new texture buffer
	  * \note This contain no data. To fill it with texture data use gfxSetTextureData()
	  */
	int gfxCreateTexture() {}
	
	/**
	  * Deletes the texture with the \e textureId
	  * \param textureId The ID of the texture to bind
	  */
	void gfxDeleteTexture(const int textureId) {}
	
	/**
	  * Binds a texture to the current drawing. This does the same as gfxSetDrawImage()
	  * \param textureId The ID of the texture to bind
	  */
	void gfxBindTexture(const int textureId) {}
	
	/**
	  * Sets the texture parameters for the current bound texture. (more info)
	  * \param pname ABC
	  * \param param ABC
	  */
	void gfxSetTextureParam(const int pname, const int param) {}

	/**
	  * Sets the texture data of the current bound texture
	  * \param width The width of the texture
	  * \param height The height of the texture
	  * \param data The pixel data of the texture. This consists of an array of colors given as rgb() encoded integers
	  */
	void gfxSetTextureData(const int width, const int height, array<int> data) {}

	/**
	  * Returns the texture data of the current bound texture as encoded rgb() integers
	  */
	array<int> gfxGetTextureData() {}

	/** 
	  *	Loads the \e filePath as a texture. Returns the ID of the loaded texture
	  *	\param filePath The file path of the file to load
	  */
	int gfxLoadImage(const string &filePath) {}

	/**
	  * Copies a rectangle from from the back buffer into an image/texture
	  * \param x The x-coordinate of the rectangle
	  * \param y The y-coordinate of the rectangle
	  * \param width The width of the rectangle
	  * \param height The height of the rectangle
	  */
	int gfxCopyToImage(const int x, const int y, const int width, const int height) {}

	/** 
	  *	Clears the texture data of the texture given by the \e textureId
	  * \param textureId The texture ID
	  */
	void gfxClearImage(const uint textureId) {}
	
	/**
	  * Exports the texture data to a file at \e filePath
	  * \param imageId The ID of the image to export
	  * \param filePath The path where to export the image
	  */
	void gfxExportImage(const uint imageId, const string &filePath) {}
	
	/**
	  * Returns the texture size of the texture with \e textureId
	  * \param textureId The ID of the texture
	  * \param w Out width
	  * \param h Out height
	  */
	void gfxImageSize(const int textureId, int &w, int &h) {}
	
	/**
	  * Clears all color from the current frame buffer
	  */
	void gfxClearColorBit() {}

	/**
	  * Creates a frame buffer object and returns it's ID
	  */
	int gfxCreateFrameBuffer() {}
	
	/**
	  * Binds a frame buffer object to the drawing context
	  * \param fboId The ID of the frame buffer. See gfxCreateFrameBuffer() for creating frame buffers
	  */
	void gfxBindFrameBuffer(const int fboId) {}
	
	/**
	  * Binds an image to the current frame buffer object
	  * \param imageId The ID of the image to bind to the current frame buffer
	  */
	void gfxAttachImageToFrameBuffer(const int imageId) {}

	/**
	  * Creates a vertex buffer object and returns it's ID
	  */
	int gfxCreateVertexBuffer() {}
};