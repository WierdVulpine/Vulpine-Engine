#include "LightAssetHandler.h"
#include "TextureAssetHandler.h"

std::shared_ptr<DirectionalLight> LightAssetHandler::CreateDirectionalLight(CommonUtilities::Vector3f aColor, float anIntensity, CommonUtilities::Vector3f aRotation)
{
    myDirectionalLight = std::make_shared<DirectionalLight>();
    myDirectionalLight->Init(aColor, anIntensity);

    CommonUtilities::Matrix4x4f lightRotation = CommonUtilities::Matrix4x4f();
    lightRotation.CreateRotationAroundX(aRotation.x);
    lightRotation.CreateRotationAroundX(aRotation.y);
    lightRotation.CreateRotationAroundX(aRotation.z);

    myDirectionalLight->myLightBufferData.Direction = CommonUtilities::Vector3f({ lightRotation(3, 1), lightRotation(3, 2), lightRotation(3, 3) }).GetNormalized();

    return myDirectionalLight;
}

std::shared_ptr<EnviromentLight> LightAssetHandler::CreateEnvirometalLight(const std::wstring& aTextureName)
{
    myEnviromentalLight = std::make_shared<EnviromentLight>();
    if (TextureAssetHandler::LoadTexture(aTextureName))
    {
        myEnviromentalLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);
    }
    return myEnviromentalLight;
}
