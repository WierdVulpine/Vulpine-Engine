#pragma once
#include "SceneObject.h"
#include "Vector2.hpp"

using namespace CommonUtilities;

class Camera : public SceneObject
{
	Matrix4x4f myProjection = {};

	float myNearPlane = 1.f;
	float myFarPlane = 1500.0f;

public:

	bool Init(float aHorizontalFov, CommonUtilities::Vector2<unsigned int> aResulution, float aNearPlane, float aFarPlane);

	Matrix4x4f const& GetProjection() const { return myProjection; }
	float GetNearPlane() const { return myNearPlane; }
	float GetFarPlane() const { return myFarPlane; }


};

