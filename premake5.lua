project "PaperEngine"

	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")

	IncludeDir = {}
	IncludeDir["glfw"] = "lib/glfw/include"
	IncludeDir["spdlog"] = "lib/spdlog/include"
	IncludeDir["glad"] = "lib/glad/include"

	pchheader "p_pch.h"
	pchsource "src/p_pch.cpp"

	files { "src/**.h", "src/**.cpp", "lib/stb_image/**.h", "lib/stb_image/**.cpp" }
	includedirs { "src/", "include/", "%{IncludeDir.spdlog}", "%{IncludeDir.glfw}", "%{IncludeDir.glad}" }

	filter "system:windows"
		links { "OpenGL32" }
	filter "system:not windows"
		links { "GL", "dl", "m", "X11" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines { "_WINDLL" }
	filter { }

	defines { "PENGINE_BUILD_SO", "GLFW_INCLUDE_NONE" }

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/build/bin/" .. outputdir .. "/Sandbox/")
	}
	postbuildmessage "Copied libs"

	filter "configurations:Debug"
		defines "PAPER_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PAPER_RELEASE"
		optimize "On"

	include "lib/glfw"
	include "lib/glad"