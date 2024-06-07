class EventInfo:
    def __init__(self, data):
        self.Name = data["Name"]
        self.FunctionName = data["FunctionName"]
        self.SignatureName = data["SignatureName"]
        self.Signature = []

        signatureString = data["Signature"]
        signatureRaw = signatureString.split(", ")

        if len(signatureString) > 0:
            for idx in range(len(signatureRaw)):
                self.Signature.append(signatureRaw[idx].rsplit(' ', 1))

def GetConfig():
    return [
        # Input Events
        EventInfo({
            "Name": "MouseButton",
            "FunctionName": "glfwSetMouseButtonCallback",
            "SignatureName": "GLFWmousebuttonfun",
            "Signature": "int button, int action, int mods"
        }),
        EventInfo({
            "Name": "MouseCursorPos",
            "FunctionName": "glfwSetCursorPosCallback",
            "SignatureName": "GLFWcursorposfun",
            "Signature": "double xpos, double ypos"
        }),
        EventInfo({
            "Name": "MouseCursorEnter",
            "FunctionName": "glfwSetCursorEnterCallback",
            "SignatureName": "GLFWcursorenterfun",
            "Signature": "int entered"
        }),
        EventInfo({
            "Name": "MouseScroll",
            "FunctionName": "glfwSetScrollCallback",
            "SignatureName": "GLFWscrollfun",
            "Signature": "double xoffset, double yoffset"
        }),
        EventInfo({
            "Name": "Key",
            "FunctionName": "glfwSetKeyCallback",
            "SignatureName": "GLFWkeyfun",
            "Signature": "int key, int scancode, int action, int mods"
        }),
        EventInfo({
            "Name": "Char",
            "FunctionName": "glfwSetCharCallback",
            "SignatureName": "GLFWcharfun",
            "Signature": "unsigned int codepoint"
        }),
        EventInfo({
            "Name": "CharMods",
            "FunctionName": "glfwSetCharModsCallback",
            "SignatureName": "GLFWcharmodsfun",
            "Signature": "unsigned int codepoint, int mods"
        }),
        EventInfo({
            "Name": "PathDrop",
            "FunctionName": "glfwSetDropCallback",
            "SignatureName": "GLFWdropfun",
            "Signature": "int path_count, const char* paths[]"
        }),

        # Window Events
        EventInfo({
            "Name": "WindowPos",
            "FunctionName": "glfwSetWindowPosCallback",
            "SignatureName": "GLFWwindowposfun",
            "Signature": "int xpos, int ypos"
        }),
        EventInfo({
            "Name": "WindowSize",
            "FunctionName": "glfwSetWindowSizeCallback",
            "SignatureName": "GLFWwindowsizefun",
            "Signature": "int width, int height"
        }),
        EventInfo({
            "Name": "WindowRefresh",
            "FunctionName": "glfwSetWindowRefreshCallback",
            "SignatureName": "GLFWwindowrefreshfun",
            "Signature": ""
        }),
        EventInfo({
            "Name": "WindowFocus",
            "FunctionName": "glfwSetWindowFocusCallback",
            "SignatureName": "GLFWwindowfocusfun",
            "Signature": "int focused"
        }),
        EventInfo({
            "Name": "WindowIconify",
            "FunctionName": "glfwSetWindowIconifyCallback",
            "SignatureName": "GLFWwindowiconifyfun",
            "Signature": "int iconified"
        }),
        EventInfo({
            "Name": "WindowMaximize",
            "FunctionName": "glfwSetWindowMaximizeCallback",
            "SignatureName": "GLFWwindowmaximizefun",
            "Signature": "int maximized"
        }),
        EventInfo({
            "Name": "FramebufferSize",
            "FunctionName": "glfwSetFramebufferSizeCallback",
            "SignatureName": "GLFWframebuffersizefun",
            "Signature": "int width, int height"
        }),
        EventInfo({
            "Name": "WindowContentScale",
            "FunctionName": "glfwSetWindowContentScaleCallback",
            "SignatureName": "GLFWwindowcontentscalefun",
            "Signature": "float xscale, float yscale"
        })
    ]

