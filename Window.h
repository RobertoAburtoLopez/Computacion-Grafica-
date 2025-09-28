#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion7() { return articulacion7; }
	GLfloat getarticulacion8() { return articulacion8; }
	GLfloat getarticulacion9() { return articulacion9; }
	GLfloat getarticulacion10() { return articulacion10; }
	GLfloat getarticulacion11() { return articulacion11; }
	GLfloat getarticulacion12() { return articulacion12; }
	GLfloat getarticulacion13() { return articulacion13; }
	GLfloat getarticulacion14() { return articulacion13; }

	// Estado del coche
	GLfloat getCarX() { return carX; }
	GLfloat getCarZ() { return carZ; }
	GLfloat getCarYaw() { return carYaw; } // en grados (0,90,180,270)

	int getCarFwdX() const { return fwdX; }
	int getCarFwdZ() const { return fwdZ; }
	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7, articulacion8, articulacion9, articulacion10, articulacion11, articulacion12, articulacion13, articulacion14;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	// Estado del coche (inicial)
	GLfloat carX = 20.0f;
	GLfloat carZ = 0.0f;
	GLfloat carYaw = 0.0f;

	// Eje local del coche (unitarios sobre X/Z)
	int fwdX = 0;  // +X = 1, -X = -1, si no, 0
	int fwdZ = 1;  // +Z = 1, -Z = -1, si no, 0  (por defecto el frente apunta +Z)
};
