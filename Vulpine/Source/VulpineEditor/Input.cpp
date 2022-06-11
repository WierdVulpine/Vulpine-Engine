#include "Input.h"



void Input::UpdateEvents(UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	myHandler.UpdateEvents(aMessage, wParam, lParam);
}

void Input::UpdateInput()
{
	myHandler.UpdateInput();
}
