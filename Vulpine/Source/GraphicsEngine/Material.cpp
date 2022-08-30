#include "Material.h"

void Material::Init(std::wstring aName, const CommonUtilities::Vector3f& anAlbedo)
{
	myName = aName;
	myAlbedo = anAlbedo;
}
