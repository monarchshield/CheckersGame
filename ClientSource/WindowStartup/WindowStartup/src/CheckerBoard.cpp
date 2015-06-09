#include "CheckerBoard.h"

CheckerBoardTile::CheckerBoardTile()
{

}

CheckerBoardTile::~CheckerBoardTile()
{


}

CheckerBoardTile::CheckerBoardTile(vec3 Pos, vec4 Color, int Ref, int Row)
{
	Identity = new mat4(1, 0, 0, 1,
						0, 1, 0, 1,
						0, 0, 1, 1,
						0, 0, 0, 1);

	
	
	m_RowRef = Row;
	ObjRefrence = Ref;
	Position = Pos;	
	Colour = Color;
	DefaultColor = Color;
	Occupied = false;
	TileSelected = false;
	MoveIndicator = false;



	offset = 3;
}

void CheckerBoardTile::Update(vec3 RayCastDir, std::vector<CheckerBoardTile *> Tiles)
{

	RayCast = RayCastDir;


	//if (ValidMoveTiles.size() > 0)
	//	ValidMoveTiles.clear();

	TileSelected = RaycastCollision(RayCast);

	Colour = DefaultColor;

	if (MoveIndicator)
	{
		Colour = vec4(1, 1, 0, 1);
	}


	if (TileSelected && checker != nullptr)
	{
		Colour = vec4(0, 1, 0, 1);
		ValidMoves(Tiles);
		
	}


	//else if (MoveIndicator != true | TileSelected != true)
	

	


}

bool CheckerBoardTile::MadatoryMove()
{

	for (unsigned int i = 0; i < m_Directions.size(); i++)
	{
		if (m_Directions[0]->Name == "FORWARD_LEFT" && m_LCheck) {
			TileSelected = true; 
			return true;
		}
		if (m_Directions[0]->Name == "FORWARD_RIGHT" && m_FRight){
			TileSelected = true;
			return true;
		}
		if (m_Directions[0]->Name == "BACK_LEFT" && m_Bleft)
		{
			TileSelected = true;
			return true;
		}
		if (m_Directions[0]->Name == "BACK_RIGHT" && m_Bright)
		{
			TileSelected = true;
			return true;
		}
	}

	return false;
}


void CheckerBoardTile::Draw()
{
	Gizmos::addAABBFilled(Position, glm::vec3(offset, 1, offset), Colour, Identity);

	

}

bool CheckerBoardTile::RaycastCollision(vec3 Pos)
{
	
	if (Pos.x <= Position.x + 3 && Pos.x >= Position.x - 3 &&
		Pos.z <= Position.z + 3 && Pos.z >= Position.z - 3)
		return true;
	
	
	else 
	return false;
}

