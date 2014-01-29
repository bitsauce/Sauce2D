

/*int server_run()
{
	// Initialize transport layer
	TransportType type = Transport_LAN;
	if(!Transport::Initialize(Transport_LAN))
	{
		printf("Failed to initialize transport layer\n");
		return -1;
	}
	
	// Create transport layer
	Transport* transport = Transport::Create();
	if (!transport)
	{
		printf("Could not create transport\n");
		return -1;
	}
	
	// Start server (transport specific)
	switch(type)
	{
		case Transport_LAN:
		{
			// Create the transport layer
			TransportLAN * lan_transport = dynamic_cast<TransportLAN*>( transport );
			char hostname[64+1] = "hostname";
			TransportLAN::GetHostName( hostname, sizeof(hostname) );
			lan_transport->StartServer( hostname );
		}
		break;
		
		default:
			break;
	}

	// Main loop
	const int DeltaTime = 333;
	const int PacketRate = 2500; //ms = 2,5s
	int PacketAccum = 0;
	Object* test = new Object;
	while(true)
	{
		if(PacketAccum >= PacketRate)
		{
			// Create packet and inform of new objects
			NetPacket packet;
			packet.WriteInteger(mSceneObjects.size());

			// Send server object info
			for(int i = 0; i < mSceneObjects.size(); i++)
			{
				mSceneObjects[i]->packUpdate(&packet);
				transport->SendPacket(1, packet.GetPacketInfo(), packet.GetPacketSize());
			}
			transport->SendPacket(1, packet.GetPacketInfo(), packet.GetPacketSize());

			// Update the transport layer
			transport->Update(DeltaTime);
			PacketAccum = 0;
		}else{

			// Update server objects
			for(int i = 0; i < mSceneObjects.size(); i++)
				mSceneObjects[i]->update();
		}
		
		// Apply delta
		PacketAccum += DeltaTime;
	}
	
	// Shutdown
	Transport::Destroy(transport);
	Transport::Shutdown();

	return 0;
}*/