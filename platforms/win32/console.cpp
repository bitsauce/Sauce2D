#include "console.h"

void Console::log(const string &str)
{
	OutputDebugString((str + "\n").c_str());
	xdConsole::log(str);
}