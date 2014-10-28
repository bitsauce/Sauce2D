/** \page main The Entry Point
  *
  * The entry point is a function required for the program to start.
  * It is defined by declaring a global function called 'void main()'
  * like follows:
  *
  * \code{.cpp}
  * void main()
  * {
  * }
  * \endcode
  *
  * The function will only be invoked once at the start of the game,
  * and is the first function to be called. Initialization and
  * loading of assets should be done in this phase.
  *
  * As the game starts it will look for the file called 'main.as'
  * in the game's root directory. From this file it is
  * then possible to include other script files using the
  * include directive (#include "file_name.as"). This way
  * you can organize your code and classes into different
  * files.
  *
  */