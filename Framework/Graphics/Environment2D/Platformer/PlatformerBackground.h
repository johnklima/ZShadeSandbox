//==================================================================================================================================
// PlatformerBackground.h
//
//==================================================================================================================================
// History
//
// -Created on 1/19/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERBACKGROUND_H
#define __PLATFORMERBACKGROUND_H
//==================================================================================================================================
//==================================================================================================================================
#include <vector>
#include "D3D.h"
#include "ZMath.h"
#include "Sprite.h"
//==================================================================================================================================
struct BackgroundInfo
{
	Sprite* backgroundImage;
	float x;
	float dx;
	boolean useParallaxScrolling;
	boolean stayStill;
	
	// The amount of screens the background takes up
	int backgroundScreenWidth;
};
//==================================================================================================================================
class PlatformerBackground
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	vector<BackgroundInfo> backgrounds;
	
	int backgroundScreenWidth;
	
	//Will have the ability to have a multiple backgrounds
	//for paralax scrolling ability
	//etc... background with foregrounds
	int numLayers;
	
public:
	
	PlatformerBackground() {}
	PlatformerBackground(D3D* d3d, int layers=1);
	
	int BackgroundWidth();
	
	void Clear();
	
	void AddBackground(string basePath, string backgroundName, int width, int height, bool useParallaxScrolling, bool stayStill, int backWidth, float dx);

	void Update();

	void Render(Camera* camera, int mapWidth, int offsetX);
	
	void Clone(PlatformerBackground* c) {}
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERBACKGROUND_H