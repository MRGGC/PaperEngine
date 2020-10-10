include "lib/glfw"
include "lib/glad"
include "lib/imgui"

project "PaperEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")

	IncludeDir = {}
	IncludeDir["glfw"] = "lib/glfw/include"
	IncludeDir["spdlog"] = "lib/spdlog/include"
	IncludeDir["glad"] = "lib/glad/include"
	IncludeDir["imgui"] = "lib/imgui"
	IncludeDir["glm"] = "lib/glm"

	pchheader "p_pch.h"
	pchsource "src/p_pch.cpp"

	files { "src/**.h", "src/**.cpp", "lib/glm/glm/**.hpp", "lib/glm/glm/**.inl" }
	includedirs { "src/", "include/", "%{IncludeDir.spdlog}", "%{IncludeDir.glfw}", "%{IncludeDir.glad}", "%{IncludeDir.imgui}", "%{IncludeDir.glm}" }

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines { "_WINDLL" }
	filter { }

	defines { "PENGINE_BUILD", "PAPER_ENABLE_ASSERTS", "PENGINE_STATIC_LINK", "GLFW_INCLUDE_NONE" }

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/build/bin/" .. outputdir .. "/Sandbox/")
	}
	postbuildmessage "Copied libs"

	filter "configurations:Debug"
		defines "PAPER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PAPER_RELEASE"
		runtime "Release"
		optimize "on"