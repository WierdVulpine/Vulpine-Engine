#pragma once
#include <math.h>
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;
		//Creates a null-vector
		Vector2<T>();
		//Destructor (compiler generated)
		~Vector2<T>() = default;
		//Creates a vector (aX, aY)
		Vector2<T>(const T& aX, const T& aY);
		//Copy constructor (compiler generated)
		Vector2<T>(const Vector2<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;
		//Returns the squared length of the vector
		T LengthSqr() const;
		//Returns the length of the vector
		T Length() const;
		//Returns a normalized copy of this
		Vector2<T> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Normalize but fast
		void FastNormalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;
	};

	

	template<class T>
	Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}


	template<class T>
	Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template<class T>
	T Vector2<T>::LengthSqr() const
	{
		return (x * x) + (y * y);
	}

	template<class T>
	T Vector2<T>::Length() const
	{
		return sqrt((x * x) + (y * y));
	}

	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const 
	{
		return Vector2<T>(x, y).Normalize();
	}


	template<class T>
	void Vector2<T>::Normalize()
	{
		assert(x != 0 || y != 0 && "You can't devide by zero");
		float magnitude = 1 / Length();
		x *= magnitude;
		y *= magnitude;
	}

	template<class T>
	void Vector2<T>::FastNormalize()
	{
		assert(x != 0 || y != 0 && "You can't devide by zero");
		float magnitude = Q_rsqrt(LengthSqr());
		x *= magnitude;
		y *= magnitude;
	}

	template<class T>
	T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return (x * aVector.x) + (y * aVector.y);
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}
	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return Vector2<T>(aScalar * aVector.x, aScalar * aVector.y);

	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVectormultiplied by 1 / aScalar)
	template <class T> Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * (1 / aScalar), aVector.y * (1 / aScalar));
	}
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}



}
