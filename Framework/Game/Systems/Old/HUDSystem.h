//*************************************************************************************************************************
// HUDSystem.h
//*************************************************************************************************************************
#ifndef __HUDSYSTEM_H
#define __HUDSYSTEM_H
//================================================================================================================
//================================================================================================================
//Includes
#include <string>
#include <map>
#include "HUD.h"
class Keyboard;
using namespace std;
//================================================================================================================
//================================================================================================================
class HUDSystem
{
public:
	
	HUDSystem(D3D* d3d);
	~HUDSystem();
	
	
	
	void RemoveImage(ZShadeSandboxGraphics::Image* im);
	void RemoveText(ZShadeSandboxGraphics::Text* t);
	
	bool HUDImageSelected(ZShadeSandboxGraphics::Image*& im, int x, int y);
	bool HUDTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y);
	
	void AddHUD(HUD* hud);
	
	void AddHUDImage(ZShadeSandboxGraphics::Image* image);
	void AddHUDText(ZShadeSandboxGraphics::Text* text);
	
	//Might need, do not know
	void UpdateHUD(Keyboard keyboard, POINT pt);
	
	void Render(Camera* camera);
	
public:
	
	vector<ZShadeSandboxGraphics::Image*> GetImages();
	vector<ZShadeSandboxGraphics::Text*> GetTexts();
	HUD* GetHUD() { return m_hud; }
	int& Width() { return m_hud->GetWidth(); }
	int Width() const { return m_hud->GetWidth(); }
	int& Height() { return m_hud->GetHeight(); }
	int Height() const { return m_hud->GetHeight(); }
	
private:
	
	D3D* m_D3DSystem;
	HUD* m_hud;
};
//================================================================================================================
//================================================================================================================
#endif//__HUDSYSTEM_H