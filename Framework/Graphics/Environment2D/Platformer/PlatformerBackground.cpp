#include "PlatformerBackground.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerBackground::PlatformerBackground(D3D* d3d, int layers)
{
	m_D3DSystem = d3d;
	m_EngineOptions = d3d->GetEngineOptions();
	backgroundScreenWidth = 1600;
	numLayers = layers;
}
//==================================================================================================================================
void PlatformerBackground::Clear()
{
	if (backgrounds.size() > 0) backgrounds.clear();
}
//==================================================================================================================================
void PlatformerBackground::AddBackground(string basePath, string backgroundName, int width, int height, bool useParallaxScrolling, bool stayStill, int backWidth, float dx)
{
	BackgroundInfo bi;

	Sprite* spr = new Sprite(m_D3DSystem);
	spr->Initialize(basePath, backgroundName, XMFLOAT3(0, 0, 0), width, height, STATIC);
	
	bi.backgroundImage = spr;
	//Start out at 0 pos
	bi.x = 0.0f;
	//X Velocity for background
	bi.dx = dx;
	//Does this background use parallax scrolling
	bi.useParallaxScrolling = useParallaxScrolling;
	//Should this background move on its own
	bi.stayStill = stayStill;
	
	bi.backgroundScreenWidth = backWidth;
	
	backgrounds.push_back(bi);
}
//==================================================================================================================================
int PlatformerBackground::BackgroundWidth()
{
	return backgroundScreenWidth;
}
//==================================================================================================================================
void PlatformerBackground::Update()
{
	for (int i = 0; i < numLayers; i++)
	{
		if (!backgrounds[i].stayStill)
		{
			float x = backgrounds[i].x;

			if (x > m_EngineOptions->m_screenWidth * -1)
			{
				x -= backgrounds[i].dx;
			}
			else
			{
				x = 0;
			}

			backgrounds[i].x = x;
		}
	}
}
//==================================================================================================================================
void PlatformerBackground::Render(Camera* camera, int mapWidth, int offsetX)
{
	int screenWidth = m_EngineOptions->m_screenWidth;
	int screenHeight = m_EngineOptions->m_screenHeight;
	
	for (int i = 0; i < numLayers; i++)
	{
		backgroundScreenWidth = backgrounds[i].backgroundScreenWidth;
		
		/*
		* Parallax Scrolling
		* Main Layer: mainLayer.x = -cameraX;
		* Middle Layer: middleLayer.x = -cameraX * 0.5;
		* Far Layer: farLayer.x = -cameraX * 0.2;
		*/
		if (backgrounds[i].useParallaxScrolling)
		{
			//float x = (offsetX * (screenWidth - backgrounds[i].backgroundImage->Width())) / (screenWidth - mapWidth);
			//float y = screenHeight - backgrounds[i].backgroundImage->Height();
			
			float levelExtentX = static_cast<float>(screenWidth * backgrounds[i].backgroundScreenWidth);
			
			float scrollPc = -offsetX / (levelExtentX - screenWidth);
			
			float x = (backgrounds[i].backgroundImage->Width() - screenWidth) * scrollPc;
			float y = screenHeight - backgrounds[i].backgroundImage->Height();
			
			backgrounds[i].backgroundImage->TopLeftPosition() = XMFLOAT3(x, y, 0);
			backgrounds[i].backgroundImage->Render(camera);
		}
		else
		{
			// Moving background
			if (!backgrounds[i].stayStill)
			{
				backgrounds[i].backgroundImage->TopLeftPosition() = XMFLOAT3(backgrounds[i].x, 0, 0);
				backgrounds[i].backgroundImage->Render(camera);
				
				backgrounds[i].backgroundImage->TopLeftPosition() = XMFLOAT3(backgrounds[i].x + screenWidth, 0, 0);
				backgrounds[i].backgroundImage->Render(camera);
			}
			else// Background does not move
			{
				backgrounds[i].backgroundImage->TopLeftPosition() = XMFLOAT3(backgrounds[i].x, 0, 0);
				backgrounds[i].backgroundImage->Render(camera);
			}
		}
	}
}
//==================================================================================================================================