void CheckerBoardTile::ValidMoves(std::vector<CheckerBoardTile *> Tiles)
{
	



	ValidMoveTiles.clear();
	m_Directions.clear();
	TempTiles.clear();

	m_LCheck = m_FRight = m_Bleft = m_Bright = false;


	TempTiles = Tiles;

	int ForwardLeft = ObjRefrence + 7, ForwardRight = ObjRefrence + 9, BackLeft = ObjRefrence - 7, BackRight = ObjRefrence - 9;


	//This is just 4 if statements to make sure that the forward left will not access a null element 

	if (ObjRefrence == 9)
		BackLeft = 0;

	if (ForwardLeft >= 64)
		ForwardLeft = ObjRefrence;

	if (ForwardRight >= 64)
		ForwardRight = ObjRefrence;

	if (BackLeft <= -1)
		BackLeft = ObjRefrence;

	if (BackRight <= -1)
		BackRight = ObjRefrence;

	//You can jump! :)
	if (ForwardLeft != ObjRefrence && ForwardLeft + 7 < 64 && TempTiles[ForwardLeft]->isOccupied() && m_RowRef != 6 && m_RowRef != 1 && checker != nullptr &&  checker->GetColour() != TempTiles[ForwardLeft]->checker->GetColour())
	{
		ForwardLeft += 7;
		m_LCheck = true;

	}


	if (ForwardRight != ObjRefrence && ForwardRight + 9 < 64 && TempTiles[ForwardRight]->isOccupied() && m_RowRef != 6 && checker != nullptr && checker->GetColour() != TempTiles[ForwardRight]->checker->GetColour())
	{
		ForwardRight += 9;
		m_FRight = true;
	}


	if (BackLeft != ObjRefrence && BackLeft - 7 >= 0 && TempTiles[BackLeft]->isOccupied() && m_RowRef != 6 && checker != nullptr && checker->GetColour() != TempTiles[BackLeft]->checker->GetColour())
	{
		BackLeft -= 7;
		m_Bleft = true;
	}

	if (BackRight != ObjRefrence && BackRight - 9 >= 0 && TempTiles[BackRight]->isOccupied() && m_RowRef != 1 && checker != nullptr&& checker->GetColour() != TempTiles[BackRight]->checker->GetColour())
	{
		BackRight -= 9;
		m_Bright = true;
	}

	//Double check that the tiles are not able to hop other tiles
	if (TempTiles[ForwardLeft]->isOccupied())	{ ForwardLeft = ObjRefrence; m_LCheck = false; }
	if (TempTiles[ForwardRight]->isOccupied())	{ ForwardRight = ObjRefrence; m_FRight = false; }

	if (TempTiles[BackLeft]->isOccupied())		{ BackLeft = ObjRefrence; m_Bleft = false; }
	if (TempTiles[BackRight]->isOccupied())		{ BackRight = ObjRefrence; m_Bright = false; }

	if (checker != nullptr)
	{

		Directions *FLEFT = new Directions(), *FRIGHT = new Directions(), *BLEFT = new Directions(), *BRIGHT = new Directions();
		FLEFT->Name = "FORWARD_LEFT";
		FRIGHT->Name = "FORWARD_RIGHT";
		BLEFT->Name = "BACK_LEFT"; 
		BRIGHT->Name = "BACK_RIGHT";
		
		if (checker->isThisKing())
		{

			//Using this switch to know what tiles to push back
			switch (m_RowRef)
			{
				case 0:	ValidMoveTiles.push_back(TempTiles[ForwardRight]);
						ValidMoveTiles.push_back(TempTiles[BackLeft]);

						m_Directions.push_back(FRIGHT);
						m_Directions.push_back(FLEFT);

						break;

				case 7:
						ValidMoveTiles.push_back(TempTiles[ForwardLeft]);
						ValidMoveTiles.push_back(TempTiles[BackRight]);


						m_Directions.push_back(FLEFT);
						m_Directions.push_back(BRIGHT);

						break;


				default:
						ValidMoveTiles.push_back(TempTiles[ForwardLeft]);
						ValidMoveTiles.push_back(TempTiles[ForwardRight]);
						ValidMoveTiles.push_back(TempTiles[BackLeft]);
						ValidMoveTiles.push_back(TempTiles[BackRight]);


						m_Directions.push_back(FLEFT);
						m_Directions.push_back(FRIGHT);
						m_Directions.push_back(BLEFT);
						m_Directions.push_back(BRIGHT);

						break;
				}
			
			std::vector<CheckerBoardTile *> temp;
			std::vector<Directions *> tempDir;
			if (m_LCheck){ temp.push_back(TempTiles[ForwardLeft]);  tempDir.push_back(FLEFT); }
		
			if (m_FRight){ temp.push_back(TempTiles[ForwardRight]); tempDir.push_back(FRIGHT);}
			if (m_Bleft) { temp.push_back(TempTiles[BackLeft]);  tempDir.push_back(BLEFT); }
		
			if (m_Bright) { temp.push_back(TempTiles[BackRight]); tempDir.push_back(BRIGHT); }
			
			
			
			if (m_LCheck || m_FRight || m_Bleft || m_Bright) {
				
				m_Directions.clear();
				m_Directions = tempDir;
				ValidMoveTiles.clear();
				ValidMoveTiles = temp;
			}

			return;
		}

		else if (checker->GetColour())
		{
				//Using this switch to know what tiles to push back
				switch (m_RowRef)
				{
					case 0:	ValidMoveTiles.push_back(TempTiles[ForwardRight]);
							m_Directions.push_back(FRIGHT);
		
							break;

				case 7:
						ValidMoveTiles.push_back(TempTiles[ForwardLeft]);
						m_Directions.push_back(FLEFT);
						break;


				default:
						ValidMoveTiles.push_back(TempTiles[ForwardLeft]);
						ValidMoveTiles.push_back(TempTiles[ForwardRight]);
						m_Directions.push_back(FLEFT);
						m_Directions.push_back(FRIGHT);
						break;
				}

				std::vector<CheckerBoardTile *> temp;
				std::vector<Directions *> tempDir;

				if (m_LCheck && m_RowRef != 0) { temp.push_back(TempTiles[ForwardLeft]); tempDir.push_back(FLEFT); }
				if (m_FRight && m_RowRef != 7) { temp.push_back(TempTiles[ForwardRight]); tempDir.push_back(FRIGHT); }
				if (m_LCheck || m_FRight)
				{
					m_Directions.clear();
					m_Directions = tempDir;
					ValidMoveTiles.clear();
					ValidMoveTiles = temp;
				}
		}

		else
		{
			//Using this switch to know what tiles to push back
			switch (m_RowRef)
			{
				case 0:
					ValidMoveTiles.push_back(TempTiles[BackLeft]);
					m_Directions.push_back(BLEFT);
					break;

				case 7:
					ValidMoveTiles.push_back(TempTiles[BackRight]);
					m_Directions.push_back(BRIGHT);
					break;


				default:

					ValidMoveTiles.push_back(TempTiles[BackLeft]);
					ValidMoveTiles.push_back(TempTiles[BackRight]);
					m_Directions.push_back(BLEFT);
					m_Directions.push_back(BRIGHT);
					break;
				}
			
			std::vector<CheckerBoardTile *> temp;
			std::vector<Directions *> tempDir;

			if (m_Bleft) { temp.push_back(TempTiles[BackLeft]);   tempDir.push_back(BLEFT); }
			if (m_Bright) { temp.push_back(TempTiles[BackRight]); tempDir.push_back(BRIGHT); }
			if (m_Bleft || m_Bright) 
			{
				m_Directions.clear();
				m_Directions = tempDir;
				ValidMoveTiles.clear();
				ValidMoveTiles = temp;
			}

		}
	}
}

