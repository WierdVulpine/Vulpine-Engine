#include "CommonStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	const float4 vertexObjectPosition = input.myPosition;

	const float4 vertexWorldPosition = mul(OB_ToWorld, vertexObjectPosition);

	const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);

	const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

	result.myPosition = vertexProjectionPosition;

	result.myVxColor  = input.myVxColor;
	result.myVxColor2 = input.myVxColor2;
	result.myVxColor3 = input.myVxColor3;
	result.myVxColor4 = input.myVxColor4;

	return result;
}