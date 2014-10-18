//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

XDebugger::XDebugger() :
	m_profiler(),
	m_command(NoCommand),
	m_prevStackSize(0),
	m_prevFunc(0),
	m_prevSystemFunc(0)
{
	m_profiler.m_debugger = this;
}

// This mutex is necessary for making sure no other thread reads the async
// packet before the thread awaiting it reads it.
mutex sockmtx;

bool XDebugger::sendPacket(XPacketType type, string data)
{
	//sockmtx.lock();

	// Create packet
	int packetSize = data.size() + 1;
	char *packet = new char[packetSize + 1];
	packet[0] = type;
	memcpy((void*)(packet + 1), data.c_str(), data.size());
	packet[packetSize] = '\0';

	// Send packet
	if(send(packet, packetSize))
	{
		// Wait for ack
		if(!recv(packet, 1) || packet[0] != XD_ACK_PACKET)
		{
			// Kill the debugger
			m_engine->killDebugger();
			LOG("XDebugger::sendPacket(): No ack packet recieved (received %i).", packet[0]);
			return false;
		}

		// Receive the rest for later processing
		string str;
		recvPacket(str, false);
		m_packet += str;
	}
	else
	{
		// Kill the debugger
		m_engine->killDebugger();
		LOG("XDebugger::sendPacket(): Unable to communicate with the debugger.");
		return false;
	}

	delete[] packet;
	//sockmtx.unlock();
	return true;
}

bool XDebugger::recvPacket(string &data, const bool blocking)
{
	// Blocking call?
	if(blocking)
	{
		// Block until there are bytes ready
		while(bytesReady() == 0);
	}
	else
	{
		// Skip when non-blocking and there is no ready packet
		if(bytesReady() == 0)
		{
			return false;
		}
	}

	// Receive packet
	int size = bytesReady();
	char *packet = new char[size + 1];
	if(!recv(packet, size))
	{
		// Kill the debugger
		m_engine->killDebugger();
		LOG("DEBUGGER: Failed to receieve packet.");
		return false;
	}
	packet[size] = '\0';

	data = packet;

	delete[] packet;

	return true;
}

void XDebugger::funcBeginCallback(asIScriptFunction *func)
{
	m_profiler.push(func->GetDeclaration());
}

void XDebugger::funcEndCallback(asIScriptFunction *func)
{
	m_profiler.pop();
}

void XDebugger::lineCallback(asIScriptContext *ctx)
{
	// Process packet
	if(!m_packet.empty())
	{
		for(int i = 0; i < m_packet.size(); i++)
		{
			switch(m_packet[i])
			{
			case XD_START_PROFILER: m_profiler.enable(); break;
			case XD_STOP_PROFILER: m_profiler.disable(); break;
			}
		}

		m_packet.clear();
	}

	// Check for actions
	/*sockmtx.lock();
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
				vector<string> bpstings = util::splitString(bpstr, ";");
				if(bpstings.size() == 2)
				{
					string file = bpstings[0];
					int line = util::strToInt(bpstings[1])+1;
					Breakpoint bp = { file, line };
					m_breakpoints.push_back(bp);
				}
			}
			break;
		case RemoveBreakpoint:
			{
				string bpstr(data+1);
				vector<string> bpstings = util::splitString(bpstr, ";");
				if(bpstings.size() == 2)
				{
					string file = bpstings[0];
					int line = util::strToInt(bpstings[1]);
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
			LOG("DEBUGGER: Unknown packet type '%i'.", data[0]);
			break;
		}
		delete[] data;
	}
	sockmtx.unlock();

	// Perform debug actions
	if(isBreakpoint(ctx))
		takeCommands(ctx);

	/*if(m_command == NoCommand)
	{
		// Continue until breakpoint
		if(!isBreakpoint(ctx))
			return;
	}
	else if(m_command == StepOver)
	{
		// Continue until stack size is equal...
		if(ctx->GetCallstackSize() > m_prevStackSize)
		{
			// ... or if a breakpoint is hit
			if(!isBreakpoint(ctx))
				return;
		}
	}
	else if(m_command == StepOut)
	{
		// Continue until stack size is smaller than previous...
		if(ctx->GetCallstackSize() >= m_prevStackSize) // TODO: ctx->GetCallstackSize != 0?
		{
			// ... or if a breakpoint is hit
			if(!isBreakpoint(ctx))
				return;
		}
	}
	else if(m_command == StepInto)
	{
		// We always break, but we call the check break point anyway 
		// to tell user when break point has been reached
		isBreakpoint(ctx);
	}*/
}

bool XDebugger::isBreakpoint(asIScriptContext *ctx)
{
	// Get line number and file
	const char* tmp;
	int line = ctx->GetLineNumber(0, 0, &tmp);
	if(!tmp) return false;

	// Get file path as string
	string filePath = ":/";
	filePath.append(tmp);
	util::toAbsoluteFilePath(filePath);
	replace(filePath.begin(), filePath.end(), '\\', '/');

	// Check if there is an breakpoint
	for(list<Breakpoint>::iterator itr = m_breakpoints.begin(); itr != m_breakpoints.end(); ++itr)
	{
		Breakpoint &bp = (*itr);
		if(bp.file == filePath && bp.line == line)
			return true;
	}
	return false;
}

void XDebugger::takeCommands(asIScriptContext *ctx)
{
	// Get line number and file
	const char* tmp;
	int line = ctx->GetLineNumber(0, 0, &tmp);
	if(!tmp) return;
	
	// Get file path as string
	string filePath = ":/";
	filePath.append(tmp);
	util::toAbsoluteFilePath(filePath);
	replace(filePath.begin(), filePath.end(), '\\', '/');

	// Tell the remote debugger a breakpoint was hit
	filePath += ";" + util::intToStr(line-1);
	sendPacket(XD_BREAK_PACKET, filePath);

	// Send local variable information
	/*asIScriptFunction *func = ctx->GetFunction();
	if(func)
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
	}
	
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
	}*/
}