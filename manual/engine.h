/**
  *	\brief Controls the flow of the engine.
  */
class Engine
{
public:
	/**
	  * End the application.
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
	  * Toggles the profiler state. For more read our Profiling page.
	  */
	void toggleProfiler() {}

	/**
	  * Pushes a profile using the given name to the profile stack tree.
	  * \param name Profile node name
	  */
	void pushProfile(const string &name) {}

	/**
	  * Pops the profile on the top of the stack.
	  */
	void popProfile() {}

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