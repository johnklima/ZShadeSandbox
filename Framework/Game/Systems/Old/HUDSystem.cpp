#include "HUDSystem.h"
#include "Input.h"
//--------------------------------------------------------------------------------------------------------------------
HUDSystem::HUDSystem(D3D* d3d)
:   m_D3DSystem(d3d)
{
	m_hud = new HUD(d3d);
}
//--------------------------------------------------------------------------------------------------------------------
HUDSystem::HUDSystem(const HUDSystem& o)
{
}
//--------------------------------------------------------------------------------------------------------------------
HUDSystem::~HUDSystem()
{
}
//--------------------------------------------------------------------------------------------------------------------
vector<ZShadeSandboxGraphics::Image*> HUDSystem::GetImagess()
{
	return m_hud->GetImages();
}
//--------------------------------------------------------------------------------------------------------------------
vector<ZShadeSandboxGraphics::Text*> HUDSystem::GetTexts()
{
	return m_hud->GetTexts();
}
//--------------------------------------------------------------------------------------------------------------------
//void HUDSystem::ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo)
//{
//	m_hud->ResetComponentsToWindow(d3d, hwnd, gd, eo);
//}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::RemoveImage(ZShadeSandboxGraphics::Image* im)
{
	m_hud->RemoveImage(im);
}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::RemoveText(ZShadeSandboxGraphics::Text* t)
{
	m_hud->RemoveText(t);
}
//--------------------------------------------------------------------------------------------------------------------
bool HUDSystem::HUDImageSelected(ZShadeSandboxGraphics::Image*& im, int x, int y)
{
	return m_hud->HUDImageSelected(im, x, y);
}
//--------------------------------------------------------------------------------------------------------------------
bool HUDSystem::HUDTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	return m_hud->HUDTextSelected(text, x, y);
}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::AddHUD(HUD* hud)
{
	m_hud = hud;
}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::AddHUDImage(ZShadeSandboxGraphics::Image* image)
{
	m_hud->AddImage(image);
}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::AddHUDText(ZShadeSandboxGraphics::Text* text)
{
	m_hud->AddText(text);
}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::UpdateHUD(Keyboard keyboard, POINT pt)
{
}
//--------------------------------------------------------------------------------------------------------------------
//void HUDSystem::Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
//{
//	m_hud->Draw10(true, textureShader, transparentShader, camera);
//}
//--------------------------------------------------------------------------------------------------------------------
void HUDSystem::Render(Camera* camera)
{
	m_hud->Draw(true, camera);
}
//--------------------------------------------------------------------------------------------------------------------