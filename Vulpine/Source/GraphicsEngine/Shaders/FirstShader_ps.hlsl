#include "CommonStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	result.myColor = input.myVxColor * float4(MB_Albedo, 1);

	return result;
}