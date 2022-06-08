project "CommonUtilities"
	location "%{wks.location}/Source/CommonUtilities/"
	language "C++"
	cppdialect "C++17"

	targetdir("../../Lib/")
	targetname("%{prj.name}-%{cfg.buildcfg}")
	objdir("../../Temp/%{prj.name}/%{cfg.buildcfg}")

	files {
		"**.h",
		"**.cpp",
		"**.hpp"
	}

	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"