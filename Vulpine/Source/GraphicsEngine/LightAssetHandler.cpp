#include "LightAssetHandler.h"
#include "TextureAssetHandler.h"

std::shared_ptr<DirectionalLight> LightAssetHandler::CreateDirectionalLight(CommonUtilities::Vector3f aColor, float anIntensity, CommonUtilities::Vector3f aRotation, CommonUtilities::Vector3f aPosition)
{
    myDirectionalLight = std::make_shared<DirectionalLight>();
    myDirectionalLight->Init(aColor, anIntensity);

    CommonUtilities::Matrix4x4f lightRotation = CommonUtilities::Matrix4x4f();
    lightRotation.CreateRotationAroundX(aRotation.x);
    lightRotation.CreateRotationAroundX(aRotation.y);
    lightRotation.CreateRotationAroundX(aRotation.z);

    myDirectionalLight->GetTransform().SetRotation(aRotation);
    myDirectionalLight->GetTransform().SetPosition(aPosition);

    myDirectionalLight->myLightBufferData.Direction = myDirectionalLight->GetTransform().GetForward();

    const float nearPlane = 1.f;
    const float farPlane = 25000.f;
    const CommonUtilities::Vector2f resolution = { 2048, 2048 };

    myDirectionalLight->myLightBufferData.NearPlane = nearPlane;
    myDirectionalLight->myLightBufferData.FarPlane = farPlane;

    Matrix4x4f lightProjection;

    lightProjection(1, 1) = 2.f / resolution.x;
    lightProjection(2, 2) = 2.f / resolution.y;
    lightProjection(3, 3) = 1.f / (farPlane - nearPlane);
    lightProjection(4, 3) = nearPlane / (nearPlane - farPlane);
    lightProjection(4, 4) = 1.f;

    myDirectionalLight->myLightBufferData.LightProjection = lightProjection;
    myDirectionalLight->myLightBufferData.LightView = Matrix4x4f::GetFastInverse(myDirectionalLight->GetTransform().GetMatrix());
    myDirectionalLight->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"Inget XD", resolution.x, resolution.y);

    myDirectionalLight->myLightBufferData.CastShadows = true;

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

std::shared_ptr<PointLight> LightAssetHandler::CreatePointLight(CommonUtilities::Vector3f aColor, float anIntensity, float aRange, CommonUtilities::Vector3f aPosition)
{
    myLights.push_back( std::make_shared<PointLight>());
    std::shared_ptr<PointLight> result = std::dynamic_pointer_cast<PointLight>(myLights.back());
    result->Init(aColor, anIntensity);
    result->SetRange(aRange);
    result->SetPosition(aPosition);
    result->myLightBufferData.LightType = 1;

    return result;
}

std::shared_ptr<SpotLight> LightAssetHandler::CreateSpotLight(Vector3f aColor, float anIntensity, float aRange, float aAttenuation,
    float aInnerCone, float aOuterCone, Vector3f aDirection, Vector3f aPosition)
{
    myLights.push_back(std::make_shared<SpotLight>());
    std::shared_ptr<SpotLight> result = std::dynamic_pointer_cast<SpotLight>(myLights.back());

    result->Init(aColor, anIntensity);
    result->SetRange(aRange);
    result->SetInnerCone(aInnerCone);
    result->SetOuterCone(aOuterCone);
    result->SetPosition(aPosition);
    result->SetDirection(aDirection);
    result->myLightBufferData.Position = aPosition;
    result->myLightBufferData.Attenuation = aAttenuation;
    result->myLightBufferData.LightType = 2;

    return result;
}
