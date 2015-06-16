#include "Application.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb-master\stb_image.h"
#include <iostream>

void OnMouseButton(GLFWwindow*, int b, int a, int m)
{
	TwEventMouseButtonGLFW(b, a);
}
void OnMousePosition(GLFWwindow*, double x, double y)
{
	TwEventMousePosGLFW((int)x, (int)y);
}
void OnMouseScroll(GLFWwindow*, double x, double y)
{
	TwEventMouseWheelGLFW((int)y);
}
void OnKey(GLFWwindow*, int k, int s, int a, int m)
{
	TwEventKeyGLFW(k, a);
}
void OnChar(GLFWwindow*, unsigned int c)
{
	TwEventCharGLFW(c, GLFW_PRESS);
}
void OnWindowResize(GLFWwindow*, int w, int h)
{
	TwWindowSize(w, h);
	glViewport(0, 0, w, h);
}



Application::Application()
{
	if(glfwInit() == false)
		return;

	window = glfwCreateWindow(1280,850,"Computer Graphics" , nullptr,nullptr);
	camera = new FlyCamera();
	previousTime = 0;
	currentTime = 0;
	DeltaTime = 0;
	m_rows = 20;
	m_collumns = 20;
	m_ModelIndexCount = 0;
	
	m_startcolour = vec4(1,1,1,1);
	m_endcolour = vec4(1,0,0,1);
	
	cooldown = 1.0f;
	m_vertexCount = 0;


	m_timer = 0;
	if(window == nullptr)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwMakeContextCurrent(window);
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}


	m_clearcolour = vec4(1,1,1,1);
	m_skybox = new Skybox();


	finishMove = false;

	Gizmos::create();
	CantSelect = true;
	

	CreateGrid();
	//CreatePieces();

	m_LightDirection = vec3(0.72,0.03,-0);
	m_AmbientColour =  vec3(0,0,0);
	m_seed = 0;
	m_amp = 1.0f;
	m_persis = 0.3f;
	m_Pclient = new Client();
	
	m_PlayerNumber = m_Pclient->GetClientID();
	


	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwSetCursorPosCallback(window, OnMousePosition);
	glfwSetScrollCallback(window, OnMouseScroll);
	glfwSetKeyCallback(window, OnKey);
	glfwSetCharCallback(window, OnChar);
	glfwSetWindowSizeCallback(window, OnWindowResize);
	
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	TurnOrder = true;

	m_bar = TwNewBar("Debug Console");
	TwAddSeparator(m_bar, "Camera position:", "");
	
	TwAddVarRW(m_bar, "X position: ", TW_TYPE_FLOAT, &camera->xPosition, "");
	TwAddVarRW(m_bar, "y position: ", TW_TYPE_FLOAT, &camera->yPosition, "");
	TwAddVarRW(m_bar, "z position: ", TW_TYPE_FLOAT, &camera->zPosition, "");
	TwAddVarRW(m_bar, "Camera Speed: ", TW_TYPE_FLOAT, &camera->Speed, "");

	TwAddVarRW(m_bar, "Commit Move: ", TW_TYPE_BOOL16, &finishMove, "");

	TwAddSeparator(m_bar, "Ray Direction:", "");
	TwAddVarRO(m_bar, "Rays Direction x: ", TW_TYPE_FLOAT, &RayCast.x, "");
	TwAddVarRO(m_bar, "Rays Direction y: ", TW_TYPE_FLOAT, &RayCast.y, "");
	TwAddVarRO(m_bar, "Rays Direction z: ", TW_TYPE_FLOAT, &RayCast.z, "");

	TwAddSeparator(m_bar, "Mouse World Position", "");
	TwAddVarRO(m_bar, "Mouse Pos x: ", TW_TYPE_FLOAT, &tempMousePos.x, "");
	TwAddVarRO(m_bar, "Mouse Pos y: ", TW_TYPE_FLOAT, &tempMousePos.y, "");
	TwAddVarRO(m_bar, "Mouse Pos z: ", TW_TYPE_FLOAT, &tempMousePos.z, "");

	TwAddVarRO(m_bar, "Cooldown: ", TW_TYPE_FLOAT, &cooldown, "");

	std::cout << "ITS REDS TURN! \n";
}

