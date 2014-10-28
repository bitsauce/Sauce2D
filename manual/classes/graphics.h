/**
  *	\brief Allows for modification of graphics options, etc
  */
class Graphics
{
public:
	/**
	  * Enables V-sync on this monitor
	  */
	void enableVsync() {}

	/**
	  * Disables V-sync on this monitor
	  */
	void disableVsync() {}

	/// Update step time delta
	float dt;

	/// Current FPS (frames per second)
	float FPS;
};