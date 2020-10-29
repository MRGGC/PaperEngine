group "Dependencies"
	include "lib/glfw"
	include "lib/glad"
	include "lib/imgui"
	include "lib/libogg"
	include "lib/openal-soft"
	include "lib/vorbis"
group ""

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
	IncludeDir["stb_image"] = "lib/stb_image"
	IncludeDir["freetype"] = "lib/freetype/include"
	IncludeDir["openal_soft"] = "lib/openal-soft/include"
	IncludeDir["libogg"] = "lib/libogg/include"
	IncludeDir["vorbis"] = "lib/vorbis/include"
	IncludeDir["minimp3"] = "lib/minimp3"

	pchheader "p_pch.h"
	pchsource "src/p_pch.cpp"

	files {
		"src/**.h", 
		"src/**.cpp", 
		"lib/stb_image/**.cpp", 
		"lib/stb_image/**.h", 
		"lib/glm/glm/**.hpp", 
		"lib/glm/glm/**.inl"
	}

	includedirs {
		"src/", 
		"include/", 
		"%{IncludeDir.spdlog}", 
		"%{IncludeDir.glfw}", 
		"%{IncludeDir.glad}", 
		"%{IncludeDir.imgui}", 
		"%{IncludeDir.glm}", 
		"%{IncludeDir.stb_image}", 
		"%{IncludeDir.freetype}",
		"%{IncludeDir.openal_soft}",
		"lib/openal-soft",
		"lib/openal-soft/common",
		"%{IncludeDir.libogg}",
		"%{IncludeDir.vorbis}",
		"%{IncludeDir.minimp3}"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines { "_WINDLL" }
	filter { }

	defines { 
		"PENGINE_BUILD", 
		"PAPER_ENABLE_ASSERTS", 
		"PENGINE_STATIC_LINK", 
		"GLFW_INCLUDE_NONE",
		"AL_LIBTYPE_STATIC"
	}

	links { "openal-soft", "vorbis" }

	-- postbuildcommands
	-- {
	-- 	("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/build/bin/" .. outputdir .. "/Sandbox/")
	-- }
	postbuildmessage "Copied libs"

	filter "configurations:Debug"
		defines "PAPER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PAPER_RELEASE"
		runtime "Release"
		optimize "on"