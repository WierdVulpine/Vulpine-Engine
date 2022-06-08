#pragma once
#include "VectorOnStack.h"
#include "Vector4.hpp"
#include <math.h>
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>();
		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

	private:
		CommonUtilities::VectorOnStack<T, 16> myMatrixVector;
	};

	typedef Matrix4x4<float> Matrix4x4f;

	template<class T>
	inline Matrix4x4<T>::Matrix4x4()
	{
		for (char i = 0; i < myMatrixVector.Size(); i++)
		{
			myMatrixVector[i] = 0;
		}
		myMatrixVector[0] = 1;
		myMatrixVector[5] = 1;
		myMatrixVector[10] = 1;
		myMatrixVector[15] = 1;
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		int count = 0;
		for (char i = 1; i < 5; ++i)
		{
			for (char j = 1; j < 5; j++)
			{
				myMatrixVector[count] = aMatrix(i, j);
				count++;
			}
		}
	}

	template<class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> tempMatrix;
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				tempMatrix(j, i) = aMatrix0(j, i) + aMatrix1(j, i);
			}
		}
		return tempMatrix;
	}

	template<class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> tempMatrix;
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				tempMatrix(j, i) = aMatrix0(j, i) - aMatrix1(j, i);
			}
		}
		return tempMatrix;
	}

	template<class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const float aScalar)
	{
		Matrix4x4<T> tempMatrix;
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				tempMatrix(j, i) = aMatrix0(j, i) * aScalar;
			}
		}
		return tempMatrix;
	}

	template<class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> tempMatrix;
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				tempMatrix(j, i) = 0;
			}
		}
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				for (char u = 1; u < 5; u++)
				{
					tempMatrix(j, i) += aMatrix0(j, u) * aMatrix1(u, i);
				}
			}
		}
		return tempMatrix;
	}

	template<class T>
	Vector4<T> operator*(const Vector4<T> aVector4, const Matrix4x4<T>& aMatrix0)
	{
		Vector4<T> tempVector;
		tempVector.x = aVector4.x * aMatrix0(1, 1) + aVector4.y * aMatrix0(2, 1) + aVector4.z * aMatrix0(3, 1) + aVector4.w * aMatrix0(4, 1);
		tempVector.y = aVector4.x * aMatrix0(1, 2) + aVector4.y * aMatrix0(2, 2) + aVector4.z * aMatrix0(3, 2) + aVector4.w * aMatrix0(4, 2);
		tempVector.z = aVector4.x * aMatrix0(1, 3) + aVector4.y * aMatrix0(2, 3) + aVector4.z * aMatrix0(3, 3) + aVector4.w * aMatrix0(4, 3);
		tempVector.w = aVector4.x * aMatrix0(1, 4) + aVector4.y * aMatrix0(2, 4) + aVector4.z * aMatrix0(3, 4) + aVector4.w * aMatrix0(4, 4);
		return tempVector;
	}

	template<class T>
	void operator+=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				aMatrix0(j, i) += aMatrix1(j, i);
			}
		}
	}

	template<class T>
	void operator-=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				aMatrix0(j, i) -= aMatrix1(j, i);
			}
		}
	}

	template<class T>
	void operator*=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> tempMatrix;
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				tempMatrix(j, i) = 0;
			}
		}
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				for (char u = 1; u < 5; u++)
				{
					tempMatrix(j, i) += aMatrix0(j, u) * aMatrix1(u, i);
				}
			}
		}
		aMatrix0 = tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4& aMatrix)
	{
		int count = 0;
		for (char i = 1; i < 5; ++i)
		{
			for (char j = 1; j < 5; j++)
			{
				myMatrixVector[count] = aMatrix(i, j);
				count++;
			}
		}
		return *this;
	}

	template<class T>
	bool operator==(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (char i = 1; i < 5; i++)
		{
			for (char j = 1; j < 5; j++)
			{
				if (aMatrix0(j, i) != aMatrix1(j, i))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<class T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow > 0 && aRow <= 4 && L"Row Needs To Be A Number Between 1 And 4");
		assert(aColumn > 0 && aColumn <= 4 && L"Column Needs To Be A Number Between 1 And 4");
		return myMatrixVector[(aColumn + (aRow * 4)) - 5];
	}

	template<class T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow > 0 && aRow <= 4 && L"Row Needs To Be A Number Between 1 And 4");
		assert(aColumn > 0 && aColumn <= 4 && L"Column Needs To Be A Number Between 1 And 4");
		return myMatrixVector[(aColumn + (aRow * 4)) - 5];
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> tempMatrix;
		tempMatrix(1, 1) = 1;
		tempMatrix(2, 2) = cos(aAngleInRadians);
		tempMatrix(2, 3) = sin(aAngleInRadians);
		tempMatrix(3, 2) = -sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);
		return tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> tempMatrix;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 3) = -sin(aAngleInRadians);
		tempMatrix(2, 2) = 1;
		tempMatrix(3, 1) = sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);
		return tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> tempMatrix;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 2) = sin(aAngleInRadians);
		tempMatrix(2, 1) = -sin(aAngleInRadians);
		tempMatrix(2, 2) = cos(aAngleInRadians);
		tempMatrix(3, 3) = 1;
		return tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4 tempMatrix;
		tempMatrix = aMatrixToTranspose;
		tempMatrix(1, 2) = aMatrixToTranspose(2, 1);
		tempMatrix(1, 3) = aMatrixToTranspose(3, 1);
		tempMatrix(1, 4) = aMatrixToTranspose(4, 1);
		tempMatrix(2, 1) = aMatrixToTranspose(1, 2);
		tempMatrix(2, 3) = aMatrixToTranspose(3, 2);
		tempMatrix(2, 4) = aMatrixToTranspose(4, 2);
		tempMatrix(3, 1) = aMatrixToTranspose(1, 3);
		tempMatrix(3, 2) = aMatrixToTranspose(2, 3);
		tempMatrix(3, 4) = aMatrixToTranspose(4, 3);
		tempMatrix(4, 1) = aMatrixToTranspose(1, 4);
		tempMatrix(4, 2) = aMatrixToTranspose(2, 4);
		tempMatrix(4, 3) = aMatrixToTranspose(3, 4);

		return tempMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		CommonUtilities::Matrix4x4<T> tempRot;
		CommonUtilities::Matrix4x4<T> tempTranslate;

		tempRot(1, 1) = aTransform(1, 1);
		tempRot(2, 1) = aTransform(1, 2);
		tempRot(3, 1) = aTransform(1, 3);
		tempRot(1, 2) = aTransform(2, 1);
		tempRot(2, 2) = aTransform(2, 2);
		tempRot(3, 2) = aTransform(2, 3);
		tempRot(1, 3) = aTransform(3, 1);
		tempRot(2, 3) = aTransform(3, 2);
		tempRot(3, 3) = aTransform(3, 3);

		tempTranslate(4, 1) = aTransform(4, 1) * -1;
		tempTranslate(4, 2) = aTransform(4, 2) * -1;
		tempTranslate(4, 3) = aTransform(4, 3) * -1;

		return tempTranslate * tempRot;
	}
}