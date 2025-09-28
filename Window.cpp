#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	articulacion11 = 0.0f;
	articulacion12 = 0.0f;
	articulacion13 = 0.0f;
	articulacion14 = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	carX = 20.0f;  // coincide con tu translate original
	carZ = 0.0f;
	carYaw = 0.0f;

	// Frente del coche inicial: +Z
	fwdX = 0;
	fwdZ = 1;

}


int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 05: Optimizaci�n y Carga de Modelos", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}


	// Coche
	const float step = 0.5f;

	if ((key == GLFW_KEY_UP) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Avanza hacia adelante en el eje Z
		theWindow->carZ -= step;

		// Animaci�n de ruedas opcional
		theWindow->articulacion11 -= 10.0f;
		theWindow->articulacion12 -= 10.0f;
	}

	if ((key == GLFW_KEY_DOWN) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Retrocede hacia atr�s en el eje Z
		theWindow->carZ += step;

		theWindow->articulacion12 += 10.0f;
		theWindow->articulacion11 += 10.0f;
	}


	// Cofre
	if (key == GLFW_KEY_M)
	{
		if (theWindow->articulacion10 > 40)
		{
		}
		else{
			theWindow->articulacion10 += 10.0;
		}
	}

	if (key == GLFW_KEY_N)
	{
		if (theWindow->articulacion10 <= 0) 
		{
		}
		else{
			theWindow->articulacion10 -= 10.0;
		}
	}


	// Goddard 
	if (key == GLFW_KEY_F)
	{
		if (theWindow->articulacion1 > 45)
		{
		}
		else
		{
			theWindow->articulacion1 += 10.0;
		}
	}

	if (key == GLFW_KEY_G)
	{
		if (theWindow->articulacion1 < -45)
		{
		}
		else
		{
			theWindow->articulacion1 -= 10.0;
		}
	}


	if (key == GLFW_KEY_H)
	{
		if (theWindow->articulacion2 > 45)
		{
		}
		else
		{
			theWindow->articulacion2 += 10.0;
		}
	}

	if (key == GLFW_KEY_J)
	{
		if (theWindow->articulacion2 < -45)
		{
		}
		else
		{
			theWindow->articulacion2 -= 10.0;
		}
	}

	if (key == GLFW_KEY_K)
	{
		if (theWindow->articulacion3 > -10)
		{
		}
		else
		{
			theWindow->articulacion3 += 10.0;
		}
	}


	if (key == GLFW_KEY_L)
	{
		if (theWindow->articulacion3 < -40)
		{
		}
		else
		{
			theWindow->articulacion3 -= 10.0;
		}
	}

	if (key == GLFW_KEY_Z)
	{
		if (theWindow->articulacion4 > 45)
		{
		}
		else
		{
			theWindow->articulacion4 += 10.0;
		}
	}

	if (key == GLFW_KEY_X)
	{
		if (theWindow->articulacion4 < -45)
		{
		}
		else
		{
			theWindow->articulacion4 -= 10.0;
		}
	}

	if (key == GLFW_KEY_C)
	{
		if (theWindow->articulacion5 > 50)
		{
		}
		else
		{
			theWindow->articulacion5 += 10.0;
		}
	}
	if (key == GLFW_KEY_V)
	{
		if (theWindow->articulacion5 < -50)
		{
		}
		else
		{
			theWindow->articulacion5 -= 10.0;
		}
	}
	if (key == GLFW_KEY_B)
	{
		if (theWindow->articulacion6 > 45)
		{
		}
		else
		{
			theWindow->articulacion6 += 10.0;
		}
	}



	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}