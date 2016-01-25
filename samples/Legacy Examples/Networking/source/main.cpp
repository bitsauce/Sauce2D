//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"

#include <x2d/x2d.h>
using namespace xd;

enum
{
	SERVER,
	CLIENT
} role;

list<class Player*> players;
Random random;
SpriteBatch *spriteBatch = nullptr;
FontPtr font = nullptr;
RakNet::RakPeerInterface *rakPeer = nullptr;
RakNet::NetworkIDManager networkIDManager;
string console; void println(string str) { console.append(str + "\n"); }
map<RakNet::RakNetGUID, bool*> input;

class Player : public RakNet::NetworkIDObject
{
public:
	Player() :
		m_position(Window::getSize()/2.0f),
		m_speed(5.0f),
		m_color(random.nextInt(255), random.nextInt(255), random.nextInt(255))
	{
		players.push_back(this);
	}

	~Player()
	{
		players.remove(this);
	}

	void update(const float dt)
	{
		if(role == CLIENT) return;
		if(input[m_owner][0]) m_position.x -= m_speed;
		if(input[m_owner][1]) m_position.x += m_speed;
		if(input[m_owner][2]) m_position.y -= m_speed;
		if(input[m_owner][3]) m_position.y += m_speed;
	}

	void draw(GraphicsContext &graphicsDevice)
	{
		graphicsDevice.drawRectangle(m_position.x, m_position.y, 50, 50, m_color);
	}

	void writePacket(RakNet::BitStream *bitStream)
	{
		bitStream->Write(m_owner);
		bitStream->Write(m_position.x);
		bitStream->Write(m_position.y);
		bitStream->Write(m_color.r);
		bitStream->Write(m_color.g);
		bitStream->Write(m_color.b);
	}

	void readPacket(RakNet::BitStream *bitStream)
	{
		bitStream->Read(m_owner);
		bitStream->Read(m_position.x);
		bitStream->Read(m_position.y);
		bitStream->Read(m_color.r);
		bitStream->Read(m_color.g);
		bitStream->Read(m_color.b);
	}

	RakNet::RakNetGUID m_owner;
	Vector2 m_position;
	float m_speed;
	Color m_color;
};

enum
{
	MAIN_STATE,
	IN_GAME
} state;

enum
{
	ID_PLAYER_PACKET = ID_USER_PACKET_ENUM,
	ID_INPUT_PACKET
};

