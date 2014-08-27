#ifndef WIN_CONSOLE_H
#define WIN_CONSOLE_H

#include <x2d/console.h>

class Console : public xdConsole
{
public:
#ifdef X2D_DEBUG
	void log(const string &str);
#endif
};

#endif // WIN_CONSOLE_H