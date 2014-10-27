/**
  *	\brief Helper class for drawing sprite animations.
  */
class Animation
{
public:
	/**
	  * Creates the key frames for an animation using texture.
	  * \param texture The texture to use for the animation.
	  * \param rows Number of frames per row.
	  * \param columns Number of frames per column.
	  */
	Animation(Texture *texture, const int rows, const int columns) {}

	/**
	  * Get the TextureRegion for the given frame.
	  * \param frameIndex Index of the frame.
	  */
	TextureRegion &getKeyFrame(int frameIndex) const {}
};