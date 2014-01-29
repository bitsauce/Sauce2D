#ifndef NET_BEACON_H
#define NET_BEACON_H

#include "platform/platform.h"
#include "sockets.h"
#include "net/transport/packet.h"

// Beacon
// - Sends broadcast UDP packets to the LAN
// - Use a beacon to advertise the existence of a server on the LAN
	
class Beacon
{
public:
		
	Beacon(string name, uint protocolId, uint listenerPort, uint serverPort) :
		m_socket(Socket::Broadcast | Socket::NonBlocking),
		m_running(false),
		m_name(name),
		m_protocolId(protocolId),
		m_listenerPort(listenerPort),
		m_serverPort(serverPort)
	{
	}
		
	~Beacon()
	{
		// Stop if running
		if(m_running)
			stop();
	}
		
	bool start(ushort port)
	{
		// Make sure its not running
		assert(!m_running);

		// Start beacon
		print("** Starting beacon on port %d **", port);
		if(!m_socket.open(port))
			return false;

		// Set running and return
		m_running = true;
		return true;
	}
		
	void stop()
	{
		// Make sure its running
		assert(m_running);

		// Stop beacon
		print("** Stoping beacon **");
		m_socket.close();
		m_running = false;
	}
		
	void update(const float dt)
	{
		// Are we running?
		assert(m_running);

		// Create packet
		NetPacket packet;

		// Write beacon info (12 bytes)
		packet.writeInteger(0);
		packet.writeInteger(m_protocolId);
		packet.writeInteger(m_serverPort);

		// Write computer name length (1 byte)
		uchar nameSize = (uchar)m_name.size();
		packet.writeByte(nameSize);

		// Write computer name (max. 64 bytes)
		assert(nameSize < 63);
		packet.writeBytes((uchar*)m_name.c_str(), nameSize);

		// Sending broadcast packet to all attached
		// hosts. http://en.wikipedia.org/wiki/Broadcast_address
		if(!m_socket.send(NetAddress(255,255,255,255, m_listenerPort), packet.data(), packet.size()))
			print("ERROR: Failed to send broadcast packet");

		// Wait for host responce
		NetAddress sender;
		while(m_socket.receive(sender, packet.data(), 256));
	}
		
private:
		
	string m_name;
	uint m_protocolId;
	uint m_listenerPort;
	uint m_serverPort;
	bool m_running;
	Socket m_socket;
};
	
// Listener entry
// - An entry in the list of servers on the LAN
	
struct ListenerEntry
{
	string name;
	NetAddress address;
	float timeoutAccum;
};

// Listener
// - Listens for broadcast packets sent over the LAN
// - Use a listener to get a list of all the servers on the LAN
	
class Listener
{
public:
		
	Listener(uint protocolId, float timeout = 10.0f) :
		m_protocolId(protocolId),
		m_timeout(timeout),
		m_running(false)
	{
		clearData();
	}
		
	~Listener()
	{
		// Stop if running
		if(m_running)
			stop();
	}
		
	bool start(ushort port)
	{
		// Make sure the listener isnt running
		assert(!m_running);

		// Start listener
		print("** Starting listener on port %d **", port);
		if(!m_socket.open(port))
			return false;

		// Set running
		m_running = true;
		return true;
	}
		
	void stop()
	{
		// Make sure the listener is running
		assert(m_running);

		// Stop listener
		print("** Stoping listener **");
		m_socket.close();
		m_running = false;
		clearData();
	}
		
	void update(const float dt)
	{
		// Make sure its running
		assert(m_running);

		// Recieve packet
		NetPacket packet;
		while(true)
		{
			NetAddress sender;
			int bytesRead = m_socket.receive(sender, packet.data(), 256);
			if(bytesRead == 0)
				break;
			if(bytesRead < 13)
				continue;

			// Read data
			uint zero;
			uint protocolId;
			uint serverPort;
			uchar nameSize;
			packet.readInteger(zero);
			packet.readInteger(protocolId);
			packet.readInteger(serverPort);
			packet.readByte(nameSize);

			// Validate data
			if(zero != 0)
				continue;
			if(protocolId != m_protocolId)
				continue;
			if(nameSize > 63)
				continue;
			if(nameSize + 13 > bytesRead)
				continue;

			// Create listener entry
			ListenerEntry entry;
			entry.address = NetAddress(sender.a(), sender.b(), sender.c(), sender.d(), serverPort);
			entry.timeoutAccum = 0.0f;

			// Check if entry already exists
			ListenerEntry *existingEntry = findEntry(entry);
			if(existingEntry)
				existingEntry->timeoutAccum = 0.0f;
			else
				m_entries.push_back(entry);
		}

		vector<ListenerEntry>::iterator itr = m_entries.begin();
		while(itr != m_entries.end())
		{
			// Check for timeout
			itr->timeoutAccum += dt;
			if(itr->timeoutAccum > m_timeout)
				itr = m_entries.erase(itr);
			else
				++itr;
		}
	}
		
	int entryCount() const
	{
		return m_entries.size();
	}
		
	const ListenerEntry &entryAt(uint index) const
	{
		// Return entry at index
		assert(index < m_entries.size());
		return m_entries[index];
	}
		
protected:
		
	ListenerEntry *findEntry(const ListenerEntry &entry)
	{
		for(uint i = 0; i < m_entries.size(); i++)
		{
			if(m_entries[i].address == entry.address &&
				m_entries[i].name == entry.name)
				return &m_entries[i];
		}
		return NULL;
	}
		
private:
		
	void clearData()
	{
		m_entries.clear();
	}
		
	vector<ListenerEntry> m_entries;
	uint m_protocolId;
	float m_timeout;
	bool m_running;
	Socket m_socket;
};

#endif // NET_BEACON_H