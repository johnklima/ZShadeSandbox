#include "input.h"
#include <memory>
//===============================================================================================================================
//===============================================================================================================================
Mouse* Mouse::prevState;
Keyboard* Keyboard::prevState = 0;
BYTE Keyboard::currState[256];
//===============================================================================================================================
Mouse::Mouse()
:	X (0)
,	Y (0)
,	DX (0)
,	DY (0)
,	IsOverWindow (false)
{
}
//===============================================================================================================================
bool Mouse::MouseOffscreen(int& dir, int w, int h, HWND hwnd)
{
	//dir: 0-up, 1-down, 2-left, 3-right
	dir = -1;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);

	bool offscreen = false;

	if (pt.x > w)
	{
		offscreen = true;
		dir = 3;
	}

	else if (pt.x < 0)
	{
		offscreen = true;
		dir = 2;
	}

	if (pt.y > h)
	{
		offscreen = true;
		dir = 1;
	}

	else if (pt.y < 0)
	{
		offscreen = true;
		dir = 0;
	}

	return offscreen;
}
//===============================================================================================================================
Mouse* Mouse::GetMouse(HWND hwnd)
{
	POINT pos;
	if (!GetCursorPos(&pos))
		;

	if (hwnd != NULL)
		if(!ScreenToClient(hwnd, &pos))
			;

	std::unique_ptr<Mouse> newState;
	newState.reset(new Mouse());
	if (!newState)
		return prevState;

	newState->X = pos.x;
	newState->Y = pos.y;
	newState->DX = pos.x - (prevState == 0) ? 0 : prevState->X;
	newState->DY = pos.y - (prevState == 0) ? 0 : prevState->Y;

	newState->LButton.Pressed = (GetKeyState(VK_LBUTTON) & 0x8000) > 0;
	newState->LButton.RisingEdge = newState->LButton.Pressed && !((prevState == 0) ? false : prevState->LButton.Pressed);
	newState->LButton.RisingEdge = !newState->LButton.Pressed && ((prevState == 0) ? false : prevState->LButton.Pressed);

	newState->MButton.Pressed = (GetKeyState(VK_MBUTTON) & 0x8000) > 0;
	newState->MButton.RisingEdge = newState->MButton.Pressed && !((prevState == 0) ? false : prevState->MButton.Pressed);
	newState->MButton.RisingEdge = !newState->MButton.Pressed && ((prevState == 0) ? false : prevState->MButton.Pressed);

	newState->RButton.Pressed = (GetKeyState(VK_RBUTTON) & 0x8000) > 0;
	newState->RButton.RisingEdge = newState->RButton.Pressed && !((prevState == 0) ? false : prevState->RButton.Pressed);
	newState->RButton.RisingEdge = !newState->RButton.Pressed && ((prevState == 0) ? false : prevState->RButton.Pressed);

    if (hwnd != NULL)
    {
        RECT clientRect;
        if (!::GetClientRect(hwnd, &clientRect))
            ;
		newState->IsOverWindow = (pos.x >= 0 && pos.x < clientRect.right && pos.y >= 0 && pos.y < clientRect.bottom);
    }
    else
		newState->IsOverWindow = false;

	prevState = newState.get();

	return prevState;
}
//===============================================================================================================================
void Mouse::SetCursorPos(int x, int y, HWND hwnd)
{
    POINT pos;
    pos.x = x;
    pos.y = y;

    if (hwnd != NULL)
        if (!ClientToScreen(hwnd, &pos))
            ;//throw Win32Exception(GetLastError());

    if (!::SetCursorPos(pos.x, pos.y))
        ;//throw Win32Exception(GetLastError());
}
//===============================================================================================================================
Keyboard::Keyboard()
{
}
//===============================================================================================================================
KeyState Keyboard::GetKeyState(Key key)
{
	return keyStates[key];
}
//===============================================================================================================================
bool Keyboard::IsKeyDown(Key key)
{
    return keyStates[key].Pressed;
}
//===============================================================================================================================
bool Keyboard::RisingEdge(Key key)
{
    return keyStates[key].RisingEdge;
}
//===============================================================================================================================
Keyboard* Keyboard::GetKeyboard()
{
	if (prevState == NULL)
		prevState = new Keyboard();
	
	::GetKeyboardState(currState);

	KeyState state;
	for (UINT i = 0; i < 256; ++i)
	{
		state.Pressed = KeyPressed(currState[i]);
		state.RisingEdge = state.Pressed && !prevState->keyStates[i].Pressed;
		state.FallingEdge = !state.Pressed && prevState->keyStates[i].Pressed;
		prevState->keyStates[i] = state;
	}

	return prevState;
}
//===============================================================================================================================
KeyState::KeyState()
:	Pressed(false)
,	RisingEdge(false)
,	FallingEdge(false)
{
}
//===============================================================================================================================