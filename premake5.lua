project "PaperEngine"

	kind "SharedLib"
	language "C++"

	targetdir ("../build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../build/bin-int/" .. outputdir .. "/%{prj.name}")

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }
	includedirs { "%{prj.name}/include/", "%{prj.name}/lib/spdlog/include/" }

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