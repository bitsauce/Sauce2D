#ifndef MATH_GRID_H
#define MATH_GRID_H

#include "x2d/platform.h"
#include "x2d/scripts.h"

void RegisterGrid();

struct SArrayBuffer;

class Grid
{
public:
	Grid(ScriptType *type, uint width = 0, uint height = 0, void *val = 0);
	~Grid();

	// Size
	void resize(uint width, uint height);
	void resize(uint width, uint height, void *value);
	void insertCol(const uint at, void *value);
	void removeCol(const uint at);
	void insertRow(const uint at, void *value);
	void removeRow(const uint at);
	void clear();
	uint width() const;
	uint height() const;
	
	// Accessing
	const void *at(uint x, uint y) const;
	void *at(uint x, uint y);
	bool isIndex(uint x, uint y) const;
	
	// Array operators
	Grid &operator=(const Grid&);
	//bool operator==(const Grid&) const;

private:

	// Properties
	SArrayBuffer *buffer;
	ScriptType *arrayType;
	int subTypeId;
	int elementSize;
	uint gridWidth;
	uint gridHeight;

	// AngelScript
	mutable int  m_refCount;
	mutable bool m_gcFlag;

	// Internal
	void setValue(uint x, uint y, void *val);
	bool checkMaxSize(uint size);
	void createBuffer(SArrayBuffer **buf, uint size);
	void deleteBuffer(SArrayBuffer *buf);
	void copyBuffer(SArrayBuffer *dst, SArrayBuffer *src);
	void construct(SArrayBuffer *buf, uint start, uint end);
	void destruct(SArrayBuffer *buf, uint start, uint end);
	void resize(int dw, int dh, uint at);
	//bool equals(const void *a, const void *b, ScriptContext *ctx) const;
	
public:
	// AngelScript
	static bool TemplateCallback(asIObjectType *ot, bool &dontGarbageCollect);
	static Grid *GridFactory1(ScriptType *type);
	static Grid *GridFactory2(ScriptType *type, uint width, uint heigth);
	static Grid *GridFactory3(ScriptType *type, uint width, uint heigth, void *val);
	void enumReferences(ScriptEngine *engine);
	void releaseAllHandles(ScriptEngine *engine);
	void addRef() const;
	void releaseRef() const;
	int  refCount() const;
	void garbageCollect();
	bool gcFlag();
};

#endif // MATH_GRID_H