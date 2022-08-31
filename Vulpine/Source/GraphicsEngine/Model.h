#pragma once
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "SceneObject.h"
#include <vector>
#include "Material.h"
#include <unordered_map>

using namespace Microsoft::WRL;

class Model : public SceneObject 
{
public:
	struct MeshData
	{
		UINT myNumberOfVertecies;
		UINT myNumberOfIndices;
		UINT myStride;
		UINT myOffset;
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;
		ComPtr<ID3D11VertexShader> myVertexShader;
		ComPtr<ID3D11PixelShader> myPixelShader;
		ComPtr<ID3D11InputLayout> myInputLayout;
		UINT myPrimitiveTopology;
		std::shared_ptr<Material> myMaterial;
	};

	struct Animation
	{
		enum class AnimationState
		{
			Playing,
			Finished
		};


		struct Frame
		{
			std::vector<CommonUtilities::Matrix4x4f> LocalTransforms;
		};

		std::vector<Frame> Frames;

		unsigned int Length;
		float Duration;
		float FramesPerSecond;
		std::wstring Name;
		AnimationState myState;
		size_t myCurrentFrame = 0;
	};

	struct Skeleton
	{
		std::string Name;

		struct Bone
		{
			CommonUtilities::Matrix4x4f BindPoseInverse;
			int Parent;
			std::vector<unsigned int> Children;
			std::string Name;
		};

		std::vector<Bone> Bones;

		std::unordered_map<std::string, size_t> BoneNameToIndex;
		std::vector<std::string> BoneName;

		std::unordered_map<std::wstring, Animation> Animations;

		FORCEINLINE const Bone* GetRoot() const
		{
			if (!Bones.empty()) return &Bones[0];
			return nullptr;
		}
	};

private:

	std::vector<MeshData> myMeshData = {};
	std::wstring myPath;

	Skeleton mySkeleton;

public:

	bool myHasSkeleton{ false };

	void Init(std::vector<MeshData>& someMeshData, const std::wstring& aPath)
	{
		myMeshData = someMeshData;
		myPath = aPath;
	}

	void Init(std::vector<MeshData>& someMeshData, const std::wstring& aPath, Skeleton& aSkeleton)
	{
		myMeshData = someMeshData;
		myPath = aPath;
		mySkeleton = aSkeleton;
		myHasSkeleton = true;
	}

	void AddAnimation(Animation& anAnimation);

	const MeshData& GetMeshData(unsigned int anIndex) const { return myMeshData[anIndex]; }
	std::wstring const& GetName() const { return myPath; }

	FORCEINLINE size_t GetNumMeshes() const { return myMeshData.size(); }
	FORCEINLINE const Skeleton* GetSkeleton() const { return &mySkeleton; }
};

