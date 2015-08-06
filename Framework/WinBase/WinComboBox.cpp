#include "WinComboBox.h"
#include "BetterString.h"
using namespace GUIComponents;
//-----------------------------------------------------------------------------------------
WinComboBox::WinComboBox(HWND hwnd, int xpos, int ypos, int width, int height, int id)
	: mXPos(xpos), mYPos(ypos), mWidth(width), mHeight(height), mParentWnd(hwnd), mChanged(false)
{
	//Create the combo box

	//Types
	//CBS_SIMPLE
	//CBS_DROPDOWN
	//CBS_DROPDOWNLIST

	mSelectedIndex = -1;
	mSelectedItem = "";
	mItemID = 0;
	mMyID = id;

	mComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VSCROLL | WS_VISIBLE,
		mXPos, mYPos, mWidth, mHeight, mParentWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
}
//-----------------------------------------------------------------------------------------
WinComboBox::~WinComboBox()
{

}
//-----------------------------------------------------------------------------------------
void WinComboBox::Init(HWND hwnd, int xpos, int ypos, int width, int height, int id)
{
	//Create the combo box

	//Types
	//CBS_SIMPLE
	//CBS_DROPDOWN
	//CBS_DROPDOWNLIST

	mXPos = xpos;
	mYPos = ypos;
	mWidth = width;
	mHeight = height;
	mParentWnd = hwnd;
	mChanged = false;

	mSelectedIndex = -1;
	mSelectedItem = "";
	mItemID = 0;
	mMyID = id;

	mComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VSCROLL | WS_VISIBLE,
		mXPos, mYPos, mWidth, mHeight, mParentWnd, (HMENU)id, GetModuleHandle(NULL), NULL);
}
//-----------------------------------------------------------------------------------------
void WinComboBox::AddItem(string item)
{
	//Add string to combobox
	SendMessage(mComboBox, (UINT)CB_ADDSTRING, (WPARAM)mItemID++, (LPARAM)item.c_str());
}
//-----------------------------------------------------------------------------------------
void WinComboBox::SetDisplayItem(string text, int id)
{
	mSelectedItem = text;
	mSelectedIndex = id;

	//Send the CB_SETCURSEL message to display an initial item
	// in the selection field
	SendMessage(mComboBox, CB_SETCURSEL, (WPARAM)id, (LPARAM)0);
}
//-----------------------------------------------------------------------------------------
void WinComboBox::Reset()
{
	//SendDlgItemMessage(hDlg, idCombo, CB_RESETCONTENT, 0, 0);
	SendMessage(mComboBox, CB_RESETCONTENT, 0, 0);
}
//-----------------------------------------------------------------------------------------
void WinComboBox::RemoveItem(string text)
{
	if (BetterString(mSelectedItem) == BetterString(text))
	{
		mSelectedItem = "";
		
		SendMessage(mComboBox, CB_DELETESTRING, mSelectedIndex, 0);

		mSelectedIndex = -1;
	}
}
//-----------------------------------------------------------------------------------------
void WinComboBox::UpdateCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == mMyID)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			TCHAR ListItem[256];
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
			mSelectedIndex = ItemIndex;
			mSelectedItem = ListItem;
			mChanged = true;
			//MessageBox(hwnd, (LPCSTR)ListItem, TEXT("Item Selected"), MB_OK);
		}
	}
}
//-----------------------------------------------------------------------------------------
void WinComboBox::SetVisible(bool bVisable)
{
	ShowWindow(mComboBox, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinComboBox::IsVisible()
{
	return IsWindowVisible(mComboBox);
}
//--------------------------------------------------------------------------------------
void WinComboBox::Enable(bool bEnable)
{
	EnableWindow(mComboBox, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinComboBox::IsEnabled()
{
	return IsWindowEnabled(mComboBox);
}
//--------------------------------------------------------------------------------------