#ifndef NET_PACKET_H
#define NET_PACKET_H

#include "platform/platform.h"

#define PACKET_MAX_SIZE 1024 // In bytes
#define FLOAT_DECIMALS 2

class NetPacket
{
public:
	NetPacket() { m_data = new uchar[PACKET_MAX_SIZE]; m_offset = 0; };
	~NetPacket() { delete m_data; };

	// Write/Read
	void writeByte(const uchar value);
	void readByte(uchar &value);

	void writeBytes(const uchar* value, int size);
	void readBytes(uchar* &value, int size);

	//void writeString(const string value);
	//void readString(string &value);

	void writeShort(const ushort value);
	void readShort(ushort &value);

	void writeBool(const bool value);
	void readBool(bool &value);

	void writeInteger(const uint value);
	void readInteger(uint &value);

	void writeFloat(const float value);
	void readFloat(float &value);

	// Packet info
	uchar* data() { return m_data; }
	uint size() { return m_offset; }
	uint offset() { return m_offset; }

private:
	unsigned char* m_data;
	uint m_offset;
};

#endif // NET_PACKET_H