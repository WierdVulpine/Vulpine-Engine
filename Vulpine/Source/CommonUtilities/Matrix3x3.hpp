#include "VectorOnStack.h"
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include <math.h>

namespace CommonUtilities
{
	template <class T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>();
		Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

	private:
		CommonUtilities::VectorOnStack<T, 9> myMatrixVector;
	};

	template<class T>
	Matrix3x3<T>::Matrix3x3()
	{
		for (char i = 0; i < myMatrixVector.Size(); i++)
		{
			myMatrixVector[i] = 0;
		}
		myMatrixVector[0] = 1;
		myMatrixVector[4] = 1;
		myMatrixVector[8] = 1;
	}

	template<class T>
	Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		int count = 0;
		for (char i = 1; i < 4; ++i)
		{
			for (char j = 1; j < 4; j++)
			{
				myMatrixVector[count] = aMatrix(i, j);
				count++;
			}
		}
	}

	template<class T>
	Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		int count = 0;
		for (char i = 1; i < 4; ++i)
		{
			for (char j = 1; j < 4; j++)
			{
				myMatrixVector[count] = aMatrix(i, j);
				count++;
			}
		}
	}

	template<class T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				tempMatrix(j, i) = aMatrix0(j, i) + aMatrix1(j, i);
			}
		}
		return tempMatrix;
	}

	template<class T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				tempMatrix(j, i) = aMatrix0(j, i) - aMatrix1(j, i);
			}
		}
		return tempMatrix;
	}

	template<class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const float aScalar)
	{
		Matrix3x3<T> tempMatrix;
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				tempMatrix(j, i) = aMatrix0(j, i) * aScalar;
			}
		}
		return tempMatrix;
	}

	template<class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				tempMatrix(j, i) = 0;
			}
		}
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				for (char u = 1; u < 4; u++)
				{
					tempMatrix(j, i) += aMatrix0(j, u) * aMatrix1(u, i);
				}
			}
		}
		return tempMatrix;
	}

	template<class T>
	Vector3<T> operator*(const Vector3<T> aVector3, const Matrix3x3<T>& aMatrix0)
	{
		Vector3<T> tempVector;
		tempVector.x = aVector3.x * aMatrix0(1, 1) + aVector3.y * aMatrix0(2, 1) + aVector3.z * aMatrix0(3, 1);
		tempVector.y = aVector3.x * aMatrix0(1, 2) + aVector3.y * aMatrix0(2, 2) + aVector3.z * aMatrix0(3, 2);
		tempVector.z = aVector3.x * aMatrix0(1, 3) + aVector3.y * aMatrix0(2, 3) + aVector3.z * aMatrix0(3, 3);
		return tempVector;
	}

	template<class T>
	void operator+=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				aMatrix0(j, i) += aMatrix1(j, i);
			}
		}
	}

	template<class T>
	void operator-=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				aMatrix0(j, i) -= aMatrix1(j, i);
			}
		}
	}

	template<class T>
	void operator*=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				tempMatrix(j, i) = 0;
			}
		}
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
			{
				for (char u = 1; u < 4; u++)
				{
					tempMatrix(j, i) += aMatrix0(j, u) * aMatrix1(u, i);
				}
			}
		}
		aMatrix0 = tempMatrix;
	}

	template<class T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3& aMatrix)
	{
		int count = 0;
		for (char i = 1; i < 4; ++i)
		{
			for (char j = 1; j < 4; j++)
			{
				myMatrixVector[count] = aMatrix(i, j);
				count++;
			}
		}
		return *this;
	}

	template<class T>
	bool operator==(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (char i = 1; i < 4; i++)
		{
			for (char j = 1; j < 4; j++)
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
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow > 0 && aRow <= 3 && L"Row Needs To Be A Number Between 1 And 3");
		assert(aColumn > 0 && aColumn <= 3 && L"Column Needs To Be A Number Between 1 And 3");
		return myMatrixVector[(aColumn + (aRow * 3)) - 4];
	}

	template<class T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow > 0 && aRow <= 3 && L"Row Needs To Be A Number Between 1 And 3");
		assert(aColumn > 0 && aColumn <= 3 && L"Column Needs To Be A Number Between 1 And 3");
		return myMatrixVector[(aColumn + (aRow * 3)) - 4];
	}
	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> tempMatrix;
		tempMatrix(1, 1) = 1;
		tempMatrix(2, 2) = cos(aAngleInRadians);
		tempMatrix(2, 3) = sin(aAngleInRadians);
		tempMatrix(3, 2) = -sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);
		return tempMatrix;
	}
	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> tempMatrix;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 3) = -sin(aAngleInRadians);
		tempMatrix(2, 2) = 1;
		tempMatrix(3, 1) = sin(aAngleInRadians);
		tempMatrix(3, 3) = cos(aAngleInRadians);
		return tempMatrix;
	}
	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> tempMatrix;
		tempMatrix(1, 1) = cos(aAngleInRadians);
		tempMatrix(1, 2) = sin(aAngleInRadians);
		tempMatrix(2, 1) = -sin(aAngleInRadians);
		tempMatrix(2, 2) = cos(aAngleInRadians);
		tempMatrix(3, 3) = 1;
		return tempMatrix;
	}
	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3 tempMatrix;
		tempMatrix = aMatrixToTranspose;
		tempMatrix(1, 2) = aMatrixToTranspose(2, 1);
		tempMatrix(2, 1) = aMatrixToTranspose(1, 2);
		tempMatrix(3, 1) = aMatrixToTranspose(1, 3);
		tempMatrix(1, 3) = aMatrixToTranspose(3, 1);
		tempMatrix(3, 2) = aMatrixToTranspose(2, 3);
		tempMatrix(2, 3) = aMatrixToTranspose(3, 2);

		return tempMatrix;
	}
}