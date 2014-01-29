#ifndef X2D_DEBUG_H
#define X2D_DEBUG_H

#include "x2d/platform.h"

/*********************************************************************
**	Default Debugger												**
**********************************************************************/
class X2DDebug
{
public:
	X2DDebug() :
		m_connected(false),
		m_command(NoCommand),
		m_prevStackSize(0),
		m_timeoutValue(60000)
	{
	}

	// Packet types
	enum PacketType {
		NullPacket = 0x00,
		ConnectedPacket,
		InitializedPacket,
		MessagePacket,
		CompilePacket,
		BreakpointPacket,
		TextVariablePacket,
		ImageVariablePacket,
	};
	
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
	void sendPacket(PacketType type, const char *data = 0);
	void sendMessage(const string &data, const X2DMessageType type);
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
	int m_prevStackSize;
	uint m_timeoutValue;
	list<Breakpoint> m_breakpoints;
};

#endif // X2D_DEBUG_H