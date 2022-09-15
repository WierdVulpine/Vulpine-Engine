#include "ParticleAssetHandler.h"
#include "filereadstream.h"
#include "document.h"

bool ParticleAssetHandler::LoadEmitterTemplate(const std::wstring& someFilePath)
{
    ParticleEmitterTemplate tempTemplate{};
    EmitterSettingsData tempSettings;
    ParticleEmitter tempEmitter;

    std::wstring filePath = L"Emitters/" + std::wstring(someFilePath.begin(), someFilePath.end());
    std::string fileAsString = std::string(filePath.begin(), filePath.end());
    FILE* fp;
    fopen_s(&fp, fileAsString.c_str(), "rb");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document d;
    d.ParseStream(is);

    std::string parentSystemName = d["SystemName"].GetString();
    std::wstring wParentName = std::wstring(parentSystemName.begin(), parentSystemName.end());

    tempSettings.SpawnRate = d["SpawnRate"].GetFloat();
    tempSettings.SpawnAngle = d["SpawnAngle"].GetFloat();
    tempSettings.LifeTime = d["LifeTime"].GetFloat();

    tempSettings.StartVelocity = { d["StartVelocity"][0].GetFloat(), d["StartVelocity"][1].GetFloat() , d["StartVelocity"][2].GetFloat() };
    tempSettings.EndVelocity = { d["EndVelocity"][0].GetFloat(), d["EndVelocity"][1].GetFloat() , d["EndVelocity"][2].GetFloat() };

    tempSettings.GravityScale = d["GravityScale"].GetFloat();

    tempSettings.StartSize = d["StartSize"].GetFloat();
    tempSettings.EndSize = d["EndSize"].GetFloat();

    tempSettings.StartColor = { d["StartColor"][0].GetFloat(), d["StartColor"][1].GetFloat() , d["StartColor"][2].GetFloat(), d["StartColor"][3].GetFloat() };
    tempSettings.EndColor = { d["EndColor"][0].GetFloat(), d["EndColor"][1].GetFloat() , d["EndColor"][2].GetFloat(), d["EndColor"][3].GetFloat() };

    tempTemplate.EmitterSettings = tempSettings;
    tempTemplate.Path = someFilePath;
    tempEmitter.Init(tempTemplate);

    myEmitterTemplates.insert({ someFilePath, tempTemplate });

    if (mySystemTemplates.find(wParentName) != mySystemTemplates.end())
    {
        mySystemTemplates[wParentName].myEmitters.push_back(tempEmitter);
    }
    else
    {
        mySystemTemplates.insert({ wParentName, ParticleSystem() });
        mySystemTemplates[wParentName].myEmitters.push_back(tempEmitter);
    }

    return true;
}

std::shared_ptr<ParticleSystem> ParticleAssetHandler::GetParticleSystem(const std::wstring& aSystemName)
{
    if (mySystemTemplates.find(aSystemName) != mySystemTemplates.end())
    {
        std::shared_ptr<ParticleSystem> tempSystem = std::make_shared<ParticleSystem>(mySystemTemplates[aSystemName]);
        return tempSystem;
    }
    return std::shared_ptr<ParticleSystem>();
}
