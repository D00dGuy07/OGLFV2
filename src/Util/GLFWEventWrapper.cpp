// This file is automatically built by a script
#include <oglfv2/Util/GLFWEventWrapper.h>

#include <stdexcept>

GLFWEventWrapper* GLFWEventWrapper::GetWrapper(GLFWwindow* window)
{
	try
	{
		return m_Wrappers.at(window);
	}
	catch (std::out_of_range)
	{
		GLFWEventWrapper* wrapper = new GLFWEventWrapper(window);
		m_Wrappers[window] = wrapper;
		return wrapper;
	}
}

// Auto gen below

GLFWEventWrapper::GLFWEventWrapper(GLFWwindow* window)
	: m_Window(window)
{
	glfwSetMouseButtonCallback(window, GLFWEventWrapper::MouseButtonCallback);
	glfwSetCursorPosCallback(window, GLFWEventWrapper::MouseCursorPosCallback);
	glfwSetCursorEnterCallback(window, GLFWEventWrapper::MouseCursorEnterCallback);
	glfwSetScrollCallback(window, GLFWEventWrapper::MouseScrollCallback);
	glfwSetKeyCallback(window, GLFWEventWrapper::KeyCallback);
	glfwSetCharCallback(window, GLFWEventWrapper::CharCallback);
	glfwSetCharModsCallback(window, GLFWEventWrapper::CharModsCallback);
	glfwSetDropCallback(window, GLFWEventWrapper::PathDropCallback);
	glfwSetWindowPosCallback(window, GLFWEventWrapper::WindowPosCallback);
	glfwSetWindowSizeCallback(window, GLFWEventWrapper::WindowSizeCallback);
	glfwSetWindowRefreshCallback(window, GLFWEventWrapper::WindowRefreshCallback);
	glfwSetWindowFocusCallback(window, GLFWEventWrapper::WindowFocusCallback);
	glfwSetWindowIconifyCallback(window, GLFWEventWrapper::WindowIconifyCallback);
	glfwSetWindowMaximizeCallback(window, GLFWEventWrapper::WindowMaximizeCallback);
	glfwSetFramebufferSizeCallback(window, GLFWEventWrapper::FramebufferSizeCallback);
	glfwSetWindowContentScaleCallback(window, GLFWEventWrapper::WindowContentScaleCallback);
	glfwSetWindowCloseCallback(window, GLFWEventWrapper::WindowCloseCallback);
}

