#include "Light.h"

void Light::Init(CommonUtilities::Vector3f aColor, float aIntensity)
{
	myLightBufferData.Color = aColor;
	myLightBufferData.Intensity = aIntensity;
}
