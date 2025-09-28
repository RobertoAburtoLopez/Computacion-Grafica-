/*
Práctica 5: Optimización y Carga de Modelos
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
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Model Goddard_M;
Model Mandibula_M; ///----------------------------------------------------------------------------------- CAMBIAAAAAAA
Model pataFrontalDer;
Model pataFrontalIzq;
Model pataTrasIzq;
Model pataTrasDer;

// Coche
Model coche;
Model cofre;
Model ruedaDelDer;
Model ruedaDelIzq;
Model ruedaTrasIzq;
Model ruedaTrasDer;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";

// Limitar el movimiento




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

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


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

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);


	// ------------------------- Goddard ------------------------------
	Goddard_M = Model();
	Goddard_M.LoadModel("Models/Goddar_Cuerpo.fbx");

	Mandibula_M = Model();
	Mandibula_M.LoadModel("Models/Goddar_Mandibula.fbx");
	
	Model pataFrontalDer = Model();
	pataFrontalDer.LoadModel("Models/Goddar_pataFrontalDer.fbx");

	Model pataFrontalIzq = Model();
	pataFrontalIzq.LoadModel("Models/Goddar_pataFrontalIzq.fbx");

	Model pataTrasIzq = Model();
	pataTrasIzq.LoadModel("Models/Goddar_pataTrasIzq.fbx");

	Model pataTrasDer = Model();
	pataTrasDer.LoadModel("Models/Goddar_pataTrasDer.fbx");

	
	// -------------------------- Coche --------------------------------------

	Model coche = Model();
	coche.LoadModel("Models/car.obj");

	Model ruedaDelDer = Model();
	ruedaDelDer.LoadModel("Models/ruedaDelDer.obj");

	Model ruedaDelIzq = Model();
	ruedaDelIzq.LoadModel("Models/ruedaDelIzq.obj");

	Model ruedaTrasDer = Model();
	ruedaTrasDer.LoadModel("Models/ruedaDelDer.obj");

	Model ruedaTrasIzq = Model();
	ruedaTrasIzq.LoadModel("Models/ruedaDelIzq.obj");

	Model cofre = Model();
	cofre.LoadModel("Models/cofre.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();
		
		// **************************************** GODDARD ********************************************************* //
		//----------------------------- INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS ---------------------------
		//Goddard
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.6, 0.0f));
		modelaux = model;
		color = glm::vec3(1.0f, 0.0f, 1.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		model = modelaux;


		// ---------------------------------------- PATA FRONTAL DERECHA --------------------------------------------

		model = glm::translate(model, glm::vec3(0.7f, -0.5f, 0.6f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFrontalDer.RenderModel();

		model = modelaux;


		// ---------------------------------------- PATA FRONTAL IZQUIERDA --------------------------------------------

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.2f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFrontalDer.RenderModel();

		model = modelaux;

		// ---------------------------------------- PATA TRASERA IZQUIERDA  --------------------------------------------

		model = glm::translate(model, glm::vec3(-1.9f, -0.8f, 0.0f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTrasDer.RenderModel();

		model = modelaux;

		// ---------------------------------------- PATA TRASERA DERECHA  --------------------------------------------

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.2f));
		modelaux = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTrasDer.RenderModel();

		model = modelaux;



		// ---------------------------------------- BOCA  --------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.2f, 1.0f, -0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Mandibula_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		




		// **************************************** COCHE ********************************************************* //


		//----------------------------- INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS ---------------------------
		//Coche
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(mainWindow.getCarX(), -1.5f, mainWindow.getCarZ()));
		model = glm::rotate(model, glm::radians(mainWindow.getCarYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		color = glm::vec3(0.3f, 0.2f, 0.4f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

		model = modelaux2;

		



		// ---------------------------------------- RUEDA FRONTAL DERECHA --------------------------------------------

		model = glm::translate(model, glm::vec3(2.5f, 0.5, -2.8f));
		modelaux2 = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ruedaDelDer.RenderModel();

		model = modelaux2;


		//// ---------------------------------------- RUEDA FRONTAL IZQUIERDA --------------------------------------------

		model = glm::translate(model, glm::vec3(-5.0f, 0.0, 0.0));
		modelaux2 = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ruedaDelIzq.RenderModel();

		model = modelaux2;

		//// ---------------------------------------- RUEDA TRASERA IZQUIERDA  --------------------------------------------

		model = glm::translate(model, glm::vec3(0.0f, 0.0, 7.8));
		modelaux2 = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ruedaTrasIzq.RenderModel();

		model = modelaux2;

		//// ---------------------------------------- RUEDA TRASERA DERECHA  --------------------------------------------

		model = glm::translate(model, glm::vec3(5.0f, 0.0, 0.0));
		modelaux2 = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ruedaTrasIzq.RenderModel();

		model = modelaux2;




		//// ---------------------------------------- COFRE  --------------------------------------------

		model = glm::translate(model, glm::vec3(-2.5f, 1.7, -7.0));
		modelaux2 = model;

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 1.0f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre.RenderModel();

		model = modelaux2;


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

/*

Model coche;
Model ruedaDelDer;
Model ruedaDelIzq;
Model ruedaTrasIzq;
Model ruedaTrasDer;

*/
