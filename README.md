# PaperEngine [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

![PaperEngineLogoBG](https://user-images.githubusercontent.com/16705954/96472153-32d62000-1241-11eb-9c2d-2780b062adfe.png)

Paper Engine is an open-source simple, lightweight, and highly customizable 2D oriented Game Engine for Linux enthusiasts. People can get into game development using this engine due to its speed and intuitiveness. Right now, Paper Engine supports the C++ programming language only; however, it soon will also support the [Carbon scripting language](https://github.com/MintSoup/Carbon).

**Documentation: [https://paperenginedocs.web.app](https://paperenginedocs.web.app)**

## Build and Usage

First, you need to clone this repository into your Game directory recursively.
```
mkdir PaperGame
cd PaperGame
git clone --recursive https://github.com/MRGGC/PaperEngine.git
touch premake5.lua
```
Then, you initialize the ```premake5.lua``` file to build the Game. Example of a premake file:
```lua
workspace "PaperGame"
	architecture "x64"
	configurations { "Debug", "Release" }
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-${cfg.architecture}" --!required

include "PaperEngine"

project "PaperGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")

	files { "**.h", "**.cpp" }
	includedirs {   --!include dirs for Engine
		"%{wks.location}/PaperEngine/src",
		"%{wks.location}/PaperEngine/lib/spdlog/include",
		"%{wks.location}/PaperEngine/lib/glm",
		"%{wks.location}/PaperEngine/lib/imgui"
	}

	links { "PaperEngine", "glfw", "glad", "imgui" }
	links { "GL", "dl", "m", "X11", "Xi", "Xrandr", "Xinerama", "Xcursor", "rt", "pthread" } --!for Linux

	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"
```
**As mentioned, this _yet_ works for Linux only!**<br>   
Now, let's create the ```Game.cpp``` file
```c++
#include <Paper.h>

class SampleLayer : public Paper::Layer
{
public:
	SampleLayer()
		: Layer("Sample")
	{
	}

	void OnUpdate(Paper::Timestep dt) override
	{
	}

	void OnEvent(Paper::Event& event) override
	{
	}

	// For Debugging
	void OnImGuiRenderer() override
	{
	}
};

class PaperGame : public Paper::Application
{
public:
	PaperGame()
	{
		PushLayer(new SampleLayer());
	}

	~PaperGame()
	{
	}
};

Paper::Application* Paper::CreateApplication()
{
	return new PaperGame();
}
```
By using this template and reading the wiki _(doesn't yet exist)_, you can begin creating a game!  
To build the project, you need to write
```
premake5 gmake2
make -j(nproc)
```
and the game executable will get located in ```build/bin/{outputdir}/PaperGame/PaperGame```

## Plans

- [ ] Complete 2D Renderer prototype
- [ ] Add Audio support
- [ ] Add Text Renderer
- [ ] Connect Carbon scripting language
- [ ] Update ReadMe
- [ ] Create Wiki for the Engine
- [ ] Make the Engine cross-platform
- [ ] Add Networking
- [ ] Migrate to Vulkan