Application::~Application()
{
	glfwDestroyWindow(window);
	Gizmos::destroy();
	glfwTerminate();
	TwTerminate();


}

void Application::RunGame()
{

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
	{
		
		
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glClearColor(0.25f,0.25f,0.25f,1);

		Update();
		Draw();

		////Update Loop Put all Crap here
		//currentTime = (float)glfwGetTime();
		//DeltaTime = currentTime - previousTime;
		//previousTime = currentTime;
	
		TwDraw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}	
}

void Application::Update()
{

	
	double xpos, ypos;
	//GetCursorPos(&mousepos);
	glfwGetCursorPos(window, &xpos, &ypos);
	
	
	
	m_Pclient->Update();
	m_PlayerNumber = m_Pclient->GetClientID();
	TurnOrder = m_Pclient->TurnOrderPacket();


	if (m_Pclient->GetMoveInfo().IDRefrence != -1)
	{
		
		CheckerPieces[m_Pclient->GetMoveInfo().IDRefrence]->SetPosition(m_Pclient->GetMoveInfo().Position);
		CheckerPieces[m_Pclient->GetMoveInfo().IDRefrence]->SetKing(m_Pclient->GetMoveInfo().isKing);
		

		
	}

	if (m_Pclient->GetDelInfo().IDRefrence != -1)
	{
		CheckerPieces[m_Pclient->GetDelInfo().IDRefrence]->setActive(false);
		//Tiles[m_Pclient->GetDelInfo().ObjectRefrence]->NullCheckerRef();
		//Tiles[m_Pclient->GetDelInfo().ObjectRefrence]->SetCheckerRefrence(CheckerPieces[m_Pclient->GetMoveInfo().IDRefrence]);
		//m_Pclient->NullDelInfo();
	}
	

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_PlayerNumber == TurnOrder)
	{
		vec3 Tileloc = camera->pickAgainstPlane((float)xpos, (float)ypos, vec4(0, 1, 0, 1));
		tempMousePos = Tileloc;
	}


	for (unsigned int i = 0; i < Tiles.size(); i++)
	{

		Tiles[i]->ValidMoves(Tiles);
		Tiles[i]->MadatoryMove();

		if (Tiles[i]->MadatoryMove() && Tiles[i]->GetPiece()->GetColour() == TurnOrder)
		{
			tempMousePos = Tiles[i]->GetPosition();
		}

		Tiles[i]->Update(tempMousePos, Tiles);
		Tiles[i]->SetlegalMove(false);


		if (Tiles[i]->GetPiece() != nullptr && Tiles[i]->GetPiece()->GetID() == m_Pclient->GetMoveInfo().IDRefrence)
		{
			Tiles[i]->NullCheckerRef();
			Tiles[m_Pclient->GetMoveInfo().ObjectRefrence]->SetCheckerRefrence(CheckerPieces[m_Pclient->GetMoveInfo().IDRefrence]);

		}


		if (Tiles[i]->GetPiece() != nullptr && Tiles[i]->GetPiece()->GetID() == m_Pclient->GetDelInfo().IDRefrence)
		{
			Tiles[i]->NullCheckerRef();
			Tiles[m_Pclient->GetDelInfo().ObjectRefrence]->SetCheckerRefrence(CheckerPieces[m_Pclient->GetMoveInfo().IDRefrence]);
			m_Pclient->NullDelInfo();
		
		}

		if (Tiles[i]->isSelected() && Tiles[i]->GetPiece() != nullptr && Tiles[i]->GetPiece()->GetColour() == TurnOrder)
		{
			std::vector<CheckerBoardTile *> TempTile = Tiles[i]->ReturnValidMoves();

			if (TempTile.size() > 0)
			{
				for (unsigned int i = 0; i < TempTile.size(); i++)
				{
					Tiles[TempTile[i]->getObjectRef()]->SetlegalMove(true);
				}
			}

		
		


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && Tiles[i]->isSelected() &&
			Tiles[i]->GetPiece() != nullptr && Tiles[i]->GetPiece()->GetColour() == TurnOrder && m_PlayerNumber == TurnOrder
			&& cooldown < 0)
		{
			cooldown = 1.0f;

			vec3 Tileloc = camera->pickAgainstPlane((float)xpos, (float)ypos, vec4(0, 1, 0, 1));
			MovePieceRayCast = Tileloc;

			tempMousePos = Tileloc;


			int ObjRefrence = Tiles[i]->GetPiece()->GetObjReference();


			CheckerPiece *checker = Tiles[i]->MovePiece(MovePieceRayCast);


			if (checker->GetObjReference() != ObjRefrence)
			{
				//if (checker != Tiles[i]->GetPiece())

				for (unsigned int j = 0; j < CheckerPieces.size(); j++)
				{
					if (CheckerPieces[j]->GetID() == checker->GetID())
					{

						CheckerPieces[j] = checker;
						Tiles[i]->NullCheckerRef();
						Tiles[checker->GetObjReference()]->SetCheckerRefrence(checker);

						m_Pclient->SendMovePacket(checker->GetPosition(), checker->GetObjReference(), checker->GetID(), checker->isThisKing());
						TurnOrder = !TurnOrder;
						m_Pclient->SetTurnOrder(TurnOrder);

						

					}

					//if (deltile != nullptr && CheckerPieces[j]->GetID() == deltile->GetID())
					//{
					//	for (int x = 0; x < Tiles.size(); x++)
					//	{
					//		if (Tiles[x]->GetPiece() == deltile)
					//			Tiles[x]->NullCheckerRef();
					//			CheckerPieces[j]->setActive(false);
					//	}
					//
					//	TurnOrder = !TurnOrder;
					//	
					//
					//}
				}
				CheckerPiece *deltile = Tiles[i]->DeletePiece();


				for (unsigned int j = 0; j < CheckerPieces.size(); j++)
				{
					if (deltile != nullptr && CheckerPieces[j]->GetID() == deltile->GetID())
					{
						for (unsigned int x = 0; x < Tiles.size(); x++)
						{
							if (Tiles[x]->GetPiece() == deltile)
							{
								Tiles[x]->NullCheckerRef();
								Tiles[checker->GetObjReference()]->ValidMoves(Tiles);
								TurnOrder = Tiles[checker->GetObjReference()]->JumpAgain(TurnOrder);
								m_Pclient->SetTurnOrder(TurnOrder);
								m_Pclient->SendDeletePacket(deltile->GetObjReference(), deltile->GetID(), TurnOrder);
								std::cout << "Deleting Object Refrence at: " << x << "\n";
							}

							CheckerPieces[j]->setActive(false);
						}
					}
				}



				if (TurnOrder)
				{
					std::cout << "ITS REDS TURN! \n";
				}
				else
				{
					std::cout << "ITS BLUES TURN! \n";
				}

				m_Pclient->SendTurnOrderPacket();

			}
		}
	}
	}
	currentTime = (float)glfwGetTime();
	DeltaTime = currentTime - previousTime;
	previousTime = currentTime;
	m_timer += DeltaTime;
	cooldown -= DeltaTime;

	camera->update(DeltaTime);
	camera->getWorldTransform();
}

