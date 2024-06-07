from StringBuilder import StringBuilder

# Header constants ----------------------------------------------------------------------------------------------------------------------------------
HEADER_HEADER = """// This file is automatically built by a script
#pragma once

#include <unordered_map>
#include <functional>

#include "Arrowhead/Events.h"

#include "GLFW/glfw3.h"

"""

HEADER_CLASS_SECTION1 = """class GLFWEventWrapper
{
public:
	// Factory definitions
	static GLFWEventWrapper* GetWrapper(GLFWwindow* window);

	// Auto Gen"""

HEADER_CLOSED_CONNECT_FUNCTION = "std::shared_ptr<arwh::EventConnection<GLFWwindow*>> ConnectWindowClose(std::function<std::remove_pointer<GLFWwindowclosefun>::type> callback);"

HEADER_CLASS_SECTION2 = """	// End Auto Gen
private:
	// Factory definitions
	inline static std::unordered_map<GLFWwindow*, GLFWEventWrapper*> m_Wrappers =
        std::unordered_map<GLFWwindow*, GLFWEventWrapper*>();

	static void DisposeWrapper(GLFWwindow* window);

	// Auto Gen"""

HEADER_CLOSED_CALLBACK_FUNCTION = "static void WindowCloseCallback(GLFWwindow* window);"

HEADER_CLASS_SECTION3 = """	// End Auto Gen

	// Object definitions
	GLFWwindow* m_Window;

	// Auto Gen"""

HEADER_CLOSED_CALLBACK_LIST = "arwh::CallbackList<GLFWwindow*> m_WindowCloseCallbacks;"

HEADER_CLASS_SECTION4 = """	// End Auto Gen

	GLFWEventWrapper(GLFWwindow* window);
};"""

# Header assembly

def HeaderGetConnectFunction(event):
    return f"std::shared_ptr<arwh::EventConnection<{GetTypeArguments(event)}>> Connect{event.Name}(std::function<void({GetTypeArguments(event)})> callback);"

def HeaderGetCallbackFunction(event):
    return f"static void {event.Name}Callback({GetParameterListWithNames(event)});"

def HeaderGetCallbackListDefinition(event):
    return f"arwh::CallbackList<{GetTypeArguments(event)}> m_{event.Name}Callbacks;"

def GetHeader(data):
    builder = StringBuilder(HEADER_HEADER)
    builder.AppendLine(HEADER_CLASS_SECTION1)

    for event in data:
        builder.AppendLine('\t' + HeaderGetConnectFunction(event))
    builder.AppendLine("\t" + HEADER_CLOSED_CONNECT_FUNCTION)

    builder.AppendLine(HEADER_CLASS_SECTION2)

    for event in data:
        builder.AppendLine('\t' + HeaderGetCallbackFunction(event))
    builder.AppendLine("\t" + HEADER_CLOSED_CALLBACK_FUNCTION)

    builder.AppendLine(HEADER_CLASS_SECTION3)

    for event in data:
        builder.AppendLine('\t' + HeaderGetCallbackListDefinition(event))
    builder.AppendLine("\t" + HEADER_CLOSED_CALLBACK_LIST)

    builder.Append(HEADER_CLASS_SECTION4)

    return builder.GetString()

# Source file constants -----------------------------------------------------------------------------------------------------------------------------
SOURCE_HEADER = """// This file is automatically built by a script
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

"""

SOURCE_CONSTRUCTOR_TOP = """GLFWEventWrapper::GLFWEventWrapper(GLFWwindow* window)
	: m_Window(window)
{
"""

SOURCE_DISPOSAL_TOP = """void GLFWEventWrapper::DisposeWrapper(GLFWwindow* window)
{
	try
	{
		GLFWEventWrapper* wrapper = m_Wrappers.at(window);
		delete wrapper;

"""

SOURCE_DISPOSAL_BOTTOM = r"""	}
	catch (std::out_of_range)
	{}
}"""

SOURCE_WINDOW_CLOSE = """std::shared_ptr<arwh::EventConnection<GLFWwindow*>> GLFWEventWrapper::ConnectWindowClose(std::function<std::remove_pointer<GLFWwindowclosefun>::type> callback)
{
	return m_WindowCloseCallbacks.Connect(callback);
}

void GLFWEventWrapper::WindowCloseCallback(GLFWwindow* window)
{
	GetWrapper(window)->m_WindowCloseCallbacks.Call(window);

	DisposeWrapper(window);
}"""

# Source file assembly

def SourceGetConstructor(data):
    builder = StringBuilder(SOURCE_CONSTRUCTOR_TOP)

    for event in data:
        builder.AppendLine("\t" + f"{event.FunctionName}(window, GLFWEventWrapper::{event.Name}Callback);")
    builder.AppendLine("\t" + "glfwSetWindowCloseCallback(window, GLFWEventWrapper::WindowCloseCallback);")

    builder.Append("}")
    return builder.GetString()

def SourceGetDisposal(data):
    builder = StringBuilder(SOURCE_DISPOSAL_TOP)

    for event in data:
        builder.AppendLine("\t\t" + f"{event.FunctionName}(window, NULL);")
    builder.AppendLine("\t\t" + "glfwSetWindowCloseCallback(window, NULL);")

    builder.Append(SOURCE_DISPOSAL_BOTTOM)
    return builder.GetString()

def SourceGetEventFunctions(event):
    builder = StringBuilder("")

    # Connect
    builder.AppendLine(f"std::shared_ptr<arwh::EventConnection<{GetTypeArguments(event)}>> GLFWEventWrapper::Connect{event.Name}(std::function<std::remove_pointer<{event.SignatureName}>::type> callback)")
    builder.AppendLine("{")
    builder.AppendLine(f"	return m_{event.Name}Callbacks.Connect(callback);")
    builder.AppendLine("}")

    builder.Append('\n')

    # Callback
    builder.AppendLine(f"void GLFWEventWrapper::{event.Name}Callback({GetParameterListWithNames(event)})")
    builder.AppendLine("{")
    builder.AppendLine(f"	GetWrapper(window)->m_{event.Name}Callbacks.Call({GetCallbackArguments(event)});")
    GetTypeArguments(event)
    builder.Append("}")

    return builder.GetString()

def GetSource(data):
    builder = StringBuilder(SOURCE_HEADER)

    builder.AppendLine(SourceGetConstructor(data) + '\n')
    builder.AppendLine(SourceGetDisposal(data) + "\n\n")

    for event in data:
        builder.AppendLine(SourceGetEventFunctions(event) + "\n\n")
    builder.AppendLine(SOURCE_WINDOW_CLOSE)

    return builder.GetString()

# Helper functions ----------------------------------------------------------------------------------------------------------------------------------

def GetParameterListWithNames(event):
    parameters = "GLFWwindow* window"
    
    for parameter in event.Signature:
        parameters += ", "
        parameters += parameter[0]
        parameters += ' '
        parameters += parameter[1]

    return parameters

def GetCallbackArguments(event):
    parameters = "window"

    for parameter in event.Signature:
        parameters += ", "
        parameters += parameter[1]

    return parameters.strip("[]*")

def GetCallbackArguments(event):
    parameters = "window"

    for parameter in event.Signature:
        parameters += ", "
        parameters += parameter[1]

    return parameters.strip("[]*")

def GetTypeArguments(event):
    types = "GLFWwindow*"

    for parameter in event.Signature:
        types += ", "
        types += parameter[0]
        if parameter[1].find("[]") != -1:
            types += "[]"

    return types