#ifndef WIN_CONSOLE_H
#define WIN_CONSOLE_H

#include <x2d/x2d.h>

class Console : public XConsole
{
public:
#ifdef X2D_DEBUG
	void syslog(const string &str);
#endif
};

#endif // WIN_CONSOLE_H