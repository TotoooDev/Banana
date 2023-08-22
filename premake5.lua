-- Stolen from Cherno's video on premake
-- https://www.youtube.com/watch?v=sULV3aB2qeU

-- Defines the name of the workspace (the Visual Studio .sln file)
workspace "Banana"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	startproject "GameProject"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Defines the project (the .vcxproj file in Banana/)
project "Banana"
	location "Banana"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	
	-- Defines the directory of the executable and the object files
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

	-- Specifies the working directory of the debugger
	debugdir ("bin/" .. outputDir .. "/%{prj.name}")

	-- Specifies the files to include in the project
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/vendor/**.c",
		"%{prj.name}/src/vendor/**.cpp",
	}

	-- Specifies the include directories
	includedirs
	{
		"Banana/src",
		"Banana/src/Banana",
		"Banana/src/vendor"
	}

	-- If the project is generated on Windows it will apply these rules
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/windows")
		-- Libraries to link
		links
		{
			"opengl32",
			"glew32",
			"glfw3dll",
			"assimp-vc143-mt",
			"reactphysics3d"
		}

	-- If the project is generated on Linux it will apply these rules
	filter "system:linux"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/linux")

	-- If the project is generated with the Debug configuration it will apply these rules
	filter "configurations:Debug"
		defines
		{
			"BANANA_DEBUG",
			"BANANA_OPENGL_DEBUG"
		}
		symbols "On"

	-- If the project is generated with the Debug configuration it will apply these rules
	filter "configurations:Release"
		optimize "On"

-- Example project
project "GameProject"
	location "GameProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

	debugdir ("bin/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"GameProject/src",
		"GameProject/src/vendor",
		"Banana/src/vendor",
		"Banana/src"
	}
	
	links ("Banana")
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/windows")
		-- links ()
		postbuildcommands ("xcopy ..\\libs\\windows\\dlls\\ ..\\bin\\" .. outputDir .. "\\%{prj.name}\\ /s /e /y /i")
		postbuildcommands ("xcopy ..\\dev-assets\\ ..\\bin\\" .. outputDir .. "\\%{prj.name}\\ /s /e /y /i")

	filter "system:linux"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/linux")

	filter "configurations:Debug"
		defines
		{
			"BANANA_DEBUG",
			"BANANA_OPENGL_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		optimize "On"