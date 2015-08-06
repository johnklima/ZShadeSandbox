#include "WinListbox.h"
#include "Windowsx.h"
using namespace GUIComponents;
//--------------------------------------------------------------------------------------
WinListbox::WinListbox(HWND hWnd, int x, int y, int width, int height, int id)
{
	m_childID = 0;

	m_id = id;
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "",
		WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
		x, y,
		width, height,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
WinListbox::WinListbox(const WinListbox& o)
{
}
//--------------------------------------------------------------------------------------
WinListbox::~WinListbox()
{
}
//--------------------------------------------------------------------------------------
void WinListbox::Init(HWND hWnd, int x, int y, int width, int height, int id)
{
	m_childID = 0;

	m_id = id;
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "",
		WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
		x, y,
		width, height,
		hWnd,
		(HMENU)id,
		GetModuleHandle(NULL),
		NULL);
}
//--------------------------------------------------------------------------------------
void WinListbox::AddItem(string text)
{
	m_childID++;
	SendMessage(hList, LB_ADDSTRING, m_childID, (LPARAM)text.c_str());
}
//--------------------------------------------------------------------------------------
string WinListbox::GetItem(WPARAM wParam)
{
	int iSelectedIndex;
	TCHAR strSelectedItem[32];
	
	iSelectedIndex = ListBox_GetCurSel(hList);
	ListBox_GetText(hList, iSelectedIndex, (LPCTSTR)strSelectedItem);

	//MessageBox(NULL, strSelectedItem, "Exercise", 0);
	
	return strSelectedItem;
	//switch (HIWORD(wParam))
	//{
	//	case LBN_DBLCLK:
	//	{
	//		int iSelectedIndex;
	//		TCHAR strSelectedItem[32];
	//		
	//		iSelectedIndex = ListBox_GetCurSel(hList);
	//		ListBox_GetText(hList, iSelectedIndex, (LPCTSTR)strSelectedItem);

	//		//MessageBox(NULL, strSelectedItem, "Exercise", 0);
	//		
	//		return strSelectedItem;
	//	}
	//}

	//return "";
}
//--------------------------------------------------------------------------------------
void WinListbox::DeleteList()
{
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	
	//for (int i = 0; i < m_childID; i++)
	//{
	//	SendMessage(hList, LB_DELETESTRING, i, 0);
	//}
}
//--------------------------------------------------------------------------------------
void WinListbox::DeleteItem(WPARAM wParam)
{
	//switch (HIWORD(wParam))
	//{
	//	case LBN_DBLCLK:
	//	{
	//		// Getting the index of the item that was double-clicked
	//		int iSelectedIndex;
	//			
	//		iSelectedIndex = ListBox_GetCurSel(hList);

	//		// Identifying the string that was double-clicked
	//		/*
	//		TCHAR strSelectedItem[32];
	//		ListBox_GetText(lbxPrimaryFamily,
	//					iSelectedIndex,
	//					(LPCTSTR)strSelectedItem);

	//		MessageBox(NULL, strSelectedItem, L"Exercise", 0);*/

	//		// Deleting the item that was double-clicked
	//		ListBox_DeleteString(hList, iSelectedIndex);
	//	}
	//}
}
//--------------------------------------------------------------------------------------
void WinListbox::SetVisible(bool bVisable)
{
	ShowWindow(hList, bVisable);
}
//--------------------------------------------------------------------------------------
bool WinListbox::IsVisible()
{
	return IsWindowVisible(hList);
}
//--------------------------------------------------------------------------------------
void WinListbox::Enable(bool bEnable)
{
	EnableWindow(hList, bEnable);
}
//--------------------------------------------------------------------------------------
bool WinListbox::IsEnabled()
{
	return IsWindowEnabled(hList);
}
//--------------------------------------------------------------------------------------