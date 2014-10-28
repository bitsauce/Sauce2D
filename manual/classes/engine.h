/**
  *	\brief Controls the flow of the engine.
  */
class Engine
{
public:
	/**
	  * Exit the application.
	  */
	void exit() {}

	/**
	  * Current platform name. Typically with a capital initial letter (eg. "Window", "Mac").
	  */
	string platform;

	/**
	  * Current working directory.
	  */
	string workDir;

	/**
	  * Pushes a Scene object to the scene stack.
	  * \param scene A Scene object
	  */
	void pushScene(Scene *scene) {}

	/**
	  * Pops the current scene from the top of the stack.
	  */
	void popScene() {}
};