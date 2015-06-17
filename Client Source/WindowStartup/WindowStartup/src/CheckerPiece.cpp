#include "CheckerPiece.h"


CheckerPiece::CheckerPiece()
{

}

CheckerPiece::CheckerPiece(vec3 Pos, vec4 Color)
{
	Identity = new mat4(1, 0, 0, 1,
						0, 1, 0, 1,
						0, 0, 1, 1,
						0, 0, 0, 1);

	Position = Pos;
	Colour = Color;

	isActive = true;
	isKing = false;
	

}


CheckerPiece::~CheckerPiece()
{

}

void CheckerPiece::Update()
{

}

void CheckerPiece::Draw()
{
	
	if (isActive)
	{
		if (isKing)
		{
			Gizmos::addCylinderFilled(Position, 2, .5f, 15, Colour, Identity);
			Gizmos::addCylinderFilled(Position + vec3(0, 1, 0), 2, .5f, 15, Colour, Identity);
		}

		else
			Gizmos::addCylinderFilled(Position, 2, .5f, 15, Colour, Identity);
	}

	else
		return;
}



void CheckerPiece::SetKing(bool val)
{
	isKing = val;
}

void CheckerPiece::SetPosition(vec3 pos)
{
	
	Position = pos;
}

void CheckerPiece::SetColour(bool val)
{
	isBlack = val;
}

bool CheckerPiece::GetColour()
{
	return isBlack;
}

bool CheckerPiece::isThisKing()
{
	return isKing;
}

void CheckerPiece::SetObjRefrence(int ref)
{
	ObjRefrence = ref;
}

int	CheckerPiece::GetObjReference()
{
	return ObjRefrence;
}

void CheckerPiece::SetIDRef(int ID)
{
	Idreference = ID;
}

int CheckerPiece::GetID()
{
	return Idreference;
}

void CheckerPiece::setActive(bool val)
{
	isActive = val;
}

vec3 CheckerPiece::GetPosition()
{
	return Position;
}

bool CheckerPiece::GetActive()
{
	return isActive;
}
