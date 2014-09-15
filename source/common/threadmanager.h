#ifndef X2D_THREAD_MANAGER_H
#define X2D_THREAD_MANAGER_H

class xdIThreadManager
{
public:
	virtual void setupThread() = 0;
	virtual void cleanupThread() = 0;
};

#endif // X2D_THREAD_MANAGER_H