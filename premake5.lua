project "PaperEngine"

	kind "SharedLib"
	language "C++"

	targetdir ("../build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../build/bin-int/" .. outputdir .. "/%{prj.name}")

	files { "src/**.h", "src/**.cpp", "lib/stb_image/**.h", "lib/stb_image/**.cpp" }
	includedirs { "src/", "include/", "lib/spdlog/include/" }

	pchheader "p_pch.h"
	pchsource "src/p_pch.cpp"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines { "_WINDLL" }
	filter { }

	defines { "PENGINE_BUILD_SO" }

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../build/bin/" .. outputdir .. "/Sandbox/")
	}

	filter "configurations:Debug"
		defines "PAPER_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PAPER_RELEASE"
		optimize "On"