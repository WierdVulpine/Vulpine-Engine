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

	CommonUtilities::Vector4<unsigned int> BoneIDs = { 0,0,0,0 };
	CommonUtilities::Vector4f BoneWeights = { 0,0,0,0 };

	CommonUtilities::Vector2f UVs[4]
	{
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0}
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

		BoneIDs.x = aVertex.BoneIDs[0];
		BoneIDs.y = aVertex.BoneIDs[1];
		BoneIDs.z = aVertex.BoneIDs[2];
		BoneIDs.w = aVertex.BoneIDs[3];

		BoneWeights.x = aVertex.BoneWeights[0];
		BoneWeights.y = aVertex.BoneWeights[1];
		BoneWeights.z = aVertex.BoneWeights[2];
		BoneWeights.w = aVertex.BoneWeights[3];

		UVs[0].x = aVertex.UVs[0][0];
		UVs[0].y = aVertex.UVs[0][1];

		UVs[1].x = aVertex.UVs[1][0];
		UVs[1].y = aVertex.UVs[1][1];

		UVs[2].x = aVertex.UVs[2][0];
		UVs[2].y = aVertex.UVs[2][1];

		return *this;
	}
};