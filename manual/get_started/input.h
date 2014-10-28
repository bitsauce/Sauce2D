/** \page input Getting input
  *
  * \code{.cpp}
  * int posX = 0;
  * int posY = 0;
  *
  * void update()
  * {
  *     if(Input.getKeyState(KEY_UP))
  *     {
  *         posY -= 10;
  *     }
  *
  *     if(Input.getKeyState(KEY_DOWN))
  *     {
  *         posY += 10;
  *     }
  *
  *     if(Input.getKeyState(KEY_RIGHT))
  *     {
  *         posX += 10;
  *     }
  *
  *     if(Input.getKeyState(KEY_LEFT))
  *     {
  *         posX -= 10;
  *     }
  * }
  * \endcode
  */