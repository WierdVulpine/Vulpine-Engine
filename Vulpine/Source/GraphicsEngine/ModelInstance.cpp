#include "ModelInstance.h"

void ModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

void ModelInstance::UpdateAnimationHierarchy(size_t aCurrentFrame, unsigned aBoneIdx, const Model::Animation* anAnimation, CommonUtilities::Matrix4x4f& aParentTransform, CommonUtilities::Matrix4x4f* outBoneTransform)
{
	size_t nextFrame = aCurrentFrame + 1;

	if (nextFrame >= myCurrentAnimation.Length)
	{
		nextFrame = 0;
	}

	const Model::Skeleton::Bone& bone = myModel->GetSkeleton()->Bones[aBoneIdx];
	const Matrix4x4f& frameTransform = anAnimation->Frames[aCurrentFrame].LocalTransforms[aBoneIdx];

	Matrix4x4f transposedFrameTransform = Matrix4x4f::Transpose(frameTransform);

	Matrix4x4f boneTransform = aParentTransform * transposedFrameTransform;
	Matrix4x4f result;
	result *= boneTransform;
	result *= bone.BindPoseInverse;

	outBoneTransform[aBoneIdx] = result;

	for (size_t i = 0; i < bone.Children.size(); i++)
	{
		UpdateAnimationHierarchy(aCurrentFrame, bone.Children[i], anAnimation, boneTransform, outBoneTransform);
	}

	myCurrentAnimation.myCurrentFrame = nextFrame;

}

void ModelInstance::Update(float someDeltaTime)
{
	if (!myModel->myHasSkeleton)
	{
		return;
	}

	myTimer += someDeltaTime;

	if (myTimer >= myCurrentAnimation.Duration)
	{
		myTimer -= myCurrentAnimation.Duration;
	}

	float frameTime = 1 / myCurrentAnimation.FramesPerSecond;
	const float result = myTimer / frameTime;
	const size_t frame = static_cast<size_t>(std::floor(result));
	Matrix4x4f identity;

	UpdateAnimationHierarchy(frame, 0, &myCurrentAnimation, identity, &myBoneTransforms[0]);

}
