#ifndef X2D_INPUT_CONTEXT_H
#define X2D_INPUT_CONTEXT_H

#include <x2d/config.h>

BEGIN_XD_NAMESPACE

/*********************************************************************
**	Input context													**
**********************************************************************/

class XDAPI InputContext
{
	friend class Input;
public:

	void bind(const string &action, function<void(int)> function, const bool singleShot = false);

private:
	void updateBindings();

	// Key bind
	struct KeyBind
	{
		bool pressed;
		function<void(int)> function;
		bool singleShot;
	};

	map<string, KeyBind> m_nameToFunc;
	map<string, VirtualKey> m_nameToKey;
};

END_XD_NAMESPACE

#endif // X2D_INPUT_CONTEXT_H