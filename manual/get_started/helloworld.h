/** \page hello_world Hello World!
  *
  * \note These tutorials assumes the reader knows the basics of OOP-programming.
  *
  * As an introduction to the x2D game engine, your first script will be the
  * simple "output Hello World to the screen".
  *
  * To accomplish this we will start of with creating a new file for our scripts.
  * This file needs to be named 'main.as', and it has to be located in the same
  * directory as our game executable, or alternatively in our game root directory.
  * After we have created our 'main.as' file, we need to define a entry-point function.
  * This is done by defining a global function called "void main()" like follows:
  *
  * \code{.cpp}
  * void main()
  * {
  * 	// ENTRY POINT BODY
  * }
  * \endcode
  *
  * Now we need to actually output the string "Hello World" to the console.
  * We do this by calling Console.log() with our desired output
  * string as the parameter.
  *
  * Example code:
  *
  * \code{.cpp}
  * void main()
  * {
  * 	// Output "Hello World" to the console
  * 	Console.log("Hello World");
  * }
  * \endcode
  *
  * 
  * \note If the 'main.as' file is not present in the game root directory,
  * the engine will produce an error.
  *
  * \note As the game starts it will look for a file called 'main.as'
  * in the game root directory. From this file it is
  * possible to include other script files using the
  * include directive (#include "file_name.as"). This way
  * you can organize your code and classes into different
  * files.
  *
  * \note The "void main()" function will only be invoked once at the start of the game,
  * and is the first function to be called. Initialization and
  * loading of assets should be done in this phase.
  *
  */