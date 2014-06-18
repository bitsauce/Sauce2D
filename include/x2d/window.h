#ifndef X2D_WINDOW_H
#define X2D_WINDOW_H

#include <x2d/config.h>
#include <x2d/engine.h>
#include <x2d/console.h>
#include <x2d/math.h>
#include <x2d/base.h>
#include <x2d/scripts/array.h>

class XDAPI xdWindow
{
public:
	AS_DECL_SINGLETON

	virtual ~xdWindow() {}

	virtual void processEvents() { NOT_IMPLEMENTED(processEvents) }

	// Virtual window functions
	virtual void enableFullscreen()									{ NOT_IMPLEMENTED(enableFullscreen) }
	virtual void disableFullscreen()								{ NOT_IMPLEMENTED(disableFullscreen) }
	virtual Array* resolutionList() const							{ NOT_IMPLEMENTED_ARR(resolutionList, "Vector2i") }

	// Window flags
	virtual void enableResize()										{ NOT_IMPLEMENTED(enableResize) }
	virtual void disableResize()									{ NOT_IMPLEMENTED(disableResize) }
	virtual bool hasFocus()											{ NOT_IMPLEMENTED_RET(hasFocus, true) }

	// Window actions
	virtual void     setPosition(const Vector2i &pos)				{ NOT_IMPLEMENTED(move) }
	virtual Vector2i getPosition() const							{ NOT_IMPLEMENTED_RET(pos, Vector2i(0)) }
	virtual void     setSize(const Vector2i &size)					{ NOT_IMPLEMENTED(resize) }
	virtual Vector2i getSize() const								{ NOT_IMPLEMENTED_RET(size, Vector2i(0)) }
	virtual void	 minimize()										{ NOT_IMPLEMENTED(minimize) }
	virtual void	 maximize()										{ NOT_IMPLEMENTED(maximize) }
	virtual void	 restore()										{ NOT_IMPLEMENTED(restore) }
};

#endif // X2D_WINDOW_H