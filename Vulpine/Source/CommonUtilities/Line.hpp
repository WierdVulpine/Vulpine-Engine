#pragma once
#include "Vector2.hpp"

namespace CommonUtilities
{
	template <class T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line() = default;

		// Copy constructor.
		Line(const Line <T>& aLine);

		// Constructor that takes two points that define the line, the direction is aPoint1 -
		//aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>&
			aDirection);

		// Returns whether a point is inside the line: it is inside when the point is on the
		//line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<T>& aPosition) const;

		// Returns the direction of the line.
		const Vector2<T>& GetDirection() const;

		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T>& GetNormal() const;

	private:
		Vector2<T> myStartPoint;
		Vector2<T> myEndPoint;
	};

	template<class T>
	inline Line<T>::Line(const Line<T>& aLine)
	{
		myStartPoint = aLine.myStartPoint;
		myEndPoint = aLine.myEndPoint;
	}

	template<class T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myStartPoint = aPoint0;
		myEndPoint = aPoint1;
	}

	template<class T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myStartPoint = aPoint0;
		myEndPoint = aPoint1;
	}

	template<class T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myStartPoint = aPoint;
		myEndPoint = aDirection.GetNormalized();
	}

	template<class T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		T dxc = aPosition.x - myStartPoint.x;
		T dyc = aPosition.y - myStartPoint.y;

		T dxl = myEndPoint.x - myStartPoint.x;
		T dyl = myEndPoint.y - myStartPoint.y;

		T cross = dxc * dyl - dyc * dxl;

		if (cross > 0)
		{
			return false;
		}
		if (cross < 0)
		{
			return true;
		}
		if (cross == 0)
		{
			return false;
		}

		return false;
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetDirection() const
	{
		return Vector2<T>((myEndPoint - myStartPoint)).GetNormalized();
	}

	template<class T>
	inline const Vector2<T>& Line<T>::GetNormal() const
	{
		return Vector2<T>(-GetDirection().y, GetDirection().x);
	}
}

