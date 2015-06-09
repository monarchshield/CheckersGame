#include <stdio.h>
#include <iostream>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h""
#include "BitStream.h"
#include "RakNetTypes.h" //MessageID
#include <vector>
#define MAX_CLIENTS 10
#define SERVER_PORT 60000


struct vec3
{
	float xpos;
	float ypos;
	float zpos;

};

struct CheckerInfo
{
	vec3 Position;
	int ObjectRefrence;
	int IDRefrence;
	bool isKing;
};

struct DeleteCheckerInfo
{
	int ObjectRefrence;
	int IDRefrence;
	bool JumpAgain;
};


enum GameMEssages
{
	SENDING_MSG = ID_USER_PACKET_ENUM + 1,
	SENDING_POSITION = ID_USER_PACKET_ENUM + 2,
	MOVEPIECE_PACKET = ID_USER_PACKET_ENUM + 3,
	DELETEPIECE_PACKET = ID_USER_PACKET_ENUM + 4,
	PLAYERNUMBER_PACKET = ID_USER_PACKET_ENUM + 5,
	TURNORDER_PACKET = ID_USER_PACKET_ENUM + 6,
};	


int main(void)
{
	std::string Message;
	vec3 position;
	std::vector<RakNet::SystemAddress> m_address;
	CheckerInfo m_checker;
	DeleteCheckerInfo m_delchecker;

	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

	bool isPlayer = false;
	RakNet::Packet* packet;
	bool TurnOrder;
	
	
	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	

	printf("Starting the server. \n");
	//We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	
	int Playernumber = 0;
	

	while (1)
	{
	
		for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
		{
			switch (packet->data[0])
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					{
						printf("Our connect request has been accepted. \n");
						//Use a bitsream to write a custom user message
						
						RakNet::BitStream bsOut;
						bsOut.Write((RakNet::MessageID)SENDING_MSG);
						bsOut.Write("Hello world");
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
						
					}
					break;
	
				case MOVEPIECE_PACKET:
				{
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(m_checker.Position.xpos);
					bsIn.Read(m_checker.Position.ypos);
					bsIn.Read(m_checker.Position.zpos);
					bsIn.Read(m_checker.ObjectRefrence);
					bsIn.Read(m_checker.IDRefrence);
					bsIn.Read(m_checker.isKing);
					printf("Move Packet Recieved!: \n");

					for (int i = 0; i < m_address.size(); i++)
					{
						if (packet->systemAddress != m_address[i])
						{
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID)MOVEPIECE_PACKET);
							bsOut.Write(m_checker.Position.xpos);
							bsOut.Write(m_checker.Position.ypos);
							bsOut.Write(m_checker.Position.zpos);
							bsOut.Write(m_checker.ObjectRefrence);
							bsOut.Write(m_checker.IDRefrence);
							bsOut.Write(m_checker.isKing);

							peer->Send(&bsOut,HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_address[i], false);
						
						}
					}


				}
					break;


				case DELETEPIECE_PACKET:
				{
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					
					bsIn.Read(m_delchecker.ObjectRefrence);
					bsIn.Read(m_delchecker.IDRefrence);
					bsIn.Read(m_delchecker.JumpAgain);
					printf("Delete Packet Recieved!: \n");

					for (int i = 0; i < m_address.size(); i++)
					{
						 if (packet->systemAddress != m_address[i])
						 {
							 RakNet::BitStream bsOut;
							 bsOut.Write((RakNet::MessageID)DELETEPIECE_PACKET);
						
							 bsOut.Write(m_delchecker.ObjectRefrence);
							 bsOut.Write(m_delchecker.IDRefrence);
							 bsOut.Write(m_delchecker.JumpAgain);

							 peer->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, m_address[i], false);
						
						 }
					}
				}
			   break;


				case TURNORDER_PACKET:
				{
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(TurnOrder);
					printf("Turn Order Packet Recieved!: \n");

					for (int i = 0; i < m_address.size(); i++)
					{
						if (packet->systemAddress != m_address[i])
						{
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID)TURNORDER_PACKET);

							bsOut.Write(TurnOrder);
							peer->Send(&bsOut, LOW_PRIORITY, RELIABLE_ORDERED, 0, m_address[i], false);
						}
					}
					

				}
				break;


				case ID_NEW_INCOMING_CONNECTION:
				{					   
					printf("Our connection request has been accepted. \n");
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)PLAYERNUMBER_PACKET);
					bsOut.Write(Playernumber);
					peer->Send(&bsOut,IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					Playernumber++;

					
					m_address.push_back(packet->systemAddress);
				}
				break;

				case SENDING_MSG:
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("%s\n", rs.C_String());


					for (int i = 0; i < m_address.size(); i++)
						{
							if (packet->systemAddress != m_address[i])
							{
								RakNet::BitStream bsOut;
								bsOut.Write((RakNet::MessageID)SENDING_MSG);
								bsOut.Write("Blah");
					
								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_address[i], false);
							
							}
						}
				}
				break;

				case SENDING_POSITION:
				{
						vec3 Currentpos;
						//Reading in a packet
						RakNet::BitStream bsIn(packet->data, packet->length, false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(Currentpos.xpos);
						bsIn.Read(Currentpos.ypos);
						bsIn.Read(Currentpos.zpos);
						std::cout << "The current Position Streamed by the world: " << Currentpos.xpos << "," << Currentpos.ypos << "," << Currentpos.zpos << "\n";
						printf("Message with identifier %i has arrived. \n", packet->data[0]);
		
						for (int i = 0; i < m_address.size(); i++)
						{
							for (int j = 0; j < m_address.size(); j++)
							{
								RakNet::BitStream bsOut;
								bsOut.Write((RakNet::MessageID)SENDING_MSG);
								bsOut.Write("Blah");

								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_address[j], false);
								
							}
						}	
				}
				break;

				case ID_NO_FREE_INCOMING_CONNECTIONS: printf("The server is full. \n");	break;
				case ID_DISCONNECTION_NOTIFICATION: printf("A client has disconnected. \n");break;
				case ID_CONNECTION_LOST: printf("A client lost the connection. \n"); break;
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:printf("Another client has disconnected. \n");break;
				case ID_REMOTE_CONNECTION_LOST: printf("Another client has lost the connection. \n");break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:printf("Another client has connected. \n");break;






				default:
				printf("Message with identifier %i has arrived. \n", packet->data[0]);
				
				break;
			}
		}
			


	}
	//--------------------------------------------





	//--------------------------------------
	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;

}

