//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "debug.h"

//----------------------------------------------------------------------------
// Debugger
//----------------------------------------------------------------------------
Debugger::Debugger() :
	m_client(0)
{
}

Debugger::~Debugger()
{
	disconnect();
}

bool Debugger::connect()
{
	// Create pipe
	m_client = CreateNamedPipe("\\\\.\\pipe\\x2d_debug_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, 1, 0, 0, 0, NULL);
	if(m_client == NULL || m_client == INVALID_HANDLE_VALUE)
	{
		LOG("Failed to create pipe.");
		return false;
	}

	// Connect clients
	BOOL result = ConnectNamedPipe(m_client, NULL); ULONG id;
	if(result == FALSE && GetNamedPipeClientProcessId(m_client, &id) == FALSE)
	{
		LOG("Failed to make connections on named pipe (error code %i)", GetLastError());
		disconnect();
		return false;
	}
	return true;
}

void Debugger::disconnect()
{
	// Close pipe
	if(m_client)
	{
		CloseHandle(m_client);
	}
	m_client = 0;
}

bool Debugger::send(const char *data, const int size)
{
	DWORD bytesWritten;
	return WriteFile(m_client, data, size, &bytesWritten, NULL) == TRUE;
}

bool Debugger::recv(char *data, int size)
{
	DWORD bytesRead;
	return ReadFile(m_client, data, size, &bytesRead, NULL) == TRUE;
}

int Debugger::bytesReady()
{
	DWORD bytesAvailable;
	PeekNamedPipe(m_client, NULL, 0, 0, &bytesAvailable, NULL);
	return bytesAvailable;
}