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

		void AddPosition(const Vector3f& aPositionIncrument) { myPosition += aPositionIncrument; }
		void AddRotation(const Vector3f& aRotationIncrument) { myRotation += aRotationIncrument; }
		void AddScale(const Vector3f& aScaleIncrument) { myScale += aScaleIncrument; }

		Vector3f GetRotation() const { return myRotation; }
		Vector3f GetPosition() const { return myPosition; }
		Vector3f GetScale() const { return myScale; }

		Matrix4x4f GetMatrix() const 
		{
			Matrix4x4f temp;

			temp = Matrix4x4f::CreateScaleMatrix(myScale);

			// X Y Z Rotation
			temp *= Matrix4x4f::CreateRotationAroundX(myRotation.x * 3.14f / 180);
			temp *= Matrix4x4f::CreateRotationAroundY(myRotation.y * 3.14f / 180);
			temp *= Matrix4x4f::CreateRotationAroundZ(myRotation.z * 3.14f / 180);

			temp(4, 1) = myPosition.x;
			temp(4, 2) = myPosition.y;
			temp(4, 3) = myPosition.z;

			return temp;
		}

		Vector3f GetForward() const
		{
			Matrix4x4f temp = GetMatrix();
			return Vector3f({ temp(3, 1), temp(3, 2), temp(3, 3) }).GetNormalized();
		}

		Vector3f GetRight() const
		{
			Matrix4x4f temp = GetMatrix();
			return Vector3f({ temp(1, 1), temp(1, 2), temp(1, 3) }).GetNormalized();
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