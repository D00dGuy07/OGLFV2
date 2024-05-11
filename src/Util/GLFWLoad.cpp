#include "oglfv2/Util/GLFWLoad.h"

#include "glad/glad.h"

#include <iostream>

#ifdef _DEBUG
void openglErrorCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* msg, const void* data)
{
    std::string _source;
    std::string _type;
    std::string _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UNDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
#ifdef GLDEBUG_OMIT_SEVERITY_HIGH
        return;
#endif // GLDEBUG_OMIT_SEVERITY_HIGH
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
#ifdef GLDEBUG_OMIT_SEVERITY_MEDIUM
        return;
#endif // GLDEBUG_OMIT_SEVERITY_MEDIUM
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
#ifdef GLDEBUG_OMIT_SEVERITY_LOW
        return;
#endif // GLDEBUG_OMIT_SEVERITY_LOW
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
#ifdef GLDEBUG_OMIT_SEVERITY_NOTIFICATION
        return;
#endif // GLDEBUG_OMIT_SEVERITY_NOTIFICATION
        break;

    default:
        _severity = "UNKNOWN";
#ifdef GLDEBUG_OMIT_SEVERITY_UNKNOWN
        return;
#endif // GLDEBUG_OMIT_SEVERITY_UNKNOWN
        break;
    }

    printf("\n0x%X: %s of %s severity, raised from %s: %s\n\n",
        id, _type.c_str(), _severity.c_str(), _source.c_str(), msg);
}
#endif // _DEBUG

GLFWwindow* LoadGLFW(uint32_t width, uint32_t height)
{
	// Make Window
	GLFWwindow* window;

	if (!glfwInit())
		return NULL;

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // !_DEBUG
	window = glfwCreateWindow(width, height, "Hello World!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeLimits(window, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// Load OpenGL functions
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
		std::cout << "Failed to load Glad!" << std::endl;

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglErrorCallback, nullptr);

	// Print debug info
	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << glGetString(GL_RENDERER) << "\n\n";
#endif // !_DEBUG

	return window;
}
