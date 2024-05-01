include "vendor"

project "oglfv2"
    language "C++"
    cppdialect "C++17"
    kind "StaticLib"
    
    staticruntime "on"
    systemversion "latest"

    targetdir (TARGET_DIR)
	objdir (OBJ_DIR)

    files
    {
        "src/**.h",
		"src/**.cpp"
    }

    includedirs
    {
        "src",
        "vendor/glad/include",
        "vendor/glfw/include",
        "vendor/glm",
        "vendor/stb"
    }

    links
    {
        "glad",
        "glfw"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "GLDEBUG_OMIT_SEVERITY_NOTIFICATION"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "Speed"

    filter "configurations:Dist"
        runtime "Release"
        optimize "Speed"
        symbols "off"