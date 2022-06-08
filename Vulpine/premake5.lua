include "Premake/extensions.lua"

workspace "VulpineSolution"
	location "."
	startproject "VulpineEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	links
	{
		"d3d11.lib",
		"dxguid.lib"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Source/VulpineEditor"
include "Source/CommonUtilities"
include "Source/GraphicsEngine"