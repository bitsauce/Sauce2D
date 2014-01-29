//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)
// Based on "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers

#include "transport.h"
#include "net/net.h"

//---------------------------------------------------------
// Lan transport
//---------------------------------------------------------

#ifdef _DEBUG
#define NET_UNIT_TEST
#endif

// Defaults
const int MeshPort = 30000;
const int ClientPort = 30001;
const int ServerPort = 30002;
const int BeaconPort = 40000;
const int ListenerPort = 40001;
const int ProtocolId = 0x12345678;
const int MeshSendRate = 0.25f;
const int Timeout = 10.0f;
const int MaxNodes = 4;

/*bool NetTransport::Initialize()
{
	return initSocket();
}

void NetTransport::Shutdown()
{
	return ShutdownSockets();
}

bool NetTransport::hostName(char hostname[], int size)
{
	return net::hostName(hostname, size);
}*/

// Lan specific interface
NetTransport::NetTransport()
{
	m_mesh = NULL;
	m_node = NULL;
	m_beacon = NULL;
	m_listener = NULL;
	m_beaconAccum = 1.0f;
	m_connectingByName = false;
	m_connectFailed = false;
}

NetTransport::~NetTransport()
{
	stop();
}

bool NetTransport::startServer(const string hostname)
{
	// Make sure transport is not initialized
	assert(!m_node);
	assert(!m_mesh);
	assert(!m_beacon);
	assert(!m_listener);

	print("** Starting server **");

	// Start beacon
	m_beacon = new Beacon(hostname, m_config.protocolId, m_config.listenerPort, m_config.meshPort);
	if(!m_beacon->start(m_config.beaconPort))
	{
		// Failed to start beacon
		print("ERROR: Failed to start beacon on port %d", m_config.beaconPort);
		stop();
		return false;
	}

	// Start mesh
	m_mesh = new Mesh(m_config.protocolId, m_config.maxNodes, m_config.meshSendRate, m_config.timeout);
	if(!m_mesh->start(m_config.meshPort))
	{
		print("ERROR: Failed to start mesh on port %d", m_config.meshPort);
		stop();
		return false;
	}

	// Start node
	m_node = new Node(m_config.protocolId, m_config.meshSendRate, m_config.timeout);
 	if(!m_node->start(m_config.serverPort))
	{
		print("ERROR: Failed to start node on port %d", m_config.serverPort);
		stop();
		return false;
	}

	m_mesh->reserve(0, Address(127,0,0,1, m_config.serverPort));
	m_node->join(Address(127,0,0,1, m_config.meshPort));
	return true;
}

bool NetTransport::connectTo(const string address)
{
	// Make sure transport is not initialized
	assert(!m_node);
	assert(!m_mesh);
	assert(!m_beacon);
	assert(!m_listener);

	// Connect by address?
	uint a = 0;
	uint b = 0;
	uint c = 0;
	uint d = 0;
	uint port = 0;
	bool isAddress = false;
	if(sscanf(address.c_str(), "%d.%d.%d.%d:%d", &a, &b, &c, &d, &port))
	{
		// The address is an ip
		isAddress = true;
	}
	else
	{
		// Set default port
		port = config.meshPort;
		if(sscanf( server, "%d.%d.%d.%d", &a, &b, &c, &d ))
			isAddress = true; // An ip
	}

	// Yes, connect by address
	if(isAddress)
	{
		print("** Connecting to address: %d.%d.%d.%d:%d **", a, b, c, d, port);
		m_node = new Node(m_config.protocolId, m_config.meshSendRate, m_config.timeout );
	 	if(!m_node->start(m_config.clientPort))
		{
			// Failed to start node
			print("ERROR: Failed to start node on port %d\n", m_config.serverPort );
			stop();
			return false;
		}
		m_config->join(Address((uchar)a, (uchar)b, (uchar)c, (uchar)d, (ushort)port));
		return true;
	}else{
		// No, connect by hostname
		print("** Connecting to url '%s'", server);
		m_listener = new Listener(m_config.protocolId, m_config.timeout);
		if(!m_listener->start(m_config.listenerPort ))
		{
			// Failed to start listner
			print("ERROR: Failed to start listener on port %d", m_config.listenerPort);
			stop();
			return false;
		}
		m_connectingByName = true;
		strncpy(m_connectName, server, sizeof(m_connectName) - 1);
		m_connectName[sizeof(m_connectName) - 1] = '\0';
		m_connectAccum = 0.0f;
		m_connectFailed = false;
	}
	return true;
}

