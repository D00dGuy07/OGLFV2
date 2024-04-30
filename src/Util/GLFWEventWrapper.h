// This file is automatically built by a script
#pragma once

#include <unordered_map>
#include <functional>

#include "Util/Events/CallbackList.h"

#include "GLFW/glfw3.h"

class GLFWEventWrapper
{
public:
	// Factory definitions
	static GLFWEventWrapper* GetWrapper(GLFWwindow* window);

	// Auto Gen
	std::shared_ptr<EventConnection<GLFWwindow*, int, int, int>> ConnectMouseButton(std::function<void(GLFWwindow*, int, int, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, double, double>> ConnectMouseCursorPos(std::function<void(GLFWwindow*, double, double)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int>> ConnectMouseCursorEnter(std::function<void(GLFWwindow*, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, double, double>> ConnectMouseScroll(std::function<void(GLFWwindow*, double, double)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int, int, int, int>> ConnectKey(std::function<void(GLFWwindow*, int, int, int, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, unsigned int>> ConnectChar(std::function<void(GLFWwindow*, unsigned int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, unsigned int, int>> ConnectCharMods(std::function<void(GLFWwindow*, unsigned int, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int, const char*[]>> ConnectPathDrop(std::function<void(GLFWwindow*, int, const char*[])> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int, int>> ConnectWindowPos(std::function<void(GLFWwindow*, int, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int, int>> ConnectWindowSize(std::function<void(GLFWwindow*, int, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*>> ConnectWindowRefresh(std::function<void(GLFWwindow*)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int>> ConnectWindowFocus(std::function<void(GLFWwindow*, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int>> ConnectWindowIconify(std::function<void(GLFWwindow*, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int>> ConnectWindowMaximize(std::function<void(GLFWwindow*, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, int, int>> ConnectFramebufferSize(std::function<void(GLFWwindow*, int, int)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*, float, float>> ConnectWindowContentScale(std::function<void(GLFWwindow*, float, float)> callback);
	std::shared_ptr<EventConnection<GLFWwindow*>> ConnectWindowClose(std::function<std::remove_pointer<GLFWwindowclosefun>::type> callback);
	// End Auto Gen
private:
	// Factory definitions
	static std::unordered_map<GLFWwindow*, GLFWEventWrapper*> m_Wrappers;

	static void DisposeWrapper(GLFWwindow* window);

	// Auto Gen
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseCursorEnterCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallback(GLFWwindow* window, unsigned int codepoint);
	static void CharModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
	static void PathDropCallback(GLFWwindow* window, int path_count, const char* paths[]);
	static void WindowPosCallback(GLFWwindow* window, int xpos, int ypos);
	static void WindowSizeCallback(GLFWwindow* window, int width, int height);
	static void WindowRefreshCallback(GLFWwindow* window);
	static void WindowFocusCallback(GLFWwindow* window, int focused);
	static void WindowIconifyCallback(GLFWwindow* window, int iconified);
	static void WindowMaximizeCallback(GLFWwindow* window, int maximized);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);
	static void WindowCloseCallback(GLFWwindow* window);
	// End Auto Gen

	// Object definitions
	GLFWwindow* m_Window;

	// Auto Gen
	CallbackList<GLFWwindow*, int, int, int> m_MouseButtonCallbacks;
	CallbackList<GLFWwindow*, double, double> m_MouseCursorPosCallbacks;
	CallbackList<GLFWwindow*, int> m_MouseCursorEnterCallbacks;
	CallbackList<GLFWwindow*, double, double> m_MouseScrollCallbacks;
	CallbackList<GLFWwindow*, int, int, int, int> m_KeyCallbacks;
	CallbackList<GLFWwindow*, unsigned int> m_CharCallbacks;
	CallbackList<GLFWwindow*, unsigned int, int> m_CharModsCallbacks;
	CallbackList<GLFWwindow*, int, const char*[]> m_PathDropCallbacks;
	CallbackList<GLFWwindow*, int, int> m_WindowPosCallbacks;
	CallbackList<GLFWwindow*, int, int> m_WindowSizeCallbacks;
	CallbackList<GLFWwindow*> m_WindowRefreshCallbacks;
	CallbackList<GLFWwindow*, int> m_WindowFocusCallbacks;
	CallbackList<GLFWwindow*, int> m_WindowIconifyCallbacks;
	CallbackList<GLFWwindow*, int> m_WindowMaximizeCallbacks;
	CallbackList<GLFWwindow*, int, int> m_FramebufferSizeCallbacks;
	CallbackList<GLFWwindow*, float, float> m_WindowContentScaleCallbacks;
	CallbackList<GLFWwindow*> m_WindowCloseCallbacks;
	// End Auto Gen

	GLFWEventWrapper(GLFWwindow* window);
};