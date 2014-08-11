#ifndef SCRIPTS_SERIALIZER_H
#define SCRIPTS_SERIALIZER_H

#include <x2d/base.h>
#include <x2d/util.h>

class Serializer
{
public:
	AS_DECL_REF

	Serializer(stringstream &ss);

	void store(void *value, int typeId);
	void store(asINT64 &value);
	void store(double &value);
	void store(string &value);

	void restore(void *value, int typeId);
	void restore(asINT64 &value);
	void restore(double &value);
	void restore(string &value);

private:

	stringstream &m_data;
};

#endif // SCRIPTS_SERIALIZER_H