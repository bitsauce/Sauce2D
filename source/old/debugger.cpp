//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "debugger.h"
#include "math/math.h"

#ifdef WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>

//----------------------------------------------------------------------------
// Debugger
//----------------------------------------------------------------------------

Debugger::Debugger() :
	m_server(INVALID_SOCKET),
	m_client(INVALID_SOCKET),
	m_prevStackSize(0),
	m_timeoutValue(60000),
	m_action(NoAction)
{
}

Debugger::~Debugger()
{
	disconnect();
}

bool Debugger::initSocket()
{
	// Initialize Winsock
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(r < 0) {
		// Error initializing socket
		print("Error: WSAStartup() failed with error code '%i'!", r);
		return false;
	}
	return true;
}

void Debugger::setTimeoutValue(const int ms)
{
	m_timeoutValue = ms;
}

bool Debugger::listen(ushort port)
{
	// Create address struct
	struct addrinfo *result = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	int r = getaddrinfo(NULL, itos(port).c_str(), &hints, &result);
	if(r < 0) {
		print("Error: getaddrinfo() failed with error code '%i'!", r);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for the server to listen for client connections
	m_server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(m_server == INVALID_SOCKET) {
		print("Error: socket() failed!");
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// Set non-blocking mode (Needed for runtime debug)
	ulong mode = 1;
	ioctlsocket(m_server, FIONBIO, &mode);

	// Setup the TCP listening socket
    r = bind(m_server, result->ai_addr, (int)result->ai_addrlen);
    if(r == SOCKET_ERROR) {
        print("Error: bind() failed with error code '%i'!", r);
        freeaddrinfo(result);
        closesocket(m_server);
        WSACleanup();
        return false;
    }
	freeaddrinfo(result);

	// Listen at the port
	r = ::listen(m_server, SOMAXCONN);
	if(r == SOCKET_ERROR) {
		print("Error: listen() failed with error code '%i'!", r);
		closesocket(m_server);
		WSACleanup();
		return false;
	}
	return true;
}

bool Debugger::connect()
{
	// Block until timeout
	ulong beginTick = GetTickCount();
	while(GetTickCount() - beginTick < m_timeoutValue)
	{
		// Look for client
		m_client = accept(m_server, NULL, NULL);
		if(m_client == INVALID_SOCKET)
			continue; // No client found, retry
		break;
	}

	// Check if client is invalid
	if(m_client == INVALID_SOCKET) {
		print("Error: accept() failed!");
		disconnect();
		return false;
	}
	return true;
}

void Debugger::disconnect()
{
	// Close client socket and clean up
	closesocket(m_client);
	WSACleanup();
}

void Debugger::sendData(const char *data, PacketType type)
{
	// Create packet
	char packet[512];
	packet[0] = char(type);
	memcpy(&packet[1], data, 511);

	// Send data
	ulong beginTick = GetTickCount();
	bool success = false;
	while(GetTickCount() - beginTick < m_timeoutValue)
	{
		// Try to send data until timeout
		if(send(m_client, packet, 512, 0) == SOCKET_ERROR)
			continue;
		success = true;
		break;
	}

	// Sending failed
	if(!success)
	{
		// Disconnect
		warn("WARN: Lost connection to external debugger");
		disconnect();
		return;
	}

	// Wait for async data
	const char *recvData = receiveData();
	if(recvData[0] != 0x01)
	{
		// Incorrect async data
		warn("WARN: Lost connection to external debugger");
		disconnect();
	}
	delete recvData;
}

const char *Debugger::receiveData()
{
	// Receive info
	int bytes = 0;
	ulong beginTick = GetTickCount();
	while(GetTickCount() - beginTick < m_timeoutValue)
	{
		if((bytes = bytesReady()) < 512)
			continue;
		break;
	}

	// Check if the correct amount was received
	if(bytes != 512) {
		// Incorrect async data
		warn("WARN: Lost connection to external debugger");
		disconnect();
		return "";
	}
	
	char *buffer = new char[512];
	int r = recv(m_client, buffer, 512, 0);
	if(r == SOCKET_ERROR) {
		// Error on recieve async data
		error(string("ERROR: recv() failed with error code '") +  itos(WSAGetLastError()) + string("'!"));
		disconnect();
		return "";
	}
	return buffer;
}

int Debugger::bytesReady()
{
	// Check bytes available
	ulong numBytes;
	ioctlsocket(m_client, FIONREAD, &numBytes);
	return numBytes;
}

void Debugger::sendData(PacketType type)
{
	// Overload
	sendData("", type);
}

void Debugger::lineCallback(ScriptContext *ctx)
{
	// Perform debug actions
	if(m_action == NoAction)
	{
		// Continue until breakpoint
		if(!isBreakpoint(ctx))
			return;
	}else if(m_action == StepOver)
	{
		// Continue until stack size is equal...
		if(ctx->GetCallstackSize() > m_prevStackSize)
		{
			// ... or if a breakpoint is hit
			if(!isBreakpoint(ctx))
				return;
		}
	}else if(m_action == StepOut)
	{
		// Continue until stack size is smaller than previous...
		if(ctx->GetCallstackSize() >= m_prevStackSize) // TODO: ctx->GetCallstackSize != 0?
		{
			// ... or if a breakpoint is hit
			if(!isBreakpoint(ctx))
				return;
		}
	}else if(m_action == StepInto)
	{
		// We always break, but we call the check break point anyway 
		// to tell user when break point has been reached
		isBreakpoint(ctx);
	}

	// Breakpoint hit
	takeCommands(ctx);
}

//int countDigits(int num)
//{
//	int count = 1;
//	float val = num;
//	while((val /= 10) > 1) count++;
//	return count;
//}
//
//string idtriplet(int id)
//{
//	int digits = countDigits(id);
//	string idtriplet = "000";
//	if(digits == 1) idtriplet = "00" + itos(id);
//	else if(digits == 2) idtriplet = "0" + itos(id);
//	else if(digits == 3) idtriplet = itos(id);
//	return idtriplet;
//}

void Debugger::takeCommands(ScriptContext *ctx) {}
/*{
	// Get line number and file
	const char* tmp;
	int line = ctx->GetLineNumber(0, 0, &tmp);
	if(!tmp) return;

	// Get file as string
	string file = tmp;
	replace(file.begin(), file.end(), '\\', '/');

	// Send local variable information
	ScriptFunction *func = ctx->GetFunction();
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

	// Breakpoint callback
	if(m_breakpointHit)
		m_breakpointHit(file.c_str(), line);

	// Check if additional debug information is required
	if(m_command == StepOverCommand)
	{
		// Store stack size
		m_prevStackSize = ctx->GetCallstackSize();

	}else if(m_command == StepOutCommand)
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

//
//	Breakpoint
//

void Debugger::addBreakpoint(const string file, const int line)
{
	Breakpoint breakpoint = { file, line };
	m_breakpoints.push_back(breakpoint);
}

void Debugger::removeBreakpoint(const string file, const int line)
{
	for(vector<Breakpoint>::iterator itr = m_breakpoints.begin(); itr != m_breakpoints.end(); ++itr)
	{
		Breakpoint &bp = (*itr);
		if(bp.file == file && bp.line == line)
		{
			m_breakpoints.erase(itr);
			break;
		}
	}
}

*/bool Debugger::isBreakpoint(ScriptContext *ctx)
{
	// Get line number and file
	const char* tmp;
	int line = ctx->GetLineNumber(0, 0, &tmp);
	if(!tmp) return false;

	// Get file as string
	string file = tmp;
	replace(file.begin(), file.end(), '\\', '/');

	// Check if there is an breakpoint
	for(uint i = 0; i < m_breakpoints.size(); i++)
	{
		if(m_breakpoints[i].file == file &&
			m_breakpoints[i].line == line)
		{
			// Breakpoint hit
			return true;
		}
	}

	return false;
}/*

// sendMembers():
// For recursively sending member variables
//void Debugger::SendMembers(string memberof, ScriptObject *obj)
//{
//	// Get object type
//	ScriptType *type = obj->GetObjectType();
//
//	// Loop properties
//	for(uint i = 0; i < obj->GetPropertyCount(); i++)
//	{
//		// Get property object and type
//		void *prop = obj->GetAddressOfProperty(i);
//		int typeId = obj->GetPropertyTypeId(i);
//
//		// Get property name
//		string decl(type->GetPropertyDeclaration(i));
//		string name = decl.substr(decl.find_first_of(' ')+1, decl.size()-decl.find_first_of(' ')-1);
//		
//		// Send member
//		DebugVarType varType = ToVarType(prop, typeId);
//		if(varType == TextVar)
//		{
//			// String variable
//			SendString("ds");
//			SendString(memberof + "." + name + "=" + ToString(prop, typeId));
//		}
//
//		// Send property members
//		if(typeId & asTYPEID_SCRIPTOBJECT && !(typeId & asTYPEID_OBJHANDLE))
//			SendMembers(memberof + "." + name, (ScriptObject*)prop);
//	}
//}
//
//Debugger::DebugVarType Debugger::ToVarType(void *value, asUINT typeId)
//{
//	// Get variable type
//	if(typeId & asTYPEID_APPOBJECT)
//	{
//		// Check for textures
//		if(typeId == Texture::TypeId)
//			return TextureVar;
//	}
//	return TextVar;
//}

string Debugger::toString(void *value, uint typeId)
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
#endif