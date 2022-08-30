#pragma once
#include <string>
#include <Vector3.hpp>

class Material
{
	std::wstring myName;
	CommonUtilities::Vector3f myAlbedo;

public:

	Material() = default;
	virtual ~Material() = default;

	void Init(std::wstring aName, const CommonUtilities::Vector3f& anAlbedo);

	const std::wstring& GetName() const { return myName; }
	const CommonUtilities::Vector3f& GetAlbedo() const { return myAlbedo; }

};

