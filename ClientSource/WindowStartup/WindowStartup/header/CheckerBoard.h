//Author: Anthony Bogli
//Description: Creating a checkboard class for my individual tiles

#ifndef CHECKERBOARD_H
#define CHECKBOARD_H

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW\glfw3.h>
#include "CheckerPiece.h"
#include <vector>
#include <string>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class CheckerBoardTile;

struct Directions
{
	std::string Name;
};


class CheckerBoardTile
{

public:
	
	CheckerBoardTile();
	CheckerBoardTile(vec3 Pos, vec4 Color, int Ref, int Row);

	
	bool RaycastCollision(vec3 Pos);

	~CheckerBoardTile();
	void Update(vec3 RayCastDir, std::vector<CheckerBoardTile *> Tiles);
	void Draw();
	void CheckForPiece();
	void ValidMoves(std::vector<CheckerBoardTile *> Tiles);
	bool MadatoryMove();



	void SetCheckerRefrence(CheckerPiece* piece);
	void NullCheckerRef();
	bool JumpAgain(bool val);



	vec3 GetPosition();
	

	CheckerPiece* DeletePiece();
	CheckerPiece* GetPiece();
	CheckerPiece* MovePiece(vec3 Ray);

	
	void SetlegalMove(bool val);
	bool isSelected();
	bool isOccupied();
	int ReturnRowRef();

	vec3 Position;

	std::vector<CheckerBoardTile *> ReturnValidMoves();

	int getObjectRef();

	

private:
	
	


	bool MoveIndicator;
	bool Occupied;
	bool TileSelected;
	



	Gizmos* Checker;

	
	vec3 RayCast;
	vec4 Colour;
	vec4 DefaultColor;

	
	int m_RowRef;		//What row it is is within from 1-8;	
	int ObjRefrence;	//Basically its ID
	float offset;

	bool m_LCheck, m_FRight, m_Bleft, m_Bright;

	int KillNumber;

	CheckerPiece* checker;
	std::vector<CheckerBoardTile *> TempTiles;
	std::vector<CheckerBoardTile *> ValidMoveTiles;
	std::vector<Directions *> m_Directions;

	//vec3 Position;
	glm::mat4* Identity;


};





#endif