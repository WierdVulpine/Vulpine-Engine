#include "Light.h"

void Light::Init(CommonUtilities::Vector3f aColor, float aIntensity)
{
	myLightBufferData.Color = aColor;
	myLightBufferData.Intensity = aIntensity;
}

void Light::ClearShadowMap()
{
	DX11::Context->ClearDepthStencilView(myShadowMap->myDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Light::SetShadowMapAsDepth(const GBuffer& aGbuffer)
{
	DX11::Context->OMSetRenderTargets(1, aGbuffer.myVRTVs.GetAddressOf(), myShadowMap->myDSV.Get());
}

void Light::SetShadowMapAsResource(int aSlot)
{
	DX11::Context->PSSetShaderResources(aSlot, 1, myShadowMap->mySRV.GetAddressOf());
}