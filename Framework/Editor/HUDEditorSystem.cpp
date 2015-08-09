#include "HUDEditorSystem.h"
#include "Convert.h"
#include "EnvironmentHUDHelper.h"
//================================================================================================================
HUDEditorSystem::HUDEditorSystem(ZShadeSandboxEnvironment::Environment* env)
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
	
	m_SelectedImageImageName = "";
	
	// Set defaults
	SwitchToEditTypeNone();

	usingGrid = false;
	gridSizeX = 64;
	gridSizeY = 64;
	stampPressed = false;
	m_selectModeImageID = -1;
	m_selectModeTextID = -1;
	m_originalImageID = -1;
	m_originalTextID = -1;
	m_image_cache_selection_changed = false;
	m_resizing_image = false;
	m_moving_image = false;
	m_cloning_image = false;
	m_moving_text = false;
	m_cloning_text = false;
	m_selected_image_cover = false;
	m_selected_text_cover = false;
	m_display_image_box_created = false;
	m_display_selected_image_box = false;
	m_display_selected_text_box = false;
	m_select_sprite_box = 0;
	m_image_with_box = 0;
	m_text_with_box = 0;
	m_sprite_box = 0;
	m_MovingImageSprite = 0;
	m_SelectedImageSprite = 0;
	m_EditSizeImageSprite = 0;
	m_MovingTextSprite = 0;
	m_SelectedTextSprite = 0;
	m_displaySpriteCover = 0;
	m_cloneDisplaySpriteCover = 0;
	m_cloneDisplayTextSprite = 0;
}
//================================================================================================================
HUDEditorSystem::~HUDEditorSystem()
{
}
//================================================================================================================
void HUDEditorSystem::GoFullscreen(bool bEnable)
{
	//m_Env2D->GoFullscreen(bEnable);
}
//================================================================================================================
void HUDEditorSystem::LoadRequiredSprites()
{
	string base_path = m_mainGameDirectory2D->m_required_textures;
	int tileSize = m_EngineOptions->TILE_SIZE;
	m_StampNormal = new Sprite(m_D3DSystem);
	m_StampNormal->Initialize(base_path, "stamp_normal.png", XMFLOAT3(0, 0, 0), tileSize, tileSize, STATIC);
	m_StampHighlight = new Sprite(m_D3DSystem);
	m_StampHighlight->Initialize(base_path, "stamp_highlight.png", XMFLOAT3(0, 0, 0), tileSize, tileSize, STATIC);
}
//================================================================================================================
string HUDEditorSystem::UpdateTitleBarText()
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
void HUDEditorSystem::SetGameDirectory2D(GameDirectory2D* gd)
{
	m_mainGameDirectory2D = gd;
	m_Env2D->SetGameDirectory2D(gd);
}
//================================================================================================================
void HUDEditorSystem::SetGameDirectory3D(GameDirectory3D* gd)
{
	m_mainGameDirectory3D = gd;
	m_Env3D->SetGameDirectory3D(gd);
}
//================================================================================================================
void HUDEditorSystem::UpdateInput(Keyboard* keyboard)
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
void HUDEditorSystem::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		stampPressed = true;
		
		CalculateSelectedMousePosition(x, y);
		
		// Adds a new image to the hud from the selected image image in the image cache if in image
		// place mode
		AddImage();
		
		// Selects a image so it can be moved, if it is moving then this will be skipped
		// so that the image can be placed back on the hud with a left click
		bool goingToMove = MoveImageStart();
		// If a image is being moved place the image at the clicked position if in image move mode
		if (!goingToMove) MoveImageEnd();
		
		// Selects a image so it can be cloned, if it is moving then this will be skipped
		// so that the image clone can be added to the hud with a left click
		bool goingToClone = CloneImageStart();
		// If a image is being moved place the image at the clicked position if in image clone mode
		if (!goingToClone) CloneImageEnd();
		
		// Selects a image on the hud and deletes it if in image delete mode
		DeleteImage();
		
		// Selects the button to be resized if in button resize mode
		ResizeImageStart();
		
		// If applicable, Add a text if in text mode
		AddText();
		
		// Selects a text so it can be moved, if it is moving then this will be skipped
		// so that the text can be placed back on the hud with a left click
		goingToMove = MoveTextStart();
		// If a text is being moved place the text at the clicked position if in text move mode
		if (!goingToMove) MoveTextEnd();
		
		// Selects a text so it can be cloned, if it is moving then this will be skipped
		// so that the text clone can be added to the hud with a left click
		goingToClone = CloneTextStart();
		// If a text is being moved place the text at the clicked position if in text clone mode
		if (!goingToClone) CloneTextEnd();
		
		// Selects a text on the hud and deletes it if in text delete mode
		DeleteText();
	}
}
//================================================================================================================
void HUDEditorSystem::OnMouseUp(WPARAM btnState, int x, int y)
{
	// Stop dragging of item
	stampPressed = false;
}
//================================================================================================================
void HUDEditorSystem::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (gridMode == GM_Snap)
	{
		XMFLOAT2 selToolPoint = SnapToGrid(x, y);
		
		m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		
		// Move a image display cover if in image mode
		UpdateDisplaySprite(selToolPoint.x, selToolPoint.y);
		
		// If a image or text is being moved then continue to move it
		MoveImage(selToolPoint.x, selToolPoint.y);
		MoveText(selToolPoint.x, selToolPoint.y);
		
		HighlightImage(selToolPoint.x, selToolPoint.y);
		HighlightText(selToolPoint.x, selToolPoint.y);
	}
	else
	{
		// Move a image display cover if in image mode
		UpdateDisplaySprite(x, y);
		
		// If a image or text is being moved then continue to move it
		MoveImage(x, y);
		MoveText(x, y);
		
		HighlightImage(x, y);
		HighlightText(x, y);
	}
	
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		CalculateSelectedMousePosition(x, y);
		
		stampPressed = true;
		
		// If applicable, Add a image
		AddImage();
		
		// If applicable, Add a text
		AddText();
		
		// Selects a image on the map and deletes it if in image delete mode
		DeleteImage();
		
		// Selects a text on the map and deletes it if in text delete mode
		DeleteText();
	}
	
	// Move a button or text display cover
	/*if (editMode == ET_Button && (action == A_Place || action == A_Move))
	{
		XMFLOAT2 selToolPoint = SnapToGrid(x, y);
		
		m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
	}*/
}
//================================================================================================================
XMFLOAT2 HUDEditorSystem::SnapToGrid(int x, int y)
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
void HUDEditorSystem::CalculateSelectedMousePosition(int x, int y)
{
	m_SelectedMousePos = SnapToGrid(x, y);
}
//================================================================================================================
void HUDEditorSystem::Render(Camera* camera)
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
	
	if (m_display_image_box_created)
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
	
	// Render a box around the selected image or text from Select mode
	if (m_display_selected_image_box || m_display_selected_text_box)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			if (m_select_sprite_box != 0) m_select_sprite_box->Render(camera, 0);
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
void HUDEditorSystem::CreateHUD(string hud_name)
{
	EnvironmentHUDHelper::CreateHUD(hud_name);
}
//================================================================================================================
void HUDEditorSystem::LoadHUD(string hud_filename, string& hud_name)
{
	EnvironmentHUDHelper::LoadHUD(hud_filename, hud_name);
}
//================================================================================================================
void HUDEditorSystem::SaveHUD(string hud_filename)
{
	EnvironmentHUDHelper::SaveHUD(hud_filename);
}
//================================================================================================================
void HUDEditorSystem::AddImage()
{
	if (m_HUDCreated && editType == ET_Image && action == A_Place)
	{
		if (BetterString(m_SelectedImageImageName) != "")
		{
			m_Updated = true;
			
			string hud_sprites_path;
			
			switch (m_EngineOptions->m_DimType)
			{
				case ZSHADE_2D:
				{
					hud_sprites_path = m_mainGameDirectory2D->m_hud_path;
				}
				break;
				case ZSHADE_3D:
				{
					hud_sprites_path = m_mainGameDirectory3D->m_hud_path;
				}
				break;
			}
			
			EnvironmentHUDHelper::AddImage(
				hud_sprites_path,
				m_SelectedImageImageName,
				m_SelectedMousePos.x,
				m_SelectedMousePos.y,
				64,
				64
			);
		}
	}
}
//================================================================================================================
void HUDEditorSystem::AddText()
{
	if (m_HUDCreated && editType == ET_Text && action == A_Place)
	{
		m_Updated = true;

		EnvironmentHUDHelper::AddText("Text", m_SelectedMousePos.x, m_SelectedMousePos.y);
	}
}
//================================================================================================================
void HUDEditorSystem::MoveImage(float x, float y)
{
	if (m_HUDCreated && (m_moving_image || m_cloning_image) && editType == ET_Image && (action == A_Move || action == A_Clone))
	{
		//The user has moved a button in the menu
		m_Updated = true;
		
		if (m_moving_image)
			EnvironmentHUDHelper::MoveImage(m_MovingImageSprite, x, y);

		if (m_cloning_image)
			EnvironmentHUDHelper::MoveImage(m_SelectedImageSprite, x, y);
	}
}
//================================================================================================================
bool HUDEditorSystem::MoveImageStart()
{
	if (m_HUDCreated && !m_moving_image && editType == ET_Image && action == A_Move)
	{
		bool clicked = EnvironmentHUDHelper::MoveImageStart(m_MovingImageSprite, m_originalImageID, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_moving_image = true;
		}
		
		return clicked;
	}
	
	// Not in image mode move and has not clicked on a image or is already moving
	return false;
}
//================================================================================================================
void HUDEditorSystem::MoveImageEnd()
{
	if (m_HUDCreated && m_moving_image && editType == ET_Image && action == A_Move)
	{
		m_Updated = true;
		m_moving_image = false;
		EnvironmentHUDHelper::MoveImageEnd(m_MovingImageSprite, m_originalImageID, m_SelectedMousePos.x, m_SelectedMousePos.y);
		m_originalImageID = -1;
	}
}
//================================================================================================================
bool HUDEditorSystem::CloneImageStart()
{
	if (m_HUDCreated && !m_cloning_image && editType == ET_Image && action == A_Clone)
	{
		bool clicked = EnvironmentHUDHelper::CloneImageStart(m_SelectedImageSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_cloning_image = true;
			m_selected_image_cover = true;
		}
		
		return clicked;
	}
	
	// Not in image mode move and has not clicked on a image or is already moving
	return false;
}
//================================================================================================================
void HUDEditorSystem::CloneImageEnd()
{
	if (m_HUDCreated && m_cloning_image && editType == ET_Image && action == A_Clone)
	{
		m_Updated = true;
		m_cloning_image = false;
		EnvironmentHUDHelper::CloneImageEnd(m_SelectedImageSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (m_cloneDisplaySpriteCover != 0)
		{
			//Need to remove the current display sprite if there is one
			m_cloneDisplaySpriteCover = 0;
		}
	}
}
//================================================================================================================
void HUDEditorSystem::DeleteImage()
{
	if (m_HUDCreated && editType == ET_Image && action == A_Delete)
	{
		ZShadeSandboxGraphics::Image* im;
		bool clicked = EnvironmentHUDHelper::ImageClicked(im, m_SelectedMousePos.x, m_SelectedMousePos.y);
		if (clicked)
		{
			//The user has deleted a image in the hud
			m_Updated = true;
			EnvironmentHUDHelper::DeleteImage(im);
		}
	}
}
//================================================================================================================
bool HUDEditorSystem::ResizeImageStart()
{
	if (m_HUDCreated && editType == ET_Image && action == A_EditSize)
	{
		//The user has started to resize a image in the hud
		bool clicked = EnvironmentHUDHelper::ImageClicked(m_EditSizeImageSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (clicked)
		{
			m_Updated = true;
			m_resizing_image = true;
		}
		
		return clicked;
	}
	
	return false;
}
//================================================================================================================
void HUDEditorSystem::ResizeImageUpdate(
	bool updatePosition,
	bool updateSize,
	bool goFast,
	bool pressedLeftKey,
	bool pressedRightKey,
	bool pressedUpKey,
	bool pressedDownKey
)
{
	
}
//================================================================================================================
void HUDEditorSystem::HighlightImage(float x, float y)
{
	if (m_HUDCreated && editType == ET_Image && (action == A_Move || action == A_Delete || action == A_EditSize))
	{
		//The user has selected a image in the hud
		m_Updated = true;
		EnvironmentHUDHelper::HighlightImage(m_image_with_box, m_sprite_box, m_display_image_box_created, x, y);
	}
}
//================================================================================================================
void HUDEditorSystem::MoveText(float x, float y)
{
	if (m_HUDCreated && (m_moving_text || m_cloning_text) && editType == ET_Text && (action == A_Move || action == A_Clone))
	{
		//The user has moved a button in the menu
		m_Updated = true;
		
		if (m_moving_text)
			EnvironmentHUDHelper::MoveText(m_MovingTextSprite, x, y);

		if (m_cloning_text)
			EnvironmentHUDHelper::MoveText(m_SelectedTextSprite, x, y);
	}
}
//================================================================================================================
bool HUDEditorSystem::MoveTextStart()
{
	if (m_HUDCreated && !m_moving_text && editType == ET_Text && action == A_Move)
	{
		bool clicked = EnvironmentHUDHelper::MoveTextStart(m_MovingTextSprite, m_originalTextID, m_SelectedMousePos.x, m_SelectedMousePos.y);

		if (clicked)
		{
			m_Updated = true;
			m_moving_text = true;
		}

		return clicked;
	}

	// Not in button mode move and has not clicked on a button or is already moving
	return false;
}
//================================================================================================================
void HUDEditorSystem::MoveTextEnd()
{
	if (m_HUDCreated && m_moving_text && editType == ET_Text && action == A_Move)
	{
		m_Updated = true;
		m_moving_text = false;
		EnvironmentHUDHelper::MoveTextEnd(m_MovingTextSprite, m_originalTextID, m_SelectedMousePos.x, m_SelectedMousePos.y);
		m_originalTextID = -1;
	}
}
//================================================================================================================
bool HUDEditorSystem::CloneTextStart()
{
	if (m_HUDCreated && !m_cloning_text && editType == ET_Text && action == A_Clone)
	{
		bool clicked = EnvironmentHUDHelper::CloneTextStart(m_SelectedTextSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);

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
void HUDEditorSystem::CloneTextEnd()
{
	if (m_HUDCreated && m_cloning_text && editType == ET_Text && action == A_Clone)
	{
		m_Updated = true;
		m_cloning_text = false;
		EnvironmentHUDHelper::CloneTextEnd(m_SelectedTextSprite, m_SelectedMousePos.x, m_SelectedMousePos.y);
		
		if (m_cloneDisplayTextSprite != 0)
		{
			//Need to remove the current display text if there is one
			m_cloneDisplayTextSprite = 0;
		}
	}
}
//================================================================================================================
void HUDEditorSystem::DeleteText()
{
	if (m_HUDCreated && editType == ET_Text && action == A_Delete)
	{
		ZShadeSandboxGraphics::Text* t;
		bool clicked = EnvironmentHUDHelper::TextClicked(t, m_SelectedMousePos.x, m_SelectedMousePos.y);
		if (clicked)
		{
			//The user has deleted a button in the menu
			m_Updated = true;
			EnvironmentHUDHelper::DeleteText(t);
		}
	}
}
//================================================================================================================
void HUDEditorSystem::HighlightText(float x, float y)
{
	if (m_HUDCreated && editType == ET_Text && (action == A_Move || action == A_Delete || action == A_EditSize))
	{
		//The user has selected a button in a menu
		m_Updated = true;
		EnvironmentHUDHelper::HighlightText(m_text_with_box, m_sprite_box, m_display_text_box_created, x, y);
	}
}
//================================================================================================================
void HUDEditorSystem::UpdateDisplaySprite(float x, float y)
{
	if (m_HUDCreated && editType == ET_Image && action == A_Place)
	{
		if (m_displaySpriteCover == NULL)
		{
			if (BetterString(m_SelectedImageImageName) != "")
			{
				string hudSpritesPath = "";
				
				switch (m_EngineOptions->m_DimType)
				{
					case ZSHADE_2D:
					{
						hudSpritesPath = m_mainGameDirectory2D->m_hud_path;
					}
					break;
					case ZSHADE_3D:
					{
						hudSpritesPath = m_mainGameDirectory3D->m_hud_path;
					}
					break;
				}
				
				m_displaySpriteCover = new Sprite(m_D3DSystem);
				m_displaySpriteCover->Initialize(hudSpritesPath, m_SelectedImageImageName,
					XMFLOAT3(x, y, 0), 64, 64, PhysicsType::STATIC);
			}
		}
		else
		{
			m_displaySpriteCover->TopLeftPosition() = XMFLOAT3(x, y, 0);
		}
	}
	if (m_HUDCreated && editType == ET_Image && action == A_Clone)
	{
		//Render the selected sprite from the sprite cache with the mouse cursor
		if (m_selected_image_cover)
		{
			if (m_cloneDisplaySpriteCover != 0)
			{
				//Need to remove the current display sprite if there is one
				m_cloneDisplaySpriteCover = 0;
			}
			
			string hudSpritesPath = "";
			
			switch (m_EngineOptions->m_DimType)
			{
				case ZSHADE_2D:
				{
					hudSpritesPath = m_mainGameDirectory2D->m_hud_path;
				}
				break;
				case ZSHADE_3D:
				{
					hudSpritesPath = m_mainGameDirectory3D->m_hud_path;
				}
				break;
			}
			
			m_displaySpriteCover = m_SelectedImageSprite->ToSprite(hudSpritesPath);

			m_selected_image_cover = false;
		}

		if (m_cloneDisplaySpriteCover != 0)
		{
			m_cloneDisplaySpriteCover->TopLeftPosition().x = x;
			m_cloneDisplaySpriteCover->TopLeftPosition().y = y;
		}
	}
	
	if (m_HUDCreated && editType == ET_Text && action == A_Clone)
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
void HUDEditorSystem::SelectModeImageClicked(float x, float y)
{
	if (m_HUDCreated && editType == ET_Image && action == A_Select)
	{
		m_display_selected_image_box = false;
		m_select_sprite_box = 0;

		if (!EnvironmentHUDHelper::GetImageID(m_selectModeImageID, x, y))
		{
			m_selectModeImageID = -1;
		}
		else
		{
			bWasClicked = true;
		}

		// Create a selected sprite box around the image that was selected
		EnvironmentHUDHelper::HighlightImage(m_image_with_box, m_select_sprite_box, m_display_image_box_created, x, y);
	}
	else
	{
		m_selectModeImageID = -1;
		m_display_selected_image_box = false;
	}
}
//================================================================================================================
void HUDEditorSystem::SelectModeTextClicked(float x, float y)
{
	if (m_HUDCreated && editType == ET_Text && action == A_Select)
	{
		m_display_text_box_created = false;
		m_select_sprite_box = 0;
		
		if (!EnvironmentHUDHelper::GetTextID(m_selectModeTextID, x, y))
		{
			m_selectModeTextID = -1;
		}
		else
		{
			bWasClicked = true;
		}
		
		// Create a selected sprite box around the text that was selected
		EnvironmentHUDHelper::HighlightText(m_text_with_box, m_select_sprite_box, m_display_text_box_created, x, y);
	}
	else
	{
		m_selectModeTextID = -1;
		m_display_text_box_created = false;
	}
}
//================================================================================================================
void HUDEditorSystem::SwitchToEditTypeNone()
{
	editType = ET_None;
}
//================================================================================================================
void HUDEditorSystem::SwitchToEditTypeImage()
{
	editType = ET_Image;
}
//================================================================================================================
void HUDEditorSystem::SwitchToEditTypeText()
{
	editType = ET_Text;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionSelect()
{
	action = A_Select;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionMove()
{
	action = A_Move;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionDelete()
{
	action = A_Delete;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionClone()
{
	action = A_Clone;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionPlace()
{
	action = A_Place;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionEditSize()
{
	action = A_EditSize;
}
//================================================================================================================
void HUDEditorSystem::SwitchToGridModeFree()
{
	gridMode = GM_Free;
}
//================================================================================================================
void HUDEditorSystem::SwitchToGridModeSnap()
{
	gridMode = GM_Snap;
}
//================================================================================================================