void GLFWEventWrapper::DisposeWrapper(GLFWwindow* window)
{
	try
	{
		GLFWEventWrapper* wrapper = m_Wrappers.at(window);
		delete wrapper;

		glfwSetMouseButtonCallback(window, NULL);
		glfwSetCursorPosCallback(window, NULL);
		glfwSetCursorEnterCallback(window, NULL);
		glfwSetScrollCallback(window, NULL);
		glfwSetKeyCallback(window, NULL);
		glfwSetCharCallback(window, NULL);
		glfwSetCharModsCallback(window, NULL);
		glfwSetDropCallback(window, NULL);
		glfwSetWindowPosCallback(window, NULL);
		glfwSetWindowSizeCallback(window, NULL);
		glfwSetWindowRefreshCallback(window, NULL);
		glfwSetWindowFocusCallback(window, NULL);
		glfwSetWindowIconifyCallback(window, NULL);
		glfwSetWindowMaximizeCallback(window, NULL);
		glfwSetFramebufferSizeCallback(window, NULL);
		glfwSetWindowContentScaleCallback(window, NULL);
		glfwSetWindowCloseCallback(window, NULL);
	}
	catch (std::out_of_range)
	{}
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int, int, int>> GLFWEventWrapper::ConnectMouseButton(std::function<std::remove_pointer<GLFWmousebuttonfun>::type> callback)
{
	return m_MouseButtonCallbacks.Connect(callback);
}

void GLFWEventWrapper::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GetWrapper(window)->m_MouseButtonCallbacks.Call(window, button, action, mods);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, double, double>> GLFWEventWrapper::ConnectMouseCursorPos(std::function<std::remove_pointer<GLFWcursorposfun>::type> callback)
{
	return m_MouseCursorPosCallbacks.Connect(callback);
}

void GLFWEventWrapper::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	GetWrapper(window)->m_MouseCursorPosCallbacks.Call(window, xpos, ypos);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int>> GLFWEventWrapper::ConnectMouseCursorEnter(std::function<std::remove_pointer<GLFWcursorenterfun>::type> callback)
{
	return m_MouseCursorEnterCallbacks.Connect(callback);
}

void GLFWEventWrapper::MouseCursorEnterCallback(GLFWwindow* window, int entered)
{
	GetWrapper(window)->m_MouseCursorEnterCallbacks.Call(window, entered);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, double, double>> GLFWEventWrapper::ConnectMouseScroll(std::function<std::remove_pointer<GLFWscrollfun>::type> callback)
{
	return m_MouseScrollCallbacks.Connect(callback);
}

void GLFWEventWrapper::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	GetWrapper(window)->m_MouseScrollCallbacks.Call(window, xoffset, yoffset);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int, int, int, int>> GLFWEventWrapper::ConnectKey(std::function<std::remove_pointer<GLFWkeyfun>::type> callback)
{
	return m_KeyCallbacks.Connect(callback);
}

void GLFWEventWrapper::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetWrapper(window)->m_KeyCallbacks.Call(window, key, scancode, action, mods);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, unsigned int>> GLFWEventWrapper::ConnectChar(std::function<std::remove_pointer<GLFWcharfun>::type> callback)
{
	return m_CharCallbacks.Connect(callback);
}

void GLFWEventWrapper::CharCallback(GLFWwindow* window, unsigned int codepoint)
{
	GetWrapper(window)->m_CharCallbacks.Call(window, codepoint);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, unsigned int, int>> GLFWEventWrapper::ConnectCharMods(std::function<std::remove_pointer<GLFWcharmodsfun>::type> callback)
{
	return m_CharModsCallbacks.Connect(callback);
}

void GLFWEventWrapper::CharModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
{
	GetWrapper(window)->m_CharModsCallbacks.Call(window, codepoint, mods);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int, const char*[]>> GLFWEventWrapper::ConnectPathDrop(std::function<std::remove_pointer<GLFWdropfun>::type> callback)
{
	return m_PathDropCallbacks.Connect(callback);
}

void GLFWEventWrapper::PathDropCallback(GLFWwindow* window, int path_count, const char* paths[])
{
	GetWrapper(window)->m_PathDropCallbacks.Call(window, path_count, paths);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int, int>> GLFWEventWrapper::ConnectWindowPos(std::function<std::remove_pointer<GLFWwindowposfun>::type> callback)
{
	return m_WindowPosCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowPosCallback(GLFWwindow* window, int xpos, int ypos)
{
	GetWrapper(window)->m_WindowPosCallbacks.Call(window, xpos, ypos);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int, int>> GLFWEventWrapper::ConnectWindowSize(std::function<std::remove_pointer<GLFWwindowsizefun>::type> callback)
{
	return m_WindowSizeCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	GetWrapper(window)->m_WindowSizeCallbacks.Call(window, width, height);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*>> GLFWEventWrapper::ConnectWindowRefresh(std::function<std::remove_pointer<GLFWwindowrefreshfun>::type> callback)
{
	return m_WindowRefreshCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowRefreshCallback(GLFWwindow* window)
{
	GetWrapper(window)->m_WindowRefreshCallbacks.Call(window);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int>> GLFWEventWrapper::ConnectWindowFocus(std::function<std::remove_pointer<GLFWwindowfocusfun>::type> callback)
{
	return m_WindowFocusCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowFocusCallback(GLFWwindow* window, int focused)
{
	GetWrapper(window)->m_WindowFocusCallbacks.Call(window, focused);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int>> GLFWEventWrapper::ConnectWindowIconify(std::function<std::remove_pointer<GLFWwindowiconifyfun>::type> callback)
{
	return m_WindowIconifyCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowIconifyCallback(GLFWwindow* window, int iconified)
{
	GetWrapper(window)->m_WindowIconifyCallbacks.Call(window, iconified);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int>> GLFWEventWrapper::ConnectWindowMaximize(std::function<std::remove_pointer<GLFWwindowmaximizefun>::type> callback)
{
	return m_WindowMaximizeCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowMaximizeCallback(GLFWwindow* window, int maximized)
{
	GetWrapper(window)->m_WindowMaximizeCallbacks.Call(window, maximized);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, int, int>> GLFWEventWrapper::ConnectFramebufferSize(std::function<std::remove_pointer<GLFWframebuffersizefun>::type> callback)
{
	return m_FramebufferSizeCallbacks.Connect(callback);
}

void GLFWEventWrapper::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GetWrapper(window)->m_FramebufferSizeCallbacks.Call(window, width, height);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*, float, float>> GLFWEventWrapper::ConnectWindowContentScale(std::function<std::remove_pointer<GLFWwindowcontentscalefun>::type> callback)
{
	return m_WindowContentScaleCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{
	GetWrapper(window)->m_WindowContentScaleCallbacks.Call(window, xscale, yscale);
}


std::shared_ptr<arwh::EventConnection<GLFWwindow*>> GLFWEventWrapper::ConnectWindowClose(std::function<std::remove_pointer<GLFWwindowclosefun>::type> callback)
{
	return m_WindowCloseCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowCloseCallback(GLFWwindow* window)
{
	GetWrapper(window)->m_WindowCloseCallbacks.Call(window);

	DisposeWrapper(window);
}
