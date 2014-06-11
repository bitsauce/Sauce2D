#ifndef WIN_CONSOLE_H
#define WIN_CONSOLE_H

#include <x2d/console.h>

class Console : public xdConsole
{
public:
	void log(const string &str);
};

#endif // WIN_CONSOLE_H