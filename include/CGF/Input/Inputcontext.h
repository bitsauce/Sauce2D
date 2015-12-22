#ifndef CGF_INPUT_CONTEXT_H
#define CGF_INPUT_CONTEXT_H

#include <CGF/config.h>

BEGIN_CGF_NAMESPACE

/*********************************************************************
**	Input context													**
**********************************************************************/

class Game;

class CGF_API InputContext
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

END_CGF_NAMESPACE

#endif // CGF_INPUT_CONTEXT_H