bool NetTransport::isConnected() const
{
	return m_node && m_node->isConnected();
}

bool NetTransport::connectFailed() const
{
	return m_node && m_node->joinFailed() || m_connectingByName && m_connectFailed;
}

bool NetTransport::enterLobby()
{
	print("** Client entering lobby **");
	m_listener = new Listener(m_config.protocolId, m_config.timeout);
	if(!m_listener->start(m_config.listenerPort))
	{
		// Failed to start listner
		print("ERROR: Failed to start listener on port %d", m_config.listenerPort);
		stop();
		return false;
	}
	return true;
}

int NetTransport::lobbyEntryCount()
{
	if(m_listener)
		return m_listener->entryCount();
	else
		return 0;
}

bool NetTransport::lobbyEntryAtIndex(int index, LobbyEntry &entry)
{
	if (!m_listener || index < 0 || index >= m_listener->entryCount() )
		return false;
	const ListenerEntry &e = m_listener->entry(index);
	sprintf(entry.address, "%d.%d.%d.%d:%d", e.address.a(), e.address.b(), e.address.c(), e.address.d(), e.address.port());
	strncpy(entry.name, e.name, sizeof(entry.name));
	entry.name[sizeof(entry.name) - 1] = '\0';
	return true;
}

void NetTransport::stop()
{
	print("** Connection closing **");
	if(m_mesh)
	{
		delete m_mesh;
		m_mesh = NULL;
	}

	if(m_node)
	{
		delete m_node;
		m_node = NULL;
	}

	if(m_beacon)
	{
		delete m_beacon;
		m_beacon = NULL;
	}

	if(m_listener)
	{
		delete m_listener;
		m_listener = NULL;
	}
		
	m_connectingByName = false;
	m_connectFailed = false;
}

// Implement transport interface
bool NetTransport::isNodeConnected(int nodeId)
{
	assert(m_node);
	return m_node->isNodeConnected(nodeId);
}

int NetTransport::localNodeId() const
{
	assert(m_node);
	return m_node->localNodeId();
}

int NetTransport::maxNodes() const
{
	assert(m_node);
	return m_node->GetMaxNodes();
}

bool NetTransport::sendPacket(int nodeId, const uchar data[], int size)
{
	assert(m_node);
	return m_node->sendPacket(nodeId, data, size);
}

int NetTransport::receivePacket(int &nodeId, unsigned char data[], int size)
{
	assert(m_node);
	return m_node->receivePacket(nodeId, data, size);
}

class net::ReliabilitySystem &NetTransport::reliabilitySystem(int nodeId)
{
	// todo: implement!
	static ReliabilitySystem reliabilitySystem;
	return reliabilitySystem;
}

void NetTransport::update(float deltaTime)
{
	if(m_connectingByName && !m_connectFailed)
	{
		assert(m_listener);
		const int entryCount = m_listener->entryCount();
		for(int i = 0; i < entryCount; i++)
		{
			const ListenerEntry & entry = m_listener->entry(i);
			if(strcmp(entry.name, m_connectName) == 0)
			{
				print("** Server found %d.%d.%d.%d:%d **", 
					entry.address.a(),
					entry.address.b(),
					entry.address.c(),
					entry.address.d(),
					entry.address.port() );
				m_node = new Node(m_config.protocolId, m_config.meshSendRate, m_config.timeout);
			 	if(!m_node->start(m_config.clientPort))
				{
					// Failed to start node
					print("ERROR: Failed to start node on port %d", m_config.serverPort);
					stop();
					m_connectFailed = true;
					return;
				}
				m_node->join(entry.address);
				delete m_listener;
				m_listener = NULL;
				m_connectingByName = false;
			}
		}

		if(m_connectingByName)
		{
			m_connectAccum += deltaTime;
			if(m_connectAccum > m_config.timeout)
				m_connectFailed = true;
		}
	}

	// Update node mesh
	if(m_mesh)
		m_mesh->update(deltaTime);

	// Update server node
	if(m_node)
		m_node->update(deltaTime);
	
	// Update beacon
	if(m_beacon)
	{
		// Increase acc
		m_beaconAccum += deltaTime;
		while(m_beaconAccum >= 1.0f)
		{
			m_beacon->update(1.0f);
			m_beaconAccum -= 1.0f;
		}
	}

	// Update listner
	if(m_listener)
		m_listener-update(deltaTime);

	// Update mesh (again?)
	if(m_mesh)
		m_mesh->update(deltaTime);

	// Update node (again?)
	if(m_node)
		m_node->update(deltaTime);
}