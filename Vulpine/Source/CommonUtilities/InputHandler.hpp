#pragma once
#include <windows.h>
#include <windowsx.h>
#include <bitset>
#include "Vector2.hpp"

#pragma region Defines
#define Zero_KEY 0x30
#define One_KEY 0x31
#define Two_KEY 0x32
#define THREE_KEY 0x33
#define FOUR_KEY 0x34
#define FIVE_KEY 0x35
#define SIX_KEY 0x36
#define SEVEN_KEY 0x37
#define EIGHT_KEY 0x38
#define NINE_KEY 0x39
#define A_KEY 0x41
#define B_KEY 0x42
#define C_KEY 0x43
#define D_KEY 0x44
#define E_KEY 0x45
#define F_KEY 0x46
#define G_KEY 0x47
#define H_KEY 0x48
#define I_KEY 0x49
#define J_KEY 0x4A
#define K_KEY 0x4B
#define L_KEY 0x4C
#define M_KEY 0x4D
#define N_KEY 0x4E
#define O_KEY 0x4F
#define P_KEY 0x50
#define Q_KEY 0x51
#define R_KEY 0x52
#define S_KEY 0x53
#define T_KEY 0x54
#define U_KEY 0x55
#define V_KEY 0x56
#define W_KEY 0x57
#define X_KEY 0x58
#define Y_KEY 0x59
#define Z_KEY 0x5A
#pragma endregion

namespace CommonUtilities
{
	struct Point
	{
		Point()
		{
			x = 0;
			y = 0;
		}
		Point(int aX, int aY)
		{
			x = aX;
			y = aY;
		}
		int x, y;
	};

	class InputHandler
	{
	public:
		InputHandler() = default;
		bool GetKeyDown(const int aKeyCode) const;
		bool GetKeyUp(const int aKeyCode) const;
		bool KeyIsPressed(const int aKeyCode) const;
		bool MouseButtonPressed(int aKeyIndex) const;
		bool GetMouseButtonDown(int aKeyIndex) const;
		bool GetMouseButtonUp(int aKeyIndex) const;
		bool UpdateEvents(UINT aMessage, WPARAM wParam, LPARAM lParam);
		void UpdateInput();
		void SetMousePosition(int aX, int aY);
		CommonUtilities::Vector2f GetMousePosition() const;

		CommonUtilities::Vector2f GetMouseDelta()
		{
			return (myMousePos - myPreviousMousePosition);
		}

		void SetHandle(HWND aHandlee) { myHandle = aHandlee; }

		~InputHandler() = default;
	private:
		std::bitset<256> myKeyStrokes;
		std::bitset<256> myPreviousKeyStroke;
		std::bitset<256> myInputState;

		std::bitset<6> myCurrentMouseInputs;
		std::bitset<3> myMouseInputs;
		CommonUtilities::Vector2f myMousePos;

		HWND myHandle;

		CommonUtilities::Vector2f myTentativeMousePosition;
		CommonUtilities::Vector2f myCurrentMousePosition;
		CommonUtilities::Vector2f myPreviousMousePosition;

		CommonUtilities::Vector2f myTentativeMouseDelta;
		CommonUtilities::Vector2f myMouseDelta;
	};
}