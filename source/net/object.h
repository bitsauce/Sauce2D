#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "net/NetPacket.h"

class Object
{
public:
	Object();
	~Object();

	void packUpdate(net::NetPacket* packet);
	void unpackUpdate(net::NetPacket* packet);

	void update();
	void interpolate();

	bool isServerObject() { return isServerObject; };
	bool isClientObject() { return !isServerObject; };

private:
	float x, y;
	float dir;
	bool isServerObject;
};
extern vector<Object*> mSceneObjects;

#endif