void CheckerBoardTile::SetCheckerRefrence(CheckerPiece* piece)
{
	checker = piece;
	Occupied = true;
}

void CheckerBoardTile::NullCheckerRef()
{
	checker = nullptr;
	Occupied = false;
	//ValidMoveTiles.clear();
}


CheckerPiece* CheckerBoardTile::DeletePiece()
{
	//This Function will return a piece refrence to delete based of the move
	//IF there is a delete;
	
	CheckerPiece* piece;
	piece = nullptr;
	
	if (m_Directions[KillNumber]->Name == "FORWARD_LEFT" && m_LCheck && ObjRefrence + 7 <= 63)
		return TempTiles[ObjRefrence + 7]->GetPiece();

	if (m_Directions[KillNumber]->Name == "FORWARD_RIGHT" && m_FRight && ObjRefrence + 9 <= 64)
		return TempTiles[ObjRefrence + 9]->GetPiece();
	

	if (m_Directions[KillNumber]->Name == "BACK_LEFT" && m_Bleft && ObjRefrence - 7 >= 0)
		return TempTiles[ObjRefrence - 7]->GetPiece();
	

	if (m_Directions[KillNumber]->Name == "BACK_RIGHT" && m_Bright && ObjRefrence - 9 >= 0)
		return TempTiles[ObjRefrence - 9]->GetPiece();

	return piece;
}


CheckerPiece* CheckerBoardTile::MovePiece(vec3 Ray)
{
	

	
	for (unsigned int i = 0; i < ValidMoveTiles.size(); i++)
	{
		if (ValidMoveTiles[i]->RaycastCollision(Ray))
		{	
			KillNumber = i;

			vec3 Position = ValidMoveTiles[i]->GetPosition();
			Position.y += 1;

			checker->SetPosition(Position);
			checker->SetObjRefrence(ValidMoveTiles[i]->getObjectRef());

			if (checker->GetColour() && ValidMoveTiles[i]->getObjectRef() >= 57 && ValidMoveTiles[i]->getObjectRef() < 64)
				checker->SetKing(true);

			else if (!checker->GetColour() && ValidMoveTiles[i]->getObjectRef() >= 0 && ValidMoveTiles[i]->getObjectRef() < 7)
				checker->SetKing(true);
			
			
			return checker;
		}

	}

	return checker;
}

bool CheckerBoardTile::JumpAgain(bool val)
{

	

	for (unsigned int i = 0; i < m_Directions.size(); i++)
	{
		// Do a switch case on directions
		// Loop thorugh all the directions and check what there string name is
		//Then check if there string counter part //bool is set to true;
		// if it is return true; and switch the turnOrder to true
		//Else return false :)
		if (m_Directions[0]->Name == "FORWARD_LEFT" && m_LCheck)
			return val = !val;
		if (m_Directions[0]->Name == "FORWARD_RIGHT" && m_FRight)
			return val = !val;
		if (m_Directions[0]->Name == "BACK_LEFT" && m_Bleft)
			return val = !val;
		if (m_Directions[0]->Name == "BACK_RIGHT" && m_Bright)
			return val = !val;
	}

	return val;

	

}

bool CheckerBoardTile::isOccupied()
{
	return Occupied;
}

bool CheckerBoardTile::isSelected()
{
	return TileSelected;
}

int CheckerBoardTile::getObjectRef()
{
	return ObjRefrence;
}

void CheckerBoardTile::SetlegalMove(bool val)
{
	MoveIndicator = val;
}

int CheckerBoardTile::ReturnRowRef()
{
	return m_RowRef;
}

std::vector<CheckerBoardTile *> CheckerBoardTile::ReturnValidMoves()
{

	return ValidMoveTiles;
}

vec3 CheckerBoardTile::GetPosition()
{
	return Position;
}

CheckerPiece* CheckerBoardTile::GetPiece()
{
	return checker;
}

