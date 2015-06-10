#include "Client.h"


#define MAX_CLIENTS 2
#define SERVER_PORT 60000

Client::Client()
{
	m_Ppeer = RakNet::RakPeerInterface::GetInstance();
	ConnectServer();
	m_checkerInfo.IDRefrence = -1;
	m_deleteInfo.IDRefrence = -1;
	m_TurnOrder = true;

	ReadPacket();
	//packet = new RakNet::Packet();
}

Client::~Client()
{
	RakNet::RakPeerInterface::DestroyInstance(m_Ppeer);
}

void Client::Update()
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_H) == GLFW_PRESS)
		TestPacket();

	ReadPacket();

}

int Client::GetClientID()
{
	return m_Playernumber;
}





void Client::SendMovePacket(vec3 Position, int ObjRefrence, int IDRefrence, bool isKing)
{
	RakNet::Packet* newPacket = new RakNet::Packet();
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)MOVEPIECE_PACKET);
	bsOut.Write(Position.x);
	bsOut.Write(Position.y);
	bsOut.Write(Position.z);
	bsOut.Write(ObjRefrence);
	bsOut.Write(IDRefrence);
	bsOut.Write(isKing);

	m_Ppeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	m_Ppeer->Receive();

}

DeleteCheckerInfo Client::GetDelInfo()
{
	return m_deleteInfo;
}


MoveCheckerInfo Client::GetMoveInfo()
{
	return m_checkerInfo;
}


bool Client::TurnOrderPacket()
{
	return m_TurnOrder;
}
void Client::SetTurnOrder(bool val)
{
	m_TurnOrder = val;
}

void Client::SendTurnOrderPacket()
{
	RakNet::Packet* newPacket = new RakNet::Packet();
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)TURNORDER_PACKET);

	bsOut.Write(m_TurnOrder);
	m_Ppeer->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	m_Ppeer->Receive();

}


void Client::SendDeletePacket(int ObjReference, int IDreference, bool JumpAgain)
{

	RakNet::Packet* newPacket = new RakNet::Packet();
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)DELETEPIECE_PACKET);

	bsOut.Write(ObjReference);
	bsOut.Write(IDreference);
	bsOut.Write(JumpAgain);
	
	m_Ppeer->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	m_Ppeer->Receive();
}

void Client::TestPacket()
{
	RakNet::Packet* newPacket = new RakNet::Packet();
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)SENDING_MSG);
	bsOut.Write("Blah");

	m_Ppeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	m_Ppeer->Receive();
}

void Client::ConnectServer()
{

	RakNet::SocketDescriptor sd;
	m_Ppeer->Startup(1, &sd, 1);

	printf("Enter Server IP or hit enter for 127.0.0.1 \n");
	gets(m_string);

	if (m_string[0] == 0)
	{
		strcpy(m_string, "127.0.0.1");
	}

	printf("Starting the client. \n");
	m_Ppeer->Connect(m_string, SERVER_PORT, 0, 0);

}

void Client::ReadPacket()
{
	for (packet = m_Ppeer->Receive(); packet; m_Ppeer->DeallocatePacket(packet), packet = m_Ppeer->Receive())
	{
		switch (packet->data[0])
		{
		
			case ID_CONNECTION_REQUEST_ACCEPTED: printf("Our connect request has been accepted. \n"); break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: printf("Another client has disconnected. \n"); break;
			case ID_REMOTE_CONNECTION_LOST: printf("Another client has lost the connection. \n"); break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:printf("Another client has connected. \n"); break;
			case ID_NEW_INCOMING_CONNECTION: printf("Our connection request has been accepted. \n");break;
			case ID_NO_FREE_INCOMING_CONNECTIONS: printf("The server is full. \n");	break;
			case ID_DISCONNECTION_NOTIFICATION: printf("We have been disconnected \n"); break;
			case ID_CONNECTION_LOST: printf("Connection lost. \n"); break;

			case MOVEPIECE_PACKET:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			
				bsIn.Read(m_checkerInfo.Position.x);
				bsIn.Read(m_checkerInfo.Position.y);
				bsIn.Read(m_checkerInfo.Position.z);
				bsIn.Read(m_checkerInfo.ObjectRefrence);
				bsIn.Read(m_checkerInfo.IDRefrence);
				bsIn.Read(m_checkerInfo.isKing);
				printf("Move Packet Recieved!: \n");

			}
			break;

			case TURNORDER_PACKET:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(m_TurnOrder);
				printf("Turn Order Recieved!: \n");
			}
			break;


			case DELETEPIECE_PACKET:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				bsIn.Read(m_deleteInfo.ObjectRefrence);
				bsIn.Read(m_deleteInfo.IDRefrence);
				bsIn.Read(m_deleteInfo.JumpAgain);
				printf("Delete Packet Recieved!: \n");
			}
			break;

			case PLAYERNUMBER_PACKET:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(m_Playernumber);
				std::cout << "Your player: " << m_Playernumber << "\n";
			}
			break;

			case SENDING_MSG:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());
			}
			break;

			case SENDING_POSITION:
			{
				vec3 Currentpos;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(Currentpos.x);
				bsIn.Read(Currentpos.y);
				bsIn.Read(Currentpos.z);
				std::cout << "The current Position Streamed by the world: " << Currentpos.x << "," << Currentpos.y << "," << Currentpos.z << "\n";
				printf("Message with identifier %i has arrived. \n", packet->data[0]);
			}
			break;

			default:
				printf("Message with identifier %i has arrived. \n", packet->data[0]);
				break;
			}
	}
}