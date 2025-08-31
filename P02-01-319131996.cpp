//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader
static const char* vShader      = "shaders/shader.vert";
static const char* fShader      = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vVerde = "shaders/shaderverdeventana.vert";
static const char* fverde = "shaders/shaderverdeventana.frag";
static const char* vRojo        = "shaders/shaderrojo.vert";
static const char* fRojo        = "shaders/shaderrojo.frag";
static const char* vCafe        = "shaders/shadercafe.vert";
static const char* fCafe        = "shaders/shadercafe.frag";
static const char* vVarbol      = "shaders/shaderverdearbol.vert";
static const char* fVarbol      = "shaders/shaderverdearbol.frag";
static const char* vAzul = "shaders/shaderazul.vert";
static const char* fAzul = "shaders/shaderazul.frag";


//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letraR[] = {
		-0.78f, 0.48f, 0.0f,	0.0f,	0.0f,	1.0f,
		-0.78f, 0.36f, 0.0f,	0.0f,	0.0f,	1.0f,
		-0.3f, 0.48f, 0.0f,		0.0f,	0.0f,	1.0f,

		-0.3f, 0.48f, 0.0f,		0.0f,	0.0f,	1.0f,
		-0.3f, 0.36f, 0.0f,		0.0f,	0.0f,	1.0f,
		-0.78f, 0.36f, 0.0f,	0.0f,	0.0f,	1.0f,

		-0.3f, 0.48f, 0.0f,		0.0f,	0.0f,	1.0f,
		-0.3f, 0.3f, 0.0f,		0.0f,	0.0f,	1.0f,
		-0.18f, 0.3f, 0.0f,		0.0f,	0.0f,	1.0f,

		-0.3f, 0.3f, 0.0f,		0.0f,	0.0f,	1.0f,
		-0.18f, 0.3f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.3f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.18f, 0.3f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.18f, 0.12f, 0.0f,	0.0f,	0.0f,	1.0f,
		-0.3f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.42f, 0.36f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.3f, 0.36f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.3f, 0.36f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.3f, -0.06f, 0.0f,	0.0f,	0.0f,	1.0f,
		-0.42f, -0.06f, 0.0f,	0.0f,	0.0f,	1.0f,

		-0.78f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, 0.12f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.78f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.78f, 0.12f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, 0.12f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.78f, 0.36f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.78f, 0.12f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.66f, 0.12f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.78f, 0.36f, 0.0f,  	0.0f,	0.0f,	1.0f,
		-0.66f, 0.36f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.66f, 0.12f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.78f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.78f, -0.54f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.66f, -0.54f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.78f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.66f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.66f, -0.54f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.54f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, -0.54f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,

		-0.42f, -0.06f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.42f, -0.54f, 0.0f, 	0.0f,	0.0f,	1.0f,
		-0.3f, -0.54f, 0.0f, 	0.0f,	0.0f,	1.0f,

	};

	MeshColor* letraR = new MeshColor();
	letraR->CreateMeshColor(vertices_letraR, 270);
	meshColorList.push_back(letraR);

	GLfloat vertices_letraA[] = {
		//A
		-0.12f, 0.0f, 0.0f,			1.0f,	0.0f,	0.0f,
		-0.12f, 0.48f, 0.0f,		1.0f,	0.0f,	0.0f,
		0.18f, 0.48f, 0.0f,			1.0f,	0.0f,	0.0f,

		0.36f, 0.0f, 0.0f, 			1.0f,	0.0f,	0.0f,
		0.36f, 0.48f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.06f, 0.48f, 0.0f,			1.0f,	0.0f,	0.0f,

		-0.12f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		-0.12f, 0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.36f, 0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,

		0.36f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.36f, 0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		-0.12f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,

		-0.12f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		-0.12f, -0.54f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.0f, -0.54f, 0.0f, 		1.0f,	0.0f,	0.0f,

		0.0f, -0.54f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.0f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		-0.12f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,

		0.24f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.24f, -0.54f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.36f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,

		0.36f, -0.18f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.36f, -0.54f, 0.0f, 		1.0f,	0.0f,	0.0f,
		0.24f, -0.54f, 0.0f, 		1.0f,	0.0f,	0.0f,

	};

	MeshColor* letraA = new MeshColor();
	letraA->CreateMeshColor(vertices_letraA, 144);
	meshColorList.push_back(letraA);


	GLfloat vertices_letraL[] = {
		0.42f, 0.48f, 0.0f,  		0.0f,	1.0f,	0.0f,
		0.42f, -0.54f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.6f, 0.48f, 0.0f,   		0.0f,	1.0f,	0.0f,

		0.6f, 0.48f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.6f, -0.54f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.42f, -0.54f, 0.0f,   		0.0f,	1.0f,	0.0f,

		0.6f, -0.36f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.6f, -0.54f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.9f, -0.36f, 0.0f,   		0.0f,	1.0f,	0.0f,

		0.9f, -0.36f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.9f, -0.54f, 0.0f,   		0.0f,	1.0f,	0.0f,
		0.6f, -0.54f, 0.0f,   		0.0f,	1.0f,	0.0f,

	};

	MeshColor* letraL = new MeshColor();
	letraL->CreateMeshColor(vertices_letraL, 72);
	meshColorList.push_back(letraL);
	// ---------------------indice [0]----------------------------------------- 
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	//-----------------------indice [1]--------------------------------------------

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		
	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	//--------------------------indice [2]----------------------------------------------------

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	//---------------------------indice [3]-------------------------------------------------

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	//----------------------------indice [4]--------------------------------------------------

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	// ------------------------------- indice[5]----------------------------------
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

}