void gameconnect(int newrole)
{
	console.clear();

	if(newrole == SERVER)
	{
		RakNet::SocketDescriptor socketDescriptor(5555, 0);
		assert(rakPeer->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
		rakPeer->SetMaximumIncomingConnections(2);
		role = SERVER;
		println("Listening to port 5555 on 127.0.0.1");

		input[rakPeer->GetMyGUID()] = new bool[4];
		
		Player *player = new Player();
		player->SetNetworkIDManager(&networkIDManager);
		player->m_owner = rakPeer->GetMyGUID();
	}
	else // if(newrole == CLIENT)
	{
		RakNet::SocketDescriptor socketDescriptor;
		assert(rakPeer->Startup(1, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
		assert(rakPeer->Connect("127.0.0.1", 5555, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);

		println("Connecting to 127.0.0.1|5555");
		
		while(true)
		{
			RakNet::Packet *p = rakPeer->Receive();
			if(p && p->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			{
				println("Connected to " + string(p->systemAddress.ToString(true)) + " with GUID " + string(p->guid.ToString()));
				break;
			}
		}

		role = CLIENT;
	}

	Input::unbindAll();
	state = IN_GAME;
}

void gamemain()
{
	Window::disableResize();

	rakPeer = RakNet::RakPeerInterface::GetInstance();

	font = ResourceManager::get<Font>("Consolas 12");
	font->setColor(Color(255, 255, 255, 255));

	println("Press '1' to host server.\nPress '2' to join as client.");
	
	Input::bind(XD_KEY_1, bind(&gameconnect, SERVER));
	Input::bind(XD_KEY_2, bind(&gameconnect, CLIENT));
}

void update()
{
	switch(state)
	{
	case MAIN_STATE:
		break;

	case IN_GAME:
		if(role == SERVER)
		{
			input[rakPeer->GetMyGUID()][0] = Input::getKeyState(XD_KEY_A);
			input[rakPeer->GetMyGUID()][1] = Input::getKeyState(XD_KEY_D);
			input[rakPeer->GetMyGUID()][2] = Input::getKeyState(XD_KEY_W);
			input[rakPeer->GetMyGUID()][3] = Input::getKeyState(XD_KEY_S);
		}
		else
		{
			RakNet::BitStream bitStream;
			bitStream.Write((RakNet::MessageID)ID_INPUT_PACKET);
			bitStream.Write(Input::getKeyState(XD_KEY_LEFT));
			bitStream.Write(Input::getKeyState(XD_KEY_RIGHT));
			bitStream.Write(Input::getKeyState(XD_KEY_UP));
			bitStream.Write(Input::getKeyState(XD_KEY_DOWN));
			assert(rakPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
		}

		for(Player *player : players)
		{
			player->update(0.0166667f);
		}
		
		if(role == SERVER)
		{
			for(RakNet::Packet *p = rakPeer->Receive(); p; rakPeer->DeallocatePacket(p), p = rakPeer->Receive())
			{
				if(p->data[0] == ID_NEW_INCOMING_CONNECTION)
				{
					println("Client connected from " + string(p->systemAddress.ToString(true)) + " with GUID " + string(p->guid.ToString()));
		
					input[p->guid] = new bool[4];

					Player *player = new Player();
					player->SetNetworkIDManager(&networkIDManager);
					player->m_owner = p->guid;
				}
				else if(p->data[0] == ID_INPUT_PACKET)
				{
					RakNet::BitStream bitStream(p->data, p->length, false);
					bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
					bitStream.Read(input[p->guid][0]);
					bitStream.Read(input[p->guid][1]);
					bitStream.Read(input[p->guid][2]);
					bitStream.Read(input[p->guid][3]);
				}
			}

			for(Player *player : players)
			{
				RakNet::BitStream bitStream;
				bitStream.Write((RakNet::MessageID)ID_PLAYER_PACKET);
				bitStream.Write(player->GetNetworkID());
				player->writePacket(&bitStream);
				assert(rakPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
			}
		}
		else
		{
			for(RakNet::Packet *p = rakPeer->Receive(); p; rakPeer->DeallocatePacket(p), p = rakPeer->Receive())
			{
				RakNet::BitStream bitStream(p->data, p->length, false);
				RakNet::MessageID messageID;
				bitStream.Read(messageID);
				if(messageID == ID_PLAYER_PACKET)
				{
					RakNet::NetworkID playerNetworkID;
					bitStream.Read(playerNetworkID);
					Player *player = networkIDManager.GET_OBJECT_FROM_ID<Player*>(playerNetworkID);
					if(!player)
					{
						player = new Player();
						player->SetNetworkIDManager(&networkIDManager);
						player->SetNetworkID(playerNetworkID);
					}
					player->readPacket(&bitStream);
				}
			}
		}
		break;
	}
}

void draw(GraphicsContext &graphicsDevice)
{
	if(spriteBatch == nullptr) spriteBatch = new SpriteBatch(graphicsDevice);
	
	spriteBatch->begin();
	
	spriteBatch->drawText(Vector2(), console, font);
	switch(state)
	{
	case MAIN_STATE:
		break;

	case IN_GAME:
		for(Player *player : players)
		{
			player->draw(graphicsDevice);
		}
		break;
	}
	
	spriteBatch->end();
}

void end()
{
	for(Player *player : players)
	{
		delete player;
	}
	RakNet::RakPeerInterface::DestroyInstance(rakPeer);
}

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Process the command-line
	int flags = 0;
	string workDir;
	for(int i = 0; i < __argc; i++)
	{
		if(__argv[i][0] == '-')
		{
			switch(__argv[i][1])
			{
			case 'v': flags |= XD_EXPORT_LOG; break;
			case 'w': workDir = string(__argv[i]+3); break;
			}
		}
	}
	flags |= XD_EXPORT_LOG; // For now we force this flag
	flags |= XD_RUN_IN_BACKGROUND;

	Engine *engine = CreateEngine();

	Config config;
	config.main = &gamemain;
	config.update = &update;
	config.draw = &draw;
	config.end = &end;
#ifdef X2D_DEBUG
	config.workDir = "..\\game\\";
#endif
	config.flags = flags;

	if(engine->init(config) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}