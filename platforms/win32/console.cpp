#include "console.h"

#ifdef X2D_DEBUG
void Console::syslog(const string &str)
{
	OutputDebugString(str.c_str());
	OutputDebugString("\n");
}
#endif