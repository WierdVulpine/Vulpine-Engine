#pragma once
#include "Vector4.hpp"

struct Vertex
{
	CommonUtilities::Vector4<float> Position = {0, 0, 0, 1};

	CommonUtilities::Vector4<float> VertexColors[4] =
	{
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	};

	Vertex(float aX, float aY, float aZ, float aR, float aG, float aB, float aA)
	{
		Position = { aX, aY, aZ, 1 };
		VertexColors[0] = { aR, aG, aB, aA };
	}
};