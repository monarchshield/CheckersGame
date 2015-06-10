//Author: Anthony Bogli
//Description: Creating a CheckerPiece class for my Individual Red and Black Pieces


#ifndef CHECKERPIECE_H
#define CHECKERPIECE_H

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW\glfw3.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;


class CheckerPiece
{
public:
	CheckerPiece();
	CheckerPiece(vec3 Pos, vec4 Color);

	void Update();
	void Draw(); 
	void SetPosition(vec3 pos);
	void SetKing(bool val);
	bool GetColour();
	bool isThisKing();
	void SetObjRefrence(int ref);
	
	vec3 GetPosition();
	void setActive(bool val);
	int	GetObjReference();
	void SetIDRef(int ID);	//This should only be called once during initliasation of a checker piece
	int GetID();	//Called to get the indepdent ID of a piece;


	//If its true the piece is Red Else its Blue
	void SetColour(bool val);

	~CheckerPiece();

private:


	bool isActive; //Alive or dead?
	bool isBlack;
	bool isKing;
	Gizmos* Checker;
	vec3 Position;
	glm::mat4* Identity;
	vec4 Colour;
	int ObjRefrence; //Just to keep track of where it should be
	int Idreference; //This is the objects indepdent idReference;


};


#endif
