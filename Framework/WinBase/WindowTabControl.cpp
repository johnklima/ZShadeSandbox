#include "WindowTabControl.h"
#include "ZShadeMessageCenter.h"
//================================================================================================================
WindowTabControl::WindowTabControl(HWND parent)
{
	Init(parent);
}
//================================================================================================================
WindowTabControl::~WindowTabControl()
{
}
//================================================================================================================
bool WindowTabControl::Init(HWND parent)
{
	RECT rcClient;
	DWORD flags;
	
	// Get dimensions of the parent window's client area
	GetClientRect(parent, &rcClient);
	
	flags = WS_CHILD | WS_VISIBLE;
	
	m_Hwnd = CreateWindow(WC_TABCONTROL, "", flags,
		0, 0, rcClient.right, rcClient.bottom,
		parent, NULL, GetModuleHandle(NULL), NULL);
	
	if (m_Hwnd == NULL)
	{
		ZShadeMessageCenter::MsgBoxError(parent, "Failed to create tab control window");
		return false;
	}
	
	m_TabIndex = 0;
	
	//Change tab font
	SendMessage(m_Hwnd, WM_SETFONT,
		(WPARAM) GetStockObject(DEFAULT_GUI_FONT), 0);
	
	return true;
}
//================================================================================================================
bool WindowTabControl::InsertTab(LPSTR name)
{
	TCITEM tie;
	tie.mask = TCIF_TEXT;
	tie.iImage = -1;
	tie.pszText = name;
	
	if (TabCtrl_InsertItem(m_Hwnd, m_TabIndex, &tie) == -1)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "Failed to insert tab in tab control window");
		return false;
	}
	
	m_TabIndex++;

	return true;
}
//================================================================================================================
void WindowTabControl::SelectTab(int id)
{
	TabCtrl_SetCurSel(m_Hwnd, id);
}
//================================================================================================================
int WindowTabControl::TabID()
{
	return TabCtrl_GetCurSel(m_Hwnd);
}
//================================================================================================================
void WindowTabControl::SetVisible(bool bVisable)
{
	ShowWindow(m_Hwnd, bVisable);
}
//================================================================================================================
bool WindowTabControl::IsVisible()
{
	return IsWindowVisible(m_Hwnd);
}
//================================================================================================================
void WindowTabControl::Enable(bool bEnable)
{
	EnableWindow(m_Hwnd, bEnable);
}
//================================================================================================================
bool WindowTabControl::IsEnabled()
{
	return IsWindowEnabled(m_Hwnd);
}
//================================================================================================================