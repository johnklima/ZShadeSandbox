#include "GDI.h"
#define ULONG_PTR ULONG
#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")
//===============================================================================================================================
//===============================================================================================================================
static GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR GDI::gdiplusToken = 0;
//===============================================================================================================================
//===============================================================================================================================
GDI::GDI()
{
}
//===============================================================================================================================
GDI::~GDI()
{
}
//===============================================================================================================================
void GDI::Init()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
//===============================================================================================================================
void GDI::Shutdown()
{
	GdiplusShutdown(gdiplusToken);
}
//===============================================================================================================================
void GDI::DrawRectangle(HWND hwnd, RGBColor::Color color, int x, int y, int width, int height)
{
	PAINTSTRUCT ps;
	Gdiplus::Color c;
	c.SetValue(color.GetValue());
	HDC hdc = BeginPaint(hwnd, &ps);
	Graphics g(hdc);
	SolidBrush brush(c);
	g.FillRectangle(&brush, x, y, width, height);
	EndPaint(hwnd, &ps);
}
//===============================================================================================================================
void GDI::DrawEllipse(HWND hwnd, RGBColor::Color color, int x, int y, int width, int height)
{
	PAINTSTRUCT ps;
	Gdiplus::Color c;
	c.SetValue(color.GetValue());
	HDC hdc = BeginPaint(hwnd, &ps);
	Graphics g(hdc);
	Pen pen(c, 2.0);
	g.DrawEllipse(&pen, x, y, width, height);
	EndPaint(hwnd, &ps);
}
//===============================================================================================================================
void GDI::DrawImage(HWND hwnd, const char* imageName, int x, int y, int width, int height)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	Graphics g(hdc);
	unique_ptr<wchar_t> name = BetterString(imageName).ToWideStr();
	Rect rect(x, y, width, height);
	Image image(name.get());
	g.DrawImage(&image, rect);
	EndPaint(hwnd, &ps);
}
//===============================================================================================================================