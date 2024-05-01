#include <iostream>

#include "Util/GLFWLoad.h"
// Enable Nvidia high performace graphics
#include <windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

#include "Application.h"

int oglTestMain(void)
{
	// Load OpenGL/GLFW
	GLFWwindow* window = LoadGLFW(1280, 720);

	if (window == NULL)
		return -1;

	// Run application
	Application* app = new Application();
	app->Run(window);

	// Clean stuff up
	glfwTerminate();

	return 0;
}