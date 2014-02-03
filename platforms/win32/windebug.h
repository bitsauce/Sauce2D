#ifndef WIN_IO
#define WIN_IO

#include "x2d/debug.h"

class WinDebug : public X2DDebug
{
public:
	WinDebug();
	~WinDebug();

	bool init();
	bool listen(const ushort port);
	bool accept();
	bool connect();
	void disconnect();
	bool send(const char *data);
	bool recv(char **data);
	int bytesReady();
	
private:

	// Sockets
	SOCKET m_server;
	SOCKET m_client;
};

#endif // WIN_IO