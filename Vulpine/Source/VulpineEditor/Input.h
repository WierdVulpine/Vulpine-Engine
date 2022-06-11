#pragma once
#include "InputHandler.hpp"

class Input
{
public:
	static void SetHandle(HWND aHandlee) { myHandler.SetHandle(aHandlee); }
	static void UpdateEvents(UINT aMessage, WPARAM wParam, LPARAM lParam);
	static void UpdateInput();

	static bool GetKeyDown(const int aKeyCode) { return myHandler.GetKeyDown(aKeyCode); }
	static bool GetKeyUp(const int aKeyCode) { return myHandler.GetKeyUp(aKeyCode); }
	static bool KeyIsPressed(const int aKeyCode) { return myHandler.KeyIsPressed(aKeyCode); }
	static bool MouseButtonPressed(int aKeyIndex) { return myHandler.MouseButtonPressed(aKeyIndex); }
	static bool GetMouseButtonDown(int aKeyIndex) { return myHandler.GetMouseButtonDown(aKeyIndex); }
	static bool GetMouseButtonUp(int aKeyIndex) { return myHandler.GetMouseButtonUp(aKeyIndex); }

	static void SetMousePosition(int aX, int aY) { myHandler.SetMousePosition(aX, aY); }
	static CommonUtilities::Vector2<int> GetMousePosition() { return { (int)myHandler.GetMousePosition().x, (int)myHandler.GetMousePosition().y }; }
	static CommonUtilities::Vector2f GetMouseDelta() { return myHandler.GetMouseDelta(); }

	static void ShowMouse() { ShowCursor(true); }
	static void HideMouse() { ShowCursor(false); }

	/*static void CaptureMouse();
	static void ReleaseMouse();*/

private:
	static inline CommonUtilities::InputHandler myHandler;
};