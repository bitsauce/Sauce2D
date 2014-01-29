/*
	Server Example using Transport Layer
	From "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#include "main.h"
#include "object.h"
#include <windows.h>

vector<Object*> mSceneObjects;
Object::Object()
{
	x = 0;
	y = 0;
	isServerObject = true;
	mSceneObjects.push_back(this);
}

Object::~Object()
{
}

// unpackUpdate() is called each time a
// new packet is received from the server.
// The function is supposed unpack all the packet
// information about the object (in the same order as it is packet).
void Object::unpackUpdate(net::NetPacket* packet)
{
	float prevx = x;
	float prevy = y;
	packet->ReadFloat(x);
	packet->ReadFloat(y);
	
	float xcoor = x-prevx;
	float ycoor = y-prevy;

	dir = ;
}

// packUpdate() is called each time a
// new packet is about to be sent to
// all it's clients. The function is supposed
// to write packet information about the object.
void Object::packUpdate(net::NetPacket* packet)
{
	packet->WriteFloat(x);
	packet->WriteFloat(y);
}

// interpolate() should be called inbetween
// packet receiveing. The function is supposed
// to perdict how the object will behave
// inbetween the packet sending/receiveing
void Object::interpolate()
{
	x += ;
	y += ;
}

// update() should be called both client
// and server side
void Object::update()
{
	if(isServerObject)
	{
		if(GetAsyncKeyState(VK_UP))
			y += 0.5f;
		if(GetAsyncKeyState(VK_DOWN))
			y -= 0.5f;
		if(GetAsyncKeyState(VK_RIGHT))
			x += 0.5f;
		if(GetAsyncKeyState(VK_LEFT))
			x -= 0.5f;
	}
}