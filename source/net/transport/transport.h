#ifndef NET_TRANSPORT_H
#define NET_TRANSPORT_H

#include "platform/platform.h"

// Lan NetTransport implementation
// - Servers are advertised via net beacon
// - Lan lobby is filled via net listener
// - A mesh runs on the server IP and manages node connections
// - A node runs on each transport, for the server with the mesh a local node also runs
class Mesh;
class Node;
class Beacon;
class Listener;
class NetTransport
{
public:

	// Static interface
	/*static bool Initialize();
	static void Shutdown();
	static bool GetHostName(char hostname[], int size);
	static void UnitTest();*/

	// Lan specific interface
	NetTransport();
	~NetTransport();

	// Transport
	bool startServer(const string hostname);
	bool connectTo(const string address);
	void update(const float dt);
	void stop();

	// State
	enum State
	{
		Disconnected,
		Connecting,
		Connected,
		ConnectionFailed
	};
	bool isConnecting() const;
	bool isConnected() const;
	bool isConnectionFailed() const;
	State state() const;

	// Lobby
	struct LobbyEntry
	{
		char name[65];
		char address[65];
	};
	bool enterLobby();
	int lobbyEntryCount();
	bool lobbyEntryAtIndex(int index, LobbyEntry &entry);

	// Implement NetTransport interface
	bool isNodeConnected( int nodeId );
	int localNodeId() const;
	int maxNodes() const;
	bool sendPacket(int nodeId, const unsigned char data[], int size);
	int receivePacket(int &nodeId, unsigned char data[], int size);
	class ReliabilitySystem &reliabilitySystem(int nodeId);
				
private:

	// NetTransport
	Config m_config;
	Mesh *m_mesh;
	Node *m_node;
	Beacon *m_beacon;
	Listener *m_listener;
	float m_beaconAccum;

	// Connecting
	bool m_connectingByName;
	char m_connectName[65];
	float m_connectAccum;
	bool m_connectFailed;
	State m_state;
};

#endif // NET_TRANSPORT_H