void CreateShaders()
{
	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1); //0

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2); // 1

	Shader* shader3 = new Shader();//shader para usar color como parte del VAO: letras 
	shader3->CreateFromFiles(vRojo, fRojo);
	shaderList.push_back(*shader3); // 2

	Shader* shader4 = new Shader();//shader para usar color como parte del VAO: letras 
	shader4->CreateFromFiles(vAzul, fAzul);
	shaderList.push_back(*shader4); // 3

	Shader* shader5 = new Shader();//shader para usar color como parte del VAO: letras 
	shader5->CreateFromFiles(vCafe, fCafe);
	shaderList.push_back(*shader5); // 4

	Shader* shader6 = new Shader();//shader para usar color como parte del VAO: letras 
	shader6->CreateFromFiles(vVarbol, fVarbol);
	shaderList.push_back(*shader6); //5

	Shader* shader7 = new Shader();//shader para usar color como parte del VAO: letras 
	shader7->CreateFromFiles(vVerde, fverde);
	shaderList.push_back(*shader7); //6
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;


	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.820f, 0.900f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		// CUBO ROJO CUBO CUBO ROJO  ROJO  CUBO ROJO CUBO ROJO 
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramide Azul  Piramide Azul  Piramide Azul  Piramide Azul
		
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, -2.0f));
		model = glm::scale(model, glm::vec3(2.1f, 0.6f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		// CUADRADO VERDE CUADRADO VERDE CUADRADO VERDE CUADRADO VERDE CUADRADO VERDE CUADRADO VERDE 
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.4f, -0.1f, -2.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.4f, -0.1f, -2.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// TRIANGULO VERDE RIANGULO VERDE RIANGULO VERDE RIANGULO VERDE RIANGULO VERDE RIANGULO VERDE 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.4f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.4f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();


		// CUADRADO CAFÉ  CUADRADO CAFÉ CUADRADO CAFÉ CUADRADO CAFÉ CUADRADO CAFÉ CUADRADO CAFÉ 

		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.1f, -1.3f, -3.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.7f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.1f, -1.3f, -3.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.7f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramide Abol Piramide Abol Piramide Abol Piramide Abol 
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, -0.5f, -3.3f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.0f, -0.5f, -3.3f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

/*
	//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
	shaderList[1].useShader();
	uniformModel = shaderList[1].getModelLocation();
	uniformProjection = shaderList[1].getProjectLocation();

	//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[0]->RenderMeshColor();

	//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[1]->RenderMeshColor();

	//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	meshColorList[2]->RenderMeshColor();
	*/

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
// 
// 
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));


//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));


////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));


///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/

/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/
