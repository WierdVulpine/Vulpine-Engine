project "VulpineEditor"
	location "%{wks.location}/Source/VulpineEditor/"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"

	targetdir("../../Bin/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir("../../Temp/%{prj.name}/%{cfg.buildcfg}")

	debugdir("../../Bin/")

	files
	{
		"**.h",
		"**.cpp"
	}

	libdirs
	{
		"../../Lib/"
	}

	links
	{
		"GraphicsEngine"
	}

	systemversion "latest"

	includedirs
	{
		"../CommonUtilities/",
		"../GraphicsEngine/",
		"../Imgui/"
	}

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		links
		{
			"CommonUtilities-Debug.lib",
			"GraphicsEngine-Debug.lib"
		}

	filter "configurations:Release"
		links
		{
			"CommonUtilities-Release.lib",
			"GraphicsEngine-Release.lib"
		}