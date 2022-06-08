#pragma once
#include <cassert>

namespace CommonUtilities
{
	template <typename Type, int size, typename
		CountType = unsigned short, bool UseSafeModeFlag = true>
		class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);
		VectorOnStack& operator=(const VectorOnStack&
			aVectorOnStack);
		inline const Type& operator[](const CountType aIndex) const;
		inline Type& operator[](const CountType aIndex);
		inline void Add(const Type& aObject);
		inline void Insert(const CountType aIndex, const Type&
			aObject);
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(const CountType aIndex);
		inline void Clear();
		_forceinline CountType Size() const;
		~VectorOnStack();

	private:
		Type myArray[size];
		CountType myCurrentSize;
		bool myUseSafeModeFlag;
	};

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		assert(size > 0 && L"ERROR! VECTOR NEEDS A SIZE!");
		myCurrentSize = size;
		myUseSafeModeFlag = UseSafeModeFlag;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
	{
		for (CountType i = 0; i < myCurrentSize; ++i)
		{
			this->myArray[i] = aVectorOnStack.myArray[i];
		}
		this->myCurrentSize = aVectorOnStack.myCurrentSize;
		this->myUseSafeModeFlag = aVectorOnStack.myUseSafeModeFlag;
		return (*this);
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::
		operator=(const VectorOnStack& aVectorOnStack)
	{
		for (CountType i = 0; i < myCurrentSize; ++i)
		{
			this->myArray[i] = aVectorOnStack.myArray[i];
		}
		this->myCurrentSize = aVectorOnStack.myCurrentSize;
		this->myUseSafeModeFlag = aVectorOnStack.myUseSafeModeFlag;
		return (*this);
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	const Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType aIndex) const
	{
		assert(myCurrentSize >= aIndex && L"ERROR! INDEX OUTOFRANGE!");
		assert(aIndex >= 0 && L"ERROR! INDEX OUTOFRANGE!");
		return myArray[aIndex];
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	Type& VectorOnStack<Type, size, CountType, UseSafeModeFlag>::operator[](const CountType aIndex)
	{
		assert(myCurrentSize >= aIndex && L"ERROR! INDEX OUTOFRANGE!");
		assert(aIndex >= 0 && L"ERROR! INDEX OUTOFRANGE!");
		return myArray[aIndex];
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Add(const Type& aObject)
	{
		assert(myCurrentSize + 1 <= size && L"ERROR! CANT ADD TO VECTOR BECAUSE IT WILL BE TO BIG!");
		myArray[myCurrentSize++] = aObject;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Insert(const CountType aIndex, const Type& aObject)
	{
		assert(myCurrentSize < size && L"ERROR! CANT INSERT TO A FULL ARRAY");
		assert(myCurrentSize > aIndex && L"ERROR! CANT INSERT ON A EMPTY SPACE! try add instead.");
		for (CountType i = size - 1; i >= aIndex; --i)
		{
			myArray[i + 1] = myArray[i];
		}
		myArray[aIndex] = aObject;
	}


	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& aObject)
	{
		CountType removeId = -1;
		for (CountType i = 0; i < myCurrentSize; ++i)
		{
			if (myArray[i] == aObject)
			{
				removeId = i;
				myArray[i] = myArray[--myCurrentSize];
			}
		}
		assert(removeId > -1 && L"ERROR! COULDNT FIND OBJECT IN VECTOR!");
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType aIndex)
	{
		assert(aIndex < myCurrentSize && L"ERROR! THERE IS NOTHING ON CHOSEN INDEX!");
		myArray[aIndex] = myArray[--myCurrentSize];
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Clear()
	{
		myCurrentSize = 0;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	CountType VectorOnStack<Type, size, CountType, UseSafeModeFlag>::Size() const
	{
		return myCurrentSize;
	}

	template <typename Type, int size, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, size, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		myCurrentSize = 0;
	}
}
