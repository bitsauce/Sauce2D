#include "console.h"

#ifdef X2D_DEBUG
void Console::log(const string &str)
{
	OutputDebugString(str.c_str());
	OutputDebugString("\n");
	xdConsole::log(str);
}
#endif