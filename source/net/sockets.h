#ifndef NET_SOCKETS_H
#define NET_SOCKETS_H

#ifdef DEPRICATION

#include "platform.h"
#include "address.h"

void macAddress(BYTE *pAddress);

#define NET_PACKET_MAX 1024
#define NET_PACKET_HEADER 4

bool initSockets();
void closeSockets();

// Socket class
class Socket
{
public:
	// Socket options
	enum Options
	{
		NonBlocking = 1,
		Broadcast = 2
	};

	// Construct socket
	Socket(int options = NonBlocking)
	{
		options = options; // Warning hack
		//this->options = options;
		socket = 0;
	}

	// Destruct socket
	~Socket()
	{
		close();
	}

	// Open socket on a port
	bool open(ushort port)
	{
		// Make sure its not open
		assert(!isOpen());
		
		// Create socket
		socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(socket <= 0)
		{
			printf("Failed to create socket\n");
			socket = 0;
			return false;
		}

		// Bind to port
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons((ushort)port);
		if(bind(socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
		{
			// Failed to bind socket
			print("ERROR: Failed to bind socket");
			close();
			return false;
		}

		// Set non-blocking io
		//if(options & NonBlocking)
		{
#ifdef WIN32
				DWORD nonBlocking = 1;
				if(ioctlsocket(socket, FIONBIO, &nonBlocking) != 0)
				{
					// Unable to swich to non-blocking
					print("ERROR: Failed to set non-blocking socket");
					close();
					return false;
				}
#elif OSX
				int nonBlocking = 1;
				if ( fcntl( socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
				{
					printf( "failed to set non-blocking socket\n" );
					Close();
					return false;
				}
#endif
		}
			
		// Set broadcast socket
		/*if(options & Broadcast)
		{
			int enable = 1;
			if(setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (const char*)&enable, sizeof( enable )) < 0)
			{
				// Failed to set broadcast mode
				print("Error: Failed to set socket to broadcast");
				close();
				return false;
			}
		}*/
		
		return true;
	}
	
	void close()
	{
		// Close socket if open
		if(socket != 0)
		{
#ifdef WIN32
			closesocket(socket);
#elif
			close(socket);
#endif
			socket = 0;
		}
	}
	
	bool isOpen() const
	{
		return socket != 0;
	}
	
	bool send(const NetAddress &dest, const void *data, int size)
	{
		// Make sure we're sending any data
		assert(data && size > 0);
		
		// Make sure we have a socket
		if(socket == 0)
			return false;
		
		// Make sure  we have an address
		assert(dest.address() != 0);
		assert(dest.port() != 0);
		
		// Set socket
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(dest.address());
		address.sin_port = htons(dest.port());

		// Send data
		int sent_bytes = sendto(socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

		// Return if successfull
		return sent_bytes == size;
	}
	
	int receive(NetAddress &sender, void *data, int size)
	{
		// Sanity checking
		assert(data && size > 0);
		
		// Make sure we have a socket
		if(socket == 0)
			return false;
			
		// Windows
#ifdef WIN32
		typedef int socklen_t;
#endif
			
		// Get socekt info
		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		// Receive from
		int received_bytes = recvfrom(socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength);
		if(received_bytes <= 0)
			return 0; // Nothing received

		// Find address and port
		uint address = ntohl(from.sin_addr.s_addr);
		ushort port = ntohs(from.sin_port);

		// Store sender
		sender = NetAddress(address, port);

		// Return received bytes
		return received_bytes;
	}
		
private:
	
	int socket;
	//int options;
};

#endif

#endif
