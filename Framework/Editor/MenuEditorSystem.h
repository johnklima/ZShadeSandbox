//================================================================================================================
// MenuEditorSystem.h
//
// Includes everything needed for the menu editor
//================================================================================================================
// History
//
// Created on 2/24/2015 by Dustin Watson
//================================================================================================================
#ifndef __MENUEDITORSYSTEM_H
#define __MENUEDITORSYSTEM_H
//================================================================================================================
//================================================================================================================
//#include "EditorEnvironment.h"
#include "Sprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "Environment.h"
#include "Environment2D.h"
#include "Environment3D.h"
#include "QuadMesh.h"
//================================================================================================================
class MenuEditorSystem
{
	typedef enum
	{
		ET_None = 0,
		ET_Button = 1,
		ET_Text = 2
	} EEditType;
	
	// Actions to take for a button or text item
	typedef enum
	{
		A_Select = 0,
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

	MenuEditorSystem(ZShadeSandboxEnvironment::Environment* env);
	~MenuEditorSystem();

	void Init();

public:
	
	void LoadAMenu(string menu_filename, string& menuName);
	void SaveMenu(string menu_filename);
	void CreateMenu(string menu_name);
	void SetActiveRenderedMenu(string menu_name);
	void RemoveMenu(string menu_name);
	void GrabMenuNames(vector<string>& names);
	
	void CalculateSelectedMousePosition(int x, int y);
	
	void AddButton();
	void AddText();
	void AddMenuBackground();
	void AddMenuNameToButton(string menu_name);
	void MoveButton(float x, float y);
	bool MoveButtonStart();
	void MoveButtonEnd();
	bool CloneButtonStart();
	void CloneButtonEnd();
	void DeleteButton();
	bool ResizeButtonStart();
	void ResizeButtonUpdate(
		bool updatePosition,
		bool updateSize,
		bool goFast,
		bool pressedLeftKey,
		bool pressedRightKey,
		bool pressedUpKey,
		bool pressedDownKey
	);
	void HighlightButton(float x, float y);
	void MoveText(float x, float y);
	bool MoveTextStart();
	void MoveTextEnd();
	bool CloneTextStart();
	void CloneTextEnd();
	void DeleteText();
	void HighlightText(float x, float y);
	void UpdateDisplaySprite(float x, float y);
	void SelectModeButtonClicked(float x, float y);
	void SelectModeTextClicked(float x, float y);
	
	// Update the text in the title bar
	string UpdateTitleBarText();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	void Render(Camera* camera);
	
	void SwitchToEditTypeNone();
	void SwitchToEditTypeButton();
	void SwitchToEditTypeText();
	void SwitchToActionSelect();
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
	void SetGD2D(GameDirectory2D* gd) { m_mainGameDirectory2D = gd; }
	GameDirectory3D* GetGD3D() { return m_mainGameDirectory3D; }
	void SetGD3D(GameDirectory3D* gd) { m_mainGameDirectory3D = gd; }
	D3D* D3DSystem() { return m_D3DSystem; }

	void UpdateInput(Keyboard* keyboard);

	string& SelectedButtonImageName() { return m_SelectedButtonImageName; }
	string SelectedButtonImageName() const { return m_SelectedButtonImageName; }

	string& ActiveMenuName() { return m_ActiveMenuName; }
	string ActiveMenuName() const { return m_ActiveMenuName; }
	
	bool& WasButtonOrTextClicked() { return bWasClicked; }
	bool WasButtonOrTextClicked() const { return bWasClicked; }
	
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
	
	int m_selectModeButtonID;
	int m_selectModeTextID;
	int m_originalButtonID;
	int m_originalTextID;
	bool m_button_cache_selection_changed;
	bool m_resizing_button;
	bool m_moving_button;
	bool m_cloning_button;
	bool m_moving_text;
	bool m_cloning_text;
	bool m_selected_button_cover;
	bool m_selected_text_cover;
	bool m_display_selected_button_box;
	bool m_display_selected_text_box;
	bool m_display_button_box_created;
	bool m_display_text_box_created;
	ZShadeSandboxGraphics::Button* m_button_with_box;
	ZShadeSandboxGraphics::Text* m_text_with_box;
	Sprite* m_sprite_box; //outline box for a sprite in sprite edit mode
	Sprite* m_select_sprite_box;
	//AISprite* m_spriteBeforeMove;
	ZShadeSandboxGraphics::Button* m_MovingButtonSprite;
	ZShadeSandboxGraphics::Button* m_SelectedButtonSprite;
	ZShadeSandboxGraphics::Button* m_EditSizeButtonSprite;
	ZShadeSandboxGraphics::Text* m_MovingTextSprite;
	ZShadeSandboxGraphics::Text* m_SelectedTextSprite;
	Sprite* m_displaySpriteCover;// Display cover for a button image
	Sprite* m_cloneDisplaySpriteCover;
	ZShadeSandboxGraphics::Text* m_cloneDisplayTextSprite;
	
	// This flag is to update the attributes of a button/text in select mode
	bool bWasClicked;
	
	string m_ActiveMenuName;
	string m_BackgroundImageName;
	string m_SelectedButtonImageName;
	
	//Controls how fast the camera will move when transforming
	float m_camera_speed;
	
	//When the user makes a change to the menu then this flag becomes true
	bool m_Updated;
	
	// Mouse position at start of drag operation
	int dragStartX, dragStartY;

	// Mouse position in previous frame of drag operation
	int dragPrevX, dragPrevY;
	
	// X and Y grid square size
	int gridSizeX, gridSizeY;
	
	XMFLOAT2 m_SelectedMousePos;
	XMFLOAT2 m_MouseMovePos;
	ZShadeSandboxGraphics::Text mousePosText;
	XMMATRIX transform;
	
	EEditType editType;
	EActions action;
	EGridMode gridMode;
	
	bool stampPressed;

	//Is the menu created
	bool m_MenuCreated;
	
	bool usingGrid;
	
	bool m_startDelayTimer;
	
	SleepTimer* m_ClickDelayTimer;
	SleepTimer* m_SelectionTimer;
};
//================================================================================================================
#endif//__MENUEDITORSYSTEM_H