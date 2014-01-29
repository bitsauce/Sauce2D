#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

// Internet Protocol address
class NetAddress
{
public:
	
	// Address object
	NetAddress()
	{
		m_address = 0;
		m_port = 0;
	}
	
	NetAddress(uchar a, uchar b, uchar c, uchar d, ushort port)
	{
		m_address = (a << 24) | (b << 16) | (c << 8) | d;
		m_port = port;
	}
	
	NetAddress(uint address, ushort port)
	{
		m_address = address;
		m_port = port;
	}
	
	uint address() const
	{
		return m_address;
	}
	
	uchar a() const
	{
		return (uchar)(m_address >> 24);
	}
	
	uchar b() const
	{
		return (uchar)(m_address >> 16);
	}
	
	uchar c() const
	{
		return (uchar)(m_address >> 8);
	}
	
	uchar d() const
	{
		return (uchar)(m_address);
	}
	
	ushort port() const
	{ 
		return m_port;
	}
	
	bool operator==(const NetAddress &other) const
	{
		return m_address == other.m_address && m_port == other.m_port;
	}
	
	bool operator!=(const NetAddress &other) const
	{
		return !(*this == other);
	}

	bool operator<(const NetAddress &other) const
	{
		return m_address < other.m_address;
	}

private:
	
	uint m_address;
	ushort m_port;
};

#endif // NET_ADDRESS