#pragma once
#include "Matrix4x4.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	class Transform
	{
	public:
		Transform() = default;
		Transform(Matrix4x4f aMatrix) { myMatrix = aMatrix; }

		void SetPosition(const Vector3f& aPosition) { myPosition = aPosition; }
		void SetRotation(const Vector3f& aRotation) { myRotation = aRotation; }
		void SetScale(const Vector3f& aScale) { myScale = aScale; }

		Vector3f GetRotation() const { return myRotation; }
		Vector3f GetPosition() const { return myPosition; }
		Vector3f GetScale() const { return myScale; }

		Matrix4x4f GetMatrix() const 
		{
			Matrix4x4f temp;

			temp = Matrix4x4f::CreateScaleMatrix(myScale);

			// X Y Z Rotation
			temp *= Matrix4x4f::CreateRotationAroundX(myRotation.x);
			temp *= Matrix4x4f::CreateRotationAroundY(myRotation.y);
			temp *= Matrix4x4f::CreateRotationAroundZ(myRotation.z);

			temp(4, 1) = myPosition.x;
			temp(4, 2) = myPosition.y;
			temp(4, 3) = myPosition.z;

			return temp;
		}

		Vector3f GetForward() const
		{
			Matrix4x4f rotation = myMatrix;

			rotation(4, 1) = 0;
			rotation(4, 2) = 0;
			rotation(4, 3) = 0;

			auto result = Vector4<float>(0, 0, 1.f, 1.f) * rotation;
			result.Normalize();
			return { result.x, result.y, result.z };
		}

		Vector3f GetRight() const
		{
			Matrix4x4f rotation = myMatrix;

			rotation(4, 1) = 0;
			rotation(4, 2) = 0;
			rotation(4, 3) = 0;

			auto result = Vector4<float>(1.f, 0, 0, 1.f) * rotation;
			result.Normalize();
			return { result.x, result.y, result.z };
		}

		Vector3f GetUp() const
		{
			Matrix4x4f rotation = myMatrix;

			rotation(4, 1) = 0;
			rotation(4, 2) = 0;
			rotation(4, 3) = 0;

			auto result = Vector4<float>(0, 1.f, 0, 1.f) * rotation;
			result.Normalize();
			return { result.x, result.y, result.z };
		}

	private:
		Matrix4x4f myMatrix;
		Vector3f myPosition;
		Vector3f myScale = { 1 ,1, 1 };
		Vector3f myRotation;
	};
}