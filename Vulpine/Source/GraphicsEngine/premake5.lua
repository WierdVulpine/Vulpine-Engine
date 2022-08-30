project "GraphicsEngine"
	location "%{wks.location}/Source/GraphicsEngine/"
	language "C++"
	cppdialect "C++17"

	targetdir("../../Lib/")
	targetname("%{prj.name}-%{cfg.buildcfg}")
	objdir("../../Temp/%{prj.name}/%{cfg.buildcfg}")

	files {
		"**.h",
		"**.cpp",
		"**.hpp",
		"**.hlsl",
		"**.hlsli"
	}

	includedirs
	{
		"../CommonUtilities/",
		"../Imgui/",
		"../TGAImporter/"
	}

	links
	{
		"CommonUtilities",
		"TGAFBXImporter.lib"
	}

	libdirs
	{
		"../../Lib/"
	}

	local shader_dir = "../../Bin/Shaders/"
    os.mkdir(shader_dir)

	shadermodel "5.0"

	filter("files:**.hlsl")
		flags("ExcludeFromBuild")
		shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
		
		filter "configurations:Debug"
			shaderdefines {"_DEBUG"}
		filter "configurations:Release"
			shaderdefines {"_RELEASE"}

    filter("files:**_ps.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Pixel")

    filter("files:**_vs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Vertex")

    filter("files:**_gs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Geometry")

    filter("files:**_cs.hlsl")
        removeflags("ExcludeFromBuild")
        shadertype("Compute")
        filter ("files:**.c")


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