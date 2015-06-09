#ifndef CLIENT_H
#define CLIENT_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h""
#include "BitStream.h"
#include "RakNetTypes.h" //MessageID
#include <iostream>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;


struct MoveCheckerInfo
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

class Client
{




enum GameMEssages
{
	SENDING_MSG = ID_USER_PACKET_ENUM + 1,
	SENDING_POSITION = ID_USER_PACKET_ENUM + 2,
	MOVEPIECE_PACKET = ID_USER_PACKET_ENUM + 3,
	DELETEPIECE_PACKET = ID_USER_PACKET_ENUM + 4,
	PLAYERNUMBER_PACKET = ID_USER_PACKET_ENUM + 5,
	TURNORDER_PACKET = ID_USER_PACKET_ENUM + 6,
};

public:
	Client();
	~Client();

	void Update();

	void TestPacket(); //Just using this for debug purposes
	void SendMovePacket(vec3 Position, int ObjRefrence, int IDRefrence, bool isKing);
	void SendDeletePacket(int ObjReference, int IDreference, bool JumpAgain);
	void SendTurnOrderPacket();
	void ConnectServer();
	
	
	bool TurnOrderPacket();
	void SetTurnOrder(bool val);

	void ReadPacket();
	int GetClientID();
	MoveCheckerInfo GetMoveInfo();
	DeleteCheckerInfo GetDelInfo();
	

protected:
	

private:

	MoveCheckerInfo m_checkerInfo;
	DeleteCheckerInfo m_deleteInfo;

	bool m_TurnOrder;
	int m_Playernumber;
	vec3 m_position;
	char m_string[512]; //Just a char array for writing stuff to
	RakNet::RakPeerInterface* m_Ppeer;
	RakNet::Packet* packet;
};

#endif