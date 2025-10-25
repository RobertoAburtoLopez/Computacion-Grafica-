/*
Pr�ctica 8: Iluminaci�n 2 
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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
// Helicoptero Helices
GLfloat giroHelices = 0.0f;
bool luzAntorchaEncendida = true; // Variable para controlar el estado de la luz
bool luzAlienEncendida = true; // Variable para controlar el estado de la luz


Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture Dado_Ocho;

Model Kitt_M;
Model Llanta_M;
Model Cofre_M;
Model Blackhawk_M;
Model BlackhawkHelices_M;
Model Antorcha_M;
Model Alien_M;



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


//funci�n de calculo de normales por promedio de v�rtices 
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
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CrearDado_OCHO()
{
	// Definici�n de los �ndices (8 caras * 3 v�rtices = 24 �ndices)
	unsigned int octahedron_indices[] = {
		 0,  1,  2,   // Cara 1 (T1)
		 3,  4,  5,   // Cara 2 (T2)
		 6,  7,  8,   // Cara 3 (T3)
		 9, 10, 11,   // Cara 4 (T4)
		12, 13, 14,   // Cara 5 (T5)
		15, 16, 17,   // Cara 6 (T6)
		18, 19, 20,   // Cara 7 (T7)
		21, 22, 23    // Cara 8 (T8)
	};


	GLfloat octahedron_vertices[] = {

		// Cara 1
		 0.0f,   0.0f,  0.5f,    0.05f, 0.79f,   -1.0f,  -1.0f,  -1.0f,
		 0.5f,   0.0f,  0.0f,    0.23f, 0.79f,   -1.0f,  -1.0f,  -1.0f,
		 0.0f,   0.5f,  0.0f,    0.125f, 1.0f,   -1.0f,  -1.0f,  -1.0f,

		// Cara 4
		  0.0f,   0.0f,  0.5f,    0.96f, 0.79f,    1.0f,  -1.0f,  -1.0f,
		  0.0f,   0.5f,  0.0f,    0.86f, 1.0f,     1.0f,  -1.0f,  -1.0f,
		 -0.5f,   0.0f,  0.0f,    0.75f, 0.79f,    1.0f,  -1.0f,  -1.0f,

		// Cara 8
		  0.0f,   0.0f,  0.5f,    0.96f, 0.77f,    1.0f,   1.0f,  -1.0f,
		 -0.5f,   0.0f,  0.0f,    0.75f, 0.77f,    1.0f,   1.0f,  -1.0f,
		  0.0f,  -0.5f,  0.0f,    0.85f, 0.60f,    1.0f,   1.0f,  -1.0f,

		// Cara 5
		  0.0f,   0.0f,  0.5f,    0.05f, 0.77f,   -1.0f,   1.0f,  -1.0f,
		  0.0f,  -0.5f,  0.0f,    0.125f, 0.60f,  -1.0f,   1.0f,  -1.0f,
		  0.5f,   0.0f,  0.0f,    0.23f, 0.77f,   -1.0f,   1.0f,  -1.0f,

		// Cara 2
		  0.0f,   0.0f, -0.5f,    0.47f, 0.79f,   -1.0f,  -1.0f,   1.0f,
		  0.0f,   0.5f,  0.0f,    0.37f, 0.99f,   -1.0f,  -1.0f,   1.0f,
		  0.5f,   0.0f,  0.0f,    0.28f, 0.79f,   -1.0f,  -1.0f,   1.0f,

	    // Cara 3
		  0.0f,   0.0f, -0.5f,    0.51f, 0.79f,    1.0f,  -1.0f,   1.0f,
	     -0.5f,   0.0f,  0.0f,    0.69f, 0.79f,    1.0f,  -1.0f,   1.0f,
		  0.0f,   0.5f,  0.0f,    0.615f, 1.0f,    1.0f,  -1.0f,   1.0f,

		// Cara 7
		  0.0f,   0.0f, -0.5f,    0.51f, 0.78f,    1.0f,   1.0f,   1.0f,
		  0.0f,  -0.5f,  0.0f,    0.615f, 0.60f,   1.0f,   1.0f,   1.0f,
		  -0.5f,   0.0f,  0.0f,    0.69f, 0.78f,    1.0f,   1.0f,   1.0f,

		// Cara 6
		  0.0f,   0.0f, -0.5f,    0.45f, 0.78f,   -1.0f,   1.0f,   1.0f,
		  0.5f,   0.0f,  0.0f,    0.28f, 0.78f,   -1.0f,   1.0f,   1.0f,
		  0.0f,  -0.5f,  0.0f,    0.37f, 0.60f,   -1.0f,   1.0f,   1.0f,


	};

	Mesh* dado_OCHO = new Mesh();
	dado_OCHO->CreateMesh(octahedron_vertices, octahedron_indices, 192, 24);
	meshList.push_back(dado_OCHO);

	calcAverageNormals(octahedron_indices, 24, octahedron_vertices, 192, 8, 5);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado_OCHO();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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
	Dado_Ocho = Texture("Textures/cubocompleto.tga");
	Dado_Ocho.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/carro.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre.obj");

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/Black Hawk_SinHelices.obj");
	BlackhawkHelices_M = Model();
	BlackhawkHelices_M.LoadModel("Models/Black Hawk uh-60_Helices.obj");

	Antorcha_M = Model();
	Antorcha_M.LoadModel("Models/antorcha.obj");

	Alien_M = Model();
	Alien_M.LoadModel("Models/tripo_convert_fb031157-1ed2-41a8-a190-6c4eb8456628.obj");

	

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


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, // Color 
		0.3f, 0.3f, 
		0.0f, 0.0f, -1.0f);


	// ************************************************** contador de luces puntuales ************************************************


	unsigned int spotLightCount = 0;
	// ------------------------------ linterna --------------------------------------
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, // Color
	//	0.0f, 2.0f, // Intensidad ambiental, intensidad difusa 
	//	0.0f, 0.0f, 0.0f, // Vetor de posicion
	//	0.0f, -1.0f, 0.0f, // Vector de direccion
	//	1.0f, 0.0f, 0.0f, // Ecuacion: constante, lineal, exponencial
	//	5.0f); // Angulo de apertura
	//spotLightCount++;

	// Luz del cofre (COLOR VERDE)
	spotLights[0] = SpotLight(
		0.0f, 1.0f, 0.0f,      // Color VERDE (R, G, B)
		1.0f, 2.0f,            // Intensidad ambiental, intensidad difusa
		0.0f, 0.0f, 0.0f,      // Posici�n inicial (se actualizar�)
		1.0f, 0.0f, 0.0f,      // Direcci�n inicial x
		1.0f, 0.0f, 0.0f,   // Ecuaci�n: constante, lineal, exponencial
		20.0f);                // �ngulo de apertura
	spotLightCount++;

	// Luz coche
	spotLights[1] = SpotLight(
		0.0f, 0.0f, 1.0f,      // Color AZUL (R, G, B)
		3.0f, 2.0f,            // Intensidad ambiental, intensidad difusa
		0.0f, 0.0f, 0.0f,     // Posici�n inicial (se actualizar�)
		1.0f, 0.0f, 0.0f,      // Direcci�n hacia X POSITIVO
		1.0f, 0.09f, 0.032f,    // Ecuaci�n: constante, lineal, exponencial
		20.0f);                // �ngulo de apertura
	spotLightCount++;

	spotLights[2] = SpotLight(
		1.0f, 0.0f, 0.0f,      // Color AZUL (R, G, B)
		3.0f, 2.0f,            // Intensidad ambiental, intensidad difusa
		0.0f, 0.0f, 0.0f,     // Posici�n inicial (se actualizar�)
		-1.0f, 0.0f, 0.0f,      // Direcci�n hacia X POSITIVO
		1.0f, 0.09f, 0.032f,    // Ecuaci�n: constante, lineal, exponencial
		20.0f);                // �ngulo de apertura
	spotLightCount++;

	// Luz Helicoptero
	spotLights[3] = SpotLight(
		1.0f, 1.0f, 0.2f,      // Color amarillo (R, G, B)
		0.5f, 2.0f,            // Intensidad ambiental, intensidad difusa
		0.0f, 0.0f, 0.0f,     // Posici�n inicial (se actualizar�)
		0.0f, -1.0f, 1.0f,      // Direcci�n hacia Y negativo
		1.0f, 0.02f, 0.01f,    // Ecuaci�n: constante, lineal, exponencial
		20.0f);                // �ngulo de apertura
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		//Recibir eventos del usuario
		glfwPollEvents();


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
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);



		// -------------------- Control de la luz de la antorcha con la tecla L ---------------------
		static bool luzAntorchaKeyPressed = false;
		if (mainWindow.getsKeys()[GLFW_KEY_L]) {
			if (!luzAntorchaKeyPressed) {
				luzAntorchaEncendida = !luzAntorchaEncendida;
				luzAntorchaKeyPressed = true;
			}
		}
		else {
			luzAntorchaKeyPressed = false;
		}

		// -------------------- Control de la luz del Alien con la tecla M ---------------------
		static bool luzAlienKeyPressed = false;
		if (mainWindow.getsKeys()[GLFW_KEY_M]) {
			if (!luzAlienKeyPressed) {
				luzAlienEncendida = !luzAlienEncendida;
				luzAlienKeyPressed = true;
			}
		}
		else {
			luzAlienKeyPressed = false;
		}

		// ------------------------------ CONFIGURACI�N DE LUCES --------------------------------------
		unsigned int pointLightCount = 0;

		// ANTORCHA - siempre en posici�n 0
		if (luzAntorchaEncendida) {
			pointLights[pointLightCount] = PointLight(1.0f, 1.0f, 1.0f, // Color blanco
				2.0f, 1.0f, // Intensidad ambiental, intensidad difusa 
				-10.0f, 9.0f, 10.0, // Vector de posicion
				1.0f, 0.09f, 0.032f); // ecuacion de segundo grado
			pointLightCount++;
		}

		// ALIEN - en la siguiente posici�n disponible
		if (luzAlienEncendida) {
			pointLights[pointLightCount] = PointLight(1.0f, 0.0f, 0.0f, // Color rojo
				2.0f, 1.0f, // Intensidad ambiental, intensidad difusa 
				10.0f, 9.0f, 10.0, // Vector de posicion
				1.0f, 0.09f, 0.032f); // ecuacion de segundo grado
			pointLightCount++;
		}








		//// ----------------------- luz ligada a la c�mara de tipo flash -----------------------------------------
		////sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		
		
		// ---------------------------------- LUZ DEL COFRE (VERDE) --------------------------------

		glm::vec3 cofreOffset = glm::vec3(3.3f, 1.4f, 0.0f);
		glm::vec3 cofrePosition = glm::vec3(mainWindow.getmuevex(), 0.6f, -3.0f) + cofreOffset;
		float cofreAngle = glm::radians(mainWindow.getarticulacion1());
		glm::vec3 direccionBase = glm::vec3(1.0f, 0.0f, 0.0f);
		float cosAngle = cos(cofreAngle);
		float sinAngle = sin(cofreAngle);
		glm::vec3 direccionLuz = glm::vec3(cosAngle, sinAngle,0.0f);
		direccionLuz = glm::normalize(direccionLuz);
		spotLights[0].SetFlash(cofrePosition, direccionLuz);



		// ---------------------------------- LUZ DEL COCHE --------------------------------
		glm::vec3 carPosition = glm::vec3(mainWindow.getmuevex(), 0.6f, -3.0f);

		// Faro Trasero AZUL (retroceder - direcci�n +X) - spotLights[1]
		if (mainWindow.getSpotlightRetroceder()) {
			glm::vec3 carLightOffsetRight = glm::vec3(8.0f, 0.0f, 0.0f);
			spotLights[1].SetFlash(carPosition + carLightOffsetRight, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else {
			// Apagar el spotlight movi�ndolo muy lejos
			spotLights[1].SetFlash(glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		}

		// Faro Frontal ROJO (avanzar - direcci�n -X) - spotLights[2]
		if (mainWindow.getSpotlightAvanzar()) {
			glm::vec3 carLightOffsetLeft = glm::vec3(-6.0f, 0.0f, 0.0f);
			spotLights[2].SetFlash(carPosition + carLightOffsetLeft, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else {
			// Apagar el spotlight movi�ndolo muy lejos
			spotLights[2].SetFlash(glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		}



		// ------------------------------ LUZ HELICOPTERO ---------------------------------
		glm::vec3 helicopterPosition = glm::vec3(0.0f, 30.0f, mainWindow.getPoszHelicoptero());

		// Faros
		glm::vec3 helicopterLightOffsetRight = glm::vec3(0.0f, -1.0f, 7.0f);
		spotLights[3].SetFlash(helicopterPosition + helicopterLightOffsetRight, glm::vec3(0.0f, -1.0f, 1.0f));


		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// ---------------------------------------------- Coche ---------------------------------------------------
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.6f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();


		// ---------------------------------  Cofre ---------------------------------
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.3f, 1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.1f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();


		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.7f, -0.4f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, -0.4f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, -0.4f, -3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.7f, -0.4f, -3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		
		// ------------------------------------------- Helicoptero ------------------------------------
		//Instancia del helic�ptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 30.0f, mainWindow.getPoszHelicoptero()));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Helices
		modelaux = model;
		giroHelices += 0.9f;
		modelaux = glm::rotate(modelaux, glm::radians(giroHelices * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		BlackhawkHelices_M.RenderModel();




		// ----------------------------------------------- Antorcha --------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 4.0f, 10.0));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Antorcha_M.RenderModel();


		// ----------------------------------------------- ALIEN --------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 4.0f, 10.0));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Alien_M.RenderModel();



		// ------------------------------- Dado 8 caras ----------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 10.0f, 10.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dado_Ocho.UseTexture();
		meshList[4]->RenderMesh();

		//-------------------------------  Pared --------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 8.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();



		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		

		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
