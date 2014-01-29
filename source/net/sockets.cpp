//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "sockets.h"

#ifdef DEPRICATED
#include <iphlpapi.h>

void macAddress(BYTE *pAddress)
{
	// Allocate information for up to 16 NICs
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo); // Save memory size of buffer
 
	// Call GetAdaptersInfo
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo,	// [out] buffer to receive data
									&dwBufLen);		// [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS); // Verify return value is valid, no buffer overflow
 
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info
	for(UINT i = 0; i < 6; i++)
		pAddress[i] = (BYTE)pAdapterInfo->Address[i];
}


// Init sockets
bool initSockets()
{
#ifdef WIN32
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
#else
	return true;
#endif
}

// Close sockets
void closeSockets()
{
#if WIN32
	WSACleanup();
#endif
}
#endif