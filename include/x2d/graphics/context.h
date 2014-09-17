#ifndef X2D_CONTEXT_H
#define X2D_CONTEXT_H

#include "../engine.h"

class XRenderContext
{
public:
	virtual void makeCurrent() = 0;

protected:
	virtual ~XRenderContext() {}
};

#endif // X2D_CONTEXT_H