#include "x2d/debug.h"

#include "x2d/engine.h"
#include "x2d/assetloader.h"
#include "x2d/iosystem.h"
#include "angelscript.h"

bool X2DDebug::connect()
{
	// Block until timeout
	ulong beginTick = GetTickCount();
	bool success = false;
	while(!(success = accept()) && (GetTickCount() - beginTick < m_timeoutValue));
	m_connected = success;

	// Check if client is invalid
	if(!success) {
		iosystem::error("accept() failed!");
		disconnect();
		return false;
	}
	return true;
}

void X2DDebug::sendPacket(PacketType type, const char *data)
{
	// Make sure we have a connection
	if(!m_connected)
		return;

	// Create packet
	char packet[512];
	packet[0] = type;
	if(data != 0) memcpy(&packet[1], data, 511);

	// Send data
	ulong beginTick = GetTickCount();
	bool success = false;
	while(!(success = send(packet)) && (GetTickCount() - beginTick < m_timeoutValue));

	// Sending failed
	if(!success) {
		m_connected = false;
		iosystem::warn("Lost connection to external debugger");
		disconnect();
		return;
	}

	// Wait for async data
	char *recvData = new char[512];
	memset(recvData, 0, 512);
	recvPacket(&recvData);
	if(recvData[0] != 0x01)
	{
		// Incorrect async data
		m_connected = false;
		iosystem::warn("Lost connection to external debugger");
		disconnect();
	}
	delete[] recvData;
}

void X2DDebug::recvPacket(char **data)
{
	// Make sure we have a connection
	if(!m_connected)
		return;

	// Receive info
	ulong beginTick = GetTickCount();
	bool success = false;
	while(((bytesReady() < 512) || !(success = recv(data))) && (GetTickCount() - beginTick < m_timeoutValue));

	if(!success) {
		// Error on recieve async data
		m_connected = false;
		iosystem::error("recv() failed with error code '%i'!", WSAGetLastError());
		disconnect();
	}
}

void X2DDebug::sendMessage(const string &data, const X2DMessageType type)
{
	// Make sure we have a connection
	if(!m_connected)
		return;

#ifdef X2D_SIMPLE
	sendData(data.c_str(), MessagePacket);
#else
	string colorTag = "";
	switch(type) {
	case X2D_SuccessMsg: colorTag = "#00adeb"; break;
	case X2D_WarningMsg: colorTag = "#e6a000"; break;
	case X2D_ErrorMsg: colorTag = "#de0000"; break;
	default: break;
	}
	sendPacket(MessagePacket, (colorTag + data).c_str());
#endif
}

void X2DDebug::lineCallback(void *ctxptr)
{
	// Cast ctx
	asIScriptContext *ctx = static_cast<asIScriptContext*>(ctxptr);

	// Check for actions
	while(bytesReady() >= 512)
	{
		char *data = new char[512];
		recvPacket(&data);
		switch(data[0])
		{
		case Interupt:
			m_command = (Command)data[0];
			break;
		case AddBreakpoint:
			{
				string bpstr(data+1);
				vector<string> bpstings = iosystem::split(bpstr, ";");
				if(bpstings.size() == 2)
				{
					string file = bpstings[0];
					int line = math::strToInt(bpstings[1])+1;
					Breakpoint bp = { file, line };
					m_breakpoints.push_back(bp);
				}
			}
			break;
		case RemoveBreakpoint:
			{
				string bpstr(data+1);
				vector<string> bpstings = iosystem::split(bpstr, ";");
				if(bpstings.size() == 2)
				{
					string file = bpstings[0];
					int line = math::strToInt(bpstings[1]);
					for(list<Breakpoint>::iterator itr = m_breakpoints.begin(); itr != m_breakpoints.end(); ++itr)
					{
						Breakpoint &bp = (*itr);
						if(bp.file == file && bp.line == line)
						{
							m_breakpoints.remove(bp);
							break;
						}
					}
				}
			}
			break;
		default:
			iosystem::error("Unknown packet type '%i'", data[0]);
			break;
		}
		delete[] data;
	}

	// Perform debug actions
	if(m_command == NoCommand)
	{
		// Continue until breakpoint
		if(!isBreakpoint(ctx))
			return;
	}else if(m_command == StepOver)
	{
		// Continue until stack size is equal...
		if(ctx->GetCallstackSize() > m_prevStackSize)
		{
			// ... or if a breakpoint is hit
			if(!isBreakpoint(ctx))
				return;
		}
	}else if(m_command == StepOut)
	{
		// Continue until stack size is smaller than previous...
		if(ctx->GetCallstackSize() >= m_prevStackSize) // TODO: ctx->GetCallstackSize != 0?
		{
			// ... or if a breakpoint is hit
			if(!isBreakpoint(ctx))
				return;
		}
	}else if(m_command == StepInto)
	{
		// We always break, but we call the check break point anyway 
		// to tell user when break point has been reached
		isBreakpoint(ctx);
	}

	// Breakpoint hit
	takeCommands(ctx);
}

