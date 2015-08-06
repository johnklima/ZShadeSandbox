#ifndef __WINCOMBOBOX_H
#define __WINCOMBOBOX_H
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
using namespace std;
//-----------------------------------------------------------------------------------------
namespace GUIComponents {
//-----------------------------------------------------------------------------------------
class WinComboBox
{
public:
	WinComboBox() {}
	WinComboBox(HWND hwnd, int xpos, int ypos, int width, int height, int id);
	~WinComboBox();

	void Init(HWND hwnd, int xpos, int ypos, int width, int height, int id);

	void AddItem(string item);

	void UpdateCommand(WPARAM wParam, LPARAM lParam);

	int    GetSelectedIndex() { return mSelectedIndex; }
	string GetSelectedItem() { return mSelectedItem; }

	void SetDisplayItem(string text, int id);

	void RemoveItem(string text);

	HWND GetHwnd() { return mComboBox; }

	bool IsChanged() { return mChanged; }
	void SetChanged(bool set) { mChanged = set; }

	void Reset();

	void SetVisible(bool bVisable);
	bool IsVisible();

	void Enable(bool bEnable);
	bool IsEnabled();

private:
	int mItemID;
	int mMyID;
	bool mChanged;
	int mXPos;       //Horizontal position of the ComboBox
	int mYPos;       //Vertical position of the ComboBox
	int mWidth;      //Width of the ComboBox
	int mHeight;     //Height of the ComboBox
	HWND mParentWnd; //Parent window handle
	HWND mComboBox;  //handle to the actual ComboBox
	string mSelectedItem; //Item that was selected
	int mSelectedIndex;   //Item index that was selected
};
//-----------------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------------
#endif//__COMBOBOX_H