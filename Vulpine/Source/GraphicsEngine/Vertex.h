#pragma once
#include "Vector4.hpp"
#include "FBXImporter.h"

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

	Vertex() = default;

	Vertex operator=(TGA::FBXVertex aVertex)
	{
		Position.x = aVertex.Position[0];
		Position.y = aVertex.Position[1];
		Position.z = aVertex.Position[2];
		Position.w = aVertex.Position[3];

		VertexColors[0].x = aVertex.VertexColors[0][0];
		VertexColors[0].y = aVertex.VertexColors[0][1];
		VertexColors[0].z = aVertex.VertexColors[0][2];
		VertexColors[0].w = aVertex.VertexColors[0][3];

		return *this;
	}
};