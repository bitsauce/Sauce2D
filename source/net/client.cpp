//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)
// Based on "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers

#include "platform/platform.h"
#include "object.h"

#include "transport/platform.h"
#include "transport/transport.h"

using namespace std;
using namespace net;

int client_run()
{
	// Initialize transport layer
	TransportType type = Transport_LAN;
	if(!Transport::Initialize(Transport_LAN))
	{
		printf("Failed to initialize transport layer\n");
		return -1;
	}
	
	// Create transport layer
	Transport* transport = Transport::Create();
	if (!transport)
	{
		printf("Could not create transport\n");
		return -1;
	}
	
	// Connect to server (transport specific)
	switch(type)
	{
		case Transport_LAN:
		{
			// Get string input
			int ipchars = 0;
			string cname = "127.0.0.1";
			bool isHostname = true;
			cout << "Connect to:" << endl;
			cin >> cname;
			for(int i = 0; i < cname.size(); i++)
			{
				if(cname[i] == '.')
					ipchars++;
			}
			if(ipchars == 3)
			{
				isHostname = false;
				cname += ":30000";
			}
			
			// Convert to char
			char* hostname = new char[cname.size() + 1];
			strcpy(hostname, cname.c_str());
			
			// Connect to ip
			TransportLAN * lan_transport = dynamic_cast<TransportLAN*>(transport);
			if(!isHostname)
				lan_transport->ConnectClient(hostname);
			else
			{
				char hostname[64+1];
				TransportLAN::GetHostName(hostname, cname.size() + 1);
				lan_transport->ConnectClient(hostname);
			}
			delete hostname;
		}
		break;
		
		default:
			break;
	}

	// Main loop
	const int DeltaTime = 333;
	const int PacketRate = 2500; //ms = 2,5s
	int PacketAccum = 0;
	bool connected = false;
	while(true)
	{
		// Check connection
		if(type == Transport_LAN)
		{
			// Check transport
			TransportLAN* lan_transport = dynamic_cast<TransportLAN*>(transport);
			if(!connected && lan_transport->IsConnected())
				connected = true;

			// Check connection
			if(connected && !lan_transport->IsConnected())
			{
				printf("Disconnected\n");
				break;
			}

			// Connection failed
			if(lan_transport->ConnectFailed())
			{
				printf("Connect failed\n");
				break;
			}
		}

		// Send packets per rate
		if(PacketAccum >= PacketRate)
		{
			// Test packets
			int node = 1; // TODO: how to know the node?

			// Receive packet
			NetPacket packet;
			transport->ReceivePacket(node, packet.GetPacketInfo(), 1024);

			// Create new client objects
			unsigned int objCount;
			packet.ReadInteger(objCount);
			for(int i = mSceneObjects.size(); i < objCount; i++)
			{
				Object* obj = new Object();
			}

			// Update all client objects
			for(int i = 0; i < mSceneObjects.size(); i++)
				mSceneObjects[i]->unpackUpdate(&packet);

			// Update transport layer
			transport->Update(PacketAccum);

			// Reset delta
			PacketAccum = 0;
		}else{
			
			// Interpolate all client objects
			for(int i = 0; i < mSceneObjects.size(); i++)
			{
				mSceneObjects[i]->interpolate();
			}
		}

		// Apply delta
		PacketAccum += DeltaTime;
	}
	
	// Shutdown
	Transport::Destroy(transport);
	Transport::Shutdown();

	return 0;
}
