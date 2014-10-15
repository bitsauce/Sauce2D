#ifndef WIN_DEBUG_H
#define WIN_DEBUG_H

#include <x2d/x2d.h>

class Debugger : public XDebugger
{
public:
	Debugger();
	~Debugger();

	bool connect();
	void disconnect();
	bool send(const char *data, const int size);
	bool recv(char *data, const int size);
	int  bytesReady();
	
private:
	HANDLE m_client;
};

#endif // WIN_DEBUG_H