void Application::Draw()
{

	Gizmos::clear();


	for (unsigned int i = 0; i < Tiles.size(); i++)
	{
		Tiles[i]->Draw();
	}

	for (unsigned int i = 0; i < CheckerPieces.size(); i++)
	{
		CheckerPieces[i]->Draw();
	}

	
	m_skybox->Draw(camera);
	Gizmos::draw(camera->getProjectionTransform());
	
	
	
}

void Application::GenerateGrid(unsigned int rows, unsigned int cols)
{
	Vertex* aoVertices = new Vertex[rows * cols];

	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			aoVertices[ i * cols + j ].position = vec4((float)j, 0, (float)i, 1);

			//Creating a colour scheme based of something random/ CRAZY!!!
			vec3 Colour = vec3(sinf( (j/ (float)(cols - 1)) * ( i/ (float)(rows - 1))));


			aoVertices[ i * cols + j].colour = vec4( 0,1,1, 1);

		}
	}

	//Defining index count based of quad counter (2 triangles per quad or 3 Vertexs for a triangle) :P
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int indexCount = 0;

	for (unsigned int i = 0; i < (rows - 1); i++)
	{
		for (unsigned int j = 0; j < (cols - 1); j++)
		{
			//Triangle 1 
			auiIndices[indexCount++] = i * cols + j;
			auiIndices[indexCount++] = (i + 1) * cols + j;
			auiIndices[indexCount++] = (i + 1) * cols + (j + 1);

			//Triangle 2
			auiIndices[indexCount++] = i * cols + j;
			auiIndices[indexCount++] = (i + 1 ) * cols + (j + 1);
			auiIndices[indexCount++] = i * cols + (j + 1);
		}
	}




	m_indexCount = indexCount;

	// Create VAO, VBO and IBO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	// Bind buffers
	glBindVertexArray( m_VAO );
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	
	// send data to the buffers	
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	// describe how the vertices are setup so they can be sent to the shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)*2));

	// m_VAO hold all our ARRAY_BUFFER and ARRAY_ELEMENT_BUFFER settings
	// so just rebind it later before using glDrawElements
	glBindVertexArray(0);


	delete[] auiIndices;
	delete[] aoVertices;
}


