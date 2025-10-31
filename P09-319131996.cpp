/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
bool avanzaDragon;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0f;
float toffsetnumerocambiau2 = 0.0f;
float angulovaria = 0.0f;

//letrero 
float toffsetLetrero = 0.0f;
float toffsetLetrerov = 0.0f;
// Dragon
float dragonavance = 0.0f;
float giroDragon;
float aleteo;
bool subiendo = true;

// LOS NÚMEROS
float tiempoNumero = 0.0f;        // Contador de tiempo para número cambiante
float tiempoTextura = 0.0f;       // Contador de tiempo para alternar texturas
bool usarNumero1 = true;          // Bandera para alternar entre Numero1 y Numero2

// PUERTAS 
float rotacionPuertaDerecha = 0.0f;      // Rotación de puerta derecha (0 a 90 grados)
float deslizamientoPuertaIzq = 0.0f;     // Deslizamiento puerta izquierda (0 a 20 unidades)
bool puertasAbiertas = false;             // Estado de las puertas
bool animandoPuertas = false;             // Bandera para controlar animación

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture LetreroTexture;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Model AlaDerecha_M;
Model AlaIzquierda_M;

Model Pilares_M;
Model PuertaDerecha_M;
Model PuertaIzquierda_M;
Model Cartel_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};



	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		    0.0f, -1.0f, 0.0f,

	};
	unsigned int letreroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat letreroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(letreroVertices, letreroIndices, 32, 6);
	meshList.push_back(obj8); 

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	LetreroTexture = Texture("Textures/letrero.tga");
	LetreroTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/carro.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/Tiamat.obj");

	AlaDerecha_M = Model();
	AlaDerecha_M.LoadModel("Models/alaDerecha.obj");
	AlaIzquierda_M = Model();
	AlaIzquierda_M.LoadModel("Models/alaIzquierda.obj");


	Pilares_M = Model();
	Pilares_M.LoadModel("Models/pilares.obj");
	PuertaDerecha_M = Model();
	PuertaDerecha_M.LoadModel("Models/puertaDerecha.obj");
	PuertaIzquierda_M = Model();
	PuertaIzquierda_M.LoadModel("Models/puertaIzquierda.obj");
	Cartel_M = Model();
	Cartel_M.LoadModel("Models/cartel.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.5f;
	rotllanta = 0.0f;
	rotllantaOffset = 15.0f;
	giroDragon = 0.0f;
	aleteo = -40.0f;
	dragonavance = 0.0f;

	// INICIALIZAR PUERTAS
	rotacionPuertaDerecha = 0.0f;
	deslizamientoPuertaIzq = 0.0f;
	puertasAbiertas = false;
	animandoPuertas = false;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 modelaux3(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		// ------------------------------ ANIMACION (SIMPLE) DEL COCHE --------------------------------
		if (avanza) {
			if (movCoche > -245.0f) //-245.0f
			{
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else
			{
				avanza = !avanza;
			}
		}
		else
		{
			if (movCoche < 345.0f) //345.0f
			{
				movCoche += movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;
			}
			else
			{
				avanza = !avanza;
			}
		}
		// --------------------------------------------------------------------------------------

		//Dragon

		// ------------------------------ ANIMACION DEL DRAGON --------------------------------
		if (avanzaDragon) {
			if (dragonavance < 10.0f)
			{
				angulovaria += 7.0f * deltaTime;
				dragonavance += 0.1f * deltaTime;
			}
			else
			{
				avanzaDragon = false;
				giroDragon += 180;
			}
		}
		else
		{
			if (dragonavance > -10.0f)
			{
				angulovaria += 7.0f * deltaTime;
				dragonavance -= 0.1f * deltaTime;
			}
			else
			{
				avanzaDragon = true;
				giroDragon += 180;
			}
		}
		// ------------------------------ ANIMACION DE ALAS (ALETEO) --------------------------------
		if (subiendo) {
			if (aleteo < 40.0f)
			{
				aleteo += 3.0f * deltaTime; // Velocidad de aleteo (ajusta según prefieras)
			}
			else
			{
				subiendo = false;
			}
		}
		else
		{
			if (aleteo > -40.0f)
			{
				aleteo -= 3.0f * deltaTime;
			}
			else
			{
				subiendo = true;
			}
		}
		// ------------------------------- ANIMACION DE PUERTA ----------------------------------------------

		  // DETECCIÓN DE TECLA 'O' PARA ABRIR/CERRAR PUERTAS
		if (mainWindow.getsKeys()[GLFW_KEY_O])
		{
			if (!animandoPuertas)
			{
				puertasAbiertas = !puertasAbiertas;
				animandoPuertas = true;
				mainWindow.getsKeys()[GLFW_KEY_O] = false;
			}
		}

		// ------------------------------- ANIMACION DE PUERTAS ----------------------------------------------
		if (animandoPuertas)
		{
			if (puertasAbiertas)  // Abrir puertas
			{
				if (rotacionPuertaDerecha < 90.0f)
				{
					rotacionPuertaDerecha += 2.0f * deltaTime;
					if (rotacionPuertaDerecha > 90.0f)
						rotacionPuertaDerecha = 90.0f;
				}

				if (deslizamientoPuertaIzq < 20.0f)
				{
					deslizamientoPuertaIzq += 0.5f * deltaTime;
					if (deslizamientoPuertaIzq > 20.0f)
						deslizamientoPuertaIzq = 20.0f;
				}

				if (rotacionPuertaDerecha >= 90.0f && deslizamientoPuertaIzq >= 20.0f)
				{
					animandoPuertas = false;
				}
			}
			else  // Cerrar puertas
			{
				if (rotacionPuertaDerecha > 0.0f)
				{
					rotacionPuertaDerecha -= 2.0f * deltaTime;
					if (rotacionPuertaDerecha < 0.0f)
						rotacionPuertaDerecha = 0.0f;
				}

				if (deslizamientoPuertaIzq > 0.0f)
				{
					deslizamientoPuertaIzq -= 0.5f * deltaTime;
					if (deslizamientoPuertaIzq < 0.0f)
						deslizamientoPuertaIzq = 0.0f;
				}

				if (rotacionPuertaDerecha <= 0.0f && deslizamientoPuertaIzq <= 0.0f)
				{
					animandoPuertas = false;
				}
			}
		}




			//Recibir eventos del usuario
			glfwPollEvents();
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

			// Clear the window
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			shaderList[0].UseShader();
			uniformModel = shaderList[0].GetModelLocation();
			uniformProjection = shaderList[0].GetProjectionLocation();
			uniformView = shaderList[0].GetViewLocation();
			uniformEyePosition = shaderList[0].GetEyePositionLocation();
			uniformColor = shaderList[0].getColorLocation();
			uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

			//información en el shader de intensidad especular y brillo
			uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
			uniformShininess = shaderList[0].GetShininessLocation();

			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			// luz ligada a la cámara de tipo flash
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

			//información al shader de fuentes de iluminación
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			shaderList[0].SetSpotLights(spotLights, spotLightCount);


			//Reinicializando variables cada ciclo de reloj
			model = glm::mat4(1.0);
			modelaux = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			toffset = glm::vec2(0.0f, 0.0f);

			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
			model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			pisoTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

			meshList[2]->RenderMesh();



			// ---------------------------------------------- Coche ---------------------------------------------------
			//Instancia del coche 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(movCoche - 50.0f, -0.4f, -2.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Kitt_M.RenderModel();

			//Llanta delantera izquierda
			model = modelaux;
			model = glm::translate(model, glm::vec3(-3.7f, -0.4f, 3.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Llanta_M.RenderModel();


			//Llanta trasera izquierda
			model = modelaux;
			model = glm::translate(model, glm::vec3(5.0f, -0.4f, 3.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Llanta_M.RenderModel();


			//Llanta delantera derecha
			model = modelaux;
			model = glm::translate(model, glm::vec3(5.0f, -0.4f, -3.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Llanta_M.RenderModel();

			//Llanta trasera derecha
			model = modelaux;
			model = glm::translate(model, glm::vec3(-3.7f, -0.4f, -3.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Llanta_M.RenderModel();


			// --------------------------------------- DRAGON --------------------------------------------------------
			//dragonavance para moverlo hacia adelante y atrás en X
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(dragonavance - 5.0f, 5.0f + sin(glm::radians(angulovaria)), 6.0));
			modelaux2 = model;
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, giroDragon * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Dragon_M.RenderModel();

			//Ala derecha
			model = modelaux2;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, giroDragon * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -aleteo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			AlaDerecha_M.RenderModel();


			//Ala Izquierda
			model = modelaux2;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, giroDragon * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, aleteo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			AlaIzquierda_M.RenderModel();


			// -------------------------------- ENTRADA -------------------------------------
			//Instancia de pilares
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(50.0f, -2.6f, -2.0f));
			modelaux3 = model;
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Pilares_M.RenderModel();

			// Puerta Derecha - ROTACIÓN sobre su eje
			model = modelaux3;
			model = glm::translate(model, glm::vec3(0.0f, 15.0f, -20.3f));
			model = glm::rotate(model, rotacionPuertaDerecha * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			PuertaDerecha_M.RenderModel();

			// Puerta Izquierda - DESLIZAMIENTO en Z
			model = modelaux3;
			model = glm::translate(model, glm::vec3(4.0f, 14.5f, 19.0f + deslizamientoPuertaIzq));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			PuertaIzquierda_M.RenderModel();

			// Cartel
			model = modelaux3;
			model = glm::translate(model, glm::vec3(0.0f, 43.5f, 0.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Cartel_M.RenderModel();



			//blending: transparencia o traslucidez
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			AgaveTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[3]->RenderMesh();

			// ----------------------------- ANIMACION DE TEXTURA LETRERO ----------------------------------

			//textura con movimiento
			//Importantes porque la variable uniform no podemos modificarla directamente
			toffsetLetrero += 0.001;
			toffsetLetrerov = 0.000;
			//para que no se desborde la variable
			if (toffsetflechau > 1.0)
				toffsetflechau = 0.0;
		
			toffset = glm::vec2(toffsetLetrero, toffsetLetrerov);

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(46.7f, 41.0f, -2.5f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(50.0f, 50.0f, 10.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			/*color = glm::vec3(1.0f, 0.0f, 0.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
			LetreroTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[7]->RenderMesh();

			// ----------------------------- ANIMACION DE TEXTURA FLECHAS ----------------------------------

			//textura con movimiento
			//Importantes porque la variable uniform no podemos modificarla directamente
			toffsetflechau += 0.001;
			toffsetflechav = 0.000;
			//para que no se desborde la variable
			if (toffsetflechau > 1.0)
				toffsetflechau = 0.0;
			//if (toffsetv > 1.0)
			//	toffsetv = 0;
			//printf("\ntfosset %f \n", toffsetu);
			//pasar a la variable uniform el valor actualizado
			toffset = glm::vec2(toffsetflechau, toffsetflechav);

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			FlechaTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();


			// -------------------------------------- NUMEROS ----------------------------------------------------
			//plano con todos los números
			toffsetnumerou = 0.0;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[5]->RenderMesh();

			//número 1
			//toffsetnumerou = 0.0;
			//toffsetnumerov = 0.0;
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

			for (int i = 1; i < 4; i++)
			{
				//números 2-4
				toffsetnumerou += 0.25;
				toffsetnumerov = 0.0;
				toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
				glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 1.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				NumerosTexture.UseTexture();
				Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
				meshList[6]->RenderMesh();

			}

			for (int j = 1; j < 5; j++)
			{
				//números 5-8
				toffsetnumerou += 0.25;
				toffsetnumerov = -0.33;
				toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
				glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 1.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				NumerosTexture.UseTexture();
				Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
				meshList[6]->RenderMesh();
			}


			//------------------------- número cambiante con velocidad visible --------------------------------
			tiempoNumero += deltaTime;
			if (tiempoNumero >= 1.0f) {
				toffsetnumerocambiau += 0.005;
				if (toffsetnumerocambiau > 1.0)
					toffsetnumerocambiau = 0.0;
				tiempoNumero = 0.0f;
			}
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();


			// -------------------------- cambiar automáticamente entre textura número 1 y número 2 ---------------------
			tiempoTextura += deltaTime;
			if (tiempoTextura >= 1.0f) {
				toffsetnumerocambiau2 += 0.02;
				if (toffsetnumerocambiau2 > 1.0)
				{
					toffsetnumerocambiau2 = 0.0f;
					usarNumero1 = !usarNumero1;
				}
				tiempoTextura = 0.0f;
			}


			toffsetnumerou = 0.0;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));

			if (usarNumero1) {
				Numero1Texture.UseTexture();
			}
			else {
				Numero2Texture.UseTexture();
			}

			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[5]->RenderMesh();




			glDisable(GL_BLEND);





			glUseProgram(0);

			mainWindow.swapBuffers();
		}

		return 0;
	}