bool X2DDebug::isBreakpoint(void *ctxptr)
{
	// Cast ctx
	asIScriptContext *ctx = static_cast<asIScriptContext*>(ctxptr);

	// Get line number and file
	const char* tmp;
	int line = ctx->GetLineNumber(0, 0, &tmp);
	if(!tmp) return false;

	// Get file as string
	string file = ":/";
	file += tmp;
	gameEngine->assetLoader->transformFilePath(file);
	std::replace(file.begin(), file.end(), '\\', '/');

	// Check if there is an breakpoint
	for(list<Breakpoint>::iterator itr = m_breakpoints.begin(); itr != m_breakpoints.end(); ++itr)
	{
		Breakpoint &bp = (*itr);
		if(bp.file == file && bp.line == line)
			return true;
	}
	return false;
}

void X2DDebug::takeCommands(void *ctxptr)
{
	// Cast ctx
	asIScriptContext *ctx = static_cast<asIScriptContext*>(ctxptr);

	// Get line number and file
	const char* tmp;
	int line = ctx->GetLineNumber(0, 0, &tmp);
	if(!tmp) return;
	
	// Get file as string
	string bpstring = ":/";
	bpstring += tmp;
	gameEngine->assetLoader->transformFilePath(bpstring);
	std::replace(bpstring.begin(), bpstring.end(), '\\', '/');

	// Tell the remote debugger a breakpoint was hit
	bpstring += ";" + math::intToStr(line-1);
	sendPacket(BreakpointPacket, bpstring.c_str());

	// Send local variable information
	asIScriptFunction *func = ctx->GetFunction();
	/*if(func)
	{
		// Send name of variables in scope
		for(uint i = 0; i < func->GetVarCount(); i++)
		{
			if(ctx->IsVarInScope(i))
			{
				// Get variable info
				void *var = ctx->GetAddressOfVar(i);
				int typeId = ctx->GetVarTypeId(i);

				// Get variable name
				string decl(func->GetVarDecl(i));
				string name = decl.substr(decl.find_first_of(' ')+1, decl.size()-decl.find_first_of(' ')-1);

				// Send variable
				//DebugVarType varType = ToVarType(var, typeId);
				//if(varType == TextVar)
				//{
					// String variable
					sendData(string(name + "=" + toString(var, typeId)).c_str(), TextVariableType);
			//	}else if(varType == TextureVar)
			//	{
			//		// Texture variable
			//		SendString("dt");

			//		// Send name = width = height
			//		Texture *tex = (Texture*)var;
			//		SendString(name + "=" + itos(tex->getWidth()) + "=" + itos(tex->getHeight()));

			//		// Get pixels
			//		vector<vector<Color>> pixels = tex->getPixels();

			//		// Send texture data in chunks of 512 bytes
			//		string bytes;
			//		for(uint x = 0; x < pixels.size(); x++)
			//		{
			//			for(uint y = 0; y < pixels.size(); y++)
			//			{
			//				// Read pixels
			//				bytes += idtriplet(pixels[x][y].red);
			//				bytes += idtriplet(pixels[x][y].green);
			//				bytes += idtriplet(pixels[x][y].blue);
			//				bytes += idtriplet(pixels[x][y].alpha);

			//				// Check if we have 512 bytes
			//				if(bytes.size() >= 512-1)
			//				{
			//					// Send chunk
			//					string chunk = bytes.substr(0, 511); 
			//					SendString(chunk);
			//					bytes = bytes.substr(511);
			//				}
			//			}
			//		}

			//		// Send remainder
			//		if(bytes.size() > 0) SendString(bytes);
			//	}
				
				// Send variable members
				if(typeId & asTYPEID_SCRIPTOBJECT && 
					!(typeId & asTYPEID_OBJHANDLE) )
					SendMembers(name, (ScriptObject*)var);
			}
		}
	}*/
	
	// Wait for command
	while(true)
	{
		if(bytesReady() < 512)
			continue;

		// Decode packet
		char *data = new char[512];
		recvPacket(&data);
		bool commandRecv = false;
		switch(data[0])
		{
		case Continue:
		case StepOver:
		case StepInto:
		case StepOut:
			commandRecv = true;
			m_command = (Command)data[0];
			break;
		default:
			commandRecv = false;
			break;
		}
		if(commandRecv)
			break;
	}

	// Check if additional debug information is required
	if(m_command == StepOver)
	{
		// Store stack size
		m_prevStackSize = ctx->GetCallstackSize();
	}
	else if(m_command == StepOut)
	{
		// Get stack size
		int stackSize = ctx->GetCallstackSize();
		
		// If the stack size is one we would get stuck
		if(stackSize == 1)
			m_command = NoCommand; // ... Therefore we just continue
		else
			m_prevStackSize = stackSize;
	}
}

