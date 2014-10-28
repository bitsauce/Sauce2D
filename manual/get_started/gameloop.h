/** \page gameloop Understanding the Game Loop
  * 
  * Since games have different run-time requirements than most ordinary programs,
  * they are often run in what is called a game loop. In x2D Game %Engine, the
  * game loop is separated into an update step, and a draw step.
  * 
  * The script writer is required to implement these steps. This is done
  * by defining a 'void update()' and a 'void draw()' globally as follows:
  *
  * \code{.cpp}
  * void update()
  * {
  *     // YOUR UPDATE STEP HERE
  * }
  * 
  * void draw()
  * {
  *     // YOUR DRAW STEP HERE
  * }
  * \endcode
  *
  * The update step is where most of the game logic should go. Movement of characters,
  * physics simulations and updating animations are examples of what should
  * go into the update step, rather than the draw step.
  *
  * The draw step is where all the drawing should happen, as this function is only
  * called once per frame.
  *
  * \section why_step Why is this separation needed?
  * This has got to do with the fact that computer hardware is varied.
  * On slow hardware we need to make sure the time runs equally fast
  * as on fast hardware. This is done by calling the update step
  * more, and the draw step less (hence why you might experience
  * low FPS on slow hardware). In an ideal scenario, the update
  * step will be called as often as the draw step.
  *
  */
