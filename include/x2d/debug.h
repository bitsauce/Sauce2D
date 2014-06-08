#ifndef X2D_DEBUG_H
#define X2D_DEBUG_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/console.h>

// Packet types
enum xdPacketType
{
	XD_NULL_PACKET = 0x00,
	XD_CONNECTED_PACKET,
	XD_INITIALIZED_PACKET,
	XD_MESSAGE_PACKET,
	XD_COMPILE_PACKET,
	XD_BREAK_PACKET,
	XD_TEXT_VAR_PACKET,
	XD_IMAGE_VAR_PACKET
};

/*********************************************************************
**	Default Debugger												**
**********************************************************************/
class xdDebug
{
public:
	xdDebug() :
		m_connected(false),
		m_command(NoCommand),
		m_prevStackSize(0),
		m_timeoutValue(60000)
	{
	}
	
	virtual ~xdDebug() {
	}

	/*********************************************************************
	**	Virtual part													**
	**********************************************************************/
	virtual bool init()											{ NOT_IMPLEMENTED_RET(init, false) }
	virtual void setTimeoutValue(const uint ms)					{ NOT_IMPLEMENTED(setTimeoutValue) }
	virtual bool listen(const ushort port)						{ NOT_IMPLEMENTED_RET(listen, false) }
	virtual bool accept()										{ NOT_IMPLEMENTED_RET(accept, false) }
	virtual void disconnect()									{ NOT_IMPLEMENTED(disconnect) }
	virtual bool send(const char *data)							{ NOT_IMPLEMENTED_RET(send, false) }
	virtual bool recv(char **data)								{ NOT_IMPLEMENTED_RET(recv, false) }
	virtual int  bytesReady()									{ NOT_IMPLEMENTED_RET(bytesReady, 0) }
	
	/*********************************************************************
	**	Implemented part												**
	**********************************************************************/
	bool connect();
	void sendPacket(xdPacketType type, const char *data = 0);
	void recvPacket(char **data);

	void lineCallback(void *ctx);
	bool isBreakpoint(void *ctx);
	void takeCommands(void *ctx);

private:
	enum Command
	{
		NoCommand = 0x00,
		Continue,
		Interupt,
		StepOver,
		StepInto,
		StepOut,
		AddBreakpoint,
		RemoveBreakpoint
	};

	struct Breakpoint
	{
		string file;
		int line;
        bool operator==(const Breakpoint &other) {
            return file == other.file && line == other.line;
        }
	};

	bool m_connected;
	Command m_command;
	uint m_prevStackSize;
	uint m_timeoutValue;
	list<Breakpoint> m_breakpoints;
};

#endif // X2D_DEBUG_H