/*string WinDebug::toString(void *value, uint typeId)
{
	stringstream s;
	if(typeId == asTYPEID_VOID)
		s << "<void>";
	else if(typeId == asTYPEID_BOOL)
		s << *(bool*)value ? "true" : "false";
	else if(typeId == asTYPEID_INT8)
		s << (int)*(signed char*)value;
	else if(typeId == asTYPEID_INT16)
		s << (int)*(signed short*)value;
	else if(typeId == asTYPEID_INT32)
		s << *(signed int*)value;
	else if(typeId == asTYPEID_INT64)
		s << *(asINT64*)value;
	else if(typeId == asTYPEID_UINT8)
		s << (unsigned int)*(unsigned char*)value;
	else if(typeId == asTYPEID_UINT16)
		s << (unsigned int)*(unsigned short*)value;
	else if(typeId == asTYPEID_UINT32)
		s << *(unsigned int*)value;
	else if(typeId == asTYPEID_UINT64)
		s << *(asQWORD*)value;
	else if(typeId == asTYPEID_FLOAT)
		s << *(float*)value;
	else if(typeId == asTYPEID_DOUBLE)
		s << *(double*)value;
	else if((typeId & asTYPEID_MASK_OBJECT) == 0)
	{
		// The type is an enum
		s << *(asUINT*)value;

		// Check if the value matches one of the defined enums
		for(int i = 0; i < scriptEngine->GetEnumValueCount(typeId); i++)
		{
			int enumVal;
			const char *enumName = scriptEngine->GetEnumValueByIndex(typeId, i, &enumVal);
			if(enumVal == *(int*)value)
			{
				s << ", " << enumName;
				break;
			}
		}
	}
	else if(typeId & asTYPEID_SCRIPTOBJECT)
	{
		// Dereference handles, so we can see what it points to
		if(typeId & asTYPEID_OBJHANDLE)
			value = *(void**)value;

		// Print address
		ScriptObject *obj = (ScriptObject*)value;
		s << "{" << obj << "}";
	}
	else
	{
		// Dereference handles, so we can see what it points to
		if(typeId & asTYPEID_OBJHANDLE)
			value = *(void**)value;

		ScriptObject *obj = (ScriptObject*)value;
		if(obj)
		{
			// Handle string
			if((int)typeId == scriptEngine->GetTypeIdByDecl("string"))
			{
				// String type
				s << "\"" << *(string*)value << "\"";
			}else if((int)typeId == scriptEngine->GetTypeIdByDecl("vec2")){
				// vec2 type
				vec2 p = *(vec2*)value;
				s << "(" << p.x << ", " << p.y << ")";
			}else{
				// Just print the address
				s << "{" << value << "}";
			}
		}
	}

	return s.str();
}*/