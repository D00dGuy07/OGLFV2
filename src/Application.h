#pragma once

#include "UI/Surface.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"

struct GLFWwindow;

class Application
{
public:
	Application();

	void LoadResources();

	void Run(GLFWwindow* window);

private:
	GLFWwindow* m_Window;

	UI::Surface m_Surface;

	Mesh m_Mesh;
	Shader m_Shader;

	void Draw();
};