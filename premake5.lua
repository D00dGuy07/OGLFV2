include "vendor"

project "glad"
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