void Application::CreateGrid()
{
	glm::mat4* Identity = new mat4(1, 0, 0, 1,
							  0, 1, 0, 1,
							  0, 0, 1, 1,
							  0, 0, 0, 1);

		

	vec4 ColourWhite = vec4(.4, .25, .13, 1); //Dark Brown Colour
	vec4 ColourBlack = vec4(1, .8, .39, 1); //Light Peache Colour

	//Rows and Collumns should always be 8x8 on a regular checkerboard/chessboard

	int GridSize = 8;
	bool isBlack = true;

	vec3 Position;
	
	int loc = 0;

	int checkRef = 0;
	int NumOfCheckers = 12; 

	for (int j = 0; j < GridSize; j++)
	{
		for (int i = 0; i < GridSize; i++)
		{

			Position = vec3(i * 6, 0, j * 6);

			if (isBlack)
			{

				Tiles.push_back(new CheckerBoardTile(Position, ColourWhite,loc,i));
				Position.y += 1;

				if (NumOfCheckers > 0)
				{
					CheckerPiece *p = new CheckerPiece(Position, vec4(1, 0, 0, 1));
					p->SetColour(true);
					p->SetIDRef(checkRef++);
					p->SetObjRefrence(loc);
					CheckerPieces.push_back(p);
					NumOfCheckers -= 1;
					Tiles[loc]->SetCheckerRefrence(p);
					
				}
				
			
				if (loc > 39)
				{
					CheckerPiece *p = new CheckerPiece(Position, vec4(0, 0, 1, 1));
					p->SetColour(false);
					p->SetIDRef(checkRef++);
					p->SetObjRefrence(loc);
					CheckerPieces.push_back(p);
					Tiles[loc]->SetCheckerRefrence(p);

				}

			}

				
			
			else
			{
				
				Tiles.push_back(new CheckerBoardTile(Position, ColourBlack, loc,i));
			}
				
			
			loc++;

			isBlack = !isBlack;
		}
		isBlack = !isBlack;
	}

	Gizmos::addCylinderFilled(vec3(0, 2, 0), 2, .5f, 40, vec4(1, 0, 0, 1), Identity);
}

vec3 Application::RayCastCallback(int mouse_x, int mouse_y)
{

	float x = (2.0f * mouse_x) / 1280 - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / 720;
	float z = 1.0f;

	vec3 ray_nds = vec3(x, y, z);

	vec4 ray_clip = vec4(ray_nds.xy, -1., 1.0);

	mat4 projview = camera->getProjectionView();
	projview = glm::inverse(projview);
	
	vec4 ray_eye = (vec4)(projview * ray_clip);
	ray_eye = vec4(ray_eye.xy, -1.0f, 0.0f);
	

	vec3 ray_world = (camera->getWorldTransform() * ray_eye).xyz;
	ray_world = normalize(ray_world);

	return ray_world;
}

std::vector<CheckerBoardTile *> Application::ReturnTiles()
{

	return Tiles;
}