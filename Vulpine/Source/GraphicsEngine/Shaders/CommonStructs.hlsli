#define MAX_LIGHTS 64

struct VertexInput
{
	float4 myPosition    : POSITION;

	float4 myVxColor     : COLOR0;
	float4 myVxColor2    : COLOR1;
	float4 myVxColor3    : COLOR2;
	float4 myVxColor4    : COLOR3;

	uint4 myBoneIDs      : BONEIDS;
	float4 myBoneWeights : BONEWEIGHTS;

	float2 myUV          : TEXCOORD0;
	float2 myUV2         : TEXCOORD1;
	float2 myUV3         : TEXCOORD2;
	float2 myUV4         : TEXCOORD3;

	float3 myTangent     : TANGENT;
	float3 myBinormal    : BINORMAL;
	float3 myNormal      : NORMAL;
};

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
	float4 myWorldPosition : POSITION;

	float4 myVxColor  : COLOR0;
	float4 myVxColor2 : COLOR1;
	float4 myVxColor3 : COLOR2;
	float4 myVxColor4 : COLOR3;

	float2 myUV       : TEXCOORD0;
	float2 myUV2      : TEXCOORD1;
	float2 myUV3      : TEXCOORD2;
	float2 myUV4      : TEXCOORD3;

	float3 myTangent  : TANGENT;
	float3 myBinormal : BINORMAL;
	float3 myNormal   : NORMAL;
};

struct DefferedVertexInput
{
	unsigned int myIndex : SV_VertexID;
};

struct DefferedVertexToPixel
{
	float4 Position : SV_POSITION;
	float2 UV       : TEXCOORD;
};

struct DefferedPixelOutput
{
	float4 Color : SV_TARGET;
};

struct GBufferOutput
{
	float4 Albedo          : SV_TARGET0;
	float4 Normal          : SV_TARGET1;
	float4 Material        : SV_TARGET2;

	float4 VertexNormal    : SV_TARGET3;

	float4 WorldPosition    : SV_TARGET4;

	float AmbientOcclusion : SV_TARGET5;
};

struct ParticleVertexData
{
	float4 Position : POSITION;
	float4 Color    : COLOR;
	float3 Velocity : VELOCITY;
	float3 Scale    : SCALE;
	float  LifeTime : LIFETIME;
};

struct ParticleGeometryToPixel
{
	float4 Position : SV_POSITION;
	float4 Color    : COLOR;
	float3 Velocity : VELOCITY;
	float3 Scale    : SCALE;
	float  LifeTime : LIFETIME;
	float2 UV       : TEXCOORD;
};

struct ParticlePixelOutput
{
	float4 Color    : SV_TARGET;
};

SamplerState defaultSampler : register(s0);
SamplerState pointClampSampler : register(s1);

Texture2D albedoTexture : register(t0);

Texture2D normalTexture : register(t1);

Texture2D materialTexture : register(t2);

TextureCube enviromentTexture : register(t10);

struct PixelOutput
{
	float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;
	float3 FB_CamTranslation;
	float FB_padding;
}

cbuffer ObjectBuffer : register(b1)
{
	float4x4 OB_ToWorld;
	bool OB_HasBones;
	float3 OB_Padding;
	float4x4 OB_BoneData[128];
}

cbuffer MaterialBuffer : register(b2)
{
	float3 MB_Albedo;
	float padding;
}



struct LightData
{
	float4x4 View;
	float4x4 Projection;

	float3 Color;
	float Intensity;
	float3 Direction;
	float Range;
	float3 Position;
	float Attenuation;
	float SpotInnerRadius;
	float SpotOuterRadius;
	uint LightType;
	bool CastShadows;

	float NearPlane;
	float FarPlane;
	float2 paddington;
};

cbuffer LightBuffer : register(b3)
{
	LightData LB_DirectionalLight;
	LightData LB_Lights[MAX_LIGHTS];
	uint LB_NumLights;
	float3 LB_Padding;
}