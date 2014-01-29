//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)
// Based on "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers

#include "packet.h"

//
// Helper functions for reading and writing values to packets
//

// A byte
void NetPacket::writeByte(const uchar value)
{
	*(m_data+m_offset) = value;
	m_offset++;
}

void NetPacket::readByte(uchar &value)
{
	value = *(m_data+m_offset);
	m_offset++;
}

// Bytes
void NetPacket::writeBytes(const uchar* value, int size)
{
	memcpy(m_data+m_offset, value, size);
	m_offset += size;
}

void NetPacket::readBytes(uchar* &value, int size)
{
	memcpy(m_data+m_offset, value, size);
	m_offset += size;
}

// Short
void NetPacket::writeShort(const ushort value)
{
	m_data[m_offset]	= (uchar)((value >> 8) & 0xFF);
	m_data[m_offset+1]	= (uchar)(value & 0xFF);
	m_offset += 2;
}

void NetPacket::readShort(ushort &value)
{
	value = (((uchar)m_data[m_offset] << 8) | ((uchar)m_data[m_offset+1]));	
	m_offset += 2;		
}

// Bool
void NetPacket::writeBool(const bool value)
{
	m_data[m_offset] = (uchar)value;
	m_offset++;
}

void NetPacket::readBool(bool &value)
{
	value = m_data[m_offset] != 0;	
	m_offset++;
}

// Integer
void NetPacket::writeInteger(const uint value)
{
	m_data[m_offset]	= (uchar)(value >> 24);
	m_data[m_offset+1]	= (uchar)((value >> 16) & 0xFF);
	m_data[m_offset+2]	= (uchar)((value >> 8) & 0xFF);
	m_data[m_offset+3]	= (uchar)(value & 0xFF);
	m_offset += 4;
}

void NetPacket::readInteger(uint &value)
{
	value = (((uchar)m_data[m_offset] << 24) | ((uchar)m_data[m_offset+1] << 16) | 
			    ((uchar)m_data[m_offset+2] << 8)  | ((uchar)m_data[m_offset+3]));
	m_offset += 4;
}

// Float
void NetPacket::writeFloat(const float value)
{
	uint floatInt		= (uint)value * 10 * FLOAT_DECIMALS;
	m_data[m_offset]	= (uchar)(floatInt >> 24);
	m_data[m_offset+1]	= (uchar)((floatInt >> 16) & 0xFF);
	m_data[m_offset+2]	= (uchar)((floatInt >> 8) & 0xFF);
	m_data[m_offset+3]	= (uchar)(floatInt & 0xFF);
	m_offset += 4;
}
	
void NetPacket::readFloat(float &value)
{
	uint floatInt = (((uchar)m_data[m_offset] << 24) | ((uchar)m_data[m_offset+1] << 16) | 
							((uchar)m_data[m_offset+2] << 8)  | ((uchar)m_data[m_offset+3]));	
	value = (float)floatInt/(10 * FLOAT_DECIMALS);
	m_offset += 4;
}