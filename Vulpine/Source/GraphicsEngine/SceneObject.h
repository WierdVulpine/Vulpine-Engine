#pragma once
#include "Transform.hpp"

class SceneObject
{
protected:
	CommonUtilities::Transform myTransform;

public:
	SceneObject() = default;
	virtual ~SceneObject() = default;

	void SetTransform(CommonUtilities::Transform someTransform) { myTransform = someTransform; }

	void SetRotation(float aPitch, float aYaw, float aRoll) { myTransform.SetRotation({ aPitch, aYaw, aRoll }); }
	void SetRotation(CommonUtilities::Vector3<float> aRotation) { myTransform.SetRotation(aRotation); }

	void SetPosition(float aX, float aY, float aZ) { myTransform.SetPosition({ aX, aY, aZ }); }
	void SetPosition(CommonUtilities::Vector3<float> aPosition) { myTransform.SetPosition(aPosition); }

	void SetScale(float aX, float aY, float aZ) { myTransform.SetScale({ aX, aY, aZ }); }
	void SetScale(CommonUtilities::Vector3<float> aScale) { myTransform.SetScale(aScale); }

	CommonUtilities::Transform const& GetTransform() const { return myTransform; }
	CommonUtilities::Transform& GetTransform() { return myTransform; }

};

