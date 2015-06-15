#ifndef APPLICATION_H
#define APPLICATION_H


#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include "Skybox.h"
#include "AntTweakBar.h"
#include <iostream>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "FlyCamera.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include "CheckerBoard.h"
#include "CheckerPiece.h"
#include "Client.h"

class Texture;
class ProceduralGen;
class Skybox;
class ObjLoader;
class FbxLoader;

struct Vertex2 {
float x, y, z, w;
float nx, ny, nz, nw;
float tx, ty, tz, tw;
float s, t;
};


class CheckerBoardTile;

struct Vertex
{
	vec4 position;
	vec4 colour;
	vec2 UV;
};

class Application
{
public:
	Application();
	~Application();
	
	void RunGame();
	void Update();
	void Draw();
	


	void GenerateGrid(unsigned int rows, unsigned int cols); //Reattempt
	void CreateGrid();
	void CreatePieces();
	std::vector<CheckerBoardTile *> ReturnTiles();
	void MouseClickCallback();
	vec3 RayCastCallback(int mouse_x, int mouse_y);
	


private:
	
	// our vertex and index buffers
	unsigned int m_VAO;	// Vertex Array Object (Just winging it)
	unsigned int m_VBO; // Vertex Buffer Object (Stores vertex Information)
	unsigned int m_IBO;	// Index Buffer Object (Stores Indicies)?
	unsigned char* data;
	int m_rows;
	int m_collumns;
	TwBar* m_bar;
	
	const char *filepath;
	
	const char *vsSource;
	const char *fsSource;

	POINT mousepos;
	vec4 m_clearcolour;
	vec3 RayCast;

	bool res;



	unsigned int m_programID;

	unsigned int vertexShader;
	unsigned int fragmentShader;


	int m_ClientNumber;



	int vertcount;
	unsigned int* indiciesCount;

	unsigned int m_indexCount;

	float m_timer;

	int success;
	//FBXFile file;

	vec3 MovePieceRayCast;
	FbxLoader* m_EnemyElite;
	FbxLoader* m_Pyro;
	FbxLoader* m_Tree;

	Skybox* m_skybox;




	ObjLoader* ObjModel;
	//Stuff to do with textures
	Texture *image;
	unsigned int m_texture;
	unsigned int imageWidth, imageHeight, imageFormat, loc;
	
	ProceduralGen* m_terrain;
	

	GLFWwindow * window;
	FlyCamera * camera;
	float previousTime;
	float currentTime;
	float DeltaTime;
	float cooldown;

	int m_PlayerNumber;
	bool m_PlayerTurn;


	int m_seed;
	float m_amp , m_persis;
	bool TurnOrder;
	bool CantSelect;

	std::vector<CheckerBoardTile *> Tiles;
	std::vector<CheckerPiece *> CheckerPieces;


	int m_vertexCount;

	vec3 m_LightDirection;
	vec3 m_AmbientColour;
	vec4 m_startcolour, m_endcolour;

	vec2 MousePosition;
	bool finishMove;


	unsigned int m_ModelIndexCount;
	std::vector<unsigned int> m_elements;
	std::vector<Vertex> ModelVerts;
	std::vector<vec2> ModelUV;
	std::vector<vec3> ModelNormals;

	Client* m_Pclient;

	vec3 tempMousePos;

};







#endif