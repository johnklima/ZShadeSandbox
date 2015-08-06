#include "MenuEditorSystem.h"
#include "Convert.h"
#include "EnvironmentMenuHelper.h"
#include "Button.h"
//================================================================================================================
MenuEditorSystem::MenuEditorSystem(ZShadeSandboxEnvironment::Environment* env)
:	m_Env2D(env->Env2D())
,	m_Env3D(env->Env3D())
{
	m_D3DSystem = m_Env2D->GetD3D();
	m_EngineOptions = m_Env2D->GetEngineOptions();

	switch (m_EngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			m_mainGameDirectory2D = m_Env2D->GetGD2D();
		}
		break;
		case ZSHADE_3D:
		{
			m_mainGameDirectory3D = m_Env3D->GetGD3D();
		}
		break;
	}
	
	m_ClickDelayTimer = new SleepTimer();
	m_SelectionTimer = new SleepTimer();
	
	m_BackgroundImageName = "";
	m_SelectedButtonImageName = "";
	m_displaySpriteCover = 0;
	m_cloneDisplaySpriteCover = 0;
	m_MovingButtonSprite = 0;
	m_MovingTextSprite = 0;
	m_originalButtonID = -1;
	m_originalTextID = -1;
	m_button_cache_selection_changed = false;
	m_resizing_button = false;
	m_moving_button = false;
	m_cloning_button = false;
	m_moving_text = false;
	m_cloning_text = false;
	m_selected_button_cover = false;
	m_selected_text_cover = false;
	m_display_button_box_created = false;
	m_button_with_box = 0;
	m_text_with_box = 0;
	m_sprite_box = 0;
	m_SelectedButtonSprite = 0;
	m_EditSizeButtonSprite = 0;
	m_SelectedTextSprite = 0;
	m_ActiveMenuName = "";
	
	// Set defaults
	SwitchToEditTypeNone();
	
	m_ActiveMenuName = "MainMenu";

	usingGrid = false;
	gridSizeX = 80;
	gridSizeY = 20;

	stampPressed = false;

	mousePosText.SetD3D(m_D3DSystem);
	mousePosText.Initialize();
	mousePosText.CreateShadow();
	mousePosText.ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
	mousePosText.TextName() = "I am a Sprite";
	mousePosText.TextColor() = XMFLOAT4(1, 0, 0, 1);
}
//================================================================================================================
MenuEditorSystem::~MenuEditorSystem()
{
}
//================================================================================================================
void MenuEditorSystem::GoFullscreen(bool bEnable)
{
	//m_Env2D->GoFullscreen(bEnable);
}
//================================================================================================================
void MenuEditorSystem::LoadRequiredSprites()
{
	string base_path = m_mainGameDirectory2D->m_required_textures;
	m_StampNormal = new Sprite(m_D3DSystem);
	m_StampNormal->Initialize(base_path, "stamp_normal.png", XMFLOAT3(0, 0, 0), gridSizeX, gridSizeY, STATIC);
	m_StampHighlight = new Sprite(m_D3DSystem);
	m_StampHighlight->Initialize(base_path, "stamp_highlight.png", XMFLOAT3(0, 0, 0), gridSizeX, gridSizeY, STATIC);
}
//================================================================================================================
string MenuEditorSystem::UpdateTitleBarText()
{
	string gameFolder = "";
	switch (m_EngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			gameFolder = m_mainGameDirectory2D->GAME_FOLDER;
		}
		break;
		case ZSHADE_3D:
		{
			gameFolder = m_mainGameDirectory3D->GAME_FOLDER;
		}
		break;
	}

	if (m_Updated)
	{
		//Place an [*] on the title bar of the window
		string path = "ZShadeSandbox [*]";

		if (gameFolder != "")
		{
			path = "ZShadeSandbox (";
			path.append(gameFolder);
			path.append(") [*]");
		}

		return path;
	}
	else
	{
		string path = "ZShadeSandbox";

		if (gameFolder != "")
		{
			path = "ZShadeSandbox (";
			path.append(gameFolder);
			path.append(")");
		}

		return path;
	}
}
//================================================================================================================
void MenuEditorSystem::SetGameDirectory2D(GameDirectory2D* gd)
{
	m_mainGameDirectory2D = gd;
	m_Env2D->SetGameDirectory2D(gd);
}
//================================================================================================================
void MenuEditorSystem::SetGameDirectory3D(GameDirectory3D* gd)
{
	m_mainGameDirectory3D = gd;
	m_Env3D->SetGameDirectory3D(gd);
}
//================================================================================================================
void MenuEditorSystem::UpdateInput(Keyboard* keyboard)
{
	/*m_ClickDelayTimer->Sleep(10);
	if (m_ClickDelayTimer->IsTimeUp())
	{
		if (keyboard->IsKeyDown(Keyboard::Key::Left) || keyboard->IsKeyDown(Keyboard::Key::A))
		{
			if (ScrollOffset.x > 0)
				ScrollOffset.x -= edgeScrollX;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Right) || keyboard->IsKeyDown(Keyboard::Key::D))
		{
			//if (ScrollOffset.x < 16)
				ScrollOffset.x += edgeScrollX;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Up) || keyboard->IsKeyDown(Keyboard::Key::W))
		{
			if (ScrollOffset.y > 0)
				ScrollOffset.y -= edgeScrollY;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Down) || keyboard->IsKeyDown(Keyboard::Key::S))
		{
			//if (ScrollOffset.y < 16)
				ScrollOffset.y += edgeScrollY;
		}
	}*/
}
//================================================================================================================
void MenuEditorSystem::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		if (dragStartX == -1)
		{
			dragStartX = x;
			dragStartY = y;
		}

		dragPrevX = x;
		dragPrevY = y;
		
		stampPressed = true;
		
		CalculateSelectedMousePosition(x, y);
		
		// Adds a new button to the menu from the selected button image in the button cache if in button
		// place mode
		AddButton();
		
		// Selects a button so it can be moved, if it is moving then this will be skipped
		// so that the button can be placed back on the menu with a left click
		bool goingToMove = MoveButtonStart();
		// If a button is being moved place the button at the clicked position if in button move mode
		if (!goingToMove) MoveButtonEnd();
		
		// Selects a button so it can be cloned, if it is moving then this will be skipped
		// so that the button clone can be added to the menu with a left click
		bool goingToClone = CloneButtonStart();
		// If a button is being moved place the button at the clicked position if in button clone mode
		if (!goingToClone) CloneButtonEnd();
		
		// Selects a button on the menu and deletes it if in button delete mode
		DeleteButton();
		
		// Selects the button to be resized if in button resize mode
		ResizeButtonStart();
		
		// If applicable, Add a text if in text mode
		AddText();
		
		// Selects a text so it can be moved, if it is moving then this will be skipped
		// so that the text can be placed back on the menu with a left click
		goingToMove = MoveTextStart();
		// If a text is being moved place the text at the clicked position if in text move mode
		if (!goingToMove) MoveTextEnd();
		
		// Selects a text so it can be cloned, if it is moving then this will be skipped
		// so that the text clone can be added to the menu with a left click
		goingToClone = CloneTextStart();
		// If a text is being moved place the text at the clicked position if in text clone mode
		if (!goingToClone) CloneTextEnd();
		
		// Selects a text on the menu and deletes it if in text delete mode
		DeleteText();
	}
}
//================================================================================================================
void MenuEditorSystem::OnMouseUp(WPARAM btnState, int x, int y)
{
	// Stop dragging of item
	dragStartX = dragStartY = dragPrevX = dragPrevY = -1;
	stampPressed = false;
}
//================================================================================================================
void MenuEditorSystem::OnMouseMove(WPARAM btnState, int x, int y)
{
	int dX, dY, tdX, tdY;
	
	// Calculate movement delta from previous frame
	if (dragPrevX != -1)
	{
		dX = x - dragPrevX;
		dY = y - dragPrevY;
		tdX = x - dragStartX;
		tdY = y - dragStartY;
	}
	
	if (gridMode == GM_Snap)
	{
		XMFLOAT2 selToolPoint = SnapToGrid(x, y);
		
		m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		
		// Move a button display cover if in button mode
		UpdateDisplaySprite(selToolPoint.x, selToolPoint.y);
		
		// If a button or text is being moved then continue to move it
		MoveButton(selToolPoint.x, selToolPoint.y);
		MoveText(selToolPoint.x, selToolPoint.y);
		
		HighlightButton(selToolPoint.x, selToolPoint.y);
		HighlightText(selToolPoint.x, selToolPoint.y);
	}
	else
	{
		// Move a button display cover if in button mode
		UpdateDisplaySprite(x, y);
		
		// If a button or text is being moved then continue to move it
		MoveButton(x, y);
		MoveText(x, y);
		
		HighlightButton(x, y);
		HighlightText(x, y);
	}
	
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		CalculateSelectedMousePosition(x, y);
		
		stampPressed = true;
		
		// If applicable, Add a button
		AddButton();
		
		// If applicable, Add a text
		AddText();
		
		// Selects a button on the map and deletes it if in button delete mode
		DeleteButton();
		
		// Selects a text on the map and deletes it if in text delete mode
		DeleteText();
	}
	
	// Remember current mouse co-ordinates for next frame
	if (dragPrevX != -1)
	{
		dragPrevX = x;
		dragPrevY = y;
	}
}
//================================================================================================================
XMFLOAT2 MenuEditorSystem::SnapToGrid(int x, int y)
{
	XMFLOAT2 point = XMFLOAT2(x, y);
	
	if (gridMode == GM_Free)
	{
		usingGrid = false;
	}
	if (gridMode == GM_Snap)
	{
		usingGrid = true;
	}
	
	if (usingGrid)
	{
		point.x = (x / gridSizeX) * gridSizeX;
		point.y = (y / gridSizeY) * gridSizeY;
	}
	
	return point;
}
//================================================================================================================
void MenuEditorSystem::CalculateSelectedMousePosition(int x, int y)
{
	m_SelectedMousePos = SnapToGrid(x, y);
}
//================================================================================================================
void MenuEditorSystem::Render(Camera* camera)
{
	// Can render display cover sprites here for buttons and text

	if (m_displaySpriteCover != NULL)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			m_displaySpriteCover->Render(camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
	
	if (m_cloneDisplayTextSprite != NULL)
	{
		m_cloneDisplayTextSprite->Render11();
	}
	
	if (m_display_button_box_created)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			m_sprite_box->Render(camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}

	if (m_display_text_box_created)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			m_sprite_box->Render(camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
	
	if (gridMode == GM_Snap)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			if (stampPressed)
				m_StampHighlight->Render(camera, 0);
			else
				m_StampNormal->Render(camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
}
//================================================================================================================
void MenuEditorSystem::GrabMenuNames(vector<string>& names)
{
	EnvironmentMenuHelper::GrabMenuNames(names);
}
//================================================================================================================
void MenuEditorSystem::SetActiveRenderedMenu(string menu_name)
{
	EnvironmentMenuHelper::SetActiveRenderedMenu(menu_name);
}
//================================================================================================================
void MenuEditorSystem::RemoveMenu(string menu_name)
{
	EnvironmentMenuHelper::RemoveMenu(menu_name);
}
//================================================================================================================
void MenuEditorSystem::CreateMenu(string menu_name)
{
	EnvironmentMenuHelper::CreateMenu(menu_name);
}
//================================================================================================================
void MenuEditorSystem::LoadAMenu(string menu_filename, string& menuName)
{
	EnvironmentMenuHelper::LoadAMenu(menu_filename, menuName);
}
//================================================================================================================
void MenuEditorSystem::SaveMenu(string menu_filename)
{
	EnvironmentMenuHelper::SaveMenu(menu_filename);
}
//================================================================================================================
void MenuEditorSystem::AddButton()
{
	if (editType == ET_Button && action == A_Place)
	{
		if (BetterString(m_SelectedButtonImageName) != "")
		{
			m_Updated = true;
			
			string menu_sprites_path;
			
			switch (m_EngineOptions->m_DimType)
			{
				case ZSHADE_2D:
				{
					menu_sprites_path = m_mainGameDirectory2D->m_menu_sprites_path;
				}
				break;
				case ZSHADE_3D:
				{
					menu_sprites_path = m_mainGameDirectory3D->m_menu_sprites_path;
				}
				break;
			}
			
			EnvironmentMenuHelper::AddButton(
				menu_sprites_path,
				m_SelectedButtonImageName,
				m_SelectedButtonImageName,
				m_SelectedButtonImageName,
				m_SelectedButtonImageName,
				m_SelectedMousePos.x,
				m_SelectedMousePos.y
			);
		}
	}
}
//================================================================================================================
void MenuEditorSystem::AddText()
{
	if (editType == ET_Text && action == A_Place)
	{
		m_Updated = true;

		EnvironmentMenuHelper::AddText("Text", m_SelectedMousePos.x, m_SelectedMousePos.y);
	}
}
//================================================================================================================
void MenuEditorSystem::AddMenuBackground()
{
	if (BetterString(m_BackgroundImageName) != "")
	{
		string spritesPath = "";
		switch (m_EngineOptions->m_DimType)
		{
			case ZSHADE_2D:
			{
				spritesPath = m_mainGameDirectory2D->m_sprites_path;
			}
			break;
			case ZSHADE_3D:
			{
				// Need to figure out where the images are loaded from in 3D
				//spritesPath = m_mainGameDirectory3D->m_sprites_path;
			}
			break;
		}
		
		m_Updated = true;

		ZShadeSandboxGraphics::Image* image = new ZShadeSandboxGraphics::Image(
			m_D3DSystem, m_BackgroundImageName, spritesPath,
			0, 0, m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight);

		EnvironmentMenuHelper::AddBackground(image);
	}
}
//================================================================================================================
void MenuEditorSystem::MoveButton(float x, float y)
{
	if (m_MenuCreated && (m_moving_button || m_cloning_button) && editType == ET_Button && (action == A_Move || action == A_Clone))
	{
		//The user has moved a button in the menu
		m_Updated = true;
		
		if (m_moving_button)
			EnvironmentMenuHelper::MoveButton(m_MovingButtonSprite, x, y);

		if (m_cloning_button)
			EnvironmentMenuHelper::MoveButton(m_SelectedButtonSprite, x, y);
	}
}
//================================================================================================================
bool MenuEditorSystem::MoveButtonStart()
{
	if (m_MenuCreated && !m_moving_button && editType == ET_Button && action == A_Move)
	{
		bool clicked = EnvironmentMenuHelper::MoveButtonStart(m_MovingButtonSprite, m_originalButtonID, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_moving_button = true;
		}
		
		return clicked;
	}
	
	// Not in button mode move and has not clicked on a button or is already moving
	return false;
}
//================================================================================================================
void MenuEditorSystem::MoveButtonEnd()
{
	if (m_MenuCreated && m_moving_button && editType == ET_Button && action == A_Move)
	{
		m_Updated = true;
		m_moving_button = false;
		EnvironmentMenuHelper::MoveButtonEnd(m_MovingButtonSprite, m_originalButtonID, m_SelectedMousePos.x, m_SelectedMousePos.y);
		m_originalButtonID = -1;
	}
}
//================================================================================================================
bool MenuEditorSystem::CloneButtonStart()
{
	if (m_MenuCreated && !m_cloning_button && editType == ET_Button && action == A_Clone)
	{
		bool clicked = EnvironmentMenuHelper::CloneButtonStart(m_SelectedButtonSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_cloning_button = true;
			m_selected_button_cover = true;
		}
		
		return clicked;
	}
	
	// Not in button mode move and has not clicked on a button or is already moving
	return false;
}
//================================================================================================================
void MenuEditorSystem::CloneButtonEnd()
{
	if (m_MenuCreated && m_cloning_button && editType == ET_Button && action == A_Clone)
	{
		m_Updated = true;
		m_cloning_button = false;
		EnvironmentMenuHelper::CloneButtonEnd(m_SelectedButtonSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (m_cloneDisplaySpriteCover != 0)
		{
			//Need to remove the current display sprite if there is one
			m_cloneDisplaySpriteCover = 0;
		}
	}
}
//================================================================================================================
void MenuEditorSystem::DeleteButton()
{
	if (m_MenuCreated && editType == ET_Button && action == A_Delete)
	{
		ZShadeSandboxGraphics::Button* b;
		bool clicked = EnvironmentMenuHelper::ButtonClicked(b, m_SelectedMousePos.x, m_SelectedMousePos.y);
		if (clicked)
		{
			//The user has deleted a button in the menu
			m_Updated = true;
			EnvironmentMenuHelper::DeleteButton(b);
		}
	}
}
//================================================================================================================
bool MenuEditorSystem::ResizeButtonStart()
{
	if (m_MenuCreated && editType == ET_Button && action == A_EditSize)
	{
		//The user has started to resize a button in a menu
		bool clicked = EnvironmentMenuHelper::ButtonClicked(m_EditSizeButtonSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_resizing_button = true;
		}
		
		return clicked;
	}
	
	return false;
}
//================================================================================================================
void MenuEditorSystem::ResizeButtonUpdate(
	bool updatePosition,
	bool updateSize,
	bool goFast,
	bool pressedLeftKey,
	bool pressedRightKey,
	bool pressedUpKey,
	bool pressedDownKey
)
{
	if (m_MenuCreated && m_resizing_button && editType == ET_Button && action == A_EditSize)
	{
		//The user has resized a button in a menu
		m_Updated = true;
		EnvironmentMenuHelper::ResizeButtonUpdate(m_EditSizeButtonSprite,
			updatePosition,
			updateSize,
			goFast,
			pressedLeftKey,
			pressedRightKey,
			pressedUpKey,
			pressedDownKey
		);
	}
}
//================================================================================================================
void MenuEditorSystem::HighlightButton(float x, float y)
{
	if (m_MenuCreated && editType == ET_Button && (action == A_Move || action == A_Delete || action == A_EditSize))
	{
		//The user has selected a button in a menu
		m_Updated = true;
		EnvironmentMenuHelper::HighlightButton(m_button_with_box, m_sprite_box, m_display_button_box_created, x, y);
	}
}
//================================================================================================================
void MenuEditorSystem::MoveText(float x, float y)
{
	if (m_MenuCreated && (m_moving_text || m_cloning_text) && editType == ET_Text && (action == A_Move || action == A_Clone))
	{
		//The user has moved a button in the menu
		m_Updated = true;
		
		if (m_moving_text)
			EnvironmentMenuHelper::MoveText(m_MovingTextSprite, x, y);

		if (m_cloning_text)
			EnvironmentMenuHelper::MoveText(m_SelectedTextSprite, x, y);
	}
}
//================================================================================================================
bool MenuEditorSystem::MoveTextStart()
{
	if (m_MenuCreated && !m_moving_text && editType == ET_Text && action == A_Move)
	{
		bool clicked = EnvironmentMenuHelper::MoveTextStart(m_MovingTextSprite, m_originalTextID, m_SelectedMousePos.x, m_SelectedMousePos.y);

		if (clicked)
		{
			m_Updated = true;
			m_moving_button = true;
		}

		return clicked;
	}

	// Not in button mode move and has not clicked on a button or is already moving
	return false;
}
//================================================================================================================
void MenuEditorSystem::MoveTextEnd()
{
	if (m_MenuCreated && m_moving_text && editType == ET_Text && action == A_Move)
	{
		m_Updated = true;
		m_moving_text = false;
		EnvironmentMenuHelper::MoveTextEnd(m_MovingTextSprite, m_originalTextID, m_SelectedMousePos.x, m_SelectedMousePos.y);
		m_originalTextID = -1;
	}
}
//================================================================================================================
bool MenuEditorSystem::CloneTextStart()
{
	if (m_MenuCreated && !m_cloning_text && editType == ET_Text && action == A_Clone)
	{
		bool clicked = EnvironmentMenuHelper::CloneTextStart(m_SelectedTextSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);

		if (clicked)
		{
			m_Updated = true;
			m_cloning_text = true;
			m_selected_text_cover = true;
		}

		return clicked;
	}

	// Not in button mode move and has not clicked on a button or is already moving
	return false;
}
//================================================================================================================
void MenuEditorSystem::CloneTextEnd()
{
	if (m_MenuCreated && m_cloning_text && editType == ET_Text && action == A_Clone)
	{
		m_Updated = true;
		m_cloning_text = false;
		EnvironmentMenuHelper::CloneTextEnd(m_SelectedTextSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (m_cloneDisplayTextSprite != 0)
		{
			//Need to remove the current display text if there is one
			m_cloneDisplayTextSprite = 0;
		}
	}
}
//================================================================================================================
void MenuEditorSystem::DeleteText()
{
	if (m_MenuCreated && editType == ET_Text && action == A_Delete)
	{
		ZShadeSandboxGraphics::Text* t;
		bool clicked = EnvironmentMenuHelper::TextClicked(t, m_SelectedMousePos.x, m_SelectedMousePos.y);
		if (clicked)
		{
			//The user has deleted a button in the menu
			m_Updated = true;
			EnvironmentMenuHelper::DeleteText(t);
		}
	}
}
//================================================================================================================
void MenuEditorSystem::HighlightText(float x, float y)
{
	if (m_MenuCreated && editType == ET_Text && (action == A_Move || action == A_Delete || action == A_EditSize))
	{
		//The user has selected a button in a menu
		m_Updated = true;
		EnvironmentMenuHelper::HighlightText(m_text_with_box, m_sprite_box, m_display_text_box_created, x, y);
	}
}
//================================================================================================================
void MenuEditorSystem::UpdateDisplaySprite(float x, float y)
{
	if (m_MenuCreated && editType == ET_Button && action == A_Place)
	{
		if (m_displaySpriteCover == NULL)
		{
			if (BetterString(m_SelectedButtonImageName) != "")
			{
				string menuSpritesPath = "";
				
				switch (m_EngineOptions->m_DimType)
				{
					case ZSHADE_2D:
					{
						menuSpritesPath = m_mainGameDirectory2D->m_menu_sprites_path;
					}
					break;
					case ZSHADE_3D:
					{
						menuSpritesPath = m_mainGameDirectory3D->m_menu_sprites_path;
					}
					break;
				}
				
				m_displaySpriteCover = new Sprite(m_D3DSystem);
				m_displaySpriteCover->Initialize(menuSpritesPath, m_SelectedButtonImageName,
					XMFLOAT3(x, y, 0), 80, 30, PhysicsType::STATIC);
			}
		}
		else
		{
			m_displaySpriteCover->TopLeftPosition() = XMFLOAT3(x, y, 0);
		}
	}
	if (m_MenuCreated && editType == ET_Button && action == A_Clone)
	{
		//Render the selected sprite from the sprite cache with the mouse cursor
		if (m_selected_button_cover)
		{
			if (m_cloneDisplaySpriteCover != 0)
			{
				//Need to remove the current display sprite if there is one
				m_cloneDisplaySpriteCover = 0;
			}
			
			string menuSpritesPath = "";
			
			switch (m_EngineOptions->m_DimType)
			{
				case ZSHADE_2D:
				{
					menuSpritesPath = m_mainGameDirectory2D->m_menu_sprites_path;
				}
				break;
				case ZSHADE_3D:
				{
					menuSpritesPath = m_mainGameDirectory3D->m_menu_sprites_path;
				}
				break;
			}
			
			m_displaySpriteCover = m_SelectedButtonSprite->ToSprite(menuSpritesPath);

			m_selected_button_cover = false;
		}

		if (m_cloneDisplaySpriteCover != 0)
		{
			m_cloneDisplaySpriteCover->TopLeftPosition().x = x;
			m_cloneDisplaySpriteCover->TopLeftPosition().y = y;
		}
	}
	
	if (m_MenuCreated && editType == ET_Text && action == A_Clone)
	{
		//Render the selected sprite from the sprite cache with the mouse cursor
		if (m_selected_text_cover)
		{
			if (m_cloneDisplayTextSprite != 0)
			{
				//Need to remove the current display sprite if there is one
				m_cloneDisplayTextSprite = 0;
			}
			
			m_cloneDisplayTextSprite = m_SelectedTextSprite->Clone();

			m_selected_text_cover = false;
		}

		if (m_cloneDisplayTextSprite != 0)
		{
			m_cloneDisplayTextSprite->TextTransform() = XMMatrixTranslation(x, y, 0);
		}
	}
}
//================================================================================================================
void MenuEditorSystem::SwitchToEditTypeNone()
{
	editType = ET_None;
}
//================================================================================================================
void MenuEditorSystem::SwitchToEditTypeButton()
{
	editType = ET_Button;
}
//================================================================================================================
void MenuEditorSystem::SwitchToEditTypeText()
{
	editType = ET_Text;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionNone()
{
	action = A_None;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionMove()
{
	action = A_Move;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionDelete()
{
	action = A_Delete;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionPlace()
{
	action = A_Place;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionClone()
{
	action = A_Clone;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionEditSize()
{
	action = A_EditSize;
}
//================================================================================================================
void MenuEditorSystem::SwitchToGridModeFree()
{
	gridMode = GM_Free;
}
//================================================================================================================
void MenuEditorSystem::SwitchToGridModeSnap()
{
	gridMode = GM_Snap;
}
//================================================================================================================
