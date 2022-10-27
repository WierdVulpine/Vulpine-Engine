#pragma once
#include "SceneObject.h"
#include "Model.h"


class ModelInstance : public SceneObject
{
	std::shared_ptr<Model> myModel;


	float myTimer;
	bool IsLooping;

	Model::Animation myCurrentAnimation;

public:

	CommonUtilities::Matrix4x4f myBoneTransforms[128]{};

	ModelInstance() = default;

	void Init(std::shared_ptr<Model> aModel);

	void Update(float someDeltaTime);
	void UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned aBoneIdx, const Model::Animation* anAnimation,
		CommonUtilities::Matrix4x4f& aParentTransform, CommonUtilities::Matrix4x4f* outBoneTransform);

	bool const HasBones() { return myModel->myHasSkeleton; }

	void SetAnimation(const std::wstring& aAnimationName) 
	{ 
		myCurrentAnimation = myModel->GetSkeleton()->Animations.at(aAnimationName);
	}

	std::wstring GetName() { return myModel->GetName(); }
	std::string GetNameStr() { return myRealName; }
	std::shared_ptr<Material> GetMaterial() { return myModel->GetMeshData(0).myMaterial; }
	void SetMaterial(std::shared_ptr<Material> aMaterial)
	{
		for (size_t i = 0; i < myModel->GetNumMeshes(); i++)
		{
			myModel->GetMeshData(i).myMaterial = aMaterial;
		}
	}
	std::string GetModelPath() { return std::string(myModel->myPath.begin(), myModel->myPath.end()); }

	FORCEINLINE Model::MeshData const& GetMeshData(unsigned int anIndex) const { return myModel->GetMeshData(anIndex); }
	FORCEINLINE size_t GetNumMeshes() const { return myModel->GetNumMeshes(); }

};

