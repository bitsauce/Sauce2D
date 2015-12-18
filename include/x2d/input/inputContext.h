#ifndef X2D_INPUT_CONTEXT_H
#define X2D_INPUT_CONTEXT_H

#include <x2d/config.h>

BEGIN_CG_NAMESPACE

/*********************************************************************
**	Input context													**
**********************************************************************/

class Game;

class XDAPI InputContext
{
	friend class Input;
public:
	/**
	 * \brief Bind a function to an action for this input context.
	 */
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

	Game *m_game;
	map<string, KeyBind> m_nameToFunc;
	map<string, VirtualKey> m_nameToKey;
};

END_CG_NAMESPACE

#endif // X2D_INPUT_CONTEXT_H