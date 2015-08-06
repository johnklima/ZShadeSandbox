//================================================================================================================
// HUDEditorSystem.h
//
// Includes everything needed for the HUD editor
//================================================================================================================
// History
//
// Created on 3/4/2015 by Dustin Watson
//================================================================================================================
#ifndef __HUDEDITORSYSTEM_H
#define __HUDEDITORSYSTEM_H
//================================================================================================================
//================================================================================================================
//#include "EditorEnvironment.h"
#include "Sprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "Environment.h"
#include "Environment2D.h"
#include "Environment3D.h"
//================================================================================================================
class HUDEditorSystem
{
	typedef enum
	{
		ET_None = 0,
		ET_Image = 1,
		ET_Text = 2
	} EEditType;
	
	// Actions to take for a image or text item
	typedef enum
	{
		A_None = 0,
		A_Move = 1,
		A_Delete = 2,
		A_Place = 3,
		A_Clone = 4,
		A_EditSize = 5
	} EActions;
	
	typedef enum
	{
		GM_Free = 0,
		GM_Snap = 1
	} EGridMode;
	
public:
	
	HUDEditorSystem(ZShadeSandboxEnvironment::Environment* env);
	~HUDEditorSystem();

	void Init();

public:
	
	//void AddImage();
	//void AddText();
	
	void LoadHUD(string hud_filename, string& hud_name);
	void SaveHUD(string hud_filename);
	void CreateHUD(string hud_name);
	
	void CalculateSelectedMousePosition(int x, int y);
	
	void AddImage();
	void AddText();
	void MoveImage(float x, float y);
	bool MoveImageStart();
	void MoveImageEnd();
	bool CloneImageStart();
	void CloneImageEnd();
	void DeleteImage();
	bool ResizeImageStart();
	void ResizeImageUpdate(
		bool updatePosition,
		bool updateSize,
		bool goFast,
		bool pressedLeftKey,
		bool pressedRightKey,
		bool pressedUpKey,
		bool pressedDownKey
	);
	void HighlightImage(float x, float y);
	void MoveText(float x, float y);
	bool MoveTextStart();
	void MoveTextEnd();
	bool CloneTextStart();
	void CloneTextEnd();
	void DeleteText();
	void HighlightText(float x, float y);
	void UpdateDisplaySprite(float x, float y);
	
	// Update the text in the title bar
	string UpdateTitleBarText();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	void Render(Camera* camera);
	
	void SwitchToEditTypeNone();
	void SwitchToEditTypeImage();
	void SwitchToEditTypeText();
	void SwitchToActionNone();
	void SwitchToActionMove();
	void SwitchToActionDelete();
	void SwitchToActionPlace();
	void SwitchToActionClone();
	void SwitchToActionEditSize();
	void SwitchToGridModeFree();
	void SwitchToGridModeSnap();

	XMFLOAT2 SnapToGrid(int x, int y);
	void LoadRequiredSprites();
	void SetGameDirectory2D(GameDirectory2D* gd);
	void SetGameDirectory3D(GameDirectory3D* gd);
	void GoFullscreen(bool bEnable);
	GameDirectory2D* GetGD2D() { return m_mainGameDirectory2D; }
	GameDirectory3D* GetGD3D() { return m_mainGameDirectory3D; }
	D3D* D3DSystem() { return m_D3DSystem; }
	
	void UpdateInput(Keyboard* keyboard);
	
	string& SelectedImageImageName() { return m_SelectedImageImageName; }
	string SelectedImageImageName() const { return m_SelectedImageImageName; }
	
private:
	
	Environment2D* m_Env2D;
	Environment3D* m_Env3D;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	GameDirectory2D* m_mainGameDirectory2D;
	GameDirectory3D* m_mainGameDirectory3D;
	
	// This is the main stamp tool to select a tile
	Sprite* m_StampNormal;
	// The stamp tool that shows when the left mouse button is clicked
	Sprite* m_StampHighlight;

	int m_originalImageID;
	int m_originalTextID;
	bool m_image_cache_selection_changed;
	bool m_resizing_image;
	bool m_moving_image;
	bool m_cloning_image;
	bool m_moving_text;
	bool m_cloning_text;
	bool m_selected_image_cover;
	bool m_selected_text_cover;
	bool m_display_image_box_created;
	bool m_display_text_box_created;
	ZShadeSandboxGraphics::Image* m_image_with_box;
	ZShadeSandboxGraphics::Text* m_text_with_box;
	Sprite* m_sprite_box; //outline box for a sprite in sprite edit mode
	//AISprite* m_spriteBeforeMove;
	ZShadeSandboxGraphics::Image* m_MovingImageSprite;
	ZShadeSandboxGraphics::Image* m_SelectedImageSprite;
	ZShadeSandboxGraphics::Image* m_EditSizeImageSprite;
	ZShadeSandboxGraphics::Text* m_MovingTextSprite;
	ZShadeSandboxGraphics::Text* m_SelectedTextSprite;
	Sprite* m_displaySpriteCover;// Display cover for a button image
	Sprite* m_cloneDisplaySpriteCover;
	ZShadeSandboxGraphics::Text* m_cloneDisplayTextSprite;
	
	string m_SelectedImageImageName;
	
	//Controls how fast the camera will move when transforming
	float m_camera_speed;
	
	//When the user makes a change to the hud then this flag becomes true
	bool m_Updated;
	
	// X and Y grid square size
	int gridSizeX, gridSizeY;
	
	XMFLOAT2 m_SelectedMousePos;
	
	EEditType editType;
	EActions action;
	EGridMode gridMode;
	
	bool stampPressed;

	//Is the hud created
	bool m_HUDCreated;
	
	bool usingGrid;
	
	bool m_startDelayTimer;
	
	SleepTimer* m_ClickDelayTimer;
	SleepTimer* m_SelectionTimer;
};
//================================================================================================================
#endif//__HUDEDITORSYSTEM_H