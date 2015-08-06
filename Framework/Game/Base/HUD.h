//*************************************************************************************************************************
// HUD.h
//*************************************************************************************************************************
#ifndef __HUD_H
#define __HUD_H
//================================================================================================================
//================================================================================================================
#include <vector>
#include "Image.h"
#include "Text.h"
//================================================================================================================
//The HUD will contain images and text
class HUD
{
public:
	
	HUD(D3D* d3d);
	~HUD();
	
	void AddImage(string base_path, string textureName, int x, int y, int w, int h);
	void AddText(string text, int x, int y);
	void AddImage(ZShadeSandboxGraphics::Image* im);
	void AddText(ZShadeSandboxGraphics::Text* t);
	void RemoveImage(ZShadeSandboxGraphics::Image* im);
	void RemoveText(ZShadeSandboxGraphics::Text* t);
	void RemoveImage(int id);
	void RemoveText(int id);
	bool ImageClicked(ZShadeSandboxGraphics::Image*& im, int x, int y);
	bool TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y);
	void MoveImage(ZShadeSandboxGraphics::Image*& im, int x, int y);
	void MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y);
	
	void Render(Camera* camera, int blendAmount);
	
public:
	
	vector<ZShadeSandboxGraphics::Image*> GetImages() { return m_images; }
	vector<ZShadeSandboxGraphics::Text*> GetTexts() { return m_texts; }
	int& Width() { return m_width; }
	int Width() const { return m_width; }
	int& Height() { return m_height; }
	int Height() const { return m_height; }
	
private:
	
	D3D* m_D3DSystem;
	
	vector<ZShadeSandboxGraphics::Image*> m_images;
	vector<ZShadeSandboxGraphics::Text*> m_texts;
	
	int mImageID;
	int mTextID;
	int m_width;
	int m_height;
};
//================================================================================================================
//================================================================================================================
#endif//__HUD_H