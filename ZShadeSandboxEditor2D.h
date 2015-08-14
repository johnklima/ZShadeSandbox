#pragma once
#include <Windows.h>
#include "EngineOptions.h"
#include "ZShadeResource.h"
#include "ZShadeFrontEnd.h"
#include "DXGameWindow.h"
#include "EditorHelper2D.h"
#include "Convert.h"
#include "Environment.h"
#include "GDI.h"

namespace ZShadeSandbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	static std::string ConvertToStandardString(System::String^ string)
	{
		using System::Runtime::InteropServices::Marshal;
		System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
		char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
		std::string returnString(charPointer, string->Length);
		Marshal::FreeHGlobal(pointer);
		return returnString;
	}

	/// <summary>
	/// Summary for ZShadeSandboxEditor
	/// </summary>
	public ref class ZShadeSandboxEditor2D : public System::Windows::Forms::Form
	{
	public:
		ZShadeSandboxEditor2D(EngineOptions* eo)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			bInitialized = false;
			bINIContainsGameFolder = false;
			bInitLoadCache = false;
			mEngineOptions = eo;
			mDXGameWindow = 0;
			mEditorHelper2D = 0;
		}
		
		// Need Saving and Closing the Game Directory
		
		void Run()
		{
			while (!mDXGameWindow->IsDestroyed())
			{
				if (mDXGameWindow)
				{
					if (!mEditorHelper2D)
					{
						mEditorHelper2D = new EditorHelper2D();
						mEditorHelper2D->ToolHwnd() = mToolHwnd;

						if (bINIContainsGameFolder)
						{
							mEditorHelper2D->LoadINIGameDirectory();
							bINIContainsGameFolder = false;
							bInitLoadCache = true;
						}

						ZShadeSandboxEnvironment::Environment::Env2D()->SetMapEditorSystem(mEditorHelper2D->GetMapEditorSystem2D());
						ZShadeSandboxEnvironment::Environment::Env2D()->SetMenuEditorSystem(mEditorHelper2D->GetMenuEditorSystem());
						ZShadeSandboxEnvironment::Environment::Env2D()->SetHUDEditorSystem(mEditorHelper2D->GetHUDEditorSystem());
						ZShadeSandboxEnvironment::Environment::Env2D()->SetInventoryEditorSystem2D(mEditorHelper2D->GetInventoryEditorSystem2D());
						mEditorHelper2D->GetMapEditorSystem2D()->SwitchToEditModeTiles();
					}
					else
					{
						if (EditorHelper2D::GameDirectoryCreated())
						{
							mDXGameWindow->Run();
						}
					}
				}
			}
		}

		void Init()
		{
			if (mEngineOptions->m_GameFolderName == "NONE")
			{
				MenuEditorButton->Enabled = false;
				HUDEditorButton->Enabled = false;
				TileModeTopdownGroupBox->Enabled = false;
				EditModeGroupBox->Enabled = false;
			}
			else
			{
				// Will be loading an existing game
				bINIContainsGameFolder = true;

				//Full path of game directory
				string username = CGlobal::GrabUserName();
				BetterString gameFolder = "C:\\Users\\";
				gameFolder += username;
				gameFolder += "\\AppData\\Roaming\\";
				gameFolder += mEngineOptions->m_GameFolderName;
				GamePathLabel->Text = gcnew String(gameFolder);
			}
			
			TileModeMultiStampSizeGroupBox->Hide();
			GamePathLabel->Text = "";
			TileModePlatformerGroupBox->Hide();
			SpriteModeGroupBox->Hide();
			HardnessModeGroupBox->Hide();
			CheckGameType();
			FillTopdownMapSizeComboBox();
			FillTopdownFOWRadiusComboBox();
			FillPlatformerMapSizeComboBox();
			SetSpriteTabDefaults();
		}

		void SwitchToTileModeTopdown()
		{
			TileModeTopdownGroupBox->Location = System::Drawing::Point(6, 166);
			TileModeTopdownGroupBox->Show();
			TileModePlatformerGroupBox->Hide();
			SpriteModeGroupBox->Hide();
			HardnessModeGroupBox->Hide();
		}
		
		void SwitchToTileModePlatformer()
		{
			TileModePlatformerGroupBox->Location = System::Drawing::Point(6, 166);
			TileModeTopdownGroupBox->Hide();
			TileModePlatformerGroupBox->Show();
			SpriteModeGroupBox->Hide();
			HardnessModeGroupBox->Hide();
		}

		void SwitchToSpriteMode()
		{
			SpriteModeGroupBox->Location = System::Drawing::Point(6, 166);
			TileModeTopdownGroupBox->Hide();
			TileModePlatformerGroupBox->Hide();
			SpriteModeGroupBox->Show();
			HardnessModeGroupBox->Hide();
			TileModeMultiStampSizeGroupBox->Hide();
		}

		/*void SwitchToMusicMode()
		{
			TileModeTopdownGroupBox->Hide();
			TileModePlatformerGroupBox->Hide();
			SpriteModeGroupBox->Hide();
			HardnessModeGroupBox->Hide();
			TileModeMultiStampSizeGroupBox->Hide();
		}*/

		void SwitchToHardnessMode()
		{
			HardnessModeGroupBox->Location = System::Drawing::Point(6, 166);
			TileModeTopdownGroupBox->Hide();
			TileModePlatformerGroupBox->Hide();
			SpriteModeGroupBox->Hide();
			HardnessModeGroupBox->Show();
			TileModeMultiStampSizeGroupBox->Hide();
		}

		void SwitchToNewTopdownMapGroupbox()
		{
			NewPlatformerMapGroupbox->Hide();
			NewTopdownMapGroupbox->Show();
			NewTopdownMapGroupbox->Location = System::Drawing::Point(18, 248);

			// Disable until a world is loaded or created
			WorldTabTopdownMapSizeComboBox->Enabled = false;
			WorldTabTopdownMapWorldNameComboBox->Enabled = false;
			WorldTabTopdownMapMapNameTextbox->Enabled = false;
			WorldTabTopdownMapStandardRadioButton->Enabled = false;
			WorldTabTopdownMapIsometricRadioButton->Enabled = false;
			WorldTabEnableFOWCheckBox->Enabled = false;
			WorldTabFOWRadiusComboBox->Enabled = false;
			WorldTabFOWFlashlightCheckBox->Enabled = false;
			WorldTabTopdownMapCreateMapButton->Enabled = false;
		}

		void SwitchToNewPlatformerMapGroupbox()
		{
			NewTopdownMapGroupbox->Hide();
			NewPlatformerMapGroupbox->Show();
			NewPlatformerMapGroupbox->Location = System::Drawing::Point(18, 248);

			// Disable until a world is loaded or created
			WorldTabPlatformerCreateMapButton->Enabled = false;
			WorldTabPlatformerMapWidthComboBox->Enabled = false;
			WorldTabPlatformerMapHeightComboBox->Enabled = false;
			WorldTabPlatformerWorldNameComboBox->Enabled = false;
			WorldTabPlatformerMapNameTextBox->Enabled = false;
		}

		void EnableTopdownCreateButtons()
		{
			WorldTabTopdownMapSizeComboBox->Enabled = true;
			WorldTabTopdownMapWorldNameComboBox->Enabled = true;
			WorldTabTopdownMapMapNameTextbox->Enabled = true;
			WorldTabTopdownMapStandardRadioButton->Enabled = true;
			WorldTabTopdownMapIsometricRadioButton->Enabled = true;
			WorldTabEnableFOWCheckBox->Enabled = true;
			WorldTabTopdownMapCreateMapButton->Enabled = true;
		}

		void EnablePlatformerCreateButtons()
		{
			WorldTabPlatformerCreateMapButton->Enabled = true;
			WorldTabPlatformerMapWidthComboBox->Enabled = true;
			WorldTabPlatformerMapHeightComboBox->Enabled = true;
			WorldTabPlatformerWorldNameComboBox->Enabled = true;
			WorldTabPlatformerMapNameTextBox->Enabled = true;
		}

		void EnableTopdownCreateFOWButtons()
		{
			if (WorldTabEnableFOWCheckBox->Checked)
			{
				WorldTabFOWRadiusComboBox->Enabled = true;
				WorldTabFOWFlashlightCheckBox->Enabled = true;
				WorldTabFOWUncoveredCheckBox->Enabled = true;
			}
			else
			{
				WorldTabFOWRadiusComboBox->Enabled = false;
				WorldTabFOWFlashlightCheckBox->Enabled = false;
				WorldTabFOWUncoveredCheckBox->Enabled = false;
			}
		}

		void MapTabUpdateTopdownCreateFOWButtons()
		{
			if (TopdownMapEnableFOWMapTabCheckBox->Checked)
			{
				TopdownMapFOWRadiusMapTabCB->Enabled = true;
				TopdownMapFOWFlashlightMapTabCheckBox->Enabled = true;
				TopdownMapFOWUncoveredMapTabCheckBox->Enabled = true;
			}
			else
			{
				TopdownMapFOWRadiusMapTabCB->Enabled = false;
				TopdownMapFOWFlashlightMapTabCheckBox->Enabled = false;
				TopdownMapFOWUncoveredMapTabCheckBox->Enabled = false;
			}
		}

		void CheckGameType()
		{
			if (mEngineOptions->m_GameFolderName != "NONE")
			{
				switch (mEngineOptions->m_GameType2D)
				{
					case TOPDOWN:
					{
						GameTypeTopdownRadioButton->Checked = true;
						GameTypePlatformerRadioButton->Checked = false;
						SwitchToNewTopdownMapGroupbox();
						SwitchToTileModeTopdown();
						FillTopdownMapTabVision();

						MapTabPlatformerMapGroupBox->Hide();
						MapTabPlatformerMapGroupBox->Enabled = false;
						MapTabTopdownMapGroupBox->Show();
						MapTabTopdownMapGroupBox->Location = System::Drawing::Point(6, 6);
					}
					break;
					case PLATFORMER:
					{
					   GameTypeTopdownRadioButton->Checked = false;
					   GameTypePlatformerRadioButton->Checked = true;
					   SwitchToNewPlatformerMapGroupbox();
					   SwitchToTileModePlatformer();

					   MapTabTopdownMapGroupBox->Hide();
					   MapTabTopdownMapGroupBox->Enabled = false;
					   MapTabPlatformerMapGroupBox->Show();
					   MapTabPlatformerMapGroupBox->Location = System::Drawing::Point(6, 6);
					}
					break;
				}

				GameTypeGroupBox->Enabled = false;
			}
		}
		
		void FillTopdownMapSizeComboBox()
		{
			// Example: NxM means N sections x M sections
			//			So 3x3 map is 3 sections x 3 sections
			//			and each section contains 12x8 tiles
			//			so that is 864 tiles in the map.
			WorldTabTopdownMapSizeComboBox->Items->Add("1x1");
			WorldTabTopdownMapSizeComboBox->Items->Add("3x3");
			WorldTabTopdownMapSizeComboBox->Items->Add("6x6");
			WorldTabTopdownMapSizeComboBox->Items->Add("8x8");
			WorldTabTopdownMapSizeComboBox->Items->Add("10x10");
			WorldTabTopdownMapSizeComboBox->Items->Add("20x20");
			WorldTabTopdownMapSizeComboBox->Items->Add("30x30");
			WorldTabTopdownMapSizeComboBox->Text = "1x1";
		}
		
		void FillTopdownFOWRadiusComboBox()
		{
			WorldTabFOWRadiusComboBox->Items->Add("3");
			WorldTabFOWRadiusComboBox->Items->Add("4");
			WorldTabFOWRadiusComboBox->Items->Add("5");
			WorldTabFOWRadiusComboBox->Items->Add("6");
			WorldTabFOWRadiusComboBox->Items->Add("7");
			WorldTabFOWRadiusComboBox->Items->Add("8");
			WorldTabFOWRadiusComboBox->Items->Add("9");
			WorldTabFOWRadiusComboBox->Text = "5";

			TopdownMapFOWRadiusMapTabCB->Items->Add("3");
			TopdownMapFOWRadiusMapTabCB->Items->Add("4");
			TopdownMapFOWRadiusMapTabCB->Items->Add("5");
			TopdownMapFOWRadiusMapTabCB->Items->Add("6");
			TopdownMapFOWRadiusMapTabCB->Items->Add("7");
			TopdownMapFOWRadiusMapTabCB->Items->Add("8");
			TopdownMapFOWRadiusMapTabCB->Items->Add("9");
			TopdownMapFOWRadiusMapTabCB->Text = "5";
		}
		
		void FillPlatformerMapSizeComboBox()
		{
			WorldTabPlatformerMapWidthComboBox->Items->Add("32");
			WorldTabPlatformerMapWidthComboBox->Items->Add("64");
			WorldTabPlatformerMapWidthComboBox->Items->Add("128");
			WorldTabPlatformerMapWidthComboBox->Items->Add("256");
			WorldTabPlatformerMapWidthComboBox->Items->Add("512");
			WorldTabPlatformerMapWidthComboBox->Items->Add("1024");
			WorldTabPlatformerMapWidthComboBox->Items->Add("2048");
			WorldTabPlatformerMapWidthComboBox->Items->Add("4096");
			WorldTabPlatformerMapWidthComboBox->Text = "32";

			WorldTabPlatformerMapHeightComboBox->Items->Add("32");
			WorldTabPlatformerMapHeightComboBox->Items->Add("64");
			WorldTabPlatformerMapHeightComboBox->Items->Add("128");
			WorldTabPlatformerMapHeightComboBox->Items->Add("256");
			WorldTabPlatformerMapHeightComboBox->Items->Add("512");
			WorldTabPlatformerMapHeightComboBox->Items->Add("1024");
			WorldTabPlatformerMapHeightComboBox->Items->Add("2048");
			WorldTabPlatformerMapHeightComboBox->Items->Add("4096");
			WorldTabPlatformerMapHeightComboBox->Text = "32";
		}

		void CheckSpriteModeGridType()
		{
			if (SpriteRadioButton->Checked)
			{
				GridTypeGroupBox->Enabled = true;
			}
			else
			{
				GridTypeGroupBox->Enabled = false;
			}
		}

		void CheckSectionPickerMode()
		{
			switch (mEngineOptions->m_GameType2D)
			{
				case TOPDOWN:
				{
					if (mEditorHelper2D->GetMapEditorSystem2D()->IsInSectionPicker())
					{
						TileRadioButton->Checked = true;
						TileRadioButton->Enabled = true;
						SpriteRadioButton->Enabled = false;
						HardnessRadioButton->Enabled = false;
						mEditorHelper2D->GetMapEditorSystem2D()->SwitchToEditModeTiles();
						mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeRegular();
					}
					else
					{
						TileRadioButton->Enabled = true;
						SpriteRadioButton->Enabled = true;
						HardnessRadioButton->Enabled = true;
					}
				}
			}
		}

		void FillTopdownMapTabVision()
		{
			TopdownMapVisionComboBox->Items->Add("0");
			TopdownMapVisionComboBox->Items->Add("1");
			TopdownMapVisionComboBox->Items->Add("2");
			TopdownMapVisionComboBox->Items->Add("3");
			TopdownMapVisionComboBox->Items->Add("4");
			TopdownMapVisionComboBox->Items->Add("5");
			TopdownMapVisionComboBox->Items->Add("6");
			TopdownMapVisionComboBox->Items->Add("7");
			TopdownMapVisionComboBox->Items->Add("8");
			TopdownMapVisionComboBox->Items->Add("9");
			TopdownMapVisionComboBox->Items->Add("10");
			TopdownMapVisionComboBox->Items->Add("11");
			TopdownMapVisionComboBox->Items->Add("12");
			TopdownMapVisionComboBox->Items->Add("13");
			TopdownMapVisionComboBox->Items->Add("14");
			TopdownMapVisionComboBox->Items->Add("15");
			TopdownMapVisionComboBox->Items->Add("16");
			TopdownMapVisionComboBox->Items->Add("17");
			TopdownMapVisionComboBox->Items->Add("18");
			TopdownMapVisionComboBox->Items->Add("19");
			TopdownMapVisionComboBox->Items->Add("20");
			TopdownMapVisionComboBox->Items->Add("21");
			TopdownMapVisionComboBox->Items->Add("22");
			TopdownMapVisionComboBox->Items->Add("23");
			TopdownMapVisionComboBox->Items->Add("24");
			TopdownMapVisionComboBox->Items->Add("25");
			TopdownMapVisionComboBox->Text = "0";
		}

		void SetTopdownMapProperties()
		{
			int width = 0;
			int height = 0;
			int vision = 0;
			string music = "";
			bool fow = false;
			int fowRadius = 5;
			bool fowFlashlight = false;

			mEditorHelper2D->GetMapEditorSystem2D()->GetMapWidthAttribute(width);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapHeightAttribute(height);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapVisionAttribute(vision);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapMusicAttribute(music);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapFOWAttribute(fow);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapFOWRadiusAttribute(fowRadius);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapFOWFlashlightAttribute(fowFlashlight);

			string fowRadius_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(fowRadius);
			string vision_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(vision);

			// Need to create a CheckedChanged event for TopdownMapHardnessCheckbox
			
			TopdownMapNameTextbox->Text = m_ActiveMapName;
			//TopdownMapVisionTrackbar->Value = vision;
			TopdownMapVisionComboBox->Text = gcnew String(vision_str.c_str());
			TopdownMapMusicTextbox->Text = gcnew String(music.c_str());
			//TopdownMapHardnessCheckbox->Checked = ;
			TopdownMapEnableFOWMapTabCheckBox->Checked = fow;
			TopdownMapFOWRadiusMapTabCB->Text = gcnew String(fowRadius_str.c_str());
			TopdownMapFOWFlashlightMapTabCheckBox->Checked = fowFlashlight;
			
			// Need to create a CheckedChanged event for the Yes/No FOW Toggle here
			if (TopdownMapEnableFOWMapTabCheckBox->Checked)
			{
				TopdownMapFOWRadiusMapTabCB->Enabled = true;
				TopdownMapFOWFlashlightMapTabCheckBox->Enabled = true;
				TopdownMapFOWUncoveredMapTabCheckBox->Enabled = true;
			}
			else
			{
				TopdownMapFOWRadiusMapTabCB->Enabled = false;
				TopdownMapFOWFlashlightMapTabCheckBox->Enabled = false;
				TopdownMapFOWUncoveredMapTabCheckBox->Enabled = false;
			}
			
			UpdateTopdownMapStats();
		}
		
		void UpdateTopdownMapStats()
		{
			int numSpritesRendered = 0;
			int totalSprites = 0;
			int tileSize = mEngineOptions->TILE_SIZE;
			int num_tiles_in_section = (mEngineOptions->ROW_SIZE * mEngineOptions->COL_SIZE);
			int num_sections = mEngineOptions->SECTION_COL_SIZE * mEngineOptions->SECTION_ROW_SIZE;
			int num_tiles = num_sections * num_tiles_in_section;
			
			mEditorHelper2D->GetMapEditorSystem2D()->GetNumSpritesRendered(numSpritesRendered);
			mEditorHelper2D->GetMapEditorSystem2D()->GetSpriteTotal(totalSprites);
			
			string numSpritesRendered_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(numSpritesRendered);
			string totalSprites_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(totalSprites);
			string tileSize_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(tileSize);
			string num_tiles_in_section_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(num_tiles_in_section);
			string num_sections_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(num_sections);
			string num_tiles_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(num_tiles);
			string row_size_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(mEngineOptions->ROW_SIZE);
			string col_size_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(mEngineOptions->COL_SIZE);
			string sec_row_size_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(mEngineOptions->SECTION_ROW_SIZE);
			string sec_col_size_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(mEngineOptions->SECTION_COL_SIZE);
			
			TileSizeLabel->Text = gcnew String(tileSize_str.c_str());
			NumTilesLabel->Text = gcnew String(num_tiles_str.c_str());
			NumSectionsLabel->Text = gcnew String(num_sections_str.c_str());
			MapRowSizeLabel->Text = gcnew String(row_size_str.c_str());
			MapColSizeLabel->Text = gcnew String(col_size_str.c_str());
			SectionRowSizeLabel->Text = gcnew String(sec_row_size_str.c_str());
			SectionColSizeLabel->Text = gcnew String(sec_col_size_str.c_str());
			SpritesRenderedLabel->Text = gcnew String(numSpritesRendered_str.c_str());
			TotalSpritesLabel->Text = gcnew String(totalSprites_str.c_str());
		}

		void SetPlatformerMapProperties()
		{
			string music = "";

			mEditorHelper2D->GetMapEditorSystem2D()->GetMapMusicAttribute(music);
			
			// Need to create a CheckedChanged event for PlatformerMapHardnessCheckbox
			
			PlatformerMapEditNameTextbox->Text = m_ActiveMapName;
			PlatformerMapEditMusicTextbox->Text = gcnew String(music.c_str());
			//PlatformerMapHardnessCheckbox->Checked = ;
			
			UpdatePlatformerMapStats();
		}
		
		void UpdatePlatformerMapStats()
		{
			int width = 0;
			int height = 0;
			int numSpritesRendered = 0;
			int totalSprites = 0;
			int visibleTiles = 0;
			int tileSize = mEngineOptions->TILE_SIZE;
			int num_tiles = (width * tileSize) + (height * tileSize);
			
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapWidthAttribute(width);
			mEditorHelper2D->GetMapEditorSystem2D()->GetMapHeightAttribute(height);
			mEditorHelper2D->GetMapEditorSystem2D()->GetNumSpritesRendered(numSpritesRendered);
			mEditorHelper2D->GetMapEditorSystem2D()->GetSpriteTotal(totalSprites);
			mEditorHelper2D->GetMapEditorSystem2D()->VisibleTileCount(visibleTiles);
			
			string numSpritesRendered_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(numSpritesRendered);
			string totalSprites_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(totalSprites);
			string tileSize_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(tileSize);
			string num_tiles_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(num_tiles);
			string visibleTiles_str = ZShadeSandboxGlobal::Convert::ConvertToString<int>(visibleTiles);
			
			PlatformerMapTileSizeLabel->Text = gcnew String(tileSize_str.c_str());
			PlatformerMapNumTilesLabel->Text = gcnew String(num_tiles_str.c_str());
			PlatformerMapVisibleTilesLabel->Text = gcnew String(visibleTiles_str.c_str());
			PlatformerMapSpritesRenderedLabel->Text = gcnew String(numSpritesRendered_str.c_str());
			PlatformerMapTotalSpritesLabel->Text = gcnew String(totalSprites_str.c_str());
		}
		
		/*
		Sprite Tab GUI Components

		Place the SpriteGroupBox for Topdown and Platformer at Location 7,168

		SpriteTabResizeGroupBox
		SpriteTabRepositionRadioButton
		SpriteTabResizeRadioButton
		SpriteTabNoUpdateRadioButton
		SpriteTabGoFastCheckbox
		SpriteTabInstantCheckbox
		SpriteTabResizeUpButton
		SpriteTabResizeLeftButton
		SpriteTabResizeRightButton
		SpriteTabResizeDownButton
		
		SpriteTabTopdownSpriteGroupBox
		SpriteTabTopdownXLabel
		SpriteTabTopdownYLabel
		SpriteTabTopdownVisionComboBox
		SpriteTabTopdownDepthTextbox
		SpriteTabTopdownTextureNameTextbox
		SpriteTabTopdownAnimationNameTextbox
		SpriteTabTopdownDefaultSequenceTextbox
		SpriteTabTopdownAnimationSpeedTextbox
		SpriteTabTopdownCollideMsgTextbox
		SpriteTabTopdownHPMaxTextbox
		SpriteTabTopdownHPTextbox
		SpriteTabTopdownManaTextbox
		SpriteTabTopdownGoldTextbox
		SpriteTabTopdownStrengthTextbox
		SpriteTabTopdownSpeedTextbox
		SpriteTabTopdownDefenseTextbox
		SpriteTabTopdownExperienceTextbox
		SpriteTabTopdownLevelTextbox
		SpriteTabTopdownScriptTextbox
		SpriteTabTopdownScriptOpenButton
		SpriteTabTopdownTeleportSpriteYesRadioButton
		SpriteTabTopdownTeleportSpriteNoRadioButton
		SpriteTabTopdownTeleportSpriteMapNameTextbox
		SpriteTabTopdownTeleportSpriteXTextbox
		SpriteTabTopdownTeleportSpriteYTextbox
		SpriteTabTopdownBrowseTextureNameButton
		SpriteTabTopdownUpdateAnimationButton
		SpriteTabTopdownRevertAnimationButton
		SpriteTabTopdownBackgroundRadioButton
		SpriteTabTopdownEntityRadioButton
		SpriteTabTopdownInvisibleRadioButton
		SpriteTabTopdownHardYesRadioButton
		SpriteTabTopdownHardNoRadioButton
		SpriteTabTopdownNoHitYesRadioButton
		SpriteTabTopdownNoHitNoRadioButton
		SpriteTabTopdownStaticRadioButton
		SpriteTabTopdownDynamicRadioButton
		SpriteTabTopdownTouchYesRadioButton
		SpriteTabTopdownTouchNoRadioButton


		SpriteTabPlatformerSpriteGroupBox
		SpriteTabPlatformerXLabel
		SpriteTabPlatformerYLabel
		SpriteTabPlatformerTextureNameTextbox
		SpriteTabPlatformerAnimationNameTextbox
		SpriteTabPlatformerDefaultSequenceTextbox
		SpriteTabPlatformerAnimationSpeedTextbox
		SpriteTabPlatformerCollideMsgTextbox
		SpriteTabPlatformerHPMaxTextbox
		SpriteTabPlatformerHPTextbox
		SpriteTabPlatformerManaTextbox
		SpriteTabPlatformerGoldTextbox
		SpriteTabPlatformerStrengthTextbox
		SpriteTabPlatformerSpeedTextbox
		SpriteTabPlatformerDefenseTextbox
		SpriteTabPlatformerExperienceTextbox
		SpriteTabPlatformerLevelTextbox
		SpriteTabPlatformerScriptTextbox
		SpriteTabPlatformerScriptOpenButton
		SpriteTabPlatformerTeleportSpriteYesRadioButton
		SpriteTabPlatformerTeleportSpriteNoRadioButton
		SpriteTabPlatformerTeleportSpriteMapNameTextbox
		SpriteTabPlatformerTeleportSpriteXTextbox
		SpriteTabPlatformerTeleportSpriteYTextbox
		SpriteTabPlatformerBrowseTextureNameButton
		SpriteTabPlatformerUpdateAnimationButton
		SpriteTabPlatformerRevertAnimationButton
		SpriteTabPlatformerBackgroundRadioButton
		SpriteTabPlatformerEntityRadioButton
		SpriteTabPlatformerInvisibleRadioButton
		SpriteTabPlatformerHardYesRadioButton
		SpriteTabPlatformerHardNoRadioButton
		SpriteTabPlatformerNoHitYesRadioButton
		SpriteTabPlatformerHoHitNoRadioButton
		SpriteTabPlatformerStaticRadioButton
		SpriteTabPlatformerDynamicRadioButton
		SpriteTabPlatformerTouchYesRadioButton
		SpriteTabPlatformerTouchNoRadioButton
		*/
		
		void SetSpriteTabSelectedSpriteAttributes()
		{
			// Only update what is seen in the interface when the sprite is clicked on
			if (mEditorHelper2D->GetMapEditorSystem2D()->WasTileOrSpriteClicked())
			{
				AISprite* selectedSprite = mEditorHelper2D->GetMapEditorSystem2D()->GetSelectedSprite();
				
				if (selectedSprite)
				{
					if (mEngineOptions->m_GameFolderName != "NONE")
					{
						switch (mEngineOptions->m_GameType2D)
						{
							case TOPDOWN:
							{
								SpriteTabTopdownXLabel->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->X())).c_str());
								SpriteTabTopdownYLabel->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Y())).c_str());
								SpriteTabTopdownVisionComboBox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Vision())).c_str());
								SpriteTabTopdownDepthTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Depth())).c_str());
								SpriteTabTopdownTextureNameTextbox->Text = gcnew String((selectedSprite->BaseTextureFilename()).c_str());
								SpriteTabTopdownAnimationNameTextbox->Text = gcnew String((selectedSprite->AnimationProfileName()).c_str());
								SpriteTabTopdownDefaultSequenceTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->DefaultSequence())).c_str());
								SpriteTabTopdownAnimationSpeedTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->AnimationSpeed())).c_str());
								SpriteTabTopdownCollideMsgTextbox->Text = gcnew String((selectedSprite->CollideMsg()).c_str());
								SpriteTabTopdownHPMaxTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->HPMax())).c_str());
								SpriteTabTopdownHPTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->HP())).c_str());
								SpriteTabTopdownManaTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Mana())).c_str());
								SpriteTabTopdownGoldTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Gold())).c_str());
								SpriteTabTopdownStrengthTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Strength())).c_str());
								// x and y are the same speed so just grab one of them
								SpriteTabTopdownSpeedTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->SpeedX())).c_str());
								//SpriteTabTopdownMaxSpeedTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->MaxSpeedX())).c_str());
								SpriteTabTopdownDefenseTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Defense())).c_str());
								SpriteTabTopdownExperienceTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Experience())).c_str());
								SpriteTabTopdownLevelTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Level())).c_str());
								SpriteTabTopdownScriptTextbox->Text = gcnew String((selectedSprite->ScriptName()).c_str());
								SpriteTabTopdownTeleportSpriteYesRadioButton->Checked = selectedSprite->CanTeleport();
								SpriteTabTopdownTeleportSpriteNoRadioButton->Checked = !selectedSprite->CanTeleport();
								SpriteTabTopdownTeleportSpriteMapNameTextbox->Text = gcnew String((selectedSprite->TeleportMapName()).c_str());
								SpriteTabTopdownTeleportSpriteXTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->TeleportX())).c_str());
								SpriteTabTopdownTeleportSpriteYTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->TeleportY())).c_str());
								SpriteTabTopdownBackgroundRadioButton->Checked = selectedSprite->IsBackgroundSprite();
								SpriteTabTopdownEntityRadioButton->Checked = selectedSprite->IsEntitySprite();
								SpriteTabTopdownInvisibleRadioButton->Checked = selectedSprite->IsInvisibleSprite();
								SpriteTabTopdownHardCheckbox->Checked = selectedSprite->IsHard();
								SpriteTabTopdownNoHitCheckbox->Checked = selectedSprite->IsNoHit();
								bool isstatic = (selectedSprite->EPhysicsType() == STATIC) ? true : false;
								SpriteTabTopdownStaticRadioButton->Checked = isstatic;
								SpriteTabTopdownDynamicRadioButton->Checked = !isstatic;
								SpriteTabTopdownTouchCheckbox->Checked = selectedSprite->IsTouch();
								SpriteTabTopdownAlwaysSeenByPlayerCheckbox->Checked = selectedSprite->AlwaysSeenByPlayer();
							}
							break;
							case PLATFORMER:
							{
								SpriteTabPlatformerXLabel->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->X())).c_str());
								SpriteTabPlatformerYLabel->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Y())).c_str());
								SpriteTabPlatformerTextureNameTextbox->Text = gcnew String((selectedSprite->BaseTextureFilename()).c_str());
								SpriteTabPlatformerAnimationNameTextbox->Text = gcnew String((selectedSprite->AnimationProfileName()).c_str());
								SpriteTabPlatformerDefaultSequenceTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->DefaultSequence())).c_str());
								SpriteTabPlatformerAnimationSpeedTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->AnimationSpeed())).c_str());
								SpriteTabPlatformerCollideMsgTextbox->Text = gcnew String((selectedSprite->CollideMsg()).c_str());
								SpriteTabPlatformerHPMaxTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->HPMax())).c_str());
								SpriteTabPlatformerHPTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->HP())).c_str());
								SpriteTabPlatformerManaTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Mana())).c_str());
								SpriteTabPlatformerGoldTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Gold())).c_str());
								SpriteTabPlatformerStrengthTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Strength())).c_str());
								// In a platformer map we don't need to update speed Y because it is for gravity
								SpriteTabPlatformerSpeedTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->SpeedX())).c_str());
								//SpriteTabPlatformerMaxSpeedTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->MaxSpeedX())).c_str());
								SpriteTabPlatformerDefenseTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Defense())).c_str());
								SpriteTabPlatformerExperienceTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Experience())).c_str());
								SpriteTabPlatformerLevelTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->Level())).c_str());
								SpriteTabPlatformerScriptTextbox->Text = gcnew String((selectedSprite->ScriptName()).c_str());
								SpriteTabPlatformerTeleportSpriteYesRadioButton->Checked = selectedSprite->CanTeleport();
								SpriteTabPlatformerTeleportSpriteNoRadioButton->Checked = !selectedSprite->CanTeleport();
								SpriteTabPlatformerTeleportSpriteMapNameTextbox->Text = gcnew String((selectedSprite->TeleportMapName()).c_str());
								SpriteTabPlatformerTeleportSpriteXTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->TeleportX())).c_str());
								SpriteTabPlatformerTeleportSpriteYTextbox->Text = gcnew String((ZShadeSandboxGlobal::Convert::ConvertIntToString(selectedSprite->TeleportY())).c_str());
								SpriteTabPlatformerBackgroundRadioButton->Checked = selectedSprite->IsBackgroundSprite();
								SpriteTabPlatformerEntityRadioButton->Checked = selectedSprite->IsEntitySprite();
								SpriteTabPlatformerInvisibleRadioButton->Checked = selectedSprite->IsInvisibleSprite();
								SpriteTabPlatformerHardCheckbox->Checked = selectedSprite->IsHard();
								SpriteTabPlatformerNoHitCheckbox->Checked = selectedSprite->IsNoHit();
								bool isstatic = (selectedSprite->EPhysicsType() == STATIC) ? true : false;
								SpriteTabPlatformerStaticRadioButton->Checked = isstatic;
								SpriteTabPlatformerDynamicRadioButton->Checked = !isstatic;
								SpriteTabPlatformerTouchCheckbox->Checked = selectedSprite->IsTouch();
								SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->Checked = selectedSprite->AlwaysSeenByPlayer();
							}
							break;
						}
					}
				}
				
				// Reset flag since attributes have been updated
				mEditorHelper2D->GetMapEditorSystem2D()->WasTileOrSpriteClicked() = false;
			}
		}
		
		void SetSpriteTabDefaults()
		{
			if (mEngineOptions->m_GameFolderName != "NONE")
			{
				switch (mEngineOptions->m_GameType2D)
				{
					case TOPDOWN:
					{
						SpriteTabPlatformerSpriteGroupBox->Hide();
						SpriteTabPlatformerSpriteGroupBox->Enabled = false;
						SpriteTabTopdownSpriteGroupBox->Show();
						SpriteTabTopdownSpriteGroupBox->Location = System::Drawing::Point(7, 168);
						
						SpriteTabTopdownVisionComboBox->Items->Add("0");
						SpriteTabTopdownVisionComboBox->Items->Add("1");
						SpriteTabTopdownVisionComboBox->Items->Add("2");
						SpriteTabTopdownVisionComboBox->Items->Add("3");
						SpriteTabTopdownVisionComboBox->Items->Add("4");
						SpriteTabTopdownVisionComboBox->Items->Add("5");
						SpriteTabTopdownVisionComboBox->Items->Add("6");
						SpriteTabTopdownVisionComboBox->Items->Add("7");
						SpriteTabTopdownVisionComboBox->Items->Add("8");
						SpriteTabTopdownVisionComboBox->Items->Add("9");
						SpriteTabTopdownVisionComboBox->Items->Add("10");
						SpriteTabTopdownVisionComboBox->Items->Add("11");
						SpriteTabTopdownVisionComboBox->Items->Add("12");
						SpriteTabTopdownVisionComboBox->Items->Add("13");
						SpriteTabTopdownVisionComboBox->Items->Add("14");
						SpriteTabTopdownVisionComboBox->Items->Add("15");
						SpriteTabTopdownVisionComboBox->Items->Add("16");
						SpriteTabTopdownVisionComboBox->Items->Add("17");
						SpriteTabTopdownVisionComboBox->Items->Add("18");
						SpriteTabTopdownVisionComboBox->Items->Add("19");
						SpriteTabTopdownVisionComboBox->Items->Add("20");
						SpriteTabTopdownVisionComboBox->Items->Add("21");
						SpriteTabTopdownVisionComboBox->Items->Add("22");
						SpriteTabTopdownVisionComboBox->Items->Add("23");
						SpriteTabTopdownVisionComboBox->Items->Add("24");
						SpriteTabTopdownVisionComboBox->Items->Add("25");
						SpriteTabTopdownVisionComboBox->Text = "0";

						SpriteTabTopdownTextureNameTextbox->Enabled = false;
					}
					break;
					case PLATFORMER:
					{
						SpriteTabTopdownSpriteGroupBox->Hide();
						SpriteTabTopdownSpriteGroupBox->Enabled = false;
						SpriteTabPlatformerSpriteGroupBox->Show();
						SpriteTabPlatformerSpriteGroupBox->Location = System::Drawing::Point(7, 168);

						SpriteTabPlatformerTextureNameTextbox->Enabled = false;
					}
					break;
				}
			}
			
			CheckSpriteTabTeleportSpriteStatus();
		}
		
		void CheckSpriteTabTeleportSpriteStatus()
		{
			if (mEngineOptions->m_GameFolderName != "NONE")
			{
				switch (mEngineOptions->m_GameType2D)
				{
					case TOPDOWN:
					{
						if (SpriteTabTopdownTeleportSpriteYesRadioButton->Checked)
						{
							SpriteTabTopdownTeleportSpriteMapNameTextbox->Enabled = true;
							SpriteTabTopdownTeleportSpriteXTextbox->Enabled = true;
							SpriteTabTopdownTeleportSpriteYTextbox->Enabled = true;
						}

						if (SpriteTabTopdownTeleportSpriteNoRadioButton->Checked)
						{
							SpriteTabTopdownTeleportSpriteMapNameTextbox->Enabled = false;
							SpriteTabTopdownTeleportSpriteXTextbox->Enabled = false;
							SpriteTabTopdownTeleportSpriteYTextbox->Enabled = false;
						}
					}
					break;
					case PLATFORMER:
					{
						if (SpriteTabPlatformerTeleportSpriteYesRadioButton->Checked)
						{
							SpriteTabPlatformerTeleportSpriteMapNameTextbox->Enabled = true;
							SpriteTabPlatformerTeleportSpriteXTextbox->Enabled = true;
							SpriteTabPlatformerTeleportSpriteYTextbox->Enabled = true;
						}

						if (SpriteTabPlatformerTeleportSpriteNoRadioButton->Checked)
						{
							SpriteTabPlatformerTeleportSpriteMapNameTextbox->Enabled = false;
							SpriteTabPlatformerTeleportSpriteXTextbox->Enabled = false;
							SpriteTabPlatformerTeleportSpriteYTextbox->Enabled = false;
						}
					}
					break;
				}
			}
		}

		// http://www.codeproject.com/Articles/7704/Arrays-in-C-CLI
		
		void FillTileCache()
		{
			//TileCacheTabCacheList->Items->Clear();
			vector<string> cacheList;
			mEditorHelper2D->FillTileCache(cacheList);
			
			mTileCacheImages = gcnew array<System::Windows::Forms::PictureBox^>(cacheList.size());
			mTileCacheImageNames = gcnew array<System::String^>(cacheList.size());

			int initialPos = 6;
			int newX = initialPos;
			int newY = initialPos;
			int imageRowSize = 6;
			int imageColSize = 7;
			int imageSize = 64;

			for (int i = 0; i < cacheList.size(); i++)
			{
				string fullImagePath = mEditorHelper2D->FullTileImagePath(cacheList[i]);
				
				System::Windows::Forms::PictureBox^ image = (gcnew System::Windows::Forms::PictureBox());
				string name = "TileImage";
				name += ZShadeSandboxGlobal::Convert::ConvertIntToString(i);
				image->Name = gcnew String(name.c_str());
				image->Size = System::Drawing::Size(imageSize, imageSize);
				image->TabIndex = 5;// Need to figure out tab index
				image->TabStop = false;
				image->BackgroundImage = System::Drawing::Image::FromFile(gcnew String(fullImagePath.c_str()));
				//image->SizeMode = PictureBoxSizeMode::AutoSize;
				image->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ZShadeSandbox::ZShadeSandboxEditor2D::TileCachePictureBox_MouseDown);
				//image->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ZShadeSandbox::ZShadeSandboxEditor2D::TileCachePictureBox_MouseMove);
				
				mTileCacheImageNames[i] = gcnew System::String(cacheList[i].c_str());

				// Set the new location of this image
				image->Location = System::Drawing::Point(newX, newY);
				
				newX += imageSize;
				
				if (newX > (imageRowSize * imageSize))
				{
					newX = initialPos;
					newY += imageSize;
				}
				
				mTileCacheImages[i] = image;
			}
			
			this->mTileCacheListPanel = gcnew System::Windows::Forms::Panel();

			// Add images to a groupbox
			for (int i = 0; i < cacheList.size(); i++)
			{
				this->mTileCacheListPanel->Controls->Add(mTileCacheImages[i]);
			}
			
			this->mTileCacheListPanel->AutoScroll = true;
			this->mTileCacheListPanel->Location = System::Drawing::Point(initialPos, initialPos);
			this->mTileCacheListPanel->Name = L"TileCacheListPanel";
			this->mTileCacheListPanel->Size = System::Drawing::Size((imageRowSize * imageSize) + imageSize / 2, (imageColSize * imageSize) + imageSize / 2);
			this->mTileCacheListPanel->TabIndex = 17;// Need to figure out tab index
			this->mTileCacheListPanel->TabStop = false;
			this->mTileCacheListPanel->Text = L"";
			this->mTileCacheListPanel->ResumeLayout(false);
			this->mTileCacheListPanel->PerformLayout();
			
			this->TileCacheTab->Controls->Add(this->mTileCacheListPanel);
			this->TileCacheTab->ResumeLayout(false);
			this->TileCacheTab->PerformLayout();
		}
		
		void FillSpriteCache()
		{
			//SpriteCacheTabCacheList->Items->Clear();
			vector<string> cacheList;
			mEditorHelper2D->FillSpriteCache(cacheList);
			
			mSpriteCacheImages = gcnew array<System::Windows::Forms::PictureBox^>(cacheList.size());
			mSpriteCacheImageNames = gcnew array<System::String^>(cacheList.size());
			
			int initialPos = 6;
			int newX = initialPos;
			int newY = initialPos;
			int imageRowSize = 6;
			int imageColSize = 7;
			int imageSize = 64;
			
			for (int i = 0; i < cacheList.size(); i++)
			{
				string fullImagePath = mEditorHelper2D->FullSpriteImagePath(cacheList[i]);
				
				System::Windows::Forms::PictureBox^ image = (gcnew System::Windows::Forms::PictureBox());
				string name = "SpriteImage";
				name += ZShadeSandboxGlobal::Convert::ConvertIntToString(i);
				image->Name = gcnew String(name.c_str());
				image->Size = System::Drawing::Size(imageSize, imageSize);
				image->TabIndex = 5;
				image->TabStop = false;
				image->BackgroundImage = System::Drawing::Image::FromFile(gcnew String(fullImagePath.c_str()));
				image->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ZShadeSandbox::ZShadeSandboxEditor2D::SpriteCachePictureBox_MouseDown);
				//image->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ZShadeSandbox::ZShadeSandboxEditor2D::SpriteCachePictureBox_MouseMove);
				
				mSpriteCacheImageNames[i] = gcnew System::String(cacheList[i].c_str());

				// Set the new location of this image
				image->Location = System::Drawing::Point(newX, newY);
				
				newX += imageSize;
				
				if (newX > (imageRowSize * imageSize))
				{
					newX = initialPos;
					newY += imageSize;
				}
				
				mSpriteCacheImages[i] = image;
			}
			
			this->mSpriteCacheListPanel = gcnew System::Windows::Forms::Panel();

			// Add images to a groupbox
			for (int i = 0; i < cacheList.size(); i++)
			{
				this->mSpriteCacheListPanel->Controls->Add(mSpriteCacheImages[i]);
			}
			
			this->mSpriteCacheListPanel->AutoScroll = true;
			this->mSpriteCacheListPanel->Location = System::Drawing::Point(initialPos, initialPos);
			this->mSpriteCacheListPanel->Name = L"SpriteCacheListPanel";
			this->mSpriteCacheListPanel->Size = System::Drawing::Size((imageRowSize * imageSize) + imageSize / 2, (imageColSize * imageSize) + imageSize / 2);
			this->mSpriteCacheListPanel->TabIndex = 17;
			this->mSpriteCacheListPanel->TabStop = false;
			this->mSpriteCacheListPanel->Text = L"";
			this->mSpriteCacheListPanel->ResumeLayout(false);
			this->mSpriteCacheListPanel->PerformLayout();
			
			this->SpriteCacheTab->Controls->Add(this->mSpriteCacheListPanel);
			this->SpriteCacheTab->ResumeLayout(false);
			this->SpriteCacheTab->PerformLayout();
		}
		
		System::Void TileCachePictureBox_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			for (int i = 0; i < mTileCacheImages->Length; i++)
			{
				if (mTileCacheImages[i] == sender)
				{
					m_SelectedTileImage = mTileCacheImageNames[i];

					TileCacheTabSelectedTileNameLabel->Text = m_SelectedTileImage;

					// Add the selected image name to the map editor system
					mEditorHelper2D->GetMapEditorSystem2D()->SelectedTileName() = ZShadeSandbox::ConvertToStandardString(m_SelectedTileImage);
					
					mTileCacheImages[i]->Invalidate();
					
					break;
				}
			}
		}
		
		/*System::Void TileCachePictureBox_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			for (int i = 0; i < mTileCacheImages->Length; i++)
			{
				mTileCacheImages[i]->BorderStyle = System::Windows::Forms::BorderStyle::None;
				if (mTileCacheImages[i] == sender)
				{
					mTileCacheImages[i]->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
					return;
				}
			}
		}*/
		
		System::Void SpriteCachePictureBox_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			for (int i = 0; i < mSpriteCacheImages->Length; i++)
			{
				if (mSpriteCacheImages[i] == sender)
				{
					m_SelectedSpriteImage = mSpriteCacheImageNames[i];

					SpriteCacheTabSelectedSpriteLabel->Text = m_SelectedSpriteImage;

					// Add the selected image name to the map editor system
					mEditorHelper2D->GetMapEditorSystem2D()->SelectedSpriteName() = ZShadeSandbox::ConvertToStandardString(m_SelectedSpriteImage);
					
					mEditorHelper2D->GetMapEditorSystem2D()->SpriteCacheSelectionChanged() = true;
					
					mSpriteCacheImages[i]->Invalidate();
					
					break;
				}
			}
		}

		/*System::Void SpriteCachePictureBox_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			for (int i = 0; i < mSpriteCacheImages->Length; i++)
			{
				mSpriteCacheImages[i]->BorderStyle = System::Windows::Forms::BorderStyle::None;
				
				if (mSpriteCacheImages[i] == sender)
				{
					mSpriteCacheImages[i]->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
					
					break;
				}
			}
		}*/
		
		//System::Void MouseMoveXY(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		//{
		//	this->Text = "X=" + e->X.ToString() + "   " + "Y=" + e->Y.ToString();
		//}
		
		//bool MouseInControl(System::Windows::Forms::Control^ ctrl, System::Drawing::Point point)
		//{
			//ctrl->MousePosition
		//	return ctrl->Bounds.Contains(ctrl->PointToClient(point));
		//}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ZShadeSandboxEditor2D()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  ZShadeSandboxEditorSplitContainer;
	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		bool bINIContainsGameFolder;
		EditorHelper2D* mEditorHelper2D;
		bool bInitialized;
		EngineOptions* mEngineOptions;
		System::String^ m_ActiveWorldName;
		System::String^ m_ActiveMapName;
		System::String^ m_SelectedTileImage;
		System::String^ m_SelectedSpriteImage;
		bool bWorldCreated;
		bool bMapCreated;
		bool bInitLoadCache;
		HWND mToolHwnd;
		System::Windows::Forms::Panel^ mTileCacheListPanel;
		System::Windows::Forms::Panel^ mSpriteCacheListPanel;
		array<System::String^>^ mTileCacheImageNames;
		array<System::String^>^ mSpriteCacheImageNames;
		array<System::Windows::Forms::PictureBox^>^ mTileCacheImages;
		array<System::Windows::Forms::PictureBox^>^ mSpriteCacheImages;

	private: System::Windows::Forms::Timer^  StartTimer;
	private: System::Windows::Forms::TabControl^  TabControl2D;

	private: System::Windows::Forms::TabPage^  EditorTab;
	private: System::Windows::Forms::TabPage^  WorldTab;
	private: System::Windows::Forms::TabPage^  MapTab;
	private: System::Windows::Forms::TabPage^  TilesTab;
	private: System::Windows::Forms::TabPage^  SpritesTab;
	private: System::Windows::Forms::TabPage^  TileCacheTab;
	private: System::Windows::Forms::TabPage^  SpriteCacheTab;
	private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::TextBox^  GameFolderNameTextBox;


	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::RadioButton^  HardnessRadioButton;



	private: System::Windows::Forms::RadioButton^  SpriteRadioButton;

	private: System::Windows::Forms::RadioButton^  TileRadioButton;
private: System::Windows::Forms::RadioButton^  RemoveTexturePlatformerRadioButton;






private: System::Windows::Forms::RadioButton^  MultiStampPlatformerRadioButton;

private: System::Windows::Forms::RadioButton^  TileModePlatformerRadioButton;

private: System::Windows::Forms::RadioButton^  TileModeTopdownMultiStampRadioButton;

private: System::Windows::Forms::RadioButton^  TileModeTopdownRegularRadioButton;


	private: System::Windows::Forms::GroupBox^  TileModeTopdownGroupBox;

	private: System::Windows::Forms::GroupBox^  EditModeGroupBox;
	private: System::Windows::Forms::GroupBox^  TileModePlatformerGroupBox;

	private: System::Windows::Forms::GroupBox^  SpriteModeGroupBox;
private: System::Windows::Forms::RadioButton^  SpriteModeSelectRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteModeSizeRadioButton;



private: System::Windows::Forms::RadioButton^  SpriteModeMoveRadioButton;
private: System::Windows::Forms::RadioButton^  SpriteModePlaceRadioButton;


private: System::Windows::Forms::RadioButton^  SpriteModeDeleteRadioButton;

	private: System::Windows::Forms::GroupBox^  HardnessModeGroupBox;
private: System::Windows::Forms::RadioButton^  TilesHardnessRadioButton;
private: System::Windows::Forms::RadioButton^  SpritesHardnessRadioButton;



private: System::Windows::Forms::Button^  OpenGameDirectoryButton;

private: System::Windows::Forms::Button^  CreateGameDirectoryButton;

	private: System::Windows::Forms::GroupBox^  GameTypeGroupBox;
private: System::Windows::Forms::RadioButton^  GameTypeTopdownRadioButton;
private: System::Windows::Forms::RadioButton^  GameTypePlatformerRadioButton;


	private: System::Windows::Forms::Button^  HUDEditorButton;

	private: System::Windows::Forms::Button^  MenuEditorButton;
private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
private: System::Windows::Forms::Label^  GamePathLabel;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Button^  SaveGameDirectoryButton;

private: System::Windows::Forms::Button^  CloseGameDirectoryButton;

private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Button^  WorldTabSaveWorldButton;

private: System::Windows::Forms::Button^  WorldTabLoadWorldButton;

private: System::Windows::Forms::Button^  WorldTabCreateWorldButton;


private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  WorldTabStatusLabel;
private: System::Windows::Forms::GroupBox^  NewTopdownMapGroupbox;


private: System::Windows::Forms::Button^  WorldTabTopdownMapCreateMapButton;
private: System::Windows::Forms::ComboBox^  WorldTabFOWRadiusComboBox;
private: System::Windows::Forms::CheckBox^  WorldTabFOWFlashlightCheckBox;








private: System::Windows::Forms::Label^  label12;

private: System::Windows::Forms::Label^  label10;

private: System::Windows::Forms::GroupBox^  groupBox3;
private: System::Windows::Forms::RadioButton^  WorldTabTopdownMapIsometricRadioButton;
private: System::Windows::Forms::RadioButton^  WorldTabTopdownMapStandardRadioButton;


private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::ComboBox^  WorldTabTopdownMapSizeComboBox;

private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::GroupBox^  groupBox5;
private: System::Windows::Forms::ComboBox^  WorldTabTopdownMapWorldNameComboBox;
private: System::Windows::Forms::GroupBox^  NewPlatformerMapGroupbox;



private: System::Windows::Forms::TextBox^  WorldTabPlatformerMapNameTextBox;

private: System::Windows::Forms::ComboBox^  WorldTabPlatformerWorldNameComboBox;

private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Label^  label11;


private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Button^  WorldTabPlatformerCreateMapButton;

private: System::Windows::Forms::TextBox^  WorldTabTopdownMapMapNameTextbox;

private: System::Windows::Forms::Button^  WorldTabRemoveMapButton;

private: System::Windows::Forms::Button^  WorldTabRenderMapButton;

private: System::Windows::Forms::ComboBox^  WorldTabMapNameComboBox;


private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::TextBox^  WorldTabWorldNameTextbox;
private: System::Windows::Forms::Button^  WorldTabSaveMapButton;


private: System::Windows::Forms::Button^  WorldTabLoadMapButton;

private: System::Windows::Forms::GroupBox^  groupBox7;
private: System::Windows::Forms::GroupBox^  MapTabTopdownMapGroupBox;



private: System::Windows::Forms::Label^  label16;

private: System::Windows::Forms::TextBox^  TopdownMapMusicTextbox;


private: System::Windows::Forms::Label^  label14;

private: System::Windows::Forms::TextBox^  TopdownMapNameTextbox;

private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::CheckBox^  TopdownMapHardnessCheckbox;

private: System::Windows::Forms::GroupBox^  groupBox9;



private: System::Windows::Forms::CheckBox^  TopdownMapFOWFlashlightMapTabCheckBox;



private: System::Windows::Forms::ComboBox^  TopdownMapFOWRadiusMapTabCB;

private: System::Windows::Forms::Label^  label18;



private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::GroupBox^  groupBox11;
private: System::Windows::Forms::Label^  TotalSpritesLabel;

private: System::Windows::Forms::Label^  SpritesRenderedLabel;
private: System::Windows::Forms::Label^  SectionColSizeLabel;
private: System::Windows::Forms::Label^  SectionRowSizeLabel;
private: System::Windows::Forms::Label^  MapColSizeLabel;
private: System::Windows::Forms::Label^  MapRowSizeLabel;









private: System::Windows::Forms::Label^  NumSectionsLabel;

private: System::Windows::Forms::Label^  NumTilesLabel;

private: System::Windows::Forms::Label^  TileSizeLabel;



private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::GroupBox^  groupBox12;
private: System::Windows::Forms::RadioButton^  TilesTabHardNoRadioButton;
private: System::Windows::Forms::RadioButton^  TilesTabHardYesRadioButton;
private: System::Windows::Forms::Button^  TilesTabRevertAnimButton;



private: System::Windows::Forms::Button^  TilesTabUpdateAnimButton;

private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::Button^  button14;
private: System::Windows::Forms::Label^  TilesTabYPosLabel;
private: System::Windows::Forms::Label^  TilesTabXPosLabel;
private: System::Windows::Forms::TextBox^  TilesTabAnimNameTextbox;

private: System::Windows::Forms::TextBox^  TilesTabAnimSpeedTextbox;

private: System::Windows::Forms::TextBox^  TilesTabTextureNameTextbox;
private: System::Windows::Forms::Label^  SpriteTabTopdownYLabel;

private: System::Windows::Forms::Label^  SpriteTabTopdownXLabel;






private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::Label^  label36;


private: System::Windows::Forms::Button^  TileCacheTabUpdateCacheButton;



private: System::Windows::Forms::Label^  TileCacheTabSelectedTileNameLabel;

private: System::Windows::Forms::Label^  label38;

private: System::Windows::Forms::Button^  SpriteCacheTabUpdateCacheButton;




private: System::Windows::Forms::Label^  SpriteCacheTabSelectedSpriteLabel;

private: System::Windows::Forms::Label^  label41;



private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::ComboBox^  WorldTabPlatformerMapHeightComboBox;

private: System::Windows::Forms::ComboBox^  WorldTabPlatformerMapWidthComboBox;


private: System::Windows::Forms::GroupBox^  TileModeMultiStampSizeGroupBox;
private: System::Windows::Forms::RadioButton^  MultiStamp10x10RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp9x9RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp8x8RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp7x7RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp6x6RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp5x5RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp4x4RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp3x3RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp2x2RadioButton;
private: System::Windows::Forms::RadioButton^  MultiStamp1x1RadioButton;
private: System::Windows::Forms::GroupBox^  GridTypeGroupBox;












private: System::Windows::Forms::Button^  ReturnToSectionPickerButton;
private: System::Windows::Forms::GroupBox^  MapTabPlatformerMapGroupBox;

private: System::Windows::Forms::GroupBox^  groupBox15;
private: System::Windows::Forms::Label^  PlatformerMapTotalSpritesLabel;

private: System::Windows::Forms::Label^  PlatformerMapSpritesRenderedLabel;

private: System::Windows::Forms::Label^  PlatformerMapNumTilesLabel;






private: System::Windows::Forms::Label^  PlatformerMapTileSizeLabel;



private: System::Windows::Forms::Label^  label53;
private: System::Windows::Forms::Label^  label54;
private: System::Windows::Forms::Label^  label55;
private: System::Windows::Forms::Label^  label56;

private: System::Windows::Forms::CheckBox^  PlatformerMapHardnessCheckbox;






private: System::Windows::Forms::Label^  label61;
private: System::Windows::Forms::TextBox^  PlatformerMapEditMusicTextbox;

private: System::Windows::Forms::Label^  label62;
private: System::Windows::Forms::TextBox^  PlatformerMapEditNameTextbox;

private: System::Windows::Forms::Label^  PlatformerMapVisibleTilesLabel;

private: System::Windows::Forms::Label^  label46;
private: System::Windows::Forms::RadioButton^  GridTypeSnap;

private: System::Windows::Forms::RadioButton^  GridTypeFree;
private: System::Windows::Forms::ComboBox^  TopdownMapVisionComboBox;
private: System::Windows::Forms::RadioButton^  SpriteModeCloneRadioButton;
private: System::Windows::Forms::GroupBox^  SpriteTabResizeGroupBox;

private: System::Windows::Forms::CheckBox^  SpriteTabGoFastCheckbox;

private: System::Windows::Forms::RadioButton^  SpriteTabNoUpdateRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabResizeRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabRepositionRadioButton;
private: System::Windows::Forms::Button^  SpriteTabResizeLeftButton;
private: System::Windows::Forms::Button^  SpriteTabResizeRightButton;



private: System::Windows::Forms::Button^  SpriteTabResizeDownButton;

private: System::Windows::Forms::Button^  SpriteTabResizeUpButton;

private: System::Windows::Forms::CheckBox^  SpriteTabInstantCheckbox;
private: System::Windows::Forms::GroupBox^  SpriteTabTopdownSpriteGroupBox;


private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::ComboBox^  SpriteTabTopdownVisionComboBox;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownDepthTextbox;


private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::Button^  SpriteTabTopdownBrowseTextureNameButton;

private: System::Windows::Forms::TextBox^  SpriteTabTopdownTextureNameTextbox;

private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::Button^  SpriteTabTopdownRevertAnimationButton;

private: System::Windows::Forms::Button^  SpriteTabTopdownUpdateAnimationButton;

private: System::Windows::Forms::TextBox^  SpriteTabTopdownAnimationNameTextbox;

private: System::Windows::Forms::Label^  label42;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownDefaultSequenceTextbox;

private: System::Windows::Forms::Label^  label43;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownAnimationSpeedTextbox;

private: System::Windows::Forms::Label^  label44;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownHPMaxTextbox;

private: System::Windows::Forms::Label^  label47;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownCollideMsgTextbox;

private: System::Windows::Forms::Label^  label45;
private: System::Windows::Forms::GroupBox^  groupBox8;
private: System::Windows::Forms::RadioButton^  SpriteTabTopdownDynamicRadioButton;
private: System::Windows::Forms::RadioButton^  SpriteTabTopdownStaticRadioButton;



private: System::Windows::Forms::TextBox^  SpriteTabTopdownHPTextbox;

private: System::Windows::Forms::Label^  label48;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownLevelTextbox;



private: System::Windows::Forms::Label^  label59;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownExperienceTextbox;

private: System::Windows::Forms::Label^  label58;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownDefenseTextbox;

private: System::Windows::Forms::Label^  label57;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownSpeedTextbox;

private: System::Windows::Forms::Label^  label52;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownStrengthTextbox;

private: System::Windows::Forms::Label^  label51;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownGoldTextbox;

private: System::Windows::Forms::Label^  label50;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownManaTextbox;

private: System::Windows::Forms::Label^  label49;
private: System::Windows::Forms::GroupBox^  groupBox18;
private: System::Windows::Forms::RadioButton^  SpriteTabTopdownInvisibleRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabTopdownEntityRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabTopdownBackgroundRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabTopdownTeleportSpriteNoRadioButton;


private: System::Windows::Forms::RadioButton^  SpriteTabTopdownTeleportSpriteYesRadioButton;











private: System::Windows::Forms::TextBox^  SpriteTabTopdownTeleportSpriteMapNameTextbox;

private: System::Windows::Forms::Label^  label60;
private: System::Windows::Forms::Button^  SpriteTabTopdownScriptOpenButton;

private: System::Windows::Forms::TextBox^  SpriteTabTopdownScriptTextbox;

private: System::Windows::Forms::Label^  label63;
private: System::Windows::Forms::GroupBox^  groupBox19;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownTeleportSpriteXTextbox;
private: System::Windows::Forms::TextBox^  SpriteTabTopdownTeleportSpriteYTextbox;


private: System::Windows::Forms::Label^  label64;
private: System::Windows::Forms::Label^  label65;
private: System::Windows::Forms::GroupBox^  SpriteTabPlatformerSpriteGroupBox;


private: System::Windows::Forms::GroupBox^  groupBox6;
private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerInvisibleRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerEntityRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerBackgroundRadioButton;

private: System::Windows::Forms::GroupBox^  groupBox17;
private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerTeleportSpriteNoRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerTeleportSpriteYesRadioButton;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerTeleportSpriteXTextbox;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerTeleportSpriteYTextbox;



private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerTeleportSpriteMapNameTextbox;

private: System::Windows::Forms::Label^  label66;
private: System::Windows::Forms::Label^  SpriteTabPlatformerYLabel;

private: System::Windows::Forms::Label^  SpriteTabPlatformerXLabel;
private: System::Windows::Forms::Button^  SpriteTabPlatformerScriptOpenButton;


private: System::Windows::Forms::Label^  label69;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerScriptTextbox;

private: System::Windows::Forms::Label^  label70;
private: System::Windows::Forms::Label^  label71;















private: System::Windows::Forms::TextBox^  SpriteTabPlatformerLevelTextbox;

private: System::Windows::Forms::Label^  label72;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerExperienceTextbox;

private: System::Windows::Forms::GroupBox^  groupBox23;
private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerDynamicRadioButton;

private: System::Windows::Forms::RadioButton^  SpriteTabPlatformerStaticRadioButton;

private: System::Windows::Forms::Label^  label73;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerDefenseTextbox;

private: System::Windows::Forms::Label^  label74;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerSpeedTextbox;

private: System::Windows::Forms::Label^  label75;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerStrengthTextbox;

private: System::Windows::Forms::Label^  label76;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerGoldTextbox;

private: System::Windows::Forms::Label^  label77;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerManaTextbox;

private: System::Windows::Forms::Label^  label78;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerHPTextbox;

private: System::Windows::Forms::Label^  label79;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerHPMaxTextbox;


private: System::Windows::Forms::Label^  label80;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerCollideMsgTextbox;

private: System::Windows::Forms::Label^  label81;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerAnimationSpeedTextbox;

private: System::Windows::Forms::Label^  label82;
private: System::Windows::Forms::TextBox^  SpriteTabPlatformerDefaultSequenceTextbox;

private: System::Windows::Forms::Label^  label83;
private: System::Windows::Forms::Button^  SpriteTabPlatformerRevertAnimationButton;

private: System::Windows::Forms::Button^  SpriteTabPlatformerUpdateAnimationButton;

private: System::Windows::Forms::TextBox^  SpriteTabPlatformerAnimationNameTextbox;

private: System::Windows::Forms::Label^  label84;
private: System::Windows::Forms::Button^  SpriteTabPlatformerBrowseTextureNameButton;

private: System::Windows::Forms::TextBox^  SpriteTabPlatformerTextureNameTextbox;

private: System::Windows::Forms::Label^  label85;
private: System::Windows::Forms::RadioButton^  TileModeTopdownSelectRadioButton;
private: System::Windows::Forms::RadioButton^  TileModePlatformerSelectRadioButton;
private: System::Windows::Forms::Button^  MapTabPlatformerUpdateMapNameButton;
private: System::Windows::Forms::Button^  MapTabTopdownUpdateMapNameButton;
private: System::Windows::Forms::CheckBox^  SpriteTabPlatformerTouchCheckbox;
private: System::Windows::Forms::CheckBox^  SpriteTabPlatformerNoHitCheckbox;
private: System::Windows::Forms::CheckBox^  SpriteTabPlatformerHardCheckbox;



private: System::Windows::Forms::CheckBox^  SpriteTabTopdownTouchCheckbox;

private: System::Windows::Forms::CheckBox^  SpriteTabTopdownNoHitCheckbox;

private: System::Windows::Forms::CheckBox^  SpriteTabTopdownHardCheckbox;
private: System::Windows::Forms::Button^  SpriteTabPlatformerScriptBrowseButton;
private: System::Windows::Forms::Button^  SpriteTabTopdownScriptBrowseButton;
private: System::Windows::Forms::CheckBox^  TopdownMapFOWUncoveredMapTabCheckBox;
private: System::Windows::Forms::CheckBox^  TopdownMapEnableFOWMapTabCheckBox;
private: System::Windows::Forms::CheckBox^  WorldTabFOWUncoveredCheckBox;
private: System::Windows::Forms::CheckBox^  WorldTabEnableFOWCheckBox;
private: System::Windows::Forms::CheckBox^  SpriteTabPlatformerAlwaysSeenByPlayerCheckbox;
private: System::Windows::Forms::CheckBox^  SpriteTabTopdownAlwaysSeenByPlayerCheckbox;



































			 DXGameWindow* mDXGameWindow;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->ZShadeSandboxEditorSplitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->TabControl2D = (gcnew System::Windows::Forms::TabControl());
			this->EditorTab = (gcnew System::Windows::Forms::TabPage());
			this->ReturnToSectionPickerButton = (gcnew System::Windows::Forms::Button());
			this->GridTypeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->GridTypeSnap = (gcnew System::Windows::Forms::RadioButton());
			this->GridTypeFree = (gcnew System::Windows::Forms::RadioButton());
			this->TileModeMultiStampSizeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->MultiStamp10x10RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp9x9RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp8x8RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp7x7RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp6x6RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp5x5RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp4x4RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp3x3RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp2x2RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStamp1x1RadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->GameFolderNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->SaveGameDirectoryButton = (gcnew System::Windows::Forms::Button());
			this->CreateGameDirectoryButton = (gcnew System::Windows::Forms::Button());
			this->OpenGameDirectoryButton = (gcnew System::Windows::Forms::Button());
			this->CloseGameDirectoryButton = (gcnew System::Windows::Forms::Button());
			this->MenuEditorButton = (gcnew System::Windows::Forms::Button());
			this->HUDEditorButton = (gcnew System::Windows::Forms::Button());
			this->GamePathLabel = (gcnew System::Windows::Forms::Label());
			this->GameTypeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->GameTypeTopdownRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->GameTypePlatformerRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->HardnessModeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->TilesHardnessRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpritesHardnessRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteModeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteModeCloneRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteModeSelectRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteModeSizeRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteModeMoveRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteModePlaceRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteModeDeleteRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileModePlatformerGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->TileModePlatformerSelectRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileModePlatformerRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->MultiStampPlatformerRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->RemoveTexturePlatformerRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileModeTopdownGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->TileModeTopdownSelectRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileModeTopdownRegularRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileModeTopdownMultiStampRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->EditModeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->HardnessRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->WorldTab = (gcnew System::Windows::Forms::TabPage());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->WorldTabSaveWorldButton = (gcnew System::Windows::Forms::Button());
			this->WorldTabRemoveMapButton = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->WorldTabRenderMapButton = (gcnew System::Windows::Forms::Button());
			this->WorldTabCreateWorldButton = (gcnew System::Windows::Forms::Button());
			this->WorldTabMapNameComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->WorldTabLoadWorldButton = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->WorldTabLoadMapButton = (gcnew System::Windows::Forms::Button());
			this->WorldTabWorldNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->WorldTabSaveMapButton = (gcnew System::Windows::Forms::Button());
			this->NewPlatformerMapGroupbox = (gcnew System::Windows::Forms::GroupBox());
			this->WorldTabPlatformerMapHeightComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->WorldTabPlatformerMapWidthComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->WorldTabPlatformerMapNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->WorldTabPlatformerWorldNameComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->WorldTabPlatformerCreateMapButton = (gcnew System::Windows::Forms::Button());
			this->NewTopdownMapGroupbox = (gcnew System::Windows::Forms::GroupBox());
			this->WorldTabTopdownMapMapNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->WorldTabTopdownMapWorldNameComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->WorldTabFOWUncoveredCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->WorldTabEnableFOWCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->WorldTabFOWFlashlightCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->WorldTabFOWRadiusComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->WorldTabTopdownMapCreateMapButton = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->WorldTabTopdownMapIsometricRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->WorldTabTopdownMapStandardRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->WorldTabTopdownMapSizeComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->WorldTabStatusLabel = (gcnew System::Windows::Forms::Label());
			this->MapTab = (gcnew System::Windows::Forms::TabPage());
			this->MapTabPlatformerMapGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->MapTabPlatformerUpdateMapNameButton = (gcnew System::Windows::Forms::Button());
			this->groupBox15 = (gcnew System::Windows::Forms::GroupBox());
			this->PlatformerMapVisibleTilesLabel = (gcnew System::Windows::Forms::Label());
			this->label46 = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapTotalSpritesLabel = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapSpritesRenderedLabel = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapNumTilesLabel = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapTileSizeLabel = (gcnew System::Windows::Forms::Label());
			this->label53 = (gcnew System::Windows::Forms::Label());
			this->label54 = (gcnew System::Windows::Forms::Label());
			this->label55 = (gcnew System::Windows::Forms::Label());
			this->label56 = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapHardnessCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->label61 = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapEditMusicTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label62 = (gcnew System::Windows::Forms::Label());
			this->PlatformerMapEditNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->MapTabTopdownMapGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->MapTabTopdownUpdateMapNameButton = (gcnew System::Windows::Forms::Button());
			this->TopdownMapVisionComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->TotalSpritesLabel = (gcnew System::Windows::Forms::Label());
			this->SpritesRenderedLabel = (gcnew System::Windows::Forms::Label());
			this->SectionColSizeLabel = (gcnew System::Windows::Forms::Label());
			this->SectionRowSizeLabel = (gcnew System::Windows::Forms::Label());
			this->MapColSizeLabel = (gcnew System::Windows::Forms::Label());
			this->MapRowSizeLabel = (gcnew System::Windows::Forms::Label());
			this->NumSectionsLabel = (gcnew System::Windows::Forms::Label());
			this->NumTilesLabel = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->TileSizeLabel = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->TopdownMapEnableFOWMapTabCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->TopdownMapFOWUncoveredMapTabCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->TopdownMapFOWFlashlightMapTabCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->TopdownMapFOWRadiusMapTabCB = (gcnew System::Windows::Forms::ComboBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->TopdownMapHardnessCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->TopdownMapMusicTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->TopdownMapNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->TilesTab = (gcnew System::Windows::Forms::TabPage());
			this->TilesTabAnimNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->TilesTabAnimSpeedTextbox = (gcnew System::Windows::Forms::TextBox());
			this->TilesTabTextureNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->TilesTabYPosLabel = (gcnew System::Windows::Forms::Label());
			this->TilesTabXPosLabel = (gcnew System::Windows::Forms::Label());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->TilesTabHardNoRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TilesTabHardYesRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TilesTabRevertAnimButton = (gcnew System::Windows::Forms::Button());
			this->TilesTabUpdateAnimButton = (gcnew System::Windows::Forms::Button());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->SpritesTab = (gcnew System::Windows::Forms::TabPage());
			this->SpriteTabPlatformerSpriteGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabPlatformerScriptBrowseButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabPlatformerTouchCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabPlatformerNoHitCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabPlatformerHardCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabPlatformerInvisibleRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabPlatformerEntityRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabPlatformerBackgroundRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox17 = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabPlatformerTeleportSpriteXTextbox = (gcnew System::Windows::Forms::TextBox());
			this->SpriteTabPlatformerTeleportSpriteYTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerTeleportSpriteMapNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label66 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerYLabel = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerXLabel = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerScriptOpenButton = (gcnew System::Windows::Forms::Button());
			this->label69 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerScriptTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label70 = (gcnew System::Windows::Forms::Label());
			this->label71 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerLevelTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label72 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerExperienceTextbox = (gcnew System::Windows::Forms::TextBox());
			this->groupBox23 = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabPlatformerDynamicRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabPlatformerStaticRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->label73 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerDefenseTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label74 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerSpeedTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label75 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerStrengthTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label76 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerGoldTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label77 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerManaTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label78 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerHPTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label79 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerHPMaxTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label80 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerCollideMsgTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label81 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerAnimationSpeedTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label82 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerDefaultSequenceTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label83 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerRevertAnimationButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabPlatformerUpdateAnimationButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabPlatformerAnimationNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label84 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabPlatformerBrowseTextureNameButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabPlatformerTextureNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label85 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownSpriteGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabTopdownScriptBrowseButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabTopdownTouchCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabTopdownNoHitCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabTopdownHardCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox18 = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabTopdownInvisibleRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabTopdownEntityRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabTopdownBackgroundRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox19 = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabTopdownTeleportSpriteNoRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabTopdownTeleportSpriteYesRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabTopdownTeleportSpriteXTextbox = (gcnew System::Windows::Forms::TextBox());
			this->SpriteTabTopdownTeleportSpriteYTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label64 = (gcnew System::Windows::Forms::Label());
			this->label65 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownTeleportSpriteMapNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label60 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownYLabel = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownXLabel = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownScriptOpenButton = (gcnew System::Windows::Forms::Button());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownScriptTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label63 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownLevelTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label59 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownExperienceTextbox = (gcnew System::Windows::Forms::TextBox());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabTopdownDynamicRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabTopdownStaticRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->label58 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownDefenseTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label57 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownSpeedTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label52 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownStrengthTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownGoldTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownManaTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownHPTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownHPMaxTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label47 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownCollideMsgTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownAnimationSpeedTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownDefaultSequenceTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownRevertAnimationButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabTopdownUpdateAnimationButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabTopdownAnimationNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownBrowseTextureNameButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabTopdownTextureNameTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownDepthTextbox = (gcnew System::Windows::Forms::TextBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->SpriteTabTopdownVisionComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->SpriteTabResizeGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->SpriteTabInstantCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabResizeLeftButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabResizeRightButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabResizeDownButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabResizeUpButton = (gcnew System::Windows::Forms::Button());
			this->SpriteTabGoFastCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabNoUpdateRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabResizeRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->SpriteTabRepositionRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->TileCacheTab = (gcnew System::Windows::Forms::TabPage());
			this->TileCacheTabUpdateCacheButton = (gcnew System::Windows::Forms::Button());
			this->TileCacheTabSelectedTileNameLabel = (gcnew System::Windows::Forms::Label());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->SpriteCacheTab = (gcnew System::Windows::Forms::TabPage());
			this->SpriteCacheTabUpdateCacheButton = (gcnew System::Windows::Forms::Button());
			this->SpriteCacheTabSelectedSpriteLabel = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->StartTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ZShadeSandboxEditorSplitContainer))->BeginInit();
			this->ZShadeSandboxEditorSplitContainer->Panel2->SuspendLayout();
			this->ZShadeSandboxEditorSplitContainer->SuspendLayout();
			this->TabControl2D->SuspendLayout();
			this->EditorTab->SuspendLayout();
			this->GridTypeGroupBox->SuspendLayout();
			this->TileModeMultiStampSizeGroupBox->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->GameTypeGroupBox->SuspendLayout();
			this->HardnessModeGroupBox->SuspendLayout();
			this->SpriteModeGroupBox->SuspendLayout();
			this->TileModePlatformerGroupBox->SuspendLayout();
			this->TileModeTopdownGroupBox->SuspendLayout();
			this->EditModeGroupBox->SuspendLayout();
			this->WorldTab->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->NewPlatformerMapGroupbox->SuspendLayout();
			this->NewTopdownMapGroupbox->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->MapTab->SuspendLayout();
			this->MapTabPlatformerMapGroupBox->SuspendLayout();
			this->groupBox15->SuspendLayout();
			this->MapTabTopdownMapGroupBox->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->TilesTab->SuspendLayout();
			this->groupBox12->SuspendLayout();
			this->SpritesTab->SuspendLayout();
			this->SpriteTabPlatformerSpriteGroupBox->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox17->SuspendLayout();
			this->groupBox23->SuspendLayout();
			this->SpriteTabTopdownSpriteGroupBox->SuspendLayout();
			this->groupBox18->SuspendLayout();
			this->groupBox19->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->SpriteTabResizeGroupBox->SuspendLayout();
			this->TileCacheTab->SuspendLayout();
			this->SpriteCacheTab->SuspendLayout();
			this->SuspendLayout();
			// 
			// ZShadeSandboxEditorSplitContainer
			// 
			this->ZShadeSandboxEditorSplitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ZShadeSandboxEditorSplitContainer->Location = System::Drawing::Point(0, 0);
			this->ZShadeSandboxEditorSplitContainer->Name = L"ZShadeSandboxEditorSplitContainer";
			// 
			// ZShadeSandboxEditorSplitContainer.Panel2
			// 
			this->ZShadeSandboxEditorSplitContainer->Panel2->Controls->Add(this->TabControl2D);
			this->ZShadeSandboxEditorSplitContainer->Size = System::Drawing::Size(1366, 768);
			this->ZShadeSandboxEditorSplitContainer->SplitterDistance = 874;
			this->ZShadeSandboxEditorSplitContainer->TabIndex = 0;
			// 
			// TabControl2D
			// 
			this->TabControl2D->Controls->Add(this->EditorTab);
			this->TabControl2D->Controls->Add(this->WorldTab);
			this->TabControl2D->Controls->Add(this->MapTab);
			this->TabControl2D->Controls->Add(this->TilesTab);
			this->TabControl2D->Controls->Add(this->SpritesTab);
			this->TabControl2D->Controls->Add(this->TileCacheTab);
			this->TabControl2D->Controls->Add(this->SpriteCacheTab);
			this->TabControl2D->Dock = System::Windows::Forms::DockStyle::Fill;
			this->TabControl2D->Location = System::Drawing::Point(0, 0);
			this->TabControl2D->Name = L"TabControl2D";
			this->TabControl2D->SelectedIndex = 0;
			this->TabControl2D->Size = System::Drawing::Size(488, 768);
			this->TabControl2D->TabIndex = 0;
			// 
			// EditorTab
			// 
			this->EditorTab->Controls->Add(this->ReturnToSectionPickerButton);
			this->EditorTab->Controls->Add(this->GridTypeGroupBox);
			this->EditorTab->Controls->Add(this->TileModeMultiStampSizeGroupBox);
			this->EditorTab->Controls->Add(this->groupBox1);
			this->EditorTab->Controls->Add(this->GamePathLabel);
			this->EditorTab->Controls->Add(this->GameTypeGroupBox);
			this->EditorTab->Controls->Add(this->HardnessModeGroupBox);
			this->EditorTab->Controls->Add(this->SpriteModeGroupBox);
			this->EditorTab->Controls->Add(this->TileModePlatformerGroupBox);
			this->EditorTab->Controls->Add(this->TileModeTopdownGroupBox);
			this->EditorTab->Controls->Add(this->EditModeGroupBox);
			this->EditorTab->Controls->Add(this->label1);
			this->EditorTab->Location = System::Drawing::Point(4, 22);
			this->EditorTab->Name = L"EditorTab";
			this->EditorTab->Padding = System::Windows::Forms::Padding(3);
			this->EditorTab->Size = System::Drawing::Size(480, 742);
			this->EditorTab->TabIndex = 0;
			this->EditorTab->Text = L"Editor";
			this->EditorTab->UseVisualStyleBackColor = true;
			// 
			// ReturnToSectionPickerButton
			// 
			this->ReturnToSectionPickerButton->Location = System::Drawing::Point(304, 419);
			this->ReturnToSectionPickerButton->Name = L"ReturnToSectionPickerButton";
			this->ReturnToSectionPickerButton->Size = System::Drawing::Size(135, 23);
			this->ReturnToSectionPickerButton->TabIndex = 42;
			this->ReturnToSectionPickerButton->Text = L"Return to section picker";
			this->ReturnToSectionPickerButton->UseVisualStyleBackColor = true;
			this->ReturnToSectionPickerButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::ReturnToSectionPickerButton_Click);
			// 
			// GridTypeGroupBox
			// 
			this->GridTypeGroupBox->Controls->Add(this->GridTypeSnap);
			this->GridTypeGroupBox->Controls->Add(this->GridTypeFree);
			this->GridTypeGroupBox->Location = System::Drawing::Point(218, 310);
			this->GridTypeGroupBox->Name = L"GridTypeGroupBox";
			this->GridTypeGroupBox->Size = System::Drawing::Size(104, 80);
			this->GridTypeGroupBox->TabIndex = 41;
			this->GridTypeGroupBox->TabStop = false;
			this->GridTypeGroupBox->Text = L"Sprite Grid Type";
			// 
			// GridTypeSnap
			// 
			this->GridTypeSnap->AutoSize = true;
			this->GridTypeSnap->Location = System::Drawing::Point(9, 43);
			this->GridTypeSnap->Name = L"GridTypeSnap";
			this->GridTypeSnap->Size = System::Drawing::Size(50, 17);
			this->GridTypeSnap->TabIndex = 1;
			this->GridTypeSnap->Text = L"Snap";
			this->GridTypeSnap->UseVisualStyleBackColor = true;
			this->GridTypeSnap->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::GridTypeSnap_CheckedChanged);
			// 
			// GridTypeFree
			// 
			this->GridTypeFree->AutoSize = true;
			this->GridTypeFree->Checked = true;
			this->GridTypeFree->Location = System::Drawing::Point(9, 19);
			this->GridTypeFree->Name = L"GridTypeFree";
			this->GridTypeFree->Size = System::Drawing::Size(46, 17);
			this->GridTypeFree->TabIndex = 0;
			this->GridTypeFree->TabStop = true;
			this->GridTypeFree->Text = L"Free";
			this->GridTypeFree->UseVisualStyleBackColor = true;
			this->GridTypeFree->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::GridTypeFree_CheckedChanged);
			// 
			// TileModeMultiStampSizeGroupBox
			// 
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp10x10RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp9x9RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp8x8RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp7x7RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp6x6RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp5x5RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp4x4RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp3x3RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp2x2RadioButton);
			this->TileModeMultiStampSizeGroupBox->Controls->Add(this->MultiStamp1x1RadioButton);
			this->TileModeMultiStampSizeGroupBox->Location = System::Drawing::Point(6, 288);
			this->TileModeMultiStampSizeGroupBox->Name = L"TileModeMultiStampSizeGroupBox";
			this->TileModeMultiStampSizeGroupBox->Size = System::Drawing::Size(111, 256);
			this->TileModeMultiStampSizeGroupBox->TabIndex = 38;
			this->TileModeMultiStampSizeGroupBox->TabStop = false;
			this->TileModeMultiStampSizeGroupBox->Text = L"Multi Stamp Size";
			// 
			// MultiStamp10x10RadioButton
			// 
			this->MultiStamp10x10RadioButton->AutoSize = true;
			this->MultiStamp10x10RadioButton->Location = System::Drawing::Point(5, 226);
			this->MultiStamp10x10RadioButton->Name = L"MultiStamp10x10RadioButton";
			this->MultiStamp10x10RadioButton->Size = System::Drawing::Size(54, 17);
			this->MultiStamp10x10RadioButton->TabIndex = 9;
			this->MultiStamp10x10RadioButton->TabStop = true;
			this->MultiStamp10x10RadioButton->Text = L"10x10";
			this->MultiStamp10x10RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp10x10RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp10x10RadioButton_CheckedChanged);
			// 
			// MultiStamp9x9RadioButton
			// 
			this->MultiStamp9x9RadioButton->AutoSize = true;
			this->MultiStamp9x9RadioButton->Location = System::Drawing::Point(5, 203);
			this->MultiStamp9x9RadioButton->Name = L"MultiStamp9x9RadioButton";
			this->MultiStamp9x9RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp9x9RadioButton->TabIndex = 8;
			this->MultiStamp9x9RadioButton->TabStop = true;
			this->MultiStamp9x9RadioButton->Text = L"9x9";
			this->MultiStamp9x9RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp9x9RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp9x9RadioButton_CheckedChanged);
			// 
			// MultiStamp8x8RadioButton
			// 
			this->MultiStamp8x8RadioButton->AutoSize = true;
			this->MultiStamp8x8RadioButton->Location = System::Drawing::Point(5, 180);
			this->MultiStamp8x8RadioButton->Name = L"MultiStamp8x8RadioButton";
			this->MultiStamp8x8RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp8x8RadioButton->TabIndex = 7;
			this->MultiStamp8x8RadioButton->TabStop = true;
			this->MultiStamp8x8RadioButton->Text = L"8x8";
			this->MultiStamp8x8RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp8x8RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp8x8RadioButton_CheckedChanged);
			// 
			// MultiStamp7x7RadioButton
			// 
			this->MultiStamp7x7RadioButton->AutoSize = true;
			this->MultiStamp7x7RadioButton->Location = System::Drawing::Point(5, 157);
			this->MultiStamp7x7RadioButton->Name = L"MultiStamp7x7RadioButton";
			this->MultiStamp7x7RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp7x7RadioButton->TabIndex = 6;
			this->MultiStamp7x7RadioButton->TabStop = true;
			this->MultiStamp7x7RadioButton->Text = L"7x7";
			this->MultiStamp7x7RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp7x7RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp7x7RadioButton_CheckedChanged);
			// 
			// MultiStamp6x6RadioButton
			// 
			this->MultiStamp6x6RadioButton->AutoSize = true;
			this->MultiStamp6x6RadioButton->Location = System::Drawing::Point(5, 134);
			this->MultiStamp6x6RadioButton->Name = L"MultiStamp6x6RadioButton";
			this->MultiStamp6x6RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp6x6RadioButton->TabIndex = 5;
			this->MultiStamp6x6RadioButton->TabStop = true;
			this->MultiStamp6x6RadioButton->Text = L"6x6";
			this->MultiStamp6x6RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp6x6RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp6x6RadioButton_CheckedChanged);
			// 
			// MultiStamp5x5RadioButton
			// 
			this->MultiStamp5x5RadioButton->AutoSize = true;
			this->MultiStamp5x5RadioButton->Location = System::Drawing::Point(6, 111);
			this->MultiStamp5x5RadioButton->Name = L"MultiStamp5x5RadioButton";
			this->MultiStamp5x5RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp5x5RadioButton->TabIndex = 4;
			this->MultiStamp5x5RadioButton->TabStop = true;
			this->MultiStamp5x5RadioButton->Text = L"5x5";
			this->MultiStamp5x5RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp5x5RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp5x5RadioButton_CheckedChanged);
			// 
			// MultiStamp4x4RadioButton
			// 
			this->MultiStamp4x4RadioButton->AutoSize = true;
			this->MultiStamp4x4RadioButton->Location = System::Drawing::Point(6, 88);
			this->MultiStamp4x4RadioButton->Name = L"MultiStamp4x4RadioButton";
			this->MultiStamp4x4RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp4x4RadioButton->TabIndex = 3;
			this->MultiStamp4x4RadioButton->TabStop = true;
			this->MultiStamp4x4RadioButton->Text = L"4x4";
			this->MultiStamp4x4RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp4x4RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp4x4RadioButton_CheckedChanged);
			// 
			// MultiStamp3x3RadioButton
			// 
			this->MultiStamp3x3RadioButton->AutoSize = true;
			this->MultiStamp3x3RadioButton->Location = System::Drawing::Point(6, 65);
			this->MultiStamp3x3RadioButton->Name = L"MultiStamp3x3RadioButton";
			this->MultiStamp3x3RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp3x3RadioButton->TabIndex = 2;
			this->MultiStamp3x3RadioButton->TabStop = true;
			this->MultiStamp3x3RadioButton->Text = L"3x3";
			this->MultiStamp3x3RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp3x3RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp3x3RadioButton_CheckedChanged);
			// 
			// MultiStamp2x2RadioButton
			// 
			this->MultiStamp2x2RadioButton->AutoSize = true;
			this->MultiStamp2x2RadioButton->Location = System::Drawing::Point(6, 42);
			this->MultiStamp2x2RadioButton->Name = L"MultiStamp2x2RadioButton";
			this->MultiStamp2x2RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp2x2RadioButton->TabIndex = 1;
			this->MultiStamp2x2RadioButton->TabStop = true;
			this->MultiStamp2x2RadioButton->Text = L"2x2";
			this->MultiStamp2x2RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp2x2RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp2x2RadioButton_CheckedChanged);
			// 
			// MultiStamp1x1RadioButton
			// 
			this->MultiStamp1x1RadioButton->AutoSize = true;
			this->MultiStamp1x1RadioButton->Checked = true;
			this->MultiStamp1x1RadioButton->Location = System::Drawing::Point(6, 19);
			this->MultiStamp1x1RadioButton->Name = L"MultiStamp1x1RadioButton";
			this->MultiStamp1x1RadioButton->Size = System::Drawing::Size(42, 17);
			this->MultiStamp1x1RadioButton->TabIndex = 0;
			this->MultiStamp1x1RadioButton->TabStop = true;
			this->MultiStamp1x1RadioButton->Text = L"1x1";
			this->MultiStamp1x1RadioButton->UseVisualStyleBackColor = true;
			this->MultiStamp1x1RadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStamp1x1RadioButton_CheckedChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->GameFolderNameTextBox);
			this->groupBox1->Controls->Add(this->SaveGameDirectoryButton);
			this->groupBox1->Controls->Add(this->CreateGameDirectoryButton);
			this->groupBox1->Controls->Add(this->OpenGameDirectoryButton);
			this->groupBox1->Controls->Add(this->CloseGameDirectoryButton);
			this->groupBox1->Controls->Add(this->MenuEditorButton);
			this->groupBox1->Controls->Add(this->HUDEditorButton);
			this->groupBox1->Location = System::Drawing::Point(218, 43);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(219, 261);
			this->groupBox1->TabIndex = 37;
			this->groupBox1->TabStop = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 21);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(98, 13);
			this->label2->TabIndex = 36;
			this->label2->Text = L"Game Folder Name";
			// 
			// GameFolderNameTextBox
			// 
			this->GameFolderNameTextBox->Location = System::Drawing::Point(9, 37);
			this->GameFolderNameTextBox->Name = L"GameFolderNameTextBox";
			this->GameFolderNameTextBox->Size = System::Drawing::Size(197, 20);
			this->GameFolderNameTextBox->TabIndex = 0;
			// 
			// SaveGameDirectoryButton
			// 
			this->SaveGameDirectoryButton->Location = System::Drawing::Point(69, 152);
			this->SaveGameDirectoryButton->Name = L"SaveGameDirectoryButton";
			this->SaveGameDirectoryButton->Size = System::Drawing::Size(137, 23);
			this->SaveGameDirectoryButton->TabIndex = 35;
			this->SaveGameDirectoryButton->Text = L"Save Game Directory";
			this->SaveGameDirectoryButton->UseVisualStyleBackColor = true;
			this->SaveGameDirectoryButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SaveGameDirectoryButton_Click);
			// 
			// CreateGameDirectoryButton
			// 
			this->CreateGameDirectoryButton->Location = System::Drawing::Point(70, 64);
			this->CreateGameDirectoryButton->Name = L"CreateGameDirectoryButton";
			this->CreateGameDirectoryButton->Size = System::Drawing::Size(136, 23);
			this->CreateGameDirectoryButton->TabIndex = 26;
			this->CreateGameDirectoryButton->Text = L"Create Game Directory";
			this->CreateGameDirectoryButton->UseVisualStyleBackColor = true;
			this->CreateGameDirectoryButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::CreateGameDirectoryButton_Click);
			// 
			// OpenGameDirectoryButton
			// 
			this->OpenGameDirectoryButton->Location = System::Drawing::Point(70, 93);
			this->OpenGameDirectoryButton->Name = L"OpenGameDirectoryButton";
			this->OpenGameDirectoryButton->Size = System::Drawing::Size(136, 23);
			this->OpenGameDirectoryButton->TabIndex = 27;
			this->OpenGameDirectoryButton->Text = L"Open Game Directory";
			this->OpenGameDirectoryButton->UseVisualStyleBackColor = true;
			this->OpenGameDirectoryButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::OpenGameDirectoryButton_Click);
			// 
			// CloseGameDirectoryButton
			// 
			this->CloseGameDirectoryButton->Location = System::Drawing::Point(69, 123);
			this->CloseGameDirectoryButton->Name = L"CloseGameDirectoryButton";
			this->CloseGameDirectoryButton->Size = System::Drawing::Size(137, 23);
			this->CloseGameDirectoryButton->TabIndex = 34;
			this->CloseGameDirectoryButton->Text = L"Close Game Directory";
			this->CloseGameDirectoryButton->UseVisualStyleBackColor = true;
			this->CloseGameDirectoryButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::CloseGameDirectoryButton_Click);
			// 
			// MenuEditorButton
			// 
			this->MenuEditorButton->Location = System::Drawing::Point(70, 181);
			this->MenuEditorButton->Name = L"MenuEditorButton";
			this->MenuEditorButton->Size = System::Drawing::Size(136, 23);
			this->MenuEditorButton->TabIndex = 31;
			this->MenuEditorButton->Text = L"Menu Editor";
			this->MenuEditorButton->UseVisualStyleBackColor = true;
			// 
			// HUDEditorButton
			// 
			this->HUDEditorButton->Location = System::Drawing::Point(70, 210);
			this->HUDEditorButton->Name = L"HUDEditorButton";
			this->HUDEditorButton->Size = System::Drawing::Size(136, 23);
			this->HUDEditorButton->TabIndex = 32;
			this->HUDEditorButton->Text = L"HUD Editor";
			this->HUDEditorButton->UseVisualStyleBackColor = true;
			// 
			// GamePathLabel
			// 
			this->GamePathLabel->AutoSize = true;
			this->GamePathLabel->Location = System::Drawing::Point(70, 6);
			this->GamePathLabel->Name = L"GamePathLabel";
			this->GamePathLabel->Size = System::Drawing::Size(33, 13);
			this->GamePathLabel->TabIndex = 33;
			this->GamePathLabel->Text = L"asdfa";
			// 
			// GameTypeGroupBox
			// 
			this->GameTypeGroupBox->Controls->Add(this->GameTypeTopdownRadioButton);
			this->GameTypeGroupBox->Controls->Add(this->GameTypePlatformerRadioButton);
			this->GameTypeGroupBox->Location = System::Drawing::Point(334, 310);
			this->GameTypeGroupBox->Name = L"GameTypeGroupBox";
			this->GameTypeGroupBox->Size = System::Drawing::Size(103, 80);
			this->GameTypeGroupBox->TabIndex = 30;
			this->GameTypeGroupBox->TabStop = false;
			this->GameTypeGroupBox->Text = L"Game Type";
			// 
			// GameTypeTopdownRadioButton
			// 
			this->GameTypeTopdownRadioButton->AutoSize = true;
			this->GameTypeTopdownRadioButton->Location = System::Drawing::Point(6, 19);
			this->GameTypeTopdownRadioButton->Name = L"GameTypeTopdownRadioButton";
			this->GameTypeTopdownRadioButton->Size = System::Drawing::Size(70, 17);
			this->GameTypeTopdownRadioButton->TabIndex = 28;
			this->GameTypeTopdownRadioButton->TabStop = true;
			this->GameTypeTopdownRadioButton->Text = L"Topdown";
			this->GameTypeTopdownRadioButton->UseVisualStyleBackColor = true;
			// 
			// GameTypePlatformerRadioButton
			// 
			this->GameTypePlatformerRadioButton->AutoSize = true;
			this->GameTypePlatformerRadioButton->Location = System::Drawing::Point(6, 43);
			this->GameTypePlatformerRadioButton->Name = L"GameTypePlatformerRadioButton";
			this->GameTypePlatformerRadioButton->Size = System::Drawing::Size(72, 17);
			this->GameTypePlatformerRadioButton->TabIndex = 29;
			this->GameTypePlatformerRadioButton->TabStop = true;
			this->GameTypePlatformerRadioButton->Text = L"Platformer";
			this->GameTypePlatformerRadioButton->UseVisualStyleBackColor = true;
			// 
			// HardnessModeGroupBox
			// 
			this->HardnessModeGroupBox->Controls->Add(this->TilesHardnessRadioButton);
			this->HardnessModeGroupBox->Controls->Add(this->SpritesHardnessRadioButton);
			this->HardnessModeGroupBox->Location = System::Drawing::Point(156, 647);
			this->HardnessModeGroupBox->Name = L"HardnessModeGroupBox";
			this->HardnessModeGroupBox->Size = System::Drawing::Size(117, 77);
			this->HardnessModeGroupBox->TabIndex = 25;
			this->HardnessModeGroupBox->TabStop = false;
			this->HardnessModeGroupBox->Text = L"Hardness Mode";
			// 
			// TilesHardnessRadioButton
			// 
			this->TilesHardnessRadioButton->AutoSize = true;
			this->TilesHardnessRadioButton->Checked = true;
			this->TilesHardnessRadioButton->Location = System::Drawing::Point(6, 19);
			this->TilesHardnessRadioButton->Name = L"TilesHardnessRadioButton";
			this->TilesHardnessRadioButton->Size = System::Drawing::Size(47, 17);
			this->TilesHardnessRadioButton->TabIndex = 23;
			this->TilesHardnessRadioButton->TabStop = true;
			this->TilesHardnessRadioButton->Text = L"Tiles";
			this->TilesHardnessRadioButton->UseVisualStyleBackColor = true;
			this->TilesHardnessRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TilesHardnessRadioButton_CheckedChanged);
			// 
			// SpritesHardnessRadioButton
			// 
			this->SpritesHardnessRadioButton->AutoSize = true;
			this->SpritesHardnessRadioButton->Location = System::Drawing::Point(6, 43);
			this->SpritesHardnessRadioButton->Name = L"SpritesHardnessRadioButton";
			this->SpritesHardnessRadioButton->Size = System::Drawing::Size(57, 17);
			this->SpritesHardnessRadioButton->TabIndex = 24;
			this->SpritesHardnessRadioButton->TabStop = true;
			this->SpritesHardnessRadioButton->Text = L"Sprites";
			this->SpritesHardnessRadioButton->UseVisualStyleBackColor = true;
			this->SpritesHardnessRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpritesHardnessRadioButton_CheckedChanged);
			// 
			// SpriteModeGroupBox
			// 
			this->SpriteModeGroupBox->Controls->Add(this->SpriteModeCloneRadioButton);
			this->SpriteModeGroupBox->Controls->Add(this->SpriteModeSelectRadioButton);
			this->SpriteModeGroupBox->Controls->Add(this->SpriteModeSizeRadioButton);
			this->SpriteModeGroupBox->Controls->Add(this->SpriteModeMoveRadioButton);
			this->SpriteModeGroupBox->Controls->Add(this->SpriteModePlaceRadioButton);
			this->SpriteModeGroupBox->Controls->Add(this->SpriteModeDeleteRadioButton);
			this->SpriteModeGroupBox->Location = System::Drawing::Point(162, 468);
			this->SpriteModeGroupBox->Name = L"SpriteModeGroupBox";
			this->SpriteModeGroupBox->Size = System::Drawing::Size(127, 173);
			this->SpriteModeGroupBox->TabIndex = 22;
			this->SpriteModeGroupBox->TabStop = false;
			this->SpriteModeGroupBox->Text = L"Sprite Mode";
			// 
			// SpriteModeCloneRadioButton
			// 
			this->SpriteModeCloneRadioButton->AutoSize = true;
			this->SpriteModeCloneRadioButton->Location = System::Drawing::Point(6, 138);
			this->SpriteModeCloneRadioButton->Name = L"SpriteModeCloneRadioButton";
			this->SpriteModeCloneRadioButton->Size = System::Drawing::Size(52, 17);
			this->SpriteModeCloneRadioButton->TabIndex = 22;
			this->SpriteModeCloneRadioButton->TabStop = true;
			this->SpriteModeCloneRadioButton->Text = L"Clone";
			this->SpriteModeCloneRadioButton->UseVisualStyleBackColor = true;
			this->SpriteModeCloneRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteModeCloneRadioButton_CheckedChanged);
			// 
			// SpriteModeSelectRadioButton
			// 
			this->SpriteModeSelectRadioButton->AutoSize = true;
			this->SpriteModeSelectRadioButton->Checked = true;
			this->SpriteModeSelectRadioButton->Location = System::Drawing::Point(6, 19);
			this->SpriteModeSelectRadioButton->Name = L"SpriteModeSelectRadioButton";
			this->SpriteModeSelectRadioButton->Size = System::Drawing::Size(55, 17);
			this->SpriteModeSelectRadioButton->TabIndex = 17;
			this->SpriteModeSelectRadioButton->TabStop = true;
			this->SpriteModeSelectRadioButton->Text = L"Select";
			this->SpriteModeSelectRadioButton->UseVisualStyleBackColor = true;
			this->SpriteModeSelectRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteModeSelectRadioButton_CheckedChanged);
			// 
			// SpriteModeSizeRadioButton
			// 
			this->SpriteModeSizeRadioButton->AutoSize = true;
			this->SpriteModeSizeRadioButton->Location = System::Drawing::Point(6, 115);
			this->SpriteModeSizeRadioButton->Name = L"SpriteModeSizeRadioButton";
			this->SpriteModeSizeRadioButton->Size = System::Drawing::Size(45, 17);
			this->SpriteModeSizeRadioButton->TabIndex = 21;
			this->SpriteModeSizeRadioButton->TabStop = true;
			this->SpriteModeSizeRadioButton->Text = L"Size";
			this->SpriteModeSizeRadioButton->UseVisualStyleBackColor = true;
			this->SpriteModeSizeRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteModeSizeRadioButton_CheckedChanged);
			// 
			// SpriteModeMoveRadioButton
			// 
			this->SpriteModeMoveRadioButton->AutoSize = true;
			this->SpriteModeMoveRadioButton->Location = System::Drawing::Point(6, 43);
			this->SpriteModeMoveRadioButton->Name = L"SpriteModeMoveRadioButton";
			this->SpriteModeMoveRadioButton->Size = System::Drawing::Size(52, 17);
			this->SpriteModeMoveRadioButton->TabIndex = 18;
			this->SpriteModeMoveRadioButton->TabStop = true;
			this->SpriteModeMoveRadioButton->Text = L"Move";
			this->SpriteModeMoveRadioButton->UseVisualStyleBackColor = true;
			this->SpriteModeMoveRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteModeMoveRadioButton_CheckedChanged);
			// 
			// SpriteModePlaceRadioButton
			// 
			this->SpriteModePlaceRadioButton->AutoSize = true;
			this->SpriteModePlaceRadioButton->Location = System::Drawing::Point(6, 91);
			this->SpriteModePlaceRadioButton->Name = L"SpriteModePlaceRadioButton";
			this->SpriteModePlaceRadioButton->Size = System::Drawing::Size(52, 17);
			this->SpriteModePlaceRadioButton->TabIndex = 20;
			this->SpriteModePlaceRadioButton->TabStop = true;
			this->SpriteModePlaceRadioButton->Text = L"Place";
			this->SpriteModePlaceRadioButton->UseVisualStyleBackColor = true;
			this->SpriteModePlaceRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteModePlaceRadioButton_CheckedChanged);
			// 
			// SpriteModeDeleteRadioButton
			// 
			this->SpriteModeDeleteRadioButton->AutoSize = true;
			this->SpriteModeDeleteRadioButton->Location = System::Drawing::Point(6, 67);
			this->SpriteModeDeleteRadioButton->Name = L"SpriteModeDeleteRadioButton";
			this->SpriteModeDeleteRadioButton->Size = System::Drawing::Size(56, 17);
			this->SpriteModeDeleteRadioButton->TabIndex = 19;
			this->SpriteModeDeleteRadioButton->TabStop = true;
			this->SpriteModeDeleteRadioButton->Text = L"Delete";
			this->SpriteModeDeleteRadioButton->UseVisualStyleBackColor = true;
			this->SpriteModeDeleteRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteModeDeleteRadioButton_CheckedChanged);
			// 
			// TileModePlatformerGroupBox
			// 
			this->TileModePlatformerGroupBox->Controls->Add(this->TileModePlatformerSelectRadioButton);
			this->TileModePlatformerGroupBox->Controls->Add(this->TileModePlatformerRadioButton);
			this->TileModePlatformerGroupBox->Controls->Add(this->MultiStampPlatformerRadioButton);
			this->TileModePlatformerGroupBox->Controls->Add(this->RemoveTexturePlatformerRadioButton);
			this->TileModePlatformerGroupBox->Location = System::Drawing::Point(304, 472);
			this->TileModePlatformerGroupBox->Name = L"TileModePlatformerGroupBox";
			this->TileModePlatformerGroupBox->Size = System::Drawing::Size(133, 128);
			this->TileModePlatformerGroupBox->TabIndex = 16;
			this->TileModePlatformerGroupBox->TabStop = false;
			this->TileModePlatformerGroupBox->Text = L"Tile Mode Platformer";
			// 
			// TileModePlatformerSelectRadioButton
			// 
			this->TileModePlatformerSelectRadioButton->AutoSize = true;
			this->TileModePlatformerSelectRadioButton->Checked = true;
			this->TileModePlatformerSelectRadioButton->Location = System::Drawing::Point(6, 19);
			this->TileModePlatformerSelectRadioButton->Name = L"TileModePlatformerSelectRadioButton";
			this->TileModePlatformerSelectRadioButton->Size = System::Drawing::Size(55, 17);
			this->TileModePlatformerSelectRadioButton->TabIndex = 14;
			this->TileModePlatformerSelectRadioButton->TabStop = true;
			this->TileModePlatformerSelectRadioButton->Text = L"Select";
			this->TileModePlatformerSelectRadioButton->UseVisualStyleBackColor = true;
			this->TileModePlatformerSelectRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileModePlatformerSelectRadioButton_CheckedChanged);
			// 
			// TileModePlatformerRadioButton
			// 
			this->TileModePlatformerRadioButton->AutoSize = true;
			this->TileModePlatformerRadioButton->Location = System::Drawing::Point(6, 42);
			this->TileModePlatformerRadioButton->Name = L"TileModePlatformerRadioButton";
			this->TileModePlatformerRadioButton->Size = System::Drawing::Size(62, 17);
			this->TileModePlatformerRadioButton->TabIndex = 9;
			this->TileModePlatformerRadioButton->Text = L"Regular";
			this->TileModePlatformerRadioButton->UseVisualStyleBackColor = true;
			this->TileModePlatformerRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileModePlatformerRadioButton_CheckedChanged);
			// 
			// MultiStampPlatformerRadioButton
			// 
			this->MultiStampPlatformerRadioButton->AutoSize = true;
			this->MultiStampPlatformerRadioButton->Location = System::Drawing::Point(6, 65);
			this->MultiStampPlatformerRadioButton->Name = L"MultiStampPlatformerRadioButton";
			this->MultiStampPlatformerRadioButton->Size = System::Drawing::Size(80, 17);
			this->MultiStampPlatformerRadioButton->TabIndex = 10;
			this->MultiStampPlatformerRadioButton->Text = L"Multi Stamp";
			this->MultiStampPlatformerRadioButton->UseVisualStyleBackColor = true;
			this->MultiStampPlatformerRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MultiStampPlatformerRadioButton_CheckedChanged);
			// 
			// RemoveTexturePlatformerRadioButton
			// 
			this->RemoveTexturePlatformerRadioButton->AutoSize = true;
			this->RemoveTexturePlatformerRadioButton->Location = System::Drawing::Point(6, 88);
			this->RemoveTexturePlatformerRadioButton->Name = L"RemoveTexturePlatformerRadioButton";
			this->RemoveTexturePlatformerRadioButton->Size = System::Drawing::Size(104, 17);
			this->RemoveTexturePlatformerRadioButton->TabIndex = 13;
			this->RemoveTexturePlatformerRadioButton->Text = L"Remove Texture";
			this->RemoveTexturePlatformerRadioButton->UseVisualStyleBackColor = true;
			this->RemoveTexturePlatformerRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::RemoveTexturePlatformerRadioButton_CheckedChanged);
			// 
			// TileModeTopdownGroupBox
			// 
			this->TileModeTopdownGroupBox->Controls->Add(this->TileModeTopdownSelectRadioButton);
			this->TileModeTopdownGroupBox->Controls->Add(this->TileModeTopdownRegularRadioButton);
			this->TileModeTopdownGroupBox->Controls->Add(this->TileModeTopdownMultiStampRadioButton);
			this->TileModeTopdownGroupBox->Location = System::Drawing::Point(6, 145);
			this->TileModeTopdownGroupBox->Name = L"TileModeTopdownGroupBox";
			this->TileModeTopdownGroupBox->Size = System::Drawing::Size(122, 101);
			this->TileModeTopdownGroupBox->TabIndex = 15;
			this->TileModeTopdownGroupBox->TabStop = false;
			this->TileModeTopdownGroupBox->Text = L"Tile Mode Topdown";
			// 
			// TileModeTopdownSelectRadioButton
			// 
			this->TileModeTopdownSelectRadioButton->AutoSize = true;
			this->TileModeTopdownSelectRadioButton->Checked = true;
			this->TileModeTopdownSelectRadioButton->Location = System::Drawing::Point(5, 19);
			this->TileModeTopdownSelectRadioButton->Name = L"TileModeTopdownSelectRadioButton";
			this->TileModeTopdownSelectRadioButton->Size = System::Drawing::Size(55, 17);
			this->TileModeTopdownSelectRadioButton->TabIndex = 9;
			this->TileModeTopdownSelectRadioButton->TabStop = true;
			this->TileModeTopdownSelectRadioButton->Text = L"Select";
			this->TileModeTopdownSelectRadioButton->UseVisualStyleBackColor = true;
			this->TileModeTopdownSelectRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileModeTopdownSelectRadioButton_CheckedChanged);
			// 
			// TileModeTopdownRegularRadioButton
			// 
			this->TileModeTopdownRegularRadioButton->AutoSize = true;
			this->TileModeTopdownRegularRadioButton->Location = System::Drawing::Point(6, 42);
			this->TileModeTopdownRegularRadioButton->Name = L"TileModeTopdownRegularRadioButton";
			this->TileModeTopdownRegularRadioButton->Size = System::Drawing::Size(62, 17);
			this->TileModeTopdownRegularRadioButton->TabIndex = 7;
			this->TileModeTopdownRegularRadioButton->Text = L"Regular";
			this->TileModeTopdownRegularRadioButton->UseVisualStyleBackColor = true;
			this->TileModeTopdownRegularRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileModeTopdownRegularRadioButton_CheckedChanged);
			// 
			// TileModeTopdownMultiStampRadioButton
			// 
			this->TileModeTopdownMultiStampRadioButton->AutoSize = true;
			this->TileModeTopdownMultiStampRadioButton->Location = System::Drawing::Point(6, 66);
			this->TileModeTopdownMultiStampRadioButton->Name = L"TileModeTopdownMultiStampRadioButton";
			this->TileModeTopdownMultiStampRadioButton->Size = System::Drawing::Size(80, 17);
			this->TileModeTopdownMultiStampRadioButton->TabIndex = 8;
			this->TileModeTopdownMultiStampRadioButton->Text = L"Multi Stamp";
			this->TileModeTopdownMultiStampRadioButton->UseVisualStyleBackColor = true;
			this->TileModeTopdownMultiStampRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileModeTopdownMultiStampRadioButton_CheckedChanged);
			// 
			// EditModeGroupBox
			// 
			this->EditModeGroupBox->Controls->Add(this->HardnessRadioButton);
			this->EditModeGroupBox->Controls->Add(this->TileRadioButton);
			this->EditModeGroupBox->Controls->Add(this->SpriteRadioButton);
			this->EditModeGroupBox->Location = System::Drawing::Point(6, 43);
			this->EditModeGroupBox->Name = L"EditModeGroupBox";
			this->EditModeGroupBox->Size = System::Drawing::Size(94, 96);
			this->EditModeGroupBox->TabIndex = 14;
			this->EditModeGroupBox->TabStop = false;
			this->EditModeGroupBox->Text = L"Edit Mode";
			// 
			// HardnessRadioButton
			// 
			this->HardnessRadioButton->AutoSize = true;
			this->HardnessRadioButton->Location = System::Drawing::Point(6, 67);
			this->HardnessRadioButton->Name = L"HardnessRadioButton";
			this->HardnessRadioButton->Size = System::Drawing::Size(70, 17);
			this->HardnessRadioButton->TabIndex = 5;
			this->HardnessRadioButton->Text = L"Hardness";
			this->HardnessRadioButton->UseVisualStyleBackColor = true;
			this->HardnessRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::HardnessRadioButton_CheckedChanged);
			// 
			// TileRadioButton
			// 
			this->TileRadioButton->AutoSize = true;
			this->TileRadioButton->Checked = true;
			this->TileRadioButton->Location = System::Drawing::Point(6, 19);
			this->TileRadioButton->Name = L"TileRadioButton";
			this->TileRadioButton->Size = System::Drawing::Size(42, 17);
			this->TileRadioButton->TabIndex = 2;
			this->TileRadioButton->TabStop = true;
			this->TileRadioButton->Text = L"Tile";
			this->TileRadioButton->UseVisualStyleBackColor = true;
			this->TileRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileRadioButton_CheckedChanged);
			// 
			// SpriteRadioButton
			// 
			this->SpriteRadioButton->AutoSize = true;
			this->SpriteRadioButton->Location = System::Drawing::Point(6, 43);
			this->SpriteRadioButton->Name = L"SpriteRadioButton";
			this->SpriteRadioButton->Size = System::Drawing::Size(52, 17);
			this->SpriteRadioButton->TabIndex = 3;
			this->SpriteRadioButton->Text = L"Sprite";
			this->SpriteRadioButton->UseVisualStyleBackColor = true;
			this->SpriteRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteRadioButton_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 6);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Game Path:";
			// 
			// WorldTab
			// 
			this->WorldTab->Controls->Add(this->groupBox7);
			this->WorldTab->Controls->Add(this->NewPlatformerMapGroupbox);
			this->WorldTab->Controls->Add(this->NewTopdownMapGroupbox);
			this->WorldTab->Controls->Add(this->label6);
			this->WorldTab->Controls->Add(this->WorldTabStatusLabel);
			this->WorldTab->Location = System::Drawing::Point(4, 22);
			this->WorldTab->Name = L"WorldTab";
			this->WorldTab->Padding = System::Windows::Forms::Padding(3);
			this->WorldTab->Size = System::Drawing::Size(480, 742);
			this->WorldTab->TabIndex = 1;
			this->WorldTab->Text = L"World";
			this->WorldTab->UseVisualStyleBackColor = true;
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->WorldTabSaveWorldButton);
			this->groupBox7->Controls->Add(this->WorldTabRemoveMapButton);
			this->groupBox7->Controls->Add(this->label4);
			this->groupBox7->Controls->Add(this->WorldTabRenderMapButton);
			this->groupBox7->Controls->Add(this->WorldTabCreateWorldButton);
			this->groupBox7->Controls->Add(this->WorldTabMapNameComboBox);
			this->groupBox7->Controls->Add(this->WorldTabLoadWorldButton);
			this->groupBox7->Controls->Add(this->label5);
			this->groupBox7->Controls->Add(this->WorldTabLoadMapButton);
			this->groupBox7->Controls->Add(this->WorldTabWorldNameTextbox);
			this->groupBox7->Controls->Add(this->WorldTabSaveMapButton);
			this->groupBox7->Location = System::Drawing::Point(18, 54);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(274, 188);
			this->groupBox7->TabIndex = 20;
			this->groupBox7->TabStop = false;
			// 
			// WorldTabSaveWorldButton
			// 
			this->WorldTabSaveWorldButton->Location = System::Drawing::Point(18, 120);
			this->WorldTabSaveWorldButton->Name = L"WorldTabSaveWorldButton";
			this->WorldTabSaveWorldButton->Size = System::Drawing::Size(94, 23);
			this->WorldTabSaveWorldButton->TabIndex = 5;
			this->WorldTabSaveWorldButton->Text = L"Save World";
			this->WorldTabSaveWorldButton->UseVisualStyleBackColor = true;
			this->WorldTabSaveWorldButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabSaveWorldButton_Click);
			// 
			// WorldTabRemoveMapButton
			// 
			this->WorldTabRemoveMapButton->Location = System::Drawing::Point(143, 91);
			this->WorldTabRemoveMapButton->Name = L"WorldTabRemoveMapButton";
			this->WorldTabRemoveMapButton->Size = System::Drawing::Size(95, 23);
			this->WorldTabRemoveMapButton->TabIndex = 19;
			this->WorldTabRemoveMapButton->Text = L"Remove Map";
			this->WorldTabRemoveMapButton->UseVisualStyleBackColor = true;
			this->WorldTabRemoveMapButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabRemoveMapButton_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(15, 20);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(66, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"World Name";
			// 
			// WorldTabRenderMapButton
			// 
			this->WorldTabRenderMapButton->Location = System::Drawing::Point(143, 62);
			this->WorldTabRenderMapButton->Name = L"WorldTabRenderMapButton";
			this->WorldTabRenderMapButton->Size = System::Drawing::Size(95, 23);
			this->WorldTabRenderMapButton->TabIndex = 18;
			this->WorldTabRenderMapButton->Text = L"Render Map";
			this->WorldTabRenderMapButton->UseVisualStyleBackColor = true;
			this->WorldTabRenderMapButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabRenderMapButton_Click);
			// 
			// WorldTabCreateWorldButton
			// 
			this->WorldTabCreateWorldButton->Location = System::Drawing::Point(17, 62);
			this->WorldTabCreateWorldButton->Name = L"WorldTabCreateWorldButton";
			this->WorldTabCreateWorldButton->Size = System::Drawing::Size(95, 23);
			this->WorldTabCreateWorldButton->TabIndex = 3;
			this->WorldTabCreateWorldButton->Text = L"Create World";
			this->WorldTabCreateWorldButton->UseVisualStyleBackColor = true;
			this->WorldTabCreateWorldButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabCreateWorldButton_Click);
			// 
			// WorldTabMapNameComboBox
			// 
			this->WorldTabMapNameComboBox->FormattingEnabled = true;
			this->WorldTabMapNameComboBox->Location = System::Drawing::Point(143, 35);
			this->WorldTabMapNameComboBox->Name = L"WorldTabMapNameComboBox";
			this->WorldTabMapNameComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabMapNameComboBox->TabIndex = 17;
			this->WorldTabMapNameComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabMapNameComboBox_SelectedIndexChanged);
			// 
			// WorldTabLoadWorldButton
			// 
			this->WorldTabLoadWorldButton->Location = System::Drawing::Point(17, 91);
			this->WorldTabLoadWorldButton->Name = L"WorldTabLoadWorldButton";
			this->WorldTabLoadWorldButton->Size = System::Drawing::Size(95, 23);
			this->WorldTabLoadWorldButton->TabIndex = 4;
			this->WorldTabLoadWorldButton->Text = L"Load World";
			this->WorldTabLoadWorldButton->UseVisualStyleBackColor = true;
			this->WorldTabLoadWorldButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabLoadWorldButton_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(140, 19);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(59, 13);
			this->label5->TabIndex = 16;
			this->label5->Text = L"Map Name";
			// 
			// WorldTabLoadMapButton
			// 
			this->WorldTabLoadMapButton->Location = System::Drawing::Point(143, 120);
			this->WorldTabLoadMapButton->Name = L"WorldTabLoadMapButton";
			this->WorldTabLoadMapButton->Size = System::Drawing::Size(95, 23);
			this->WorldTabLoadMapButton->TabIndex = 9;
			this->WorldTabLoadMapButton->Text = L"Load Map";
			this->WorldTabLoadMapButton->UseVisualStyleBackColor = true;
			this->WorldTabLoadMapButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabLoadMapButton_Click);
			// 
			// WorldTabWorldNameTextbox
			// 
			this->WorldTabWorldNameTextbox->Location = System::Drawing::Point(18, 36);
			this->WorldTabWorldNameTextbox->Name = L"WorldTabWorldNameTextbox";
			this->WorldTabWorldNameTextbox->Size = System::Drawing::Size(100, 20);
			this->WorldTabWorldNameTextbox->TabIndex = 15;
			// 
			// WorldTabSaveMapButton
			// 
			this->WorldTabSaveMapButton->Location = System::Drawing::Point(143, 149);
			this->WorldTabSaveMapButton->Name = L"WorldTabSaveMapButton";
			this->WorldTabSaveMapButton->Size = System::Drawing::Size(95, 23);
			this->WorldTabSaveMapButton->TabIndex = 10;
			this->WorldTabSaveMapButton->Text = L"Save Map";
			this->WorldTabSaveMapButton->UseVisualStyleBackColor = true;
			this->WorldTabSaveMapButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabSaveMapButton_Click);
			// 
			// NewPlatformerMapGroupbox
			// 
			this->NewPlatformerMapGroupbox->Controls->Add(this->WorldTabPlatformerMapHeightComboBox);
			this->NewPlatformerMapGroupbox->Controls->Add(this->WorldTabPlatformerMapWidthComboBox);
			this->NewPlatformerMapGroupbox->Controls->Add(this->label39);
			this->NewPlatformerMapGroupbox->Controls->Add(this->WorldTabPlatformerMapNameTextBox);
			this->NewPlatformerMapGroupbox->Controls->Add(this->WorldTabPlatformerWorldNameComboBox);
			this->NewPlatformerMapGroupbox->Controls->Add(this->label9);
			this->NewPlatformerMapGroupbox->Controls->Add(this->label11);
			this->NewPlatformerMapGroupbox->Controls->Add(this->label13);
			this->NewPlatformerMapGroupbox->Controls->Add(this->WorldTabPlatformerCreateMapButton);
			this->NewPlatformerMapGroupbox->Location = System::Drawing::Point(306, 603);
			this->NewPlatformerMapGroupbox->Name = L"NewPlatformerMapGroupbox";
			this->NewPlatformerMapGroupbox->Size = System::Drawing::Size(178, 271);
			this->NewPlatformerMapGroupbox->TabIndex = 8;
			this->NewPlatformerMapGroupbox->TabStop = false;
			this->NewPlatformerMapGroupbox->Text = L"New Platformer Map";
			// 
			// WorldTabPlatformerMapHeightComboBox
			// 
			this->WorldTabPlatformerMapHeightComboBox->FormattingEnabled = true;
			this->WorldTabPlatformerMapHeightComboBox->Location = System::Drawing::Point(10, 91);
			this->WorldTabPlatformerMapHeightComboBox->Name = L"WorldTabPlatformerMapHeightComboBox";
			this->WorldTabPlatformerMapHeightComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabPlatformerMapHeightComboBox->TabIndex = 24;
			// 
			// WorldTabPlatformerMapWidthComboBox
			// 
			this->WorldTabPlatformerMapWidthComboBox->FormattingEnabled = true;
			this->WorldTabPlatformerMapWidthComboBox->Location = System::Drawing::Point(10, 37);
			this->WorldTabPlatformerMapWidthComboBox->Name = L"WorldTabPlatformerMapWidthComboBox";
			this->WorldTabPlatformerMapWidthComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabPlatformerMapWidthComboBox->TabIndex = 23;
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(7, 75);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(62, 13);
			this->label39->TabIndex = 22;
			this->label39->Text = L"Map Height";
			// 
			// WorldTabPlatformerMapNameTextBox
			// 
			this->WorldTabPlatformerMapNameTextBox->Location = System::Drawing::Point(14, 204);
			this->WorldTabPlatformerMapNameTextBox->Name = L"WorldTabPlatformerMapNameTextBox";
			this->WorldTabPlatformerMapNameTextBox->Size = System::Drawing::Size(100, 20);
			this->WorldTabPlatformerMapNameTextBox->TabIndex = 20;
			// 
			// WorldTabPlatformerWorldNameComboBox
			// 
			this->WorldTabPlatformerWorldNameComboBox->FormattingEnabled = true;
			this->WorldTabPlatformerWorldNameComboBox->Location = System::Drawing::Point(10, 158);
			this->WorldTabPlatformerWorldNameComboBox->Name = L"WorldTabPlatformerWorldNameComboBox";
			this->WorldTabPlatformerWorldNameComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabPlatformerWorldNameComboBox->TabIndex = 19;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(11, 188);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(59, 13);
			this->label9->TabIndex = 18;
			this->label9->Text = L"Map Name";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(11, 142);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(66, 13);
			this->label11->TabIndex = 17;
			this->label11->Text = L"World Name";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(7, 19);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(59, 13);
			this->label13->TabIndex = 15;
			this->label13->Text = L"Map Width";
			// 
			// WorldTabPlatformerCreateMapButton
			// 
			this->WorldTabPlatformerCreateMapButton->Location = System::Drawing::Point(10, 240);
			this->WorldTabPlatformerCreateMapButton->Name = L"WorldTabPlatformerCreateMapButton";
			this->WorldTabPlatformerCreateMapButton->Size = System::Drawing::Size(75, 23);
			this->WorldTabPlatformerCreateMapButton->TabIndex = 0;
			this->WorldTabPlatformerCreateMapButton->Text = L"Create Map";
			this->WorldTabPlatformerCreateMapButton->UseVisualStyleBackColor = true;
			this->WorldTabPlatformerCreateMapButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabPlatformerCreateMapButton_Click);
			// 
			// NewTopdownMapGroupbox
			// 
			this->NewTopdownMapGroupbox->Controls->Add(this->WorldTabTopdownMapMapNameTextbox);
			this->NewTopdownMapGroupbox->Controls->Add(this->WorldTabTopdownMapWorldNameComboBox);
			this->NewTopdownMapGroupbox->Controls->Add(this->groupBox5);
			this->NewTopdownMapGroupbox->Controls->Add(this->WorldTabTopdownMapCreateMapButton);
			this->NewTopdownMapGroupbox->Controls->Add(this->label10);
			this->NewTopdownMapGroupbox->Controls->Add(this->groupBox3);
			this->NewTopdownMapGroupbox->Controls->Add(this->label8);
			this->NewTopdownMapGroupbox->Controls->Add(this->WorldTabTopdownMapSizeComboBox);
			this->NewTopdownMapGroupbox->Controls->Add(this->label7);
			this->NewTopdownMapGroupbox->Location = System::Drawing::Point(18, 248);
			this->NewTopdownMapGroupbox->Name = L"NewTopdownMapGroupbox";
			this->NewTopdownMapGroupbox->Size = System::Drawing::Size(337, 318);
			this->NewTopdownMapGroupbox->TabIndex = 7;
			this->NewTopdownMapGroupbox->TabStop = false;
			this->NewTopdownMapGroupbox->Text = L"New Topdown Map";
			// 
			// WorldTabTopdownMapMapNameTextbox
			// 
			this->WorldTabTopdownMapMapNameTextbox->Location = System::Drawing::Point(20, 152);
			this->WorldTabTopdownMapMapNameTextbox->Name = L"WorldTabTopdownMapMapNameTextbox";
			this->WorldTabTopdownMapMapNameTextbox->Size = System::Drawing::Size(100, 20);
			this->WorldTabTopdownMapMapNameTextbox->TabIndex = 14;
			// 
			// WorldTabTopdownMapWorldNameComboBox
			// 
			this->WorldTabTopdownMapWorldNameComboBox->FormattingEnabled = true;
			this->WorldTabTopdownMapWorldNameComboBox->Location = System::Drawing::Point(16, 106);
			this->WorldTabTopdownMapWorldNameComboBox->Name = L"WorldTabTopdownMapWorldNameComboBox";
			this->WorldTabTopdownMapWorldNameComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabTopdownMapWorldNameComboBox->TabIndex = 13;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->WorldTabFOWUncoveredCheckBox);
			this->groupBox5->Controls->Add(this->WorldTabEnableFOWCheckBox);
			this->groupBox5->Controls->Add(this->WorldTabFOWFlashlightCheckBox);
			this->groupBox5->Controls->Add(this->WorldTabFOWRadiusComboBox);
			this->groupBox5->Controls->Add(this->label12);
			this->groupBox5->Location = System::Drawing::Point(183, 106);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(144, 168);
			this->groupBox5->TabIndex = 12;
			this->groupBox5->TabStop = false;
			// 
			// WorldTabFOWUncoveredCheckBox
			// 
			this->WorldTabFOWUncoveredCheckBox->AutoSize = true;
			this->WorldTabFOWUncoveredCheckBox->Location = System::Drawing::Point(17, 129);
			this->WorldTabFOWUncoveredCheckBox->Name = L"WorldTabFOWUncoveredCheckBox";
			this->WorldTabFOWUncoveredCheckBox->Size = System::Drawing::Size(107, 17);
			this->WorldTabFOWUncoveredCheckBox->TabIndex = 14;
			this->WorldTabFOWUncoveredCheckBox->Text = L"FOW Uncovered";
			this->WorldTabFOWUncoveredCheckBox->UseVisualStyleBackColor = true;
			this->WorldTabFOWUncoveredCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabFOWUncoveredCheckBox_CheckedChanged);
			// 
			// WorldTabEnableFOWCheckBox
			// 
			this->WorldTabEnableFOWCheckBox->AutoSize = true;
			this->WorldTabEnableFOWCheckBox->Location = System::Drawing::Point(6, 19);
			this->WorldTabEnableFOWCheckBox->Name = L"WorldTabEnableFOWCheckBox";
			this->WorldTabEnableFOWCheckBox->Size = System::Drawing::Size(87, 17);
			this->WorldTabEnableFOWCheckBox->TabIndex = 13;
			this->WorldTabEnableFOWCheckBox->Text = L"Enable FOW";
			this->WorldTabEnableFOWCheckBox->UseVisualStyleBackColor = true;
			this->WorldTabEnableFOWCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabEnableFOWCheckBox_CheckedChanged);
			// 
			// WorldTabFOWFlashlightCheckBox
			// 
			this->WorldTabFOWFlashlightCheckBox->AutoSize = true;
			this->WorldTabFOWFlashlightCheckBox->Location = System::Drawing::Point(17, 106);
			this->WorldTabFOWFlashlightCheckBox->Name = L"WorldTabFOWFlashlightCheckBox";
			this->WorldTabFOWFlashlightCheckBox->Size = System::Drawing::Size(98, 17);
			this->WorldTabFOWFlashlightCheckBox->TabIndex = 9;
			this->WorldTabFOWFlashlightCheckBox->Text = L"FOW Flashlight";
			this->WorldTabFOWFlashlightCheckBox->UseVisualStyleBackColor = true;
			// 
			// WorldTabFOWRadiusComboBox
			// 
			this->WorldTabFOWRadiusComboBox->FormattingEnabled = true;
			this->WorldTabFOWRadiusComboBox->Location = System::Drawing::Point(17, 71);
			this->WorldTabFOWRadiusComboBox->Name = L"WorldTabFOWRadiusComboBox";
			this->WorldTabFOWRadiusComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabFOWRadiusComboBox->TabIndex = 10;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(14, 55);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(68, 13);
			this->label12->TabIndex = 7;
			this->label12->Text = L"FOW Radius";
			// 
			// WorldTabTopdownMapCreateMapButton
			// 
			this->WorldTabTopdownMapCreateMapButton->Location = System::Drawing::Point(8, 289);
			this->WorldTabTopdownMapCreateMapButton->Name = L"WorldTabTopdownMapCreateMapButton";
			this->WorldTabTopdownMapCreateMapButton->Size = System::Drawing::Size(75, 23);
			this->WorldTabTopdownMapCreateMapButton->TabIndex = 11;
			this->WorldTabTopdownMapCreateMapButton->Text = L"Create Map";
			this->WorldTabTopdownMapCreateMapButton->UseVisualStyleBackColor = true;
			this->WorldTabTopdownMapCreateMapButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::WorldTabTopdownMapCreateMapButton_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(17, 136);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(59, 13);
			this->label10->TabIndex = 5;
			this->label10->Text = L"Map Name";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->WorldTabTopdownMapIsometricRadioButton);
			this->groupBox3->Controls->Add(this->WorldTabTopdownMapStandardRadioButton);
			this->groupBox3->Location = System::Drawing::Point(242, 19);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(85, 74);
			this->groupBox3->TabIndex = 3;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Map Type";
			// 
			// WorldTabTopdownMapIsometricRadioButton
			// 
			this->WorldTabTopdownMapIsometricRadioButton->AutoSize = true;
			this->WorldTabTopdownMapIsometricRadioButton->Location = System::Drawing::Point(6, 42);
			this->WorldTabTopdownMapIsometricRadioButton->Name = L"WorldTabTopdownMapIsometricRadioButton";
			this->WorldTabTopdownMapIsometricRadioButton->Size = System::Drawing::Size(67, 17);
			this->WorldTabTopdownMapIsometricRadioButton->TabIndex = 1;
			this->WorldTabTopdownMapIsometricRadioButton->TabStop = true;
			this->WorldTabTopdownMapIsometricRadioButton->Text = L"Isometric";
			this->WorldTabTopdownMapIsometricRadioButton->UseVisualStyleBackColor = true;
			// 
			// WorldTabTopdownMapStandardRadioButton
			// 
			this->WorldTabTopdownMapStandardRadioButton->AutoSize = true;
			this->WorldTabTopdownMapStandardRadioButton->Checked = true;
			this->WorldTabTopdownMapStandardRadioButton->Location = System::Drawing::Point(6, 19);
			this->WorldTabTopdownMapStandardRadioButton->Name = L"WorldTabTopdownMapStandardRadioButton";
			this->WorldTabTopdownMapStandardRadioButton->Size = System::Drawing::Size(68, 17);
			this->WorldTabTopdownMapStandardRadioButton->TabIndex = 0;
			this->WorldTabTopdownMapStandardRadioButton->TabStop = true;
			this->WorldTabTopdownMapStandardRadioButton->Text = L"Standard";
			this->WorldTabTopdownMapStandardRadioButton->UseVisualStyleBackColor = true;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(17, 90);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(66, 13);
			this->label8->TabIndex = 2;
			this->label8->Text = L"World Name";
			// 
			// WorldTabTopdownMapSizeComboBox
			// 
			this->WorldTabTopdownMapSizeComboBox->FormattingEnabled = true;
			this->WorldTabTopdownMapSizeComboBox->Location = System::Drawing::Point(16, 44);
			this->WorldTabTopdownMapSizeComboBox->Name = L"WorldTabTopdownMapSizeComboBox";
			this->WorldTabTopdownMapSizeComboBox->Size = System::Drawing::Size(121, 21);
			this->WorldTabTopdownMapSizeComboBox->TabIndex = 1;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(13, 28);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(51, 13);
			this->label7->TabIndex = 0;
			this->label7->Text = L"Map Size";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(15, 22);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(40, 13);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Status:";
			// 
			// WorldTabStatusLabel
			// 
			this->WorldTabStatusLabel->AutoSize = true;
			this->WorldTabStatusLabel->Location = System::Drawing::Point(51, 22);
			this->WorldTabStatusLabel->Name = L"WorldTabStatusLabel";
			this->WorldTabStatusLabel->Size = System::Drawing::Size(92, 13);
			this->WorldTabStatusLabel->TabIndex = 0;
			this->WorldTabStatusLabel->Text = L"No World Created";
			// 
			// MapTab
			// 
			this->MapTab->Controls->Add(this->MapTabPlatformerMapGroupBox);
			this->MapTab->Controls->Add(this->MapTabTopdownMapGroupBox);
			this->MapTab->Location = System::Drawing::Point(4, 22);
			this->MapTab->Name = L"MapTab";
			this->MapTab->Padding = System::Windows::Forms::Padding(3);
			this->MapTab->Size = System::Drawing::Size(480, 742);
			this->MapTab->TabIndex = 2;
			this->MapTab->Text = L"Map";
			this->MapTab->UseVisualStyleBackColor = true;
			// 
			// MapTabPlatformerMapGroupBox
			// 
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->MapTabPlatformerUpdateMapNameButton);
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->groupBox15);
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->PlatformerMapHardnessCheckbox);
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->label61);
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->PlatformerMapEditMusicTextbox);
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->label62);
			this->MapTabPlatformerMapGroupBox->Controls->Add(this->PlatformerMapEditNameTextbox);
			this->MapTabPlatformerMapGroupBox->Location = System::Drawing::Point(52, 550);
			this->MapTabPlatformerMapGroupBox->Name = L"MapTabPlatformerMapGroupBox";
			this->MapTabPlatformerMapGroupBox->Size = System::Drawing::Size(295, 236);
			this->MapTabPlatformerMapGroupBox->TabIndex = 5;
			this->MapTabPlatformerMapGroupBox->TabStop = false;
			// 
			// MapTabPlatformerUpdateMapNameButton
			// 
			this->MapTabPlatformerUpdateMapNameButton->Location = System::Drawing::Point(185, 18);
			this->MapTabPlatformerUpdateMapNameButton->Name = L"MapTabPlatformerUpdateMapNameButton";
			this->MapTabPlatformerUpdateMapNameButton->Size = System::Drawing::Size(90, 23);
			this->MapTabPlatformerUpdateMapNameButton->TabIndex = 33;
			this->MapTabPlatformerUpdateMapNameButton->Text = L"Update Name";
			this->MapTabPlatformerUpdateMapNameButton->UseVisualStyleBackColor = true;
			this->MapTabPlatformerUpdateMapNameButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MapTabPlatformerUpdateMapNameButton_Click);
			// 
			// groupBox15
			// 
			this->groupBox15->Controls->Add(this->PlatformerMapVisibleTilesLabel);
			this->groupBox15->Controls->Add(this->label46);
			this->groupBox15->Controls->Add(this->PlatformerMapTotalSpritesLabel);
			this->groupBox15->Controls->Add(this->PlatformerMapSpritesRenderedLabel);
			this->groupBox15->Controls->Add(this->PlatformerMapNumTilesLabel);
			this->groupBox15->Controls->Add(this->PlatformerMapTileSizeLabel);
			this->groupBox15->Controls->Add(this->label53);
			this->groupBox15->Controls->Add(this->label54);
			this->groupBox15->Controls->Add(this->label55);
			this->groupBox15->Controls->Add(this->label56);
			this->groupBox15->Location = System::Drawing::Point(20, 124);
			this->groupBox15->Name = L"groupBox15";
			this->groupBox15->Size = System::Drawing::Size(181, 94);
			this->groupBox15->TabIndex = 32;
			this->groupBox15->TabStop = false;
			// 
			// PlatformerMapVisibleTilesLabel
			// 
			this->PlatformerMapVisibleTilesLabel->AutoSize = true;
			this->PlatformerMapVisibleTilesLabel->Location = System::Drawing::Point(66, 39);
			this->PlatformerMapVisibleTilesLabel->Name = L"PlatformerMapVisibleTilesLabel";
			this->PlatformerMapVisibleTilesLabel->Size = System::Drawing::Size(41, 13);
			this->PlatformerMapVisibleTilesLabel->TabIndex = 34;
			this->PlatformerMapVisibleTilesLabel->Text = L"label29";
			// 
			// label46
			// 
			this->label46->AutoSize = true;
			this->label46->Location = System::Drawing::Point(6, 39);
			this->label46->Name = L"label46";
			this->label46->Size = System::Drawing::Size(62, 13);
			this->label46->TabIndex = 33;
			this->label46->Text = L"Visible Tiles";
			// 
			// PlatformerMapTotalSpritesLabel
			// 
			this->PlatformerMapTotalSpritesLabel->AutoSize = true;
			this->PlatformerMapTotalSpritesLabel->Location = System::Drawing::Point(76, 65);
			this->PlatformerMapTotalSpritesLabel->Name = L"PlatformerMapTotalSpritesLabel";
			this->PlatformerMapTotalSpritesLabel->Size = System::Drawing::Size(41, 13);
			this->PlatformerMapTotalSpritesLabel->TabIndex = 32;
			this->PlatformerMapTotalSpritesLabel->Text = L"label36";
			// 
			// PlatformerMapSpritesRenderedLabel
			// 
			this->PlatformerMapSpritesRenderedLabel->AutoSize = true;
			this->PlatformerMapSpritesRenderedLabel->Location = System::Drawing::Point(96, 52);
			this->PlatformerMapSpritesRenderedLabel->Name = L"PlatformerMapSpritesRenderedLabel";
			this->PlatformerMapSpritesRenderedLabel->Size = System::Drawing::Size(41, 13);
			this->PlatformerMapSpritesRenderedLabel->TabIndex = 31;
			this->PlatformerMapSpritesRenderedLabel->Text = L"label35";
			// 
			// PlatformerMapNumTilesLabel
			// 
			this->PlatformerMapNumTilesLabel->AutoSize = true;
			this->PlatformerMapNumTilesLabel->Location = System::Drawing::Point(59, 26);
			this->PlatformerMapNumTilesLabel->Name = L"PlatformerMapNumTilesLabel";
			this->PlatformerMapNumTilesLabel->Size = System::Drawing::Size(41, 13);
			this->PlatformerMapNumTilesLabel->TabIndex = 25;
			this->PlatformerMapNumTilesLabel->Text = L"label29";
			// 
			// PlatformerMapTileSizeLabel
			// 
			this->PlatformerMapTileSizeLabel->AutoSize = true;
			this->PlatformerMapTileSizeLabel->Location = System::Drawing::Point(59, 13);
			this->PlatformerMapTileSizeLabel->Name = L"PlatformerMapTileSizeLabel";
			this->PlatformerMapTileSizeLabel->Size = System::Drawing::Size(41, 13);
			this->PlatformerMapTileSizeLabel->TabIndex = 24;
			this->PlatformerMapTileSizeLabel->Text = L"label28";
			// 
			// label53
			// 
			this->label53->AutoSize = true;
			this->label53->Location = System::Drawing::Point(6, 13);
			this->label53->Name = L"label53";
			this->label53->Size = System::Drawing::Size(47, 13);
			this->label53->TabIndex = 15;
			this->label53->Text = L"Tile Size";
			// 
			// label54
			// 
			this->label54->AutoSize = true;
			this->label54->Location = System::Drawing::Point(6, 65);
			this->label54->Name = L"label54";
			this->label54->Size = System::Drawing::Size(66, 13);
			this->label54->TabIndex = 23;
			this->label54->Text = L"Total Sprites";
			// 
			// label55
			// 
			this->label55->AutoSize = true;
			this->label55->Location = System::Drawing::Point(6, 26);
			this->label55->Name = L"label55";
			this->label55->Size = System::Drawing::Size(54, 13);
			this->label55->TabIndex = 16;
			this->label55->Text = L"Num Tiles";
			// 
			// label56
			// 
			this->label56->AutoSize = true;
			this->label56->Location = System::Drawing::Point(6, 52);
			this->label56->Name = L"label56";
			this->label56->Size = System::Drawing::Size(89, 13);
			this->label56->TabIndex = 22;
			this->label56->Text = L"Sprites Rendered";
			// 
			// PlatformerMapHardnessCheckbox
			// 
			this->PlatformerMapHardnessCheckbox->AutoSize = true;
			this->PlatformerMapHardnessCheckbox->Location = System::Drawing::Point(20, 83);
			this->PlatformerMapHardnessCheckbox->Name = L"PlatformerMapHardnessCheckbox";
			this->PlatformerMapHardnessCheckbox->Size = System::Drawing::Size(71, 17);
			this->PlatformerMapHardnessCheckbox->TabIndex = 30;
			this->PlatformerMapHardnessCheckbox->Text = L"Hardness";
			this->PlatformerMapHardnessCheckbox->UseVisualStyleBackColor = true;
			this->PlatformerMapHardnessCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::PlatformerMapHardnessCheckbox_CheckedChanged);
			// 
			// label61
			// 
			this->label61->AutoSize = true;
			this->label61->Location = System::Drawing::Point(17, 49);
			this->label61->Name = L"label61";
			this->label61->Size = System::Drawing::Size(56, 13);
			this->label61->TabIndex = 29;
			this->label61->Text = L"Edit Music";
			// 
			// PlatformerMapEditMusicTextbox
			// 
			this->PlatformerMapEditMusicTextbox->Location = System::Drawing::Point(79, 46);
			this->PlatformerMapEditMusicTextbox->Name = L"PlatformerMapEditMusicTextbox";
			this->PlatformerMapEditMusicTextbox->Size = System::Drawing::Size(100, 20);
			this->PlatformerMapEditMusicTextbox->TabIndex = 28;
			this->PlatformerMapEditMusicTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::PlatformerMapEditMusicTextbox_TextChanged);
			// 
			// label62
			// 
			this->label62->AutoSize = true;
			this->label62->Location = System::Drawing::Point(17, 23);
			this->label62->Name = L"label62";
			this->label62->Size = System::Drawing::Size(56, 13);
			this->label62->TabIndex = 26;
			this->label62->Text = L"Edit Name";
			// 
			// PlatformerMapEditNameTextbox
			// 
			this->PlatformerMapEditNameTextbox->Location = System::Drawing::Point(79, 20);
			this->PlatformerMapEditNameTextbox->Name = L"PlatformerMapEditNameTextbox";
			this->PlatformerMapEditNameTextbox->Size = System::Drawing::Size(100, 20);
			this->PlatformerMapEditNameTextbox->TabIndex = 27;
			// 
			// MapTabTopdownMapGroupBox
			// 
			this->MapTabTopdownMapGroupBox->Controls->Add(this->MapTabTopdownUpdateMapNameButton);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->TopdownMapVisionComboBox);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->groupBox11);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->groupBox9);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->TopdownMapHardnessCheckbox);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->label17);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->label16);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->TopdownMapMusicTextbox);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->label14);
			this->MapTabTopdownMapGroupBox->Controls->Add(this->TopdownMapNameTextbox);
			this->MapTabTopdownMapGroupBox->Location = System::Drawing::Point(6, 6);
			this->MapTabTopdownMapGroupBox->Name = L"MapTabTopdownMapGroupBox";
			this->MapTabTopdownMapGroupBox->Size = System::Drawing::Size(300, 517);
			this->MapTabTopdownMapGroupBox->TabIndex = 4;
			this->MapTabTopdownMapGroupBox->TabStop = false;
			// 
			// MapTabTopdownUpdateMapNameButton
			// 
			this->MapTabTopdownUpdateMapNameButton->Location = System::Drawing::Point(185, 29);
			this->MapTabTopdownUpdateMapNameButton->Name = L"MapTabTopdownUpdateMapNameButton";
			this->MapTabTopdownUpdateMapNameButton->Size = System::Drawing::Size(90, 23);
			this->MapTabTopdownUpdateMapNameButton->TabIndex = 27;
			this->MapTabTopdownUpdateMapNameButton->Text = L"Update Name";
			this->MapTabTopdownUpdateMapNameButton->UseVisualStyleBackColor = true;
			this->MapTabTopdownUpdateMapNameButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::MapTabTopdownUpdateMapNameButton_Click);
			// 
			// TopdownMapVisionComboBox
			// 
			this->TopdownMapVisionComboBox->FormattingEnabled = true;
			this->TopdownMapVisionComboBox->Location = System::Drawing::Point(79, 57);
			this->TopdownMapVisionComboBox->Name = L"TopdownMapVisionComboBox";
			this->TopdownMapVisionComboBox->Size = System::Drawing::Size(121, 21);
			this->TopdownMapVisionComboBox->TabIndex = 26;
			this->TopdownMapVisionComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapVisionComboBox_SelectedIndexChanged);
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->TotalSpritesLabel);
			this->groupBox11->Controls->Add(this->SpritesRenderedLabel);
			this->groupBox11->Controls->Add(this->SectionColSizeLabel);
			this->groupBox11->Controls->Add(this->SectionRowSizeLabel);
			this->groupBox11->Controls->Add(this->MapColSizeLabel);
			this->groupBox11->Controls->Add(this->MapRowSizeLabel);
			this->groupBox11->Controls->Add(this->NumSectionsLabel);
			this->groupBox11->Controls->Add(this->NumTilesLabel);
			this->groupBox11->Controls->Add(this->label23);
			this->groupBox11->Controls->Add(this->TileSizeLabel);
			this->groupBox11->Controls->Add(this->label19);
			this->groupBox11->Controls->Add(this->label27);
			this->groupBox11->Controls->Add(this->label20);
			this->groupBox11->Controls->Add(this->label26);
			this->groupBox11->Controls->Add(this->label21);
			this->groupBox11->Controls->Add(this->label25);
			this->groupBox11->Controls->Add(this->label22);
			this->groupBox11->Controls->Add(this->label24);
			this->groupBox11->Location = System::Drawing::Point(20, 356);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(181, 137);
			this->groupBox11->TabIndex = 25;
			this->groupBox11->TabStop = false;
			// 
			// TotalSpritesLabel
			// 
			this->TotalSpritesLabel->AutoSize = true;
			this->TotalSpritesLabel->Location = System::Drawing::Point(76, 117);
			this->TotalSpritesLabel->Name = L"TotalSpritesLabel";
			this->TotalSpritesLabel->Size = System::Drawing::Size(41, 13);
			this->TotalSpritesLabel->TabIndex = 32;
			this->TotalSpritesLabel->Text = L"label36";
			// 
			// SpritesRenderedLabel
			// 
			this->SpritesRenderedLabel->AutoSize = true;
			this->SpritesRenderedLabel->Location = System::Drawing::Point(96, 104);
			this->SpritesRenderedLabel->Name = L"SpritesRenderedLabel";
			this->SpritesRenderedLabel->Size = System::Drawing::Size(41, 13);
			this->SpritesRenderedLabel->TabIndex = 31;
			this->SpritesRenderedLabel->Text = L"label35";
			// 
			// SectionColSizeLabel
			// 
			this->SectionColSizeLabel->AutoSize = true;
			this->SectionColSizeLabel->Location = System::Drawing::Point(89, 91);
			this->SectionColSizeLabel->Name = L"SectionColSizeLabel";
			this->SectionColSizeLabel->Size = System::Drawing::Size(41, 13);
			this->SectionColSizeLabel->TabIndex = 30;
			this->SectionColSizeLabel->Text = L"label34";
			// 
			// SectionRowSizeLabel
			// 
			this->SectionRowSizeLabel->AutoSize = true;
			this->SectionRowSizeLabel->Location = System::Drawing::Point(96, 78);
			this->SectionRowSizeLabel->Name = L"SectionRowSizeLabel";
			this->SectionRowSizeLabel->Size = System::Drawing::Size(41, 13);
			this->SectionRowSizeLabel->TabIndex = 29;
			this->SectionRowSizeLabel->Text = L"label33";
			// 
			// MapColSizeLabel
			// 
			this->MapColSizeLabel->AutoSize = true;
			this->MapColSizeLabel->Location = System::Drawing::Point(76, 65);
			this->MapColSizeLabel->Name = L"MapColSizeLabel";
			this->MapColSizeLabel->Size = System::Drawing::Size(41, 13);
			this->MapColSizeLabel->TabIndex = 28;
			this->MapColSizeLabel->Text = L"label32";
			// 
			// MapRowSizeLabel
			// 
			this->MapRowSizeLabel->AutoSize = true;
			this->MapRowSizeLabel->Location = System::Drawing::Point(81, 52);
			this->MapRowSizeLabel->Name = L"MapRowSizeLabel";
			this->MapRowSizeLabel->Size = System::Drawing::Size(41, 13);
			this->MapRowSizeLabel->TabIndex = 27;
			this->MapRowSizeLabel->Text = L"label31";
			// 
			// NumSectionsLabel
			// 
			this->NumSectionsLabel->AutoSize = true;
			this->NumSectionsLabel->Location = System::Drawing::Point(76, 39);
			this->NumSectionsLabel->Name = L"NumSectionsLabel";
			this->NumSectionsLabel->Size = System::Drawing::Size(41, 13);
			this->NumSectionsLabel->TabIndex = 26;
			this->NumSectionsLabel->Text = L"label30";
			// 
			// NumTilesLabel
			// 
			this->NumTilesLabel->AutoSize = true;
			this->NumTilesLabel->Location = System::Drawing::Point(59, 26);
			this->NumTilesLabel->Name = L"NumTilesLabel";
			this->NumTilesLabel->Size = System::Drawing::Size(41, 13);
			this->NumTilesLabel->TabIndex = 25;
			this->NumTilesLabel->Text = L"label29";
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(6, 65);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(69, 13);
			this->label23->TabIndex = 19;
			this->label23->Text = L"Map Col Size";
			// 
			// TileSizeLabel
			// 
			this->TileSizeLabel->AutoSize = true;
			this->TileSizeLabel->Location = System::Drawing::Point(59, 13);
			this->TileSizeLabel->Name = L"TileSizeLabel";
			this->TileSizeLabel->Size = System::Drawing::Size(41, 13);
			this->TileSizeLabel->TabIndex = 24;
			this->TileSizeLabel->Text = L"label28";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(6, 13);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(47, 13);
			this->label19->TabIndex = 15;
			this->label19->Text = L"Tile Size";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(6, 117);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(66, 13);
			this->label27->TabIndex = 23;
			this->label27->Text = L"Total Sprites";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(6, 26);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(54, 13);
			this->label20->TabIndex = 16;
			this->label20->Text = L"Num Tiles";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(6, 104);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(89, 13);
			this->label26->TabIndex = 22;
			this->label26->Text = L"Sprites Rendered";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(6, 39);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(73, 13);
			this->label21->TabIndex = 17;
			this->label21->Text = L"Num Sections";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(6, 91);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(84, 13);
			this->label25->TabIndex = 21;
			this->label25->Text = L"Section Col Size";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(6, 52);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(76, 13);
			this->label22->TabIndex = 18;
			this->label22->Text = L"Map Row Size";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(6, 78);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(91, 13);
			this->label24->TabIndex = 20;
			this->label24->Text = L"Section Row Size";
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->TopdownMapEnableFOWMapTabCheckBox);
			this->groupBox9->Controls->Add(this->TopdownMapFOWUncoveredMapTabCheckBox);
			this->groupBox9->Controls->Add(this->TopdownMapFOWFlashlightMapTabCheckBox);
			this->groupBox9->Controls->Add(this->TopdownMapFOWRadiusMapTabCB);
			this->groupBox9->Controls->Add(this->label18);
			this->groupBox9->Location = System::Drawing::Point(20, 152);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(163, 164);
			this->groupBox9->TabIndex = 13;
			this->groupBox9->TabStop = false;
			// 
			// TopdownMapEnableFOWMapTabCheckBox
			// 
			this->TopdownMapEnableFOWMapTabCheckBox->AutoSize = true;
			this->TopdownMapEnableFOWMapTabCheckBox->Location = System::Drawing::Point(6, 19);
			this->TopdownMapEnableFOWMapTabCheckBox->Name = L"TopdownMapEnableFOWMapTabCheckBox";
			this->TopdownMapEnableFOWMapTabCheckBox->Size = System::Drawing::Size(87, 17);
			this->TopdownMapEnableFOWMapTabCheckBox->TabIndex = 12;
			this->TopdownMapEnableFOWMapTabCheckBox->Text = L"Enable FOW";
			this->TopdownMapEnableFOWMapTabCheckBox->UseVisualStyleBackColor = true;
			this->TopdownMapEnableFOWMapTabCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapEnableFOWMapTabCheckBox_CheckedChanged);
			// 
			// TopdownMapFOWUncoveredMapTabCheckBox
			// 
			this->TopdownMapFOWUncoveredMapTabCheckBox->AutoSize = true;
			this->TopdownMapFOWUncoveredMapTabCheckBox->Location = System::Drawing::Point(26, 126);
			this->TopdownMapFOWUncoveredMapTabCheckBox->Name = L"TopdownMapFOWUncoveredMapTabCheckBox";
			this->TopdownMapFOWUncoveredMapTabCheckBox->Size = System::Drawing::Size(107, 17);
			this->TopdownMapFOWUncoveredMapTabCheckBox->TabIndex = 11;
			this->TopdownMapFOWUncoveredMapTabCheckBox->Text = L"FOW Uncovered";
			this->TopdownMapFOWUncoveredMapTabCheckBox->UseVisualStyleBackColor = true;
			this->TopdownMapFOWUncoveredMapTabCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapFOWUncoveredMapTabCheckBox_CheckedChanged);
			// 
			// TopdownMapFOWFlashlightMapTabCheckBox
			// 
			this->TopdownMapFOWFlashlightMapTabCheckBox->AutoSize = true;
			this->TopdownMapFOWFlashlightMapTabCheckBox->Location = System::Drawing::Point(26, 104);
			this->TopdownMapFOWFlashlightMapTabCheckBox->Name = L"TopdownMapFOWFlashlightMapTabCheckBox";
			this->TopdownMapFOWFlashlightMapTabCheckBox->Size = System::Drawing::Size(98, 17);
			this->TopdownMapFOWFlashlightMapTabCheckBox->TabIndex = 9;
			this->TopdownMapFOWFlashlightMapTabCheckBox->Text = L"FOW Flashlight";
			this->TopdownMapFOWFlashlightMapTabCheckBox->UseVisualStyleBackColor = true;
			this->TopdownMapFOWFlashlightMapTabCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapFOWFlashlightMapTabCheckBox_CheckedChanged);
			// 
			// TopdownMapFOWRadiusMapTabCB
			// 
			this->TopdownMapFOWRadiusMapTabCB->FormattingEnabled = true;
			this->TopdownMapFOWRadiusMapTabCB->Location = System::Drawing::Point(26, 69);
			this->TopdownMapFOWRadiusMapTabCB->Name = L"TopdownMapFOWRadiusMapTabCB";
			this->TopdownMapFOWRadiusMapTabCB->Size = System::Drawing::Size(121, 21);
			this->TopdownMapFOWRadiusMapTabCB->TabIndex = 10;
			this->TopdownMapFOWRadiusMapTabCB->SelectedIndexChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapFOWRadiusMapTabCB_SelectedIndexChanged);
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(23, 53);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(68, 13);
			this->label18->TabIndex = 7;
			this->label18->Text = L"FOW Radius";
			// 
			// TopdownMapHardnessCheckbox
			// 
			this->TopdownMapHardnessCheckbox->AutoSize = true;
			this->TopdownMapHardnessCheckbox->Location = System::Drawing::Point(20, 129);
			this->TopdownMapHardnessCheckbox->Name = L"TopdownMapHardnessCheckbox";
			this->TopdownMapHardnessCheckbox->Size = System::Drawing::Size(71, 17);
			this->TopdownMapHardnessCheckbox->TabIndex = 8;
			this->TopdownMapHardnessCheckbox->Text = L"Hardness";
			this->TopdownMapHardnessCheckbox->UseVisualStyleBackColor = true;
			this->TopdownMapHardnessCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapHardnessCheckbox_CheckedChanged);
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(17, 95);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(56, 13);
			this->label17->TabIndex = 7;
			this->label17->Text = L"Edit Music";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(17, 60);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(56, 13);
			this->label16->TabIndex = 5;
			this->label16->Text = L"Edit Vision";
			// 
			// TopdownMapMusicTextbox
			// 
			this->TopdownMapMusicTextbox->Location = System::Drawing::Point(79, 92);
			this->TopdownMapMusicTextbox->Name = L"TopdownMapMusicTextbox";
			this->TopdownMapMusicTextbox->Size = System::Drawing::Size(100, 20);
			this->TopdownMapMusicTextbox->TabIndex = 3;
			this->TopdownMapMusicTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TopdownMapMusicTextbox_TextChanged);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(17, 34);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(56, 13);
			this->label14->TabIndex = 0;
			this->label14->Text = L"Edit Name";
			// 
			// TopdownMapNameTextbox
			// 
			this->TopdownMapNameTextbox->Location = System::Drawing::Point(79, 31);
			this->TopdownMapNameTextbox->Name = L"TopdownMapNameTextbox";
			this->TopdownMapNameTextbox->Size = System::Drawing::Size(100, 20);
			this->TopdownMapNameTextbox->TabIndex = 1;
			// 
			// TilesTab
			// 
			this->TilesTab->Controls->Add(this->TilesTabAnimNameTextbox);
			this->TilesTab->Controls->Add(this->TilesTabAnimSpeedTextbox);
			this->TilesTab->Controls->Add(this->TilesTabTextureNameTextbox);
			this->TilesTab->Controls->Add(this->TilesTabYPosLabel);
			this->TilesTab->Controls->Add(this->TilesTabXPosLabel);
			this->TilesTab->Controls->Add(this->button14);
			this->TilesTab->Controls->Add(this->groupBox12);
			this->TilesTab->Controls->Add(this->TilesTabRevertAnimButton);
			this->TilesTab->Controls->Add(this->TilesTabUpdateAnimButton);
			this->TilesTab->Controls->Add(this->label33);
			this->TilesTab->Controls->Add(this->label32);
			this->TilesTab->Controls->Add(this->label31);
			this->TilesTab->Controls->Add(this->label30);
			this->TilesTab->Controls->Add(this->label29);
			this->TilesTab->Location = System::Drawing::Point(4, 22);
			this->TilesTab->Name = L"TilesTab";
			this->TilesTab->Padding = System::Windows::Forms::Padding(3);
			this->TilesTab->Size = System::Drawing::Size(480, 742);
			this->TilesTab->TabIndex = 3;
			this->TilesTab->Text = L"Tiles";
			this->TilesTab->UseVisualStyleBackColor = true;
			// 
			// TilesTabAnimNameTextbox
			// 
			this->TilesTabAnimNameTextbox->Location = System::Drawing::Point(112, 253);
			this->TilesTabAnimNameTextbox->Name = L"TilesTabAnimNameTextbox";
			this->TilesTabAnimNameTextbox->Size = System::Drawing::Size(100, 20);
			this->TilesTabAnimNameTextbox->TabIndex = 15;
			// 
			// TilesTabAnimSpeedTextbox
			// 
			this->TilesTabAnimSpeedTextbox->Location = System::Drawing::Point(112, 227);
			this->TilesTabAnimSpeedTextbox->Name = L"TilesTabAnimSpeedTextbox";
			this->TilesTabAnimSpeedTextbox->Size = System::Drawing::Size(100, 20);
			this->TilesTabAnimSpeedTextbox->TabIndex = 14;
			// 
			// TilesTabTextureNameTextbox
			// 
			this->TilesTabTextureNameTextbox->Location = System::Drawing::Point(112, 201);
			this->TilesTabTextureNameTextbox->Name = L"TilesTabTextureNameTextbox";
			this->TilesTabTextureNameTextbox->Size = System::Drawing::Size(100, 20);
			this->TilesTabTextureNameTextbox->TabIndex = 13;
			// 
			// TilesTabYPosLabel
			// 
			this->TilesTabYPosLabel->AutoSize = true;
			this->TilesTabYPosLabel->Location = System::Drawing::Point(29, 26);
			this->TilesTabYPosLabel->Name = L"TilesTabYPosLabel";
			this->TilesTabYPosLabel->Size = System::Drawing::Size(41, 13);
			this->TilesTabYPosLabel->TabIndex = 12;
			this->TilesTabYPosLabel->Text = L"label35";
			// 
			// TilesTabXPosLabel
			// 
			this->TilesTabXPosLabel->AutoSize = true;
			this->TilesTabXPosLabel->Location = System::Drawing::Point(29, 13);
			this->TilesTabXPosLabel->Name = L"TilesTabXPosLabel";
			this->TilesTabXPosLabel->Size = System::Drawing::Size(41, 13);
			this->TilesTabXPosLabel->TabIndex = 11;
			this->TilesTabXPosLabel->Text = L"label34";
			// 
			// button14
			// 
			this->button14->Location = System::Drawing::Point(19, 324);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(88, 23);
			this->button14->TabIndex = 10;
			this->button14->Text = L"Update";
			this->button14->UseVisualStyleBackColor = true;
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->TilesTabHardNoRadioButton);
			this->groupBox12->Controls->Add(this->TilesTabHardYesRadioButton);
			this->groupBox12->Location = System::Drawing::Point(29, 101);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(64, 72);
			this->groupBox12->TabIndex = 9;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"Hard";
			// 
			// TilesTabHardNoRadioButton
			// 
			this->TilesTabHardNoRadioButton->AutoSize = true;
			this->TilesTabHardNoRadioButton->Location = System::Drawing::Point(6, 42);
			this->TilesTabHardNoRadioButton->Name = L"TilesTabHardNoRadioButton";
			this->TilesTabHardNoRadioButton->Size = System::Drawing::Size(39, 17);
			this->TilesTabHardNoRadioButton->TabIndex = 1;
			this->TilesTabHardNoRadioButton->TabStop = true;
			this->TilesTabHardNoRadioButton->Text = L"No";
			this->TilesTabHardNoRadioButton->UseVisualStyleBackColor = true;
			// 
			// TilesTabHardYesRadioButton
			// 
			this->TilesTabHardYesRadioButton->AutoSize = true;
			this->TilesTabHardYesRadioButton->Location = System::Drawing::Point(6, 19);
			this->TilesTabHardYesRadioButton->Name = L"TilesTabHardYesRadioButton";
			this->TilesTabHardYesRadioButton->Size = System::Drawing::Size(43, 17);
			this->TilesTabHardYesRadioButton->TabIndex = 0;
			this->TilesTabHardYesRadioButton->TabStop = true;
			this->TilesTabHardYesRadioButton->Text = L"Yes";
			this->TilesTabHardYesRadioButton->UseVisualStyleBackColor = true;
			// 
			// TilesTabRevertAnimButton
			// 
			this->TilesTabRevertAnimButton->Location = System::Drawing::Point(113, 295);
			this->TilesTabRevertAnimButton->Name = L"TilesTabRevertAnimButton";
			this->TilesTabRevertAnimButton->Size = System::Drawing::Size(88, 23);
			this->TilesTabRevertAnimButton->TabIndex = 8;
			this->TilesTabRevertAnimButton->Text = L"Revert Anim";
			this->TilesTabRevertAnimButton->UseVisualStyleBackColor = true;
			// 
			// TilesTabUpdateAnimButton
			// 
			this->TilesTabUpdateAnimButton->Location = System::Drawing::Point(19, 295);
			this->TilesTabUpdateAnimButton->Name = L"TilesTabUpdateAnimButton";
			this->TilesTabUpdateAnimButton->Size = System::Drawing::Size(88, 23);
			this->TilesTabUpdateAnimButton->TabIndex = 7;
			this->TilesTabUpdateAnimButton->Text = L"Update Anim";
			this->TilesTabUpdateAnimButton->UseVisualStyleBackColor = true;
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(26, 256);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(81, 13);
			this->label33->TabIndex = 6;
			this->label33->Text = L"AnimationName";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(26, 230);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(64, 13);
			this->label32->TabIndex = 5;
			this->label32->Text = L"Anim Speed";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(26, 204);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(74, 13);
			this->label31->TabIndex = 4;
			this->label31->Text = L"Texture Name";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(9, 26);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(14, 13);
			this->label30->TabIndex = 3;
			this->label30->Text = L"Y";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(9, 13);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(14, 13);
			this->label29->TabIndex = 2;
			this->label29->Text = L"X";
			// 
			// SpritesTab
			// 
			this->SpritesTab->Controls->Add(this->SpriteTabTopdownSpriteGroupBox);
			this->SpritesTab->Controls->Add(this->SpriteTabResizeGroupBox);
			this->SpritesTab->Location = System::Drawing::Point(4, 22);
			this->SpritesTab->Name = L"SpritesTab";
			this->SpritesTab->Padding = System::Windows::Forms::Padding(3);
			this->SpritesTab->Size = System::Drawing::Size(480, 742);
			this->SpritesTab->TabIndex = 4;
			this->SpritesTab->Text = L"Sprites";
			this->SpritesTab->UseVisualStyleBackColor = true;
			// 
			// SpriteTabPlatformerSpriteGroupBox
			// 
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerScriptBrowseButton);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerTouchCheckbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerNoHitCheckbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerHardCheckbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->groupBox6);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->groupBox17);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerYLabel);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerXLabel);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerScriptOpenButton);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label69);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerScriptTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label70);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label71);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerLevelTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label72);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerExperienceTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->groupBox23);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label73);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerDefenseTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label74);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerSpeedTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label75);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerStrengthTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label76);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerGoldTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label77);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerManaTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label78);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerHPTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label79);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerHPMaxTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label80);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerCollideMsgTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label81);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerAnimationSpeedTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label82);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerDefaultSequenceTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label83);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerRevertAnimationButton);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerUpdateAnimationButton);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerAnimationNameTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label84);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerBrowseTextureNameButton);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerTextureNameTextbox);
			this->SpriteTabPlatformerSpriteGroupBox->Controls->Add(this->label85);
			this->SpriteTabPlatformerSpriteGroupBox->Location = System::Drawing::Point(15, 0);
			this->SpriteTabPlatformerSpriteGroupBox->Name = L"SpriteTabPlatformerSpriteGroupBox";
			this->SpriteTabPlatformerSpriteGroupBox->Size = System::Drawing::Size(447, 510);
			this->SpriteTabPlatformerSpriteGroupBox->TabIndex = 22;
			this->SpriteTabPlatformerSpriteGroupBox->TabStop = false;
			// 
			// SpriteTabPlatformerScriptBrowseButton
			// 
			this->SpriteTabPlatformerScriptBrowseButton->Location = System::Drawing::Point(240, 419);
			this->SpriteTabPlatformerScriptBrowseButton->Name = L"SpriteTabPlatformerScriptBrowseButton";
			this->SpriteTabPlatformerScriptBrowseButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabPlatformerScriptBrowseButton->TabIndex = 53;
			this->SpriteTabPlatformerScriptBrowseButton->Text = L"Browse";
			this->SpriteTabPlatformerScriptBrowseButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerScriptBrowseButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerScriptBrowseButton_Click);
			// 
			// SpriteTabPlatformerTouchCheckbox
			// 
			this->SpriteTabPlatformerTouchCheckbox->AutoSize = true;
			this->SpriteTabPlatformerTouchCheckbox->Location = System::Drawing::Point(346, 459);
			this->SpriteTabPlatformerTouchCheckbox->Name = L"SpriteTabPlatformerTouchCheckbox";
			this->SpriteTabPlatformerTouchCheckbox->Size = System::Drawing::Size(57, 17);
			this->SpriteTabPlatformerTouchCheckbox->TabIndex = 51;
			this->SpriteTabPlatformerTouchCheckbox->Text = L"Touch";
			this->SpriteTabPlatformerTouchCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerTouchCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerTouchCheckbox_CheckedChanged);
			// 
			// SpriteTabPlatformerNoHitCheckbox
			// 
			this->SpriteTabPlatformerNoHitCheckbox->AutoSize = true;
			this->SpriteTabPlatformerNoHitCheckbox->Location = System::Drawing::Point(346, 435);
			this->SpriteTabPlatformerNoHitCheckbox->Name = L"SpriteTabPlatformerNoHitCheckbox";
			this->SpriteTabPlatformerNoHitCheckbox->Size = System::Drawing::Size(56, 17);
			this->SpriteTabPlatformerNoHitCheckbox->TabIndex = 50;
			this->SpriteTabPlatformerNoHitCheckbox->Text = L"No Hit";
			this->SpriteTabPlatformerNoHitCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerNoHitCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerNoHitCheckbox_CheckedChanged);
			// 
			// SpriteTabPlatformerHardCheckbox
			// 
			this->SpriteTabPlatformerHardCheckbox->AutoSize = true;
			this->SpriteTabPlatformerHardCheckbox->Location = System::Drawing::Point(346, 412);
			this->SpriteTabPlatformerHardCheckbox->Name = L"SpriteTabPlatformerHardCheckbox";
			this->SpriteTabPlatformerHardCheckbox->Size = System::Drawing::Size(49, 17);
			this->SpriteTabPlatformerHardCheckbox->TabIndex = 49;
			this->SpriteTabPlatformerHardCheckbox->Text = L"Hard";
			this->SpriteTabPlatformerHardCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerHardCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerHardCheckbox_CheckedChanged);
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->SpriteTabPlatformerInvisibleRadioButton);
			this->groupBox6->Controls->Add(this->SpriteTabPlatformerEntityRadioButton);
			this->groupBox6->Controls->Add(this->SpriteTabPlatformerBackgroundRadioButton);
			this->groupBox6->Location = System::Drawing::Point(343, 242);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(96, 89);
			this->groupBox6->TabIndex = 39;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Type";
			// 
			// SpriteTabPlatformerInvisibleRadioButton
			// 
			this->SpriteTabPlatformerInvisibleRadioButton->AutoSize = true;
			this->SpriteTabPlatformerInvisibleRadioButton->Location = System::Drawing::Point(6, 65);
			this->SpriteTabPlatformerInvisibleRadioButton->Name = L"SpriteTabPlatformerInvisibleRadioButton";
			this->SpriteTabPlatformerInvisibleRadioButton->Size = System::Drawing::Size(63, 17);
			this->SpriteTabPlatformerInvisibleRadioButton->TabIndex = 2;
			this->SpriteTabPlatformerInvisibleRadioButton->Text = L"Invisible";
			this->SpriteTabPlatformerInvisibleRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerInvisibleRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerInvisibleRadioButton_CheckedChanged);
			// 
			// SpriteTabPlatformerEntityRadioButton
			// 
			this->SpriteTabPlatformerEntityRadioButton->AutoSize = true;
			this->SpriteTabPlatformerEntityRadioButton->Checked = true;
			this->SpriteTabPlatformerEntityRadioButton->Location = System::Drawing::Point(6, 42);
			this->SpriteTabPlatformerEntityRadioButton->Name = L"SpriteTabPlatformerEntityRadioButton";
			this->SpriteTabPlatformerEntityRadioButton->Size = System::Drawing::Size(51, 17);
			this->SpriteTabPlatformerEntityRadioButton->TabIndex = 1;
			this->SpriteTabPlatformerEntityRadioButton->TabStop = true;
			this->SpriteTabPlatformerEntityRadioButton->Text = L"Entity";
			this->SpriteTabPlatformerEntityRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerEntityRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerEntityRadioButton_CheckedChanged);
			// 
			// SpriteTabPlatformerBackgroundRadioButton
			// 
			this->SpriteTabPlatformerBackgroundRadioButton->AutoSize = true;
			this->SpriteTabPlatformerBackgroundRadioButton->Location = System::Drawing::Point(6, 19);
			this->SpriteTabPlatformerBackgroundRadioButton->Name = L"SpriteTabPlatformerBackgroundRadioButton";
			this->SpriteTabPlatformerBackgroundRadioButton->Size = System::Drawing::Size(83, 17);
			this->SpriteTabPlatformerBackgroundRadioButton->TabIndex = 0;
			this->SpriteTabPlatformerBackgroundRadioButton->Text = L"Background";
			this->SpriteTabPlatformerBackgroundRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerBackgroundRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerBackgroundRadioButton_CheckedChanged);
			// 
			// groupBox17
			// 
			this->groupBox17->Controls->Add(this->SpriteTabPlatformerTeleportSpriteNoRadioButton);
			this->groupBox17->Controls->Add(this->SpriteTabPlatformerTeleportSpriteYesRadioButton);
			this->groupBox17->Controls->Add(this->SpriteTabPlatformerTeleportSpriteXTextbox);
			this->groupBox17->Controls->Add(this->SpriteTabPlatformerTeleportSpriteYTextbox);
			this->groupBox17->Controls->Add(this->label3);
			this->groupBox17->Controls->Add(this->label34);
			this->groupBox17->Controls->Add(this->SpriteTabPlatformerTeleportSpriteMapNameTextbox);
			this->groupBox17->Controls->Add(this->label66);
			this->groupBox17->Location = System::Drawing::Point(247, 12);
			this->groupBox17->Name = L"groupBox17";
			this->groupBox17->Size = System::Drawing::Size(194, 130);
			this->groupBox17->TabIndex = 48;
			this->groupBox17->TabStop = false;
			this->groupBox17->Text = L"Teleport Sprite";
			// 
			// SpriteTabPlatformerTeleportSpriteNoRadioButton
			// 
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->AutoSize = true;
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->Checked = true;
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->Location = System::Drawing::Point(56, 19);
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->Name = L"SpriteTabPlatformerTeleportSpriteNoRadioButton";
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->Size = System::Drawing::Size(39, 17);
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->TabIndex = 1;
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->TabStop = true;
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->Text = L"No";
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerTeleportSpriteNoRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerTeleportSpriteNoRadioButton_CheckedChanged);
			// 
			// SpriteTabPlatformerTeleportSpriteYesRadioButton
			// 
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->AutoSize = true;
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->Location = System::Drawing::Point(7, 19);
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->Name = L"SpriteTabPlatformerTeleportSpriteYesRadioButton";
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->Size = System::Drawing::Size(43, 17);
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->TabIndex = 0;
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->Text = L"Yes";
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerTeleportSpriteYesRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerTeleportSpriteYesRadioButton_CheckedChanged);
			// 
			// SpriteTabPlatformerTeleportSpriteXTextbox
			// 
			this->SpriteTabPlatformerTeleportSpriteXTextbox->Location = System::Drawing::Point(69, 71);
			this->SpriteTabPlatformerTeleportSpriteXTextbox->Name = L"SpriteTabPlatformerTeleportSpriteXTextbox";
			this->SpriteTabPlatformerTeleportSpriteXTextbox->Size = System::Drawing::Size(112, 20);
			this->SpriteTabPlatformerTeleportSpriteXTextbox->TabIndex = 45;
			this->SpriteTabPlatformerTeleportSpriteXTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerTeleportSpriteXTextbox_TextChanged);
			// 
			// SpriteTabPlatformerTeleportSpriteYTextbox
			// 
			this->SpriteTabPlatformerTeleportSpriteYTextbox->Location = System::Drawing::Point(69, 97);
			this->SpriteTabPlatformerTeleportSpriteYTextbox->Name = L"SpriteTabPlatformerTeleportSpriteYTextbox";
			this->SpriteTabPlatformerTeleportSpriteYTextbox->Size = System::Drawing::Size(112, 20);
			this->SpriteTabPlatformerTeleportSpriteYTextbox->TabIndex = 47;
			this->SpriteTabPlatformerTeleportSpriteYTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerTeleportSpriteYTextbox_TextChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(45, 74);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(14, 13);
			this->label3->TabIndex = 44;
			this->label3->Text = L"X";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(45, 100);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(14, 13);
			this->label34->TabIndex = 46;
			this->label34->Text = L"Y";
			// 
			// SpriteTabPlatformerTeleportSpriteMapNameTextbox
			// 
			this->SpriteTabPlatformerTeleportSpriteMapNameTextbox->Location = System::Drawing::Point(69, 45);
			this->SpriteTabPlatformerTeleportSpriteMapNameTextbox->Name = L"SpriteTabPlatformerTeleportSpriteMapNameTextbox";
			this->SpriteTabPlatformerTeleportSpriteMapNameTextbox->Size = System::Drawing::Size(112, 20);
			this->SpriteTabPlatformerTeleportSpriteMapNameTextbox->TabIndex = 40;
			this->SpriteTabPlatformerTeleportSpriteMapNameTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerTeleportSpriteMapNameTextbox_TextChanged);
			// 
			// label66
			// 
			this->label66->AutoSize = true;
			this->label66->Location = System::Drawing::Point(4, 48);
			this->label66->Name = L"label66";
			this->label66->Size = System::Drawing::Size(59, 13);
			this->label66->TabIndex = 39;
			this->label66->Text = L"Map Name";
			// 
			// SpriteTabPlatformerYLabel
			// 
			this->SpriteTabPlatformerYLabel->AutoSize = true;
			this->SpriteTabPlatformerYLabel->Location = System::Drawing::Point(35, 31);
			this->SpriteTabPlatformerYLabel->Name = L"SpriteTabPlatformerYLabel";
			this->SpriteTabPlatformerYLabel->Size = System::Drawing::Size(41, 13);
			this->SpriteTabPlatformerYLabel->TabIndex = 19;
			this->SpriteTabPlatformerYLabel->Text = L"label35";
			// 
			// SpriteTabPlatformerXLabel
			// 
			this->SpriteTabPlatformerXLabel->AutoSize = true;
			this->SpriteTabPlatformerXLabel->Location = System::Drawing::Point(35, 18);
			this->SpriteTabPlatformerXLabel->Name = L"SpriteTabPlatformerXLabel";
			this->SpriteTabPlatformerXLabel->Size = System::Drawing::Size(41, 13);
			this->SpriteTabPlatformerXLabel->TabIndex = 18;
			this->SpriteTabPlatformerXLabel->Text = L"label34";
			// 
			// SpriteTabPlatformerScriptOpenButton
			// 
			this->SpriteTabPlatformerScriptOpenButton->Location = System::Drawing::Point(159, 419);
			this->SpriteTabPlatformerScriptOpenButton->Name = L"SpriteTabPlatformerScriptOpenButton";
			this->SpriteTabPlatformerScriptOpenButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabPlatformerScriptOpenButton->TabIndex = 43;
			this->SpriteTabPlatformerScriptOpenButton->Text = L"Open";
			this->SpriteTabPlatformerScriptOpenButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerScriptOpenButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerScriptOpenButton_Click);
			// 
			// label69
			// 
			this->label69->AutoSize = true;
			this->label69->Location = System::Drawing::Point(15, 31);
			this->label69->Name = L"label69";
			this->label69->Size = System::Drawing::Size(14, 13);
			this->label69->TabIndex = 16;
			this->label69->Text = L"Y";
			// 
			// SpriteTabPlatformerScriptTextbox
			// 
			this->SpriteTabPlatformerScriptTextbox->Location = System::Drawing::Point(53, 421);
			this->SpriteTabPlatformerScriptTextbox->Name = L"SpriteTabPlatformerScriptTextbox";
			this->SpriteTabPlatformerScriptTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabPlatformerScriptTextbox->TabIndex = 42;
			this->SpriteTabPlatformerScriptTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerScriptTextbox_TextChanged);
			// 
			// label70
			// 
			this->label70->AutoSize = true;
			this->label70->Location = System::Drawing::Point(15, 18);
			this->label70->Name = L"label70";
			this->label70->Size = System::Drawing::Size(14, 13);
			this->label70->TabIndex = 15;
			this->label70->Text = L"X";
			// 
			// label71
			// 
			this->label71->AutoSize = true;
			this->label71->Location = System::Drawing::Point(11, 424);
			this->label71->Name = L"label71";
			this->label71->Size = System::Drawing::Size(34, 13);
			this->label71->TabIndex = 41;
			this->label71->Text = L"Script";
			// 
			// SpriteTabPlatformerLevelTextbox
			// 
			this->SpriteTabPlatformerLevelTextbox->Location = System::Drawing::Point(54, 397);
			this->SpriteTabPlatformerLevelTextbox->Name = L"SpriteTabPlatformerLevelTextbox";
			this->SpriteTabPlatformerLevelTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerLevelTextbox->TabIndex = 35;
			this->SpriteTabPlatformerLevelTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerLevelTextbox_TextChanged);
			// 
			// label72
			// 
			this->label72->AutoSize = true;
			this->label72->Location = System::Drawing::Point(14, 400);
			this->label72->Name = L"label72";
			this->label72->Size = System::Drawing::Size(33, 13);
			this->label72->TabIndex = 34;
			this->label72->Text = L"Level";
			// 
			// SpriteTabPlatformerExperienceTextbox
			// 
			this->SpriteTabPlatformerExperienceTextbox->Location = System::Drawing::Point(80, 371);
			this->SpriteTabPlatformerExperienceTextbox->Name = L"SpriteTabPlatformerExperienceTextbox";
			this->SpriteTabPlatformerExperienceTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerExperienceTextbox->TabIndex = 33;
			this->SpriteTabPlatformerExperienceTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerExperienceTextbox_TextChanged);
			// 
			// groupBox23
			// 
			this->groupBox23->Controls->Add(this->SpriteTabPlatformerDynamicRadioButton);
			this->groupBox23->Controls->Add(this->SpriteTabPlatformerStaticRadioButton);
			this->groupBox23->Location = System::Drawing::Point(346, 334);
			this->groupBox23->Name = L"groupBox23";
			this->groupBox23->Size = System::Drawing::Size(93, 72);
			this->groupBox23->TabIndex = 19;
			this->groupBox23->TabStop = false;
			this->groupBox23->Text = L"Physics Type";
			// 
			// SpriteTabPlatformerDynamicRadioButton
			// 
			this->SpriteTabPlatformerDynamicRadioButton->AutoSize = true;
			this->SpriteTabPlatformerDynamicRadioButton->Location = System::Drawing::Point(6, 42);
			this->SpriteTabPlatformerDynamicRadioButton->Name = L"SpriteTabPlatformerDynamicRadioButton";
			this->SpriteTabPlatformerDynamicRadioButton->Size = System::Drawing::Size(66, 17);
			this->SpriteTabPlatformerDynamicRadioButton->TabIndex = 1;
			this->SpriteTabPlatformerDynamicRadioButton->Text = L"Dynamic";
			this->SpriteTabPlatformerDynamicRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerDynamicRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerDynamicRadioButton_CheckedChanged);
			// 
			// SpriteTabPlatformerStaticRadioButton
			// 
			this->SpriteTabPlatformerStaticRadioButton->AutoSize = true;
			this->SpriteTabPlatformerStaticRadioButton->Checked = true;
			this->SpriteTabPlatformerStaticRadioButton->Location = System::Drawing::Point(6, 19);
			this->SpriteTabPlatformerStaticRadioButton->Name = L"SpriteTabPlatformerStaticRadioButton";
			this->SpriteTabPlatformerStaticRadioButton->Size = System::Drawing::Size(52, 17);
			this->SpriteTabPlatformerStaticRadioButton->TabIndex = 0;
			this->SpriteTabPlatformerStaticRadioButton->TabStop = true;
			this->SpriteTabPlatformerStaticRadioButton->Text = L"Static";
			this->SpriteTabPlatformerStaticRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerStaticRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerStaticRadioButton_CheckedChanged);
			// 
			// label73
			// 
			this->label73->AutoSize = true;
			this->label73->Location = System::Drawing::Point(14, 374);
			this->label73->Name = L"label73";
			this->label73->Size = System::Drawing::Size(60, 13);
			this->label73->TabIndex = 32;
			this->label73->Text = L"Experience";
			// 
			// SpriteTabPlatformerDefenseTextbox
			// 
			this->SpriteTabPlatformerDefenseTextbox->Location = System::Drawing::Point(64, 345);
			this->SpriteTabPlatformerDefenseTextbox->Name = L"SpriteTabPlatformerDefenseTextbox";
			this->SpriteTabPlatformerDefenseTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerDefenseTextbox->TabIndex = 31;
			this->SpriteTabPlatformerDefenseTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerDefenseTextbox_TextChanged);
			// 
			// label74
			// 
			this->label74->AutoSize = true;
			this->label74->Location = System::Drawing::Point(14, 348);
			this->label74->Name = L"label74";
			this->label74->Size = System::Drawing::Size(47, 13);
			this->label74->TabIndex = 30;
			this->label74->Text = L"Defense";
			// 
			// SpriteTabPlatformerSpeedTextbox
			// 
			this->SpriteTabPlatformerSpeedTextbox->Location = System::Drawing::Point(54, 319);
			this->SpriteTabPlatformerSpeedTextbox->Name = L"SpriteTabPlatformerSpeedTextbox";
			this->SpriteTabPlatformerSpeedTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerSpeedTextbox->TabIndex = 29;
			this->SpriteTabPlatformerSpeedTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerSpeedTextbox_TextChanged);
			// 
			// label75
			// 
			this->label75->AutoSize = true;
			this->label75->Location = System::Drawing::Point(14, 322);
			this->label75->Name = L"label75";
			this->label75->Size = System::Drawing::Size(38, 13);
			this->label75->TabIndex = 28;
			this->label75->Text = L"Speed";
			// 
			// SpriteTabPlatformerStrengthTextbox
			// 
			this->SpriteTabPlatformerStrengthTextbox->Location = System::Drawing::Point(66, 293);
			this->SpriteTabPlatformerStrengthTextbox->Name = L"SpriteTabPlatformerStrengthTextbox";
			this->SpriteTabPlatformerStrengthTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerStrengthTextbox->TabIndex = 27;
			this->SpriteTabPlatformerStrengthTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerStrengthTextbox_TextChanged);
			// 
			// label76
			// 
			this->label76->AutoSize = true;
			this->label76->Location = System::Drawing::Point(12, 296);
			this->label76->Name = L"label76";
			this->label76->Size = System::Drawing::Size(47, 13);
			this->label76->TabIndex = 26;
			this->label76->Text = L"Strength";
			// 
			// SpriteTabPlatformerGoldTextbox
			// 
			this->SpriteTabPlatformerGoldTextbox->Location = System::Drawing::Point(54, 267);
			this->SpriteTabPlatformerGoldTextbox->Name = L"SpriteTabPlatformerGoldTextbox";
			this->SpriteTabPlatformerGoldTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerGoldTextbox->TabIndex = 25;
			this->SpriteTabPlatformerGoldTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerGoldTextbox_TextChanged);
			// 
			// label77
			// 
			this->label77->AutoSize = true;
			this->label77->Location = System::Drawing::Point(14, 270);
			this->label77->Name = L"label77";
			this->label77->Size = System::Drawing::Size(29, 13);
			this->label77->TabIndex = 24;
			this->label77->Text = L"Gold";
			// 
			// SpriteTabPlatformerManaTextbox
			// 
			this->SpriteTabPlatformerManaTextbox->Location = System::Drawing::Point(54, 241);
			this->SpriteTabPlatformerManaTextbox->Name = L"SpriteTabPlatformerManaTextbox";
			this->SpriteTabPlatformerManaTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerManaTextbox->TabIndex = 23;
			this->SpriteTabPlatformerManaTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerManaTextbox_TextChanged);
			// 
			// label78
			// 
			this->label78->AutoSize = true;
			this->label78->Location = System::Drawing::Point(14, 244);
			this->label78->Name = L"label78";
			this->label78->Size = System::Drawing::Size(34, 13);
			this->label78->TabIndex = 22;
			this->label78->Text = L"Mana";
			// 
			// SpriteTabPlatformerHPTextbox
			// 
			this->SpriteTabPlatformerHPTextbox->Location = System::Drawing::Point(40, 215);
			this->SpriteTabPlatformerHPTextbox->Name = L"SpriteTabPlatformerHPTextbox";
			this->SpriteTabPlatformerHPTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerHPTextbox->TabIndex = 21;
			this->SpriteTabPlatformerHPTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerHPTextbox_TextChanged);
			// 
			// label79
			// 
			this->label79->AutoSize = true;
			this->label79->Location = System::Drawing::Point(12, 218);
			this->label79->Name = L"label79";
			this->label79->Size = System::Drawing::Size(22, 13);
			this->label79->TabIndex = 20;
			this->label79->Text = L"HP";
			// 
			// SpriteTabPlatformerHPMaxTextbox
			// 
			this->SpriteTabPlatformerHPMaxTextbox->Location = System::Drawing::Point(64, 189);
			this->SpriteTabPlatformerHPMaxTextbox->Name = L"SpriteTabPlatformerHPMaxTextbox";
			this->SpriteTabPlatformerHPMaxTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabPlatformerHPMaxTextbox->TabIndex = 18;
			this->SpriteTabPlatformerHPMaxTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerHPMaxTextbox_TextChanged);
			// 
			// label80
			// 
			this->label80->AutoSize = true;
			this->label80->Location = System::Drawing::Point(12, 192);
			this->label80->Name = L"label80";
			this->label80->Size = System::Drawing::Size(45, 13);
			this->label80->TabIndex = 17;
			this->label80->Text = L"HP Max";
			// 
			// SpriteTabPlatformerCollideMsgTextbox
			// 
			this->SpriteTabPlatformerCollideMsgTextbox->Location = System::Drawing::Point(79, 163);
			this->SpriteTabPlatformerCollideMsgTextbox->Name = L"SpriteTabPlatformerCollideMsgTextbox";
			this->SpriteTabPlatformerCollideMsgTextbox->Size = System::Drawing::Size(123, 20);
			this->SpriteTabPlatformerCollideMsgTextbox->TabIndex = 16;
			this->SpriteTabPlatformerCollideMsgTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerCollideMsgTextbox_TextChanged);
			// 
			// label81
			// 
			this->label81->AutoSize = true;
			this->label81->Location = System::Drawing::Point(12, 166);
			this->label81->Name = L"label81";
			this->label81->Size = System::Drawing::Size(61, 13);
			this->label81->TabIndex = 15;
			this->label81->Text = L"Collide Msg";
			// 
			// SpriteTabPlatformerAnimationSpeedTextbox
			// 
			this->SpriteTabPlatformerAnimationSpeedTextbox->Location = System::Drawing::Point(111, 137);
			this->SpriteTabPlatformerAnimationSpeedTextbox->Name = L"SpriteTabPlatformerAnimationSpeedTextbox";
			this->SpriteTabPlatformerAnimationSpeedTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabPlatformerAnimationSpeedTextbox->TabIndex = 14;
			this->SpriteTabPlatformerAnimationSpeedTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerAnimationSpeedTextbox_TextChanged);
			// 
			// label82
			// 
			this->label82->AutoSize = true;
			this->label82->Location = System::Drawing::Point(12, 140);
			this->label82->Name = L"label82";
			this->label82->Size = System::Drawing::Size(87, 13);
			this->label82->TabIndex = 13;
			this->label82->Text = L"Animation Speed";
			// 
			// SpriteTabPlatformerDefaultSequenceTextbox
			// 
			this->SpriteTabPlatformerDefaultSequenceTextbox->Location = System::Drawing::Point(111, 111);
			this->SpriteTabPlatformerDefaultSequenceTextbox->Name = L"SpriteTabPlatformerDefaultSequenceTextbox";
			this->SpriteTabPlatformerDefaultSequenceTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabPlatformerDefaultSequenceTextbox->TabIndex = 12;
			this->SpriteTabPlatformerDefaultSequenceTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerDefaultSequenceTextbox_TextChanged);
			// 
			// label83
			// 
			this->label83->AutoSize = true;
			this->label83->Location = System::Drawing::Point(12, 114);
			this->label83->Name = L"label83";
			this->label83->Size = System::Drawing::Size(93, 13);
			this->label83->TabIndex = 11;
			this->label83->Text = L"Default Sequence";
			// 
			// SpriteTabPlatformerRevertAnimationButton
			// 
			this->SpriteTabPlatformerRevertAnimationButton->Location = System::Drawing::Point(308, 213);
			this->SpriteTabPlatformerRevertAnimationButton->Name = L"SpriteTabPlatformerRevertAnimationButton";
			this->SpriteTabPlatformerRevertAnimationButton->Size = System::Drawing::Size(131, 23);
			this->SpriteTabPlatformerRevertAnimationButton->TabIndex = 10;
			this->SpriteTabPlatformerRevertAnimationButton->Text = L"Revert Animation";
			this->SpriteTabPlatformerRevertAnimationButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerRevertAnimationButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerRevertAnimationButton_Click);
			// 
			// SpriteTabPlatformerUpdateAnimationButton
			// 
			this->SpriteTabPlatformerUpdateAnimationButton->Location = System::Drawing::Point(308, 184);
			this->SpriteTabPlatformerUpdateAnimationButton->Name = L"SpriteTabPlatformerUpdateAnimationButton";
			this->SpriteTabPlatformerUpdateAnimationButton->Size = System::Drawing::Size(131, 23);
			this->SpriteTabPlatformerUpdateAnimationButton->TabIndex = 9;
			this->SpriteTabPlatformerUpdateAnimationButton->Text = L"Update Animation";
			this->SpriteTabPlatformerUpdateAnimationButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerUpdateAnimationButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerUpdateAnimationButton_Click);
			// 
			// SpriteTabPlatformerAnimationNameTextbox
			// 
			this->SpriteTabPlatformerAnimationNameTextbox->Location = System::Drawing::Point(102, 84);
			this->SpriteTabPlatformerAnimationNameTextbox->Name = L"SpriteTabPlatformerAnimationNameTextbox";
			this->SpriteTabPlatformerAnimationNameTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabPlatformerAnimationNameTextbox->TabIndex = 8;
			this->SpriteTabPlatformerAnimationNameTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerAnimationNameTextbox_TextChanged);
			// 
			// label84
			// 
			this->label84->AutoSize = true;
			this->label84->Location = System::Drawing::Point(12, 87);
			this->label84->Name = L"label84";
			this->label84->Size = System::Drawing::Size(84, 13);
			this->label84->TabIndex = 7;
			this->label84->Text = L"Animation Name";
			// 
			// SpriteTabPlatformerBrowseTextureNameButton
			// 
			this->SpriteTabPlatformerBrowseTextureNameButton->Location = System::Drawing::Point(308, 155);
			this->SpriteTabPlatformerBrowseTextureNameButton->Name = L"SpriteTabPlatformerBrowseTextureNameButton";
			this->SpriteTabPlatformerBrowseTextureNameButton->Size = System::Drawing::Size(131, 23);
			this->SpriteTabPlatformerBrowseTextureNameButton->TabIndex = 6;
			this->SpriteTabPlatformerBrowseTextureNameButton->Text = L"Browse Texture Name";
			this->SpriteTabPlatformerBrowseTextureNameButton->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerBrowseTextureNameButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerBrowseTextureNameButton_Click);
			// 
			// SpriteTabPlatformerTextureNameTextbox
			// 
			this->SpriteTabPlatformerTextureNameTextbox->Location = System::Drawing::Point(91, 57);
			this->SpriteTabPlatformerTextureNameTextbox->Name = L"SpriteTabPlatformerTextureNameTextbox";
			this->SpriteTabPlatformerTextureNameTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabPlatformerTextureNameTextbox->TabIndex = 5;
			// 
			// label85
			// 
			this->label85->AutoSize = true;
			this->label85->Location = System::Drawing::Point(11, 60);
			this->label85->Name = L"label85";
			this->label85->Size = System::Drawing::Size(74, 13);
			this->label85->TabIndex = 4;
			this->label85->Text = L"Texture Name";
			// 
			// SpriteTabTopdownSpriteGroupBox
			// 
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabPlatformerSpriteGroupBox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownScriptBrowseButton);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownTouchCheckbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownNoHitCheckbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownHardCheckbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->groupBox18);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->groupBox19);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownYLabel);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownXLabel);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownScriptOpenButton);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label35);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownScriptTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label36);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label63);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownLevelTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label59);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownExperienceTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->groupBox8);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label58);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownDefenseTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label57);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownSpeedTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label52);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownStrengthTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label51);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownGoldTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label50);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownManaTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label49);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownHPTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label48);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownHPMaxTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label47);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownCollideMsgTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label45);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownAnimationSpeedTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label44);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownDefaultSequenceTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label43);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownRevertAnimationButton);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownUpdateAnimationButton);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownAnimationNameTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label42);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownBrowseTextureNameButton);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownTextureNameTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label40);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownDepthTextbox);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label37);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->label28);
			this->SpriteTabTopdownSpriteGroupBox->Controls->Add(this->SpriteTabTopdownVisionComboBox);
			this->SpriteTabTopdownSpriteGroupBox->Location = System::Drawing::Point(15, 168);
			this->SpriteTabTopdownSpriteGroupBox->Name = L"SpriteTabTopdownSpriteGroupBox";
			this->SpriteTabTopdownSpriteGroupBox->Size = System::Drawing::Size(447, 534);
			this->SpriteTabTopdownSpriteGroupBox->TabIndex = 21;
			this->SpriteTabTopdownSpriteGroupBox->TabStop = false;
			// 
			// SpriteTabTopdownScriptBrowseButton
			// 
			this->SpriteTabTopdownScriptBrowseButton->Location = System::Drawing::Point(243, 467);
			this->SpriteTabTopdownScriptBrowseButton->Name = L"SpriteTabTopdownScriptBrowseButton";
			this->SpriteTabTopdownScriptBrowseButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabTopdownScriptBrowseButton->TabIndex = 57;
			this->SpriteTabTopdownScriptBrowseButton->Text = L"Browse";
			this->SpriteTabTopdownScriptBrowseButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownScriptBrowseButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownScriptBrowseButton_Click);
			// 
			// SpriteTabTopdownTouchCheckbox
			// 
			this->SpriteTabTopdownTouchCheckbox->AutoSize = true;
			this->SpriteTabTopdownTouchCheckbox->Location = System::Drawing::Point(349, 462);
			this->SpriteTabTopdownTouchCheckbox->Name = L"SpriteTabTopdownTouchCheckbox";
			this->SpriteTabTopdownTouchCheckbox->Size = System::Drawing::Size(57, 17);
			this->SpriteTabTopdownTouchCheckbox->TabIndex = 55;
			this->SpriteTabTopdownTouchCheckbox->Text = L"Touch";
			this->SpriteTabTopdownTouchCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownTouchCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownTouchCheckbox_CheckedChanged);
			// 
			// SpriteTabTopdownNoHitCheckbox
			// 
			this->SpriteTabTopdownNoHitCheckbox->AutoSize = true;
			this->SpriteTabTopdownNoHitCheckbox->Location = System::Drawing::Point(349, 438);
			this->SpriteTabTopdownNoHitCheckbox->Name = L"SpriteTabTopdownNoHitCheckbox";
			this->SpriteTabTopdownNoHitCheckbox->Size = System::Drawing::Size(56, 17);
			this->SpriteTabTopdownNoHitCheckbox->TabIndex = 54;
			this->SpriteTabTopdownNoHitCheckbox->Text = L"No Hit";
			this->SpriteTabTopdownNoHitCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownNoHitCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownNoHitCheckbox_CheckedChanged);
			// 
			// SpriteTabTopdownHardCheckbox
			// 
			this->SpriteTabTopdownHardCheckbox->AutoSize = true;
			this->SpriteTabTopdownHardCheckbox->Location = System::Drawing::Point(349, 415);
			this->SpriteTabTopdownHardCheckbox->Name = L"SpriteTabTopdownHardCheckbox";
			this->SpriteTabTopdownHardCheckbox->Size = System::Drawing::Size(49, 17);
			this->SpriteTabTopdownHardCheckbox->TabIndex = 53;
			this->SpriteTabTopdownHardCheckbox->Text = L"Hard";
			this->SpriteTabTopdownHardCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownHardCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownHardCheckbox_CheckedChanged);
			// 
			// groupBox18
			// 
			this->groupBox18->Controls->Add(this->SpriteTabTopdownInvisibleRadioButton);
			this->groupBox18->Controls->Add(this->SpriteTabTopdownEntityRadioButton);
			this->groupBox18->Controls->Add(this->SpriteTabTopdownBackgroundRadioButton);
			this->groupBox18->Location = System::Drawing::Point(343, 242);
			this->groupBox18->Name = L"groupBox18";
			this->groupBox18->Size = System::Drawing::Size(96, 89);
			this->groupBox18->TabIndex = 39;
			this->groupBox18->TabStop = false;
			this->groupBox18->Text = L"Type";
			// 
			// SpriteTabTopdownInvisibleRadioButton
			// 
			this->SpriteTabTopdownInvisibleRadioButton->AutoSize = true;
			this->SpriteTabTopdownInvisibleRadioButton->Location = System::Drawing::Point(6, 65);
			this->SpriteTabTopdownInvisibleRadioButton->Name = L"SpriteTabTopdownInvisibleRadioButton";
			this->SpriteTabTopdownInvisibleRadioButton->Size = System::Drawing::Size(63, 17);
			this->SpriteTabTopdownInvisibleRadioButton->TabIndex = 2;
			this->SpriteTabTopdownInvisibleRadioButton->Text = L"Invisible";
			this->SpriteTabTopdownInvisibleRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownInvisibleRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownInvisibleRadioButton_CheckedChanged);
			// 
			// SpriteTabTopdownEntityRadioButton
			// 
			this->SpriteTabTopdownEntityRadioButton->AutoSize = true;
			this->SpriteTabTopdownEntityRadioButton->Checked = true;
			this->SpriteTabTopdownEntityRadioButton->Location = System::Drawing::Point(6, 42);
			this->SpriteTabTopdownEntityRadioButton->Name = L"SpriteTabTopdownEntityRadioButton";
			this->SpriteTabTopdownEntityRadioButton->Size = System::Drawing::Size(51, 17);
			this->SpriteTabTopdownEntityRadioButton->TabIndex = 1;
			this->SpriteTabTopdownEntityRadioButton->TabStop = true;
			this->SpriteTabTopdownEntityRadioButton->Text = L"Entity";
			this->SpriteTabTopdownEntityRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownEntityRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownEntityRadioButton_CheckedChanged);
			// 
			// SpriteTabTopdownBackgroundRadioButton
			// 
			this->SpriteTabTopdownBackgroundRadioButton->AutoSize = true;
			this->SpriteTabTopdownBackgroundRadioButton->Location = System::Drawing::Point(6, 19);
			this->SpriteTabTopdownBackgroundRadioButton->Name = L"SpriteTabTopdownBackgroundRadioButton";
			this->SpriteTabTopdownBackgroundRadioButton->Size = System::Drawing::Size(83, 17);
			this->SpriteTabTopdownBackgroundRadioButton->TabIndex = 0;
			this->SpriteTabTopdownBackgroundRadioButton->Text = L"Background";
			this->SpriteTabTopdownBackgroundRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownBackgroundRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownBackgroundRadioButton_CheckedChanged);
			// 
			// groupBox19
			// 
			this->groupBox19->Controls->Add(this->SpriteTabTopdownTeleportSpriteNoRadioButton);
			this->groupBox19->Controls->Add(this->SpriteTabTopdownTeleportSpriteYesRadioButton);
			this->groupBox19->Controls->Add(this->SpriteTabTopdownTeleportSpriteXTextbox);
			this->groupBox19->Controls->Add(this->SpriteTabTopdownTeleportSpriteYTextbox);
			this->groupBox19->Controls->Add(this->label64);
			this->groupBox19->Controls->Add(this->label65);
			this->groupBox19->Controls->Add(this->SpriteTabTopdownTeleportSpriteMapNameTextbox);
			this->groupBox19->Controls->Add(this->label60);
			this->groupBox19->Location = System::Drawing::Point(247, 12);
			this->groupBox19->Name = L"groupBox19";
			this->groupBox19->Size = System::Drawing::Size(194, 130);
			this->groupBox19->TabIndex = 48;
			this->groupBox19->TabStop = false;
			this->groupBox19->Text = L"Teleport Sprite";
			// 
			// SpriteTabTopdownTeleportSpriteNoRadioButton
			// 
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->AutoSize = true;
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->Checked = true;
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->Location = System::Drawing::Point(56, 19);
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->Name = L"SpriteTabTopdownTeleportSpriteNoRadioButton";
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->Size = System::Drawing::Size(39, 17);
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->TabIndex = 1;
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->TabStop = true;
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->Text = L"No";
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownTeleportSpriteNoRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownTeleportSpriteNoRadioButton_CheckedChanged);
			// 
			// SpriteTabTopdownTeleportSpriteYesRadioButton
			// 
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->AutoSize = true;
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->Location = System::Drawing::Point(7, 19);
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->Name = L"SpriteTabTopdownTeleportSpriteYesRadioButton";
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->Size = System::Drawing::Size(43, 17);
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->TabIndex = 0;
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->Text = L"Yes";
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownTeleportSpriteYesRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownTeleportSpriteYesRadioButton_CheckedChanged);
			// 
			// SpriteTabTopdownTeleportSpriteXTextbox
			// 
			this->SpriteTabTopdownTeleportSpriteXTextbox->Location = System::Drawing::Point(69, 71);
			this->SpriteTabTopdownTeleportSpriteXTextbox->Name = L"SpriteTabTopdownTeleportSpriteXTextbox";
			this->SpriteTabTopdownTeleportSpriteXTextbox->Size = System::Drawing::Size(112, 20);
			this->SpriteTabTopdownTeleportSpriteXTextbox->TabIndex = 45;
			this->SpriteTabTopdownTeleportSpriteXTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownTeleportSpriteXTextbox_TextChanged);
			// 
			// SpriteTabTopdownTeleportSpriteYTextbox
			// 
			this->SpriteTabTopdownTeleportSpriteYTextbox->Location = System::Drawing::Point(69, 97);
			this->SpriteTabTopdownTeleportSpriteYTextbox->Name = L"SpriteTabTopdownTeleportSpriteYTextbox";
			this->SpriteTabTopdownTeleportSpriteYTextbox->Size = System::Drawing::Size(112, 20);
			this->SpriteTabTopdownTeleportSpriteYTextbox->TabIndex = 47;
			this->SpriteTabTopdownTeleportSpriteYTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownTeleportSpriteYTextbox_TextChanged);
			// 
			// label64
			// 
			this->label64->AutoSize = true;
			this->label64->Location = System::Drawing::Point(45, 74);
			this->label64->Name = L"label64";
			this->label64->Size = System::Drawing::Size(14, 13);
			this->label64->TabIndex = 44;
			this->label64->Text = L"X";
			// 
			// label65
			// 
			this->label65->AutoSize = true;
			this->label65->Location = System::Drawing::Point(45, 100);
			this->label65->Name = L"label65";
			this->label65->Size = System::Drawing::Size(14, 13);
			this->label65->TabIndex = 46;
			this->label65->Text = L"Y";
			// 
			// SpriteTabTopdownTeleportSpriteMapNameTextbox
			// 
			this->SpriteTabTopdownTeleportSpriteMapNameTextbox->Location = System::Drawing::Point(69, 45);
			this->SpriteTabTopdownTeleportSpriteMapNameTextbox->Name = L"SpriteTabTopdownTeleportSpriteMapNameTextbox";
			this->SpriteTabTopdownTeleportSpriteMapNameTextbox->Size = System::Drawing::Size(112, 20);
			this->SpriteTabTopdownTeleportSpriteMapNameTextbox->TabIndex = 40;
			this->SpriteTabTopdownTeleportSpriteMapNameTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownTeleportSpriteMapNameTextbox_TextChanged);
			// 
			// label60
			// 
			this->label60->AutoSize = true;
			this->label60->Location = System::Drawing::Point(4, 48);
			this->label60->Name = L"label60";
			this->label60->Size = System::Drawing::Size(59, 13);
			this->label60->TabIndex = 39;
			this->label60->Text = L"Map Name";
			// 
			// SpriteTabTopdownYLabel
			// 
			this->SpriteTabTopdownYLabel->AutoSize = true;
			this->SpriteTabTopdownYLabel->Location = System::Drawing::Point(35, 31);
			this->SpriteTabTopdownYLabel->Name = L"SpriteTabTopdownYLabel";
			this->SpriteTabTopdownYLabel->Size = System::Drawing::Size(41, 13);
			this->SpriteTabTopdownYLabel->TabIndex = 19;
			this->SpriteTabTopdownYLabel->Text = L"label35";
			// 
			// SpriteTabTopdownXLabel
			// 
			this->SpriteTabTopdownXLabel->AutoSize = true;
			this->SpriteTabTopdownXLabel->Location = System::Drawing::Point(35, 18);
			this->SpriteTabTopdownXLabel->Name = L"SpriteTabTopdownXLabel";
			this->SpriteTabTopdownXLabel->Size = System::Drawing::Size(41, 13);
			this->SpriteTabTopdownXLabel->TabIndex = 18;
			this->SpriteTabTopdownXLabel->Text = L"label34";
			// 
			// SpriteTabTopdownScriptOpenButton
			// 
			this->SpriteTabTopdownScriptOpenButton->Location = System::Drawing::Point(162, 467);
			this->SpriteTabTopdownScriptOpenButton->Name = L"SpriteTabTopdownScriptOpenButton";
			this->SpriteTabTopdownScriptOpenButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabTopdownScriptOpenButton->TabIndex = 43;
			this->SpriteTabTopdownScriptOpenButton->Text = L"Open";
			this->SpriteTabTopdownScriptOpenButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownScriptOpenButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownScriptOpenButton_Click);
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(15, 31);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(14, 13);
			this->label35->TabIndex = 16;
			this->label35->Text = L"Y";
			// 
			// SpriteTabTopdownScriptTextbox
			// 
			this->SpriteTabTopdownScriptTextbox->Location = System::Drawing::Point(56, 469);
			this->SpriteTabTopdownScriptTextbox->Name = L"SpriteTabTopdownScriptTextbox";
			this->SpriteTabTopdownScriptTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabTopdownScriptTextbox->TabIndex = 42;
			this->SpriteTabTopdownScriptTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownScriptTextbox_TextChanged);
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(15, 18);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(14, 13);
			this->label36->TabIndex = 15;
			this->label36->Text = L"X";
			// 
			// label63
			// 
			this->label63->AutoSize = true;
			this->label63->Location = System::Drawing::Point(14, 472);
			this->label63->Name = L"label63";
			this->label63->Size = System::Drawing::Size(34, 13);
			this->label63->TabIndex = 41;
			this->label63->Text = L"Script";
			// 
			// SpriteTabTopdownLevelTextbox
			// 
			this->SpriteTabTopdownLevelTextbox->Location = System::Drawing::Point(57, 445);
			this->SpriteTabTopdownLevelTextbox->Name = L"SpriteTabTopdownLevelTextbox";
			this->SpriteTabTopdownLevelTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownLevelTextbox->TabIndex = 35;
			this->SpriteTabTopdownLevelTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownLevelTextbox_TextChanged);
			// 
			// label59
			// 
			this->label59->AutoSize = true;
			this->label59->Location = System::Drawing::Point(17, 448);
			this->label59->Name = L"label59";
			this->label59->Size = System::Drawing::Size(33, 13);
			this->label59->TabIndex = 34;
			this->label59->Text = L"Level";
			// 
			// SpriteTabTopdownExperienceTextbox
			// 
			this->SpriteTabTopdownExperienceTextbox->Location = System::Drawing::Point(83, 419);
			this->SpriteTabTopdownExperienceTextbox->Name = L"SpriteTabTopdownExperienceTextbox";
			this->SpriteTabTopdownExperienceTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownExperienceTextbox->TabIndex = 33;
			this->SpriteTabTopdownExperienceTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownExperienceTextbox_TextChanged);
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->SpriteTabTopdownDynamicRadioButton);
			this->groupBox8->Controls->Add(this->SpriteTabTopdownStaticRadioButton);
			this->groupBox8->Location = System::Drawing::Point(348, 337);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(93, 72);
			this->groupBox8->TabIndex = 19;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"Physics Type";
			// 
			// SpriteTabTopdownDynamicRadioButton
			// 
			this->SpriteTabTopdownDynamicRadioButton->AutoSize = true;
			this->SpriteTabTopdownDynamicRadioButton->Location = System::Drawing::Point(6, 42);
			this->SpriteTabTopdownDynamicRadioButton->Name = L"SpriteTabTopdownDynamicRadioButton";
			this->SpriteTabTopdownDynamicRadioButton->Size = System::Drawing::Size(66, 17);
			this->SpriteTabTopdownDynamicRadioButton->TabIndex = 1;
			this->SpriteTabTopdownDynamicRadioButton->Text = L"Dynamic";
			this->SpriteTabTopdownDynamicRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownDynamicRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownDynamicRadioButton_CheckedChanged);
			// 
			// SpriteTabTopdownStaticRadioButton
			// 
			this->SpriteTabTopdownStaticRadioButton->AutoSize = true;
			this->SpriteTabTopdownStaticRadioButton->Checked = true;
			this->SpriteTabTopdownStaticRadioButton->Location = System::Drawing::Point(6, 19);
			this->SpriteTabTopdownStaticRadioButton->Name = L"SpriteTabTopdownStaticRadioButton";
			this->SpriteTabTopdownStaticRadioButton->Size = System::Drawing::Size(52, 17);
			this->SpriteTabTopdownStaticRadioButton->TabIndex = 0;
			this->SpriteTabTopdownStaticRadioButton->TabStop = true;
			this->SpriteTabTopdownStaticRadioButton->Text = L"Static";
			this->SpriteTabTopdownStaticRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownStaticRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownStaticRadioButton_CheckedChanged);
			// 
			// label58
			// 
			this->label58->AutoSize = true;
			this->label58->Location = System::Drawing::Point(17, 422);
			this->label58->Name = L"label58";
			this->label58->Size = System::Drawing::Size(60, 13);
			this->label58->TabIndex = 32;
			this->label58->Text = L"Experience";
			// 
			// SpriteTabTopdownDefenseTextbox
			// 
			this->SpriteTabTopdownDefenseTextbox->Location = System::Drawing::Point(67, 393);
			this->SpriteTabTopdownDefenseTextbox->Name = L"SpriteTabTopdownDefenseTextbox";
			this->SpriteTabTopdownDefenseTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownDefenseTextbox->TabIndex = 31;
			this->SpriteTabTopdownDefenseTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownDefenseTextbox_TextChanged);
			// 
			// label57
			// 
			this->label57->AutoSize = true;
			this->label57->Location = System::Drawing::Point(17, 396);
			this->label57->Name = L"label57";
			this->label57->Size = System::Drawing::Size(47, 13);
			this->label57->TabIndex = 30;
			this->label57->Text = L"Defense";
			// 
			// SpriteTabTopdownSpeedTextbox
			// 
			this->SpriteTabTopdownSpeedTextbox->Location = System::Drawing::Point(57, 367);
			this->SpriteTabTopdownSpeedTextbox->Name = L"SpriteTabTopdownSpeedTextbox";
			this->SpriteTabTopdownSpeedTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownSpeedTextbox->TabIndex = 29;
			this->SpriteTabTopdownSpeedTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownSpeedTextbox_TextChanged);
			// 
			// label52
			// 
			this->label52->AutoSize = true;
			this->label52->Location = System::Drawing::Point(17, 370);
			this->label52->Name = L"label52";
			this->label52->Size = System::Drawing::Size(38, 13);
			this->label52->TabIndex = 28;
			this->label52->Text = L"Speed";
			// 
			// SpriteTabTopdownStrengthTextbox
			// 
			this->SpriteTabTopdownStrengthTextbox->Location = System::Drawing::Point(69, 341);
			this->SpriteTabTopdownStrengthTextbox->Name = L"SpriteTabTopdownStrengthTextbox";
			this->SpriteTabTopdownStrengthTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownStrengthTextbox->TabIndex = 27;
			this->SpriteTabTopdownStrengthTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownStrengthTextbox_TextChanged);
			// 
			// label51
			// 
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(15, 344);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(47, 13);
			this->label51->TabIndex = 26;
			this->label51->Text = L"Strength";
			// 
			// SpriteTabTopdownGoldTextbox
			// 
			this->SpriteTabTopdownGoldTextbox->Location = System::Drawing::Point(57, 315);
			this->SpriteTabTopdownGoldTextbox->Name = L"SpriteTabTopdownGoldTextbox";
			this->SpriteTabTopdownGoldTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownGoldTextbox->TabIndex = 25;
			this->SpriteTabTopdownGoldTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownGoldTextbox_TextChanged);
			// 
			// label50
			// 
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(17, 318);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(29, 13);
			this->label50->TabIndex = 24;
			this->label50->Text = L"Gold";
			// 
			// SpriteTabTopdownManaTextbox
			// 
			this->SpriteTabTopdownManaTextbox->Location = System::Drawing::Point(57, 289);
			this->SpriteTabTopdownManaTextbox->Name = L"SpriteTabTopdownManaTextbox";
			this->SpriteTabTopdownManaTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownManaTextbox->TabIndex = 23;
			this->SpriteTabTopdownManaTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownManaTextbox_TextChanged);
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(17, 292);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(34, 13);
			this->label49->TabIndex = 22;
			this->label49->Text = L"Mana";
			// 
			// SpriteTabTopdownHPTextbox
			// 
			this->SpriteTabTopdownHPTextbox->Location = System::Drawing::Point(43, 263);
			this->SpriteTabTopdownHPTextbox->Name = L"SpriteTabTopdownHPTextbox";
			this->SpriteTabTopdownHPTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownHPTextbox->TabIndex = 21;
			this->SpriteTabTopdownHPTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownHPTextbox_TextChanged);
			// 
			// label48
			// 
			this->label48->AutoSize = true;
			this->label48->Location = System::Drawing::Point(15, 266);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(22, 13);
			this->label48->TabIndex = 20;
			this->label48->Text = L"HP";
			// 
			// SpriteTabTopdownHPMaxTextbox
			// 
			this->SpriteTabTopdownHPMaxTextbox->Location = System::Drawing::Point(67, 237);
			this->SpriteTabTopdownHPMaxTextbox->Name = L"SpriteTabTopdownHPMaxTextbox";
			this->SpriteTabTopdownHPMaxTextbox->Size = System::Drawing::Size(81, 20);
			this->SpriteTabTopdownHPMaxTextbox->TabIndex = 18;
			this->SpriteTabTopdownHPMaxTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownHPMaxTextbox_TextChanged);
			// 
			// label47
			// 
			this->label47->AutoSize = true;
			this->label47->Location = System::Drawing::Point(15, 240);
			this->label47->Name = L"label47";
			this->label47->Size = System::Drawing::Size(45, 13);
			this->label47->TabIndex = 17;
			this->label47->Text = L"HP Max";
			// 
			// SpriteTabTopdownCollideMsgTextbox
			// 
			this->SpriteTabTopdownCollideMsgTextbox->Location = System::Drawing::Point(82, 211);
			this->SpriteTabTopdownCollideMsgTextbox->Name = L"SpriteTabTopdownCollideMsgTextbox";
			this->SpriteTabTopdownCollideMsgTextbox->Size = System::Drawing::Size(123, 20);
			this->SpriteTabTopdownCollideMsgTextbox->TabIndex = 16;
			this->SpriteTabTopdownCollideMsgTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownCollideMsgTextbox_TextChanged);
			// 
			// label45
			// 
			this->label45->AutoSize = true;
			this->label45->Location = System::Drawing::Point(15, 214);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(61, 13);
			this->label45->TabIndex = 15;
			this->label45->Text = L"Collide Msg";
			// 
			// SpriteTabTopdownAnimationSpeedTextbox
			// 
			this->SpriteTabTopdownAnimationSpeedTextbox->Location = System::Drawing::Point(114, 185);
			this->SpriteTabTopdownAnimationSpeedTextbox->Name = L"SpriteTabTopdownAnimationSpeedTextbox";
			this->SpriteTabTopdownAnimationSpeedTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabTopdownAnimationSpeedTextbox->TabIndex = 14;
			this->SpriteTabTopdownAnimationSpeedTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownAnimationSpeedTextbox_TextChanged);
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(15, 188);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(87, 13);
			this->label44->TabIndex = 13;
			this->label44->Text = L"Animation Speed";
			// 
			// SpriteTabTopdownDefaultSequenceTextbox
			// 
			this->SpriteTabTopdownDefaultSequenceTextbox->Location = System::Drawing::Point(114, 159);
			this->SpriteTabTopdownDefaultSequenceTextbox->Name = L"SpriteTabTopdownDefaultSequenceTextbox";
			this->SpriteTabTopdownDefaultSequenceTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabTopdownDefaultSequenceTextbox->TabIndex = 12;
			this->SpriteTabTopdownDefaultSequenceTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownDefaultSequenceTextbox_TextChanged);
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Location = System::Drawing::Point(15, 162);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(93, 13);
			this->label43->TabIndex = 11;
			this->label43->Text = L"Default Sequence";
			// 
			// SpriteTabTopdownRevertAnimationButton
			// 
			this->SpriteTabTopdownRevertAnimationButton->Location = System::Drawing::Point(308, 213);
			this->SpriteTabTopdownRevertAnimationButton->Name = L"SpriteTabTopdownRevertAnimationButton";
			this->SpriteTabTopdownRevertAnimationButton->Size = System::Drawing::Size(131, 23);
			this->SpriteTabTopdownRevertAnimationButton->TabIndex = 10;
			this->SpriteTabTopdownRevertAnimationButton->Text = L"Revert Animation";
			this->SpriteTabTopdownRevertAnimationButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownRevertAnimationButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownRevertAnimationButton_Click);
			// 
			// SpriteTabTopdownUpdateAnimationButton
			// 
			this->SpriteTabTopdownUpdateAnimationButton->Location = System::Drawing::Point(308, 184);
			this->SpriteTabTopdownUpdateAnimationButton->Name = L"SpriteTabTopdownUpdateAnimationButton";
			this->SpriteTabTopdownUpdateAnimationButton->Size = System::Drawing::Size(131, 23);
			this->SpriteTabTopdownUpdateAnimationButton->TabIndex = 9;
			this->SpriteTabTopdownUpdateAnimationButton->Text = L"Update Animation";
			this->SpriteTabTopdownUpdateAnimationButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownUpdateAnimationButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownUpdateAnimationButton_Click);
			// 
			// SpriteTabTopdownAnimationNameTextbox
			// 
			this->SpriteTabTopdownAnimationNameTextbox->Location = System::Drawing::Point(105, 132);
			this->SpriteTabTopdownAnimationNameTextbox->Name = L"SpriteTabTopdownAnimationNameTextbox";
			this->SpriteTabTopdownAnimationNameTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabTopdownAnimationNameTextbox->TabIndex = 8;
			this->SpriteTabTopdownAnimationNameTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownAnimationNameTextbox_TextChanged);
			// 
			// label42
			// 
			this->label42->AutoSize = true;
			this->label42->Location = System::Drawing::Point(15, 135);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(84, 13);
			this->label42->TabIndex = 7;
			this->label42->Text = L"Animation Name";
			// 
			// SpriteTabTopdownBrowseTextureNameButton
			// 
			this->SpriteTabTopdownBrowseTextureNameButton->Location = System::Drawing::Point(308, 155);
			this->SpriteTabTopdownBrowseTextureNameButton->Name = L"SpriteTabTopdownBrowseTextureNameButton";
			this->SpriteTabTopdownBrowseTextureNameButton->Size = System::Drawing::Size(131, 23);
			this->SpriteTabTopdownBrowseTextureNameButton->TabIndex = 6;
			this->SpriteTabTopdownBrowseTextureNameButton->Text = L"Browse Texture Name";
			this->SpriteTabTopdownBrowseTextureNameButton->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownBrowseTextureNameButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownBrowseTextureNameButton_Click);
			// 
			// SpriteTabTopdownTextureNameTextbox
			// 
			this->SpriteTabTopdownTextureNameTextbox->Location = System::Drawing::Point(94, 105);
			this->SpriteTabTopdownTextureNameTextbox->Name = L"SpriteTabTopdownTextureNameTextbox";
			this->SpriteTabTopdownTextureNameTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabTopdownTextureNameTextbox->TabIndex = 5;
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Location = System::Drawing::Point(14, 108);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(74, 13);
			this->label40->TabIndex = 4;
			this->label40->Text = L"Texture Name";
			// 
			// SpriteTabTopdownDepthTextbox
			// 
			this->SpriteTabTopdownDepthTextbox->Location = System::Drawing::Point(73, 79);
			this->SpriteTabTopdownDepthTextbox->Name = L"SpriteTabTopdownDepthTextbox";
			this->SpriteTabTopdownDepthTextbox->Size = System::Drawing::Size(100, 20);
			this->SpriteTabTopdownDepthTextbox->TabIndex = 3;
			this->SpriteTabTopdownDepthTextbox->TextChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownDepthTextbox_TextChanged);
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(18, 82);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(36, 13);
			this->label37->TabIndex = 2;
			this->label37->Text = L"Depth";
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(18, 55);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(35, 13);
			this->label28->TabIndex = 1;
			this->label28->Text = L"Vision";
			// 
			// SpriteTabTopdownVisionComboBox
			// 
			this->SpriteTabTopdownVisionComboBox->FormattingEnabled = true;
			this->SpriteTabTopdownVisionComboBox->Location = System::Drawing::Point(73, 52);
			this->SpriteTabTopdownVisionComboBox->Name = L"SpriteTabTopdownVisionComboBox";
			this->SpriteTabTopdownVisionComboBox->Size = System::Drawing::Size(121, 21);
			this->SpriteTabTopdownVisionComboBox->TabIndex = 0;
			this->SpriteTabTopdownVisionComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownVisionComboBox_SelectedIndexChanged);
			// 
			// SpriteTabResizeGroupBox
			// 
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabInstantCheckbox);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabResizeLeftButton);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabResizeRightButton);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabResizeDownButton);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabResizeUpButton);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabGoFastCheckbox);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabNoUpdateRadioButton);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabResizeRadioButton);
			this->SpriteTabResizeGroupBox->Controls->Add(this->SpriteTabRepositionRadioButton);
			this->SpriteTabResizeGroupBox->Location = System::Drawing::Point(210, 13);
			this->SpriteTabResizeGroupBox->Name = L"SpriteTabResizeGroupBox";
			this->SpriteTabResizeGroupBox->Size = System::Drawing::Size(252, 149);
			this->SpriteTabResizeGroupBox->TabIndex = 20;
			this->SpriteTabResizeGroupBox->TabStop = false;
			this->SpriteTabResizeGroupBox->Text = L"Resize Sprite";
			// 
			// SpriteTabInstantCheckbox
			// 
			this->SpriteTabInstantCheckbox->AutoSize = true;
			this->SpriteTabInstantCheckbox->Location = System::Drawing::Point(6, 121);
			this->SpriteTabInstantCheckbox->Name = L"SpriteTabInstantCheckbox";
			this->SpriteTabInstantCheckbox->Size = System::Drawing::Size(58, 17);
			this->SpriteTabInstantCheckbox->TabIndex = 8;
			this->SpriteTabInstantCheckbox->Text = L"Instant";
			this->SpriteTabInstantCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabInstantCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabInstantCheckbox_CheckedChanged);
			// 
			// SpriteTabResizeLeftButton
			// 
			this->SpriteTabResizeLeftButton->Location = System::Drawing::Point(88, 51);
			this->SpriteTabResizeLeftButton->Name = L"SpriteTabResizeLeftButton";
			this->SpriteTabResizeLeftButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabResizeLeftButton->TabIndex = 7;
			this->SpriteTabResizeLeftButton->Text = L"Left";
			this->SpriteTabResizeLeftButton->UseVisualStyleBackColor = true;
			this->SpriteTabResizeLeftButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabResizeLeftButton_Click);
			// 
			// SpriteTabResizeRightButton
			// 
			this->SpriteTabResizeRightButton->Location = System::Drawing::Point(169, 51);
			this->SpriteTabResizeRightButton->Name = L"SpriteTabResizeRightButton";
			this->SpriteTabResizeRightButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabResizeRightButton->TabIndex = 6;
			this->SpriteTabResizeRightButton->Text = L"Right";
			this->SpriteTabResizeRightButton->UseVisualStyleBackColor = true;
			this->SpriteTabResizeRightButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabResizeRightButton_Click);
			// 
			// SpriteTabResizeDownButton
			// 
			this->SpriteTabResizeDownButton->Location = System::Drawing::Point(126, 80);
			this->SpriteTabResizeDownButton->Name = L"SpriteTabResizeDownButton";
			this->SpriteTabResizeDownButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabResizeDownButton->TabIndex = 5;
			this->SpriteTabResizeDownButton->Text = L"Down";
			this->SpriteTabResizeDownButton->UseVisualStyleBackColor = true;
			this->SpriteTabResizeDownButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabResizeDownButton_Click);
			// 
			// SpriteTabResizeUpButton
			// 
			this->SpriteTabResizeUpButton->Location = System::Drawing::Point(126, 22);
			this->SpriteTabResizeUpButton->Name = L"SpriteTabResizeUpButton";
			this->SpriteTabResizeUpButton->Size = System::Drawing::Size(75, 23);
			this->SpriteTabResizeUpButton->TabIndex = 4;
			this->SpriteTabResizeUpButton->Text = L"Up";
			this->SpriteTabResizeUpButton->UseVisualStyleBackColor = true;
			this->SpriteTabResizeUpButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabResizeUpButton_Click);
			// 
			// SpriteTabGoFastCheckbox
			// 
			this->SpriteTabGoFastCheckbox->AutoSize = true;
			this->SpriteTabGoFastCheckbox->Location = System::Drawing::Point(6, 99);
			this->SpriteTabGoFastCheckbox->Name = L"SpriteTabGoFastCheckbox";
			this->SpriteTabGoFastCheckbox->Size = System::Drawing::Size(63, 17);
			this->SpriteTabGoFastCheckbox->TabIndex = 3;
			this->SpriteTabGoFastCheckbox->Text = L"Go Fast";
			this->SpriteTabGoFastCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabGoFastCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabGoFastCheckbox_CheckedChanged);
			// 
			// SpriteTabNoUpdateRadioButton
			// 
			this->SpriteTabNoUpdateRadioButton->AutoSize = true;
			this->SpriteTabNoUpdateRadioButton->Checked = true;
			this->SpriteTabNoUpdateRadioButton->Location = System::Drawing::Point(6, 65);
			this->SpriteTabNoUpdateRadioButton->Name = L"SpriteTabNoUpdateRadioButton";
			this->SpriteTabNoUpdateRadioButton->Size = System::Drawing::Size(77, 17);
			this->SpriteTabNoUpdateRadioButton->TabIndex = 2;
			this->SpriteTabNoUpdateRadioButton->TabStop = true;
			this->SpriteTabNoUpdateRadioButton->Text = L"No Update";
			this->SpriteTabNoUpdateRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabNoUpdateRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabNoUpdateRadioButton_CheckedChanged);
			// 
			// SpriteTabResizeRadioButton
			// 
			this->SpriteTabResizeRadioButton->AutoSize = true;
			this->SpriteTabResizeRadioButton->Location = System::Drawing::Point(6, 42);
			this->SpriteTabResizeRadioButton->Name = L"SpriteTabResizeRadioButton";
			this->SpriteTabResizeRadioButton->Size = System::Drawing::Size(57, 17);
			this->SpriteTabResizeRadioButton->TabIndex = 1;
			this->SpriteTabResizeRadioButton->Text = L"Resize";
			this->SpriteTabResizeRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabResizeRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabResizeRadioButton_CheckedChanged);
			// 
			// SpriteTabRepositionRadioButton
			// 
			this->SpriteTabRepositionRadioButton->AutoSize = true;
			this->SpriteTabRepositionRadioButton->Location = System::Drawing::Point(6, 19);
			this->SpriteTabRepositionRadioButton->Name = L"SpriteTabRepositionRadioButton";
			this->SpriteTabRepositionRadioButton->Size = System::Drawing::Size(75, 17);
			this->SpriteTabRepositionRadioButton->TabIndex = 0;
			this->SpriteTabRepositionRadioButton->Text = L"Reposition";
			this->SpriteTabRepositionRadioButton->UseVisualStyleBackColor = true;
			this->SpriteTabRepositionRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabRepositionRadioButton_CheckedChanged);
			// 
			// TileCacheTab
			// 
			this->TileCacheTab->Controls->Add(this->TileCacheTabUpdateCacheButton);
			this->TileCacheTab->Controls->Add(this->TileCacheTabSelectedTileNameLabel);
			this->TileCacheTab->Controls->Add(this->label38);
			this->TileCacheTab->Location = System::Drawing::Point(4, 22);
			this->TileCacheTab->Name = L"TileCacheTab";
			this->TileCacheTab->Padding = System::Windows::Forms::Padding(3);
			this->TileCacheTab->Size = System::Drawing::Size(480, 742);
			this->TileCacheTab->TabIndex = 5;
			this->TileCacheTab->Text = L"Tile Cache";
			this->TileCacheTab->UseVisualStyleBackColor = true;
			// 
			// TileCacheTabUpdateCacheButton
			// 
			this->TileCacheTabUpdateCacheButton->Location = System::Drawing::Point(19, 643);
			this->TileCacheTabUpdateCacheButton->Name = L"TileCacheTabUpdateCacheButton";
			this->TileCacheTabUpdateCacheButton->Size = System::Drawing::Size(91, 23);
			this->TileCacheTabUpdateCacheButton->TabIndex = 4;
			this->TileCacheTabUpdateCacheButton->Text = L"Update Cache";
			this->TileCacheTabUpdateCacheButton->UseVisualStyleBackColor = true;
			this->TileCacheTabUpdateCacheButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::TileCacheTabUpdateCacheButton_Click);
			// 
			// TileCacheTabSelectedTileNameLabel
			// 
			this->TileCacheTabSelectedTileNameLabel->AutoSize = true;
			this->TileCacheTabSelectedTileNameLabel->Location = System::Drawing::Point(94, 614);
			this->TileCacheTabSelectedTileNameLabel->Name = L"TileCacheTabSelectedTileNameLabel";
			this->TileCacheTabSelectedTileNameLabel->Size = System::Drawing::Size(41, 13);
			this->TileCacheTabSelectedTileNameLabel->TabIndex = 2;
			this->TileCacheTabSelectedTileNameLabel->Text = L"label39";
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(16, 614);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(72, 13);
			this->label38->TabIndex = 1;
			this->label38->Text = L"Selected Tile:";
			// 
			// SpriteCacheTab
			// 
			this->SpriteCacheTab->Controls->Add(this->SpriteCacheTabUpdateCacheButton);
			this->SpriteCacheTab->Controls->Add(this->SpriteCacheTabSelectedSpriteLabel);
			this->SpriteCacheTab->Controls->Add(this->label41);
			this->SpriteCacheTab->Location = System::Drawing::Point(4, 22);
			this->SpriteCacheTab->Name = L"SpriteCacheTab";
			this->SpriteCacheTab->Padding = System::Windows::Forms::Padding(3);
			this->SpriteCacheTab->Size = System::Drawing::Size(480, 742);
			this->SpriteCacheTab->TabIndex = 6;
			this->SpriteCacheTab->Text = L"Sprite Cache";
			this->SpriteCacheTab->UseVisualStyleBackColor = true;
			// 
			// SpriteCacheTabUpdateCacheButton
			// 
			this->SpriteCacheTabUpdateCacheButton->Location = System::Drawing::Point(19, 643);
			this->SpriteCacheTabUpdateCacheButton->Name = L"SpriteCacheTabUpdateCacheButton";
			this->SpriteCacheTabUpdateCacheButton->Size = System::Drawing::Size(91, 23);
			this->SpriteCacheTabUpdateCacheButton->TabIndex = 9;
			this->SpriteCacheTabUpdateCacheButton->Text = L"Update Cache";
			this->SpriteCacheTabUpdateCacheButton->UseVisualStyleBackColor = true;
			this->SpriteCacheTabUpdateCacheButton->Click += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteCacheTabUpdateCacheButton_Click);
			// 
			// SpriteCacheTabSelectedSpriteLabel
			// 
			this->SpriteCacheTabSelectedSpriteLabel->AutoSize = true;
			this->SpriteCacheTabSelectedSpriteLabel->Location = System::Drawing::Point(94, 614);
			this->SpriteCacheTabSelectedSpriteLabel->Name = L"SpriteCacheTabSelectedSpriteLabel";
			this->SpriteCacheTabSelectedSpriteLabel->Size = System::Drawing::Size(41, 13);
			this->SpriteCacheTabSelectedSpriteLabel->TabIndex = 7;
			this->SpriteCacheTabSelectedSpriteLabel->Text = L"label40";
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(16, 614);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(82, 13);
			this->label41->TabIndex = 6;
			this->label41->Text = L"Selected Sprite:";
			// 
			// StartTimer
			// 
			this->StartTimer->Enabled = true;
			this->StartTimer->Tick += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::StartTimer_Tick);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// SpriteTabPlatformerAlwaysSeenByPlayerCheckbox
			// 
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->AutoSize = true;
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->Location = System::Drawing::Point(301, 483);
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->Name = L"SpriteTabPlatformerAlwaysSeenByPlayerCheckbox";
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->Size = System::Drawing::Size(134, 17);
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->TabIndex = 54;
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->Text = L"Always Seen By Player";
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabPlatformerAlwaysSeenByPlayerCheckbox_CheckedChanged);
			// 
			// SpriteTabTopdownAlwaysSeenByPlayerCheckbox
			// 
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->AutoSize = true;
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->Location = System::Drawing::Point(305, 505);
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->Name = L"SpriteTabTopdownAlwaysSeenByPlayerCheckbox";
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->Size = System::Drawing::Size(134, 17);
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->TabIndex = 58;
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->Text = L"Always Seen By Player";
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->UseVisualStyleBackColor = true;
			this->SpriteTabTopdownAlwaysSeenByPlayerCheckbox->CheckedChanged += gcnew System::EventHandler(this, &ZShadeSandboxEditor2D::SpriteTabTopdownAlwaysSeenByPlayerCheckbox_CheckedChanged);
			// 
			// ZShadeSandboxEditor2D
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1366, 768);
			this->ControlBox = false;
			this->Controls->Add(this->ZShadeSandboxEditorSplitContainer);
			this->Name = L"ZShadeSandboxEditor2D";
			this->Text = L"ZShadeSandbox";
			this->ZShadeSandboxEditorSplitContainer->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ZShadeSandboxEditorSplitContainer))->EndInit();
			this->ZShadeSandboxEditorSplitContainer->ResumeLayout(false);
			this->TabControl2D->ResumeLayout(false);
			this->EditorTab->ResumeLayout(false);
			this->EditorTab->PerformLayout();
			this->GridTypeGroupBox->ResumeLayout(false);
			this->GridTypeGroupBox->PerformLayout();
			this->TileModeMultiStampSizeGroupBox->ResumeLayout(false);
			this->TileModeMultiStampSizeGroupBox->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->GameTypeGroupBox->ResumeLayout(false);
			this->GameTypeGroupBox->PerformLayout();
			this->HardnessModeGroupBox->ResumeLayout(false);
			this->HardnessModeGroupBox->PerformLayout();
			this->SpriteModeGroupBox->ResumeLayout(false);
			this->SpriteModeGroupBox->PerformLayout();
			this->TileModePlatformerGroupBox->ResumeLayout(false);
			this->TileModePlatformerGroupBox->PerformLayout();
			this->TileModeTopdownGroupBox->ResumeLayout(false);
			this->TileModeTopdownGroupBox->PerformLayout();
			this->EditModeGroupBox->ResumeLayout(false);
			this->EditModeGroupBox->PerformLayout();
			this->WorldTab->ResumeLayout(false);
			this->WorldTab->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->NewPlatformerMapGroupbox->ResumeLayout(false);
			this->NewPlatformerMapGroupbox->PerformLayout();
			this->NewTopdownMapGroupbox->ResumeLayout(false);
			this->NewTopdownMapGroupbox->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->MapTab->ResumeLayout(false);
			this->MapTabPlatformerMapGroupBox->ResumeLayout(false);
			this->MapTabPlatformerMapGroupBox->PerformLayout();
			this->groupBox15->ResumeLayout(false);
			this->groupBox15->PerformLayout();
			this->MapTabTopdownMapGroupBox->ResumeLayout(false);
			this->MapTabTopdownMapGroupBox->PerformLayout();
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			this->TilesTab->ResumeLayout(false);
			this->TilesTab->PerformLayout();
			this->groupBox12->ResumeLayout(false);
			this->groupBox12->PerformLayout();
			this->SpritesTab->ResumeLayout(false);
			this->SpriteTabPlatformerSpriteGroupBox->ResumeLayout(false);
			this->SpriteTabPlatformerSpriteGroupBox->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox17->ResumeLayout(false);
			this->groupBox17->PerformLayout();
			this->groupBox23->ResumeLayout(false);
			this->groupBox23->PerformLayout();
			this->SpriteTabTopdownSpriteGroupBox->ResumeLayout(false);
			this->SpriteTabTopdownSpriteGroupBox->PerformLayout();
			this->groupBox18->ResumeLayout(false);
			this->groupBox18->PerformLayout();
			this->groupBox19->ResumeLayout(false);
			this->groupBox19->PerformLayout();
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			this->SpriteTabResizeGroupBox->ResumeLayout(false);
			this->SpriteTabResizeGroupBox->PerformLayout();
			this->TileCacheTab->ResumeLayout(false);
			this->TileCacheTab->PerformLayout();
			this->SpriteCacheTab->ResumeLayout(false);
			this->SpriteCacheTab->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void StartTimer_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		if (!bInitialized)
		{
			HWND hwnd = static_cast<HWND>(ZShadeSandboxEditorSplitContainer->Panel1->Handle.ToPointer());
			mToolHwnd = static_cast<HWND>(ZShadeSandboxEditorSplitContainer->Panel2->Handle.ToPointer());

			bInitialized = true;

			mDXGameWindow = new DXGameWindow();
			mDXGameWindow->Init(mEngineOptions, hwnd);

			ThreadStart^ threadStart = gcnew ThreadStart(this, &ZShadeSandbox::ZShadeSandboxEditor2D::Run);
			Thread^ thread = gcnew Thread(threadStart);
			thread->Start();
		}

		if (mEditorHelper2D)
		{
			GamePathLabel->Text = gcnew String(mEditorHelper2D->GetEngineOptions()->m_GD2D->GAME_FOLDER.c_str());
			this->Text = gcnew System::String(mEditorHelper2D->GetMapEditorSystem2D()->UpdateTitleBarText().c_str());
			mEditorHelper2D->GetMapEditorSystem2D()->ToolWindowWidth() = ZShadeSandboxEditorSplitContainer->Panel2->Width;

			//XMFLOAT2 offset = mEditorHelper2D->GetMapEditorSystem2D()->GetScrollOffset();
			//string scrollX = ZShadeSandboxGlobal::Convert::ConvertToString<float>(offset.x);
			//string scrollY = ZShadeSandboxGlobal::Convert::ConvertToString<float>(offset.y);
			//ScrollOffsetXLabel->Text = gcnew String(scrollX.c_str());
			//ScrollOffsetYLabel->Text = gcnew String(scrollY.c_str());
			
			switch (mEngineOptions->m_GameType2D)
			{
				case TOPDOWN:
				{
					UpdateTopdownMapStats();
				}
				break;
				case PLATFORMER:
				{
				   UpdatePlatformerMapStats();
				}
				break;
			}

			CheckSectionPickerMode();
			CheckSpriteModeGridType();
			
			// Load the attributes of a selected sprite/tile into the sprite/tile tab
			SetSpriteTabSelectedSpriteAttributes();
		}

		if (bInitLoadCache)
		{
			FillTileCache();
			FillSpriteCache();
			bInitLoadCache = false;
			//this->StartTimer->Enabled = false;
		}
		
		// Disable the tool window if the DXGameWindow is in focus
		/*if (mEditorHelper2D)
		{
			if (mEditorHelper2D->GameWindowInForeground())
			{
				TabControl2D->Enabled = false;
			}
			else
			{
				TabControl2D->Enabled = true;
			}
		}*/
	}
	
	//
	// Editor Tab Functionality
	//
	
	#pragma region "Editor Tab Functionality"
	private: System::Void OpenGameDirectoryButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		string username = CGlobal::GrabUserName();
		BetterString gameFolder = "C:\\Users\\";
		gameFolder += username;
		gameFolder += "\\AppData\\Roaming\\";
		folderBrowserDialog1->SelectedPath = gcnew String(gameFolder);

		if (folderBrowserDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			//GameFolderNameTextBox->Text = folderBrowserDialog1->SelectedPath;
			mEditorHelper2D->LoadGameDirectory(ZShadeSandbox::ConvertToStandardString(folderBrowserDialog1->SelectedPath), false);
			GamePathLabel->Text = gcnew String(mEditorHelper2D->GetEngineOptions()->m_GD2D->GAME_FOLDER.c_str());
			FillTileCache();
			FillSpriteCache();
		}
	}
	private: System::Void CreateGameDirectoryButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (GameFolderNameTextBox->Text == "")
		{
			ZShadeMessageCenter::MsgBoxOK(NULL, "Must have a game name !!!", "Create Game Directory");
		}
		else
		{
			if (GameTypeTopdownRadioButton->Checked)
			{
				mEngineOptions->m_GameType2D = TOPDOWN;
			}

			if (GameTypePlatformerRadioButton->Checked)
			{
				mEngineOptions->m_GameType2D = PLATFORMER;
			}

			if (GameTypeTopdownRadioButton->Checked || GameTypePlatformerRadioButton->Checked)
			{
				mEditorHelper2D->LoadGameDirectory(ZShadeSandbox::ConvertToStandardString(GameFolderNameTextBox->Text), true);
				GamePathLabel->Text = gcnew String(mEditorHelper2D->GetEngineOptions()->m_GD2D->GAME_FOLDER.c_str());
				GameFolderNameTextBox->Text = "";
				FillTileCache();
				FillSpriteCache();
			}
			else
			{
				ZShadeMessageCenter::MsgBoxOK(NULL, "Game Type not checked !!!", "Create Game Directory");
			}
		}
	}
	private: System::Void CloseGameDirectoryButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->CloseGameDirectory();
		
		// Need to make sure controls are greyed out that no longer need to be used
		// Need to add an "Are You Sure" Yes/No message box to the user
	}
	private: System::Void SaveGameDirectoryButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->SaveGameDirectory();
	}
	private: System::Void TileRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		switch (mEngineOptions->m_GameType2D)
		{
			case TOPDOWN:
			{
				SwitchToTileModeTopdown();
			}
			break;
			case PLATFORMER:
			{
				SwitchToTileModePlatformer();
			}
			break;
		}

		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToEditModeTiles();
	}
	private: System::Void SpriteRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		SwitchToSpriteMode();
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToEditModeSprites();
	}
	private: System::Void HardnessRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		SwitchToHardnessMode();
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToEditModeHardness();
	}
	private: System::Void TileModeTopdownRegularRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeRegular();
		TileModeMultiStampSizeGroupBox->Hide();
	}
	private: System::Void TileModeTopdownMultiStampRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeMultiStamp();
		TileModeMultiStampSizeGroupBox->Show();
	}
	private: System::Void TileModePlatformerRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeRegular();
		TileModeMultiStampSizeGroupBox->Hide();
	}
	private: System::Void TileModeTopdownSelectRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeSelect();
	}
	private: System::Void TileModePlatformerSelectRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeSelect();
	}
	private: System::Void MultiStampPlatformerRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeMultiStamp();
		TileModeMultiStampSizeGroupBox->Show();
	}
	private: System::Void RemoveTexturePlatformerRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToTileModeRemoveTexture();
		TileModeMultiStampSizeGroupBox->Hide();
	}
	private: System::Void TilesHardnessRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToHardnessModeTiles();
	}
	private: System::Void SpritesHardnessRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToHardnessModeSprites();
	}
	private: System::Void SpriteModeSelectRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToSpriteModeSelect();
	}
	private: System::Void SpriteModeMoveRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToSpriteModeMove();
	}
	private: System::Void SpriteModeDeleteRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToSpriteModeDelete();
	}
	private: System::Void SpriteModePlaceRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToSpriteModePlace();
	}
	private: System::Void SpriteModeSizeRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToSpriteModeSize();
	}
	private: System::Void SpriteModeCloneRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToSpriteModeClone();
	}
	private: System::Void MultiStamp1x1RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize1x1();
	}
	private: System::Void MultiStamp2x2RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize2x2();
	}
	private: System::Void MultiStamp3x3RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize3x3();
	}
	private: System::Void MultiStamp4x4RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize4x4();
	}
	private: System::Void MultiStamp5x5RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize5x5();
	}
	private: System::Void MultiStamp6x6RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize6x6();
	}
	private: System::Void MultiStamp7x7RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize7x7();
	}
	private: System::Void MultiStamp8x8RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize8x8();
	}
	private: System::Void MultiStamp9x9RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize9x9();
	}
	private: System::Void MultiStamp10x10RadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToStampSize10x10();
	}
	private: System::Void GridTypeFree_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToGridModeFree();
	}
	private: System::Void GridTypeSnap_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SwitchToGridModeSnap();
	}
	private: System::Void ReturnToSectionPickerButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ReturnToTopdownSectionPicker();
	}
	#pragma endregion
	
	//
	// World Tab New Map Functionality
	//
	
	#pragma region "World Tab New Map Functionality"
	private: System::Void WorldTabTopdownMapCreateMapButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			if (WorldTabTopdownMapMapNameTextbox->Text == "")
			{
				ZShadeMessageCenter::MsgBoxOK(NULL, "Must have a map name !!!", "Create Topdown map");
			}
			else
			{
				string mapName = ZShadeSandbox::ConvertToStandardString(WorldTabTopdownMapMapNameTextbox->Text);
				string worldName = ZShadeSandbox::ConvertToStandardString(WorldTabTopdownMapWorldNameComboBox->SelectedItem->ToString());
				string mapSizeStr = ZShadeSandbox::ConvertToStandardString(WorldTabTopdownMapSizeComboBox->SelectedItem->ToString());
				string fowRadiusStr = "5";

				BetterString str(mapSizeStr);
				vector<string> vstr = str.split('x');

				int mapSize = atoi(vstr[0].c_str());
				int fowRadius = atoi(fowRadiusStr.c_str());
				bool fowFlashlight = false;
				
				bool fow;
				if (WorldTabEnableFOWCheckBox->Checked)
				{
					fow = true;
					fowRadiusStr = ZShadeSandbox::ConvertToStandardString(WorldTabFOWRadiusComboBox->SelectedItem->ToString());
					fowRadius = atoi(fowRadiusStr.c_str());
					fowFlashlight = WorldTabFOWFlashlightCheckBox->Checked;
				}
				else
					fow = false;

				bool iso;
				if (WorldTabTopdownMapIsometricRadioButton->Checked)
					iso = true;
				if (WorldTabTopdownMapStandardRadioButton->Checked)
					iso = false;

				mEditorHelper2D->NewMapTopdown(worldName, mapName, mapSize, mapSize, fow, fowRadius, fowFlashlight, iso, true);

				WorldTabMapNameComboBox->Items->Add(gcnew String(mapName.c_str()));
				WorldTabMapNameComboBox->Text = gcnew String(mapName.c_str());

				m_ActiveMapName = WorldTabTopdownMapMapNameTextbox->Text;

				WorldTabTopdownMapMapNameTextbox->Text = "";

				bMapCreated = true;
			}
		}
	}
	private: System::Void WorldTabPlatformerCreateMapButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			if (WorldTabPlatformerMapNameTextBox->Text == "")
			{
				ZShadeMessageCenter::MsgBoxOK(NULL, "Must have a map name !!!", "Create Platformer map");
			}
			else
			{
				string mapName = ZShadeSandbox::ConvertToStandardString(WorldTabPlatformerMapNameTextBox->Text);
				string worldName = ZShadeSandbox::ConvertToStandardString(WorldTabPlatformerWorldNameComboBox->SelectedItem->ToString());
				string mapWidthStr = ZShadeSandbox::ConvertToStandardString(WorldTabPlatformerMapWidthComboBox->SelectedItem->ToString());
				string mapHeightStr = ZShadeSandbox::ConvertToStandardString(WorldTabPlatformerMapHeightComboBox->SelectedItem->ToString());
				int mapWidth = atoi(mapWidthStr.c_str());
				int mapHeight = atoi(mapHeightStr.c_str());

				mEditorHelper2D->NewMapPlatformer(worldName, mapName, mapWidth, mapHeight, true);

				WorldTabMapNameComboBox->Items->Add(gcnew String(mapName.c_str()));
				WorldTabMapNameComboBox->Text = gcnew String(mapName.c_str());

				m_ActiveMapName = WorldTabPlatformerMapNameTextBox->Text;

				WorldTabPlatformerMapNameTextBox->Text = "";

				bMapCreated = true;
			}
		}
	}
	#pragma endregion
	
	//
	// World Tab Functionality
	//
	
	#pragma region "World Tab Functionality"
	private: System::Void WorldTabCreateWorldButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (WorldTabWorldNameTextbox->Text == "")
		{
			ZShadeMessageCenter::MsgBoxOK(NULL, "Must have a world name !!!", "Create World");
		}
		else
		{
			m_ActiveWorldName = WorldTabWorldNameTextbox->Text;

			// Add the world name to the CreateMap world name combobox
			WorldTabTopdownMapWorldNameComboBox->Items->Add(m_ActiveWorldName);
			WorldTabPlatformerWorldNameComboBox->Items->Add(m_ActiveWorldName);
			WorldTabTopdownMapWorldNameComboBox->Text = m_ActiveWorldName;
			WorldTabPlatformerWorldNameComboBox->Text = m_ActiveWorldName;
			
			WorldTabStatusLabel->Text = m_ActiveWorldName;

			string world_name = ZShadeSandbox::ConvertToStandardString(WorldTabWorldNameTextbox->Text);

			WorldTabWorldNameTextbox->Text = "";

			mEditorHelper2D->CreateWorld(world_name);
			
			bWorldCreated = true;

			if (mEngineOptions->m_GameType2D == GameType2D::TOPDOWN)
				EnableTopdownCreateButtons();
			else if (mEngineOptions->m_GameType2D == GameType2D::PLATFORMER)
				EnablePlatformerCreateButtons();
		}
	}
	private: System::Void WorldTabLoadWorldButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		string username = CGlobal::GrabUserName();
		BetterString gameFolder = "C:\\Users\\";
		gameFolder += username;
		gameFolder += "\\AppData\\Roaming\\";
		gameFolder += mEngineOptions->m_GD2D->m_xml_worlds_path;
		gameFolder += "\\";
		openFileDialog1->Filter = "XML Files|*.xml";
		openFileDialog1->InitialDirectory = gcnew String(gameFolder);

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			string world_name_path = ZShadeSandbox::ConvertToStandardString(openFileDialog1->FileName);
			
			string worldName;
			mEditorHelper2D->LoadWorld(worldName, world_name_path, true);
			
			m_ActiveWorldName = gcnew String(worldName.c_str());
			
			if (m_ActiveWorldName != "")
			{
				// Add the world name to the CreateMap world name combobox
				WorldTabTopdownMapWorldNameComboBox->Items->Add(m_ActiveWorldName);
				WorldTabPlatformerWorldNameComboBox->Items->Add(m_ActiveWorldName);
				WorldTabTopdownMapWorldNameComboBox->Text = m_ActiveWorldName;
				WorldTabPlatformerWorldNameComboBox->Text = m_ActiveWorldName;

				WorldTabStatusLabel->Text = m_ActiveWorldName;

				vector<string> map_names;
				mEditorHelper2D->GetMapEditorSystem2D()->GrabMapNames(ZShadeSandbox::ConvertToStandardString(m_ActiveWorldName), map_names);

				if (map_names.size() > 0)
				{
					WorldTabMapNameComboBox->Items->Clear();

					string first_name = map_names[0];
					for (int i = 0; i < map_names.size(); i++)
					{
						string map_name = map_names[i];

						WorldTabMapNameComboBox->Items->Add(gcnew String(map_name.c_str()));
					}

					WorldTabMapNameComboBox->Text = gcnew String(first_name.c_str());

					bMapCreated = true;
				}
				
				bWorldCreated = true;

				if (mEngineOptions->m_GameType2D == GameType2D::TOPDOWN)
					EnableTopdownCreateButtons();
				else if (mEngineOptions->m_GameType2D == GameType2D::PLATFORMER)
					EnablePlatformerCreateButtons();
			}
		}
	}
	private: System::Void WorldTabSaveWorldButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_xml_worlds_path;
			gameFolder += "\\";
			saveFileDialog1->Filter = "XML Files|*.xml";
			saveFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string world_name_path = ZShadeSandbox::ConvertToStandardString(saveFileDialog1->FileName);
				
				mEditorHelper2D->SaveWorld(world_name_path);
			}
		}
	}
	private: System::Void WorldTabRenderMapButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Set the currently selected map to be the one rendered
		if (bWorldCreated && bMapCreated && m_ActiveWorldName != ""  && m_ActiveMapName != "")
		{
			std::string worldName = ZShadeSandbox::ConvertToStandardString(m_ActiveWorldName);
			std::string mapName = ZShadeSandbox::ConvertToStandardString(m_ActiveMapName);
			
			mEditorHelper2D->GetMapEditorSystem2D()->ActiveWorldName() = worldName;
			mEditorHelper2D->GetMapEditorSystem2D()->ActiveMapName() = mapName;
			
			mEditorHelper2D->SetActiveRenderedMap(worldName, mapName);
			
			// Add stats and updatable values for the map that is being rendered
			if (mEngineOptions->m_GameType2D == GameType2D::TOPDOWN)
			{
				SetTopdownMapProperties();
			}
			else if (mEngineOptions->m_GameType2D == GameType2D::PLATFORMER)
			{
				mEditorHelper2D->GetMapEditorSystem2D()->IsInSectionPicker() = false;
				
				SetPlatformerMapProperties();
			}
		}
	}
	private: System::Void WorldTabRemoveMapButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Remove the selected map from the combo box and the world
		
		if (bWorldCreated)
		{
			System::String^ selected_map = WorldTabMapNameComboBox->SelectedItem->ToString();
			
			mEditorHelper2D->RemoveMap(ZShadeSandbox::ConvertToStandardString(selected_map));
			
			// Clear the map name from the map names combo box
			WorldTabMapNameComboBox->Items->Remove(WorldTabMapNameComboBox->SelectedItem);
			
			// Was this the currently rendered map
			if (selected_map == m_ActiveMapName)
			{
				// No more map
				bMapCreated = false;
				
				m_ActiveMapName = "";
			}
		}
	}
	private: System::Void WorldTabLoadMapButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_xml_maps_path;
			gameFolder += "\\";
			openFileDialog1->Filter = "XML Files|*.xml";
			openFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string map_name_path = ZShadeSandbox::ConvertToStandardString(openFileDialog1->FileName);
				
				string mapName;
				mEditorHelper2D->LoadMap(mapName, map_name_path, true);
				
				m_ActiveMapName = gcnew String(mapName.c_str());
				
				bMapCreated = true;
			}
		}
	}
	private: System::Void WorldTabSaveMapButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_xml_maps_path;
			gameFolder += "\\";
			saveFileDialog1->Filter = "XML Files|*.xml";
			saveFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string map_name_path = ZShadeSandbox::ConvertToStandardString(saveFileDialog1->FileName);
				
				mEditorHelper2D->SaveMap(map_name_path);
			}
		}
	}
	private: System::Void WorldTabMapNameComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		m_ActiveMapName = WorldTabMapNameComboBox->SelectedItem->ToString();
	}
	private: System::Void WorldTabEnableFOWCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		EnableTopdownCreateFOWButtons();
	}
	private: System::Void WorldTabFOWUncoveredCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SetMapFOWUncoveredAttribute(WorldTabFOWUncoveredCheckBox->Checked);
	}
	#pragma endregion
	
	//
	// Map Tab Functionality
	//
	
	#pragma region "Map Tab Functionality"
	private: System::Void MapTabTopdownUpdateMapNameButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Update the name of the map and update contents in world tab that use the map name
		if (TopdownMapNameTextbox->Text != "" && TopdownMapNameTextbox->Text != m_ActiveMapName)
		{
			string mapName = ZShadeSandbox::ConvertToStandardString(TopdownMapNameTextbox->Text);

			// Need to update map names for the world
			mEditorHelper2D->GetMapEditorSystem2D()->UpdateNewMapName(
				ZShadeSandbox::ConvertToStandardString(m_ActiveWorldName),
				ZShadeSandbox::ConvertToStandardString(m_ActiveMapName),
				mapName
			);

			mEditorHelper2D->GetMapEditorSystem2D()->SetMapNameAttribute(mapName);

			m_ActiveMapName = gcnew String(mapName.c_str());

			vector<string> map_names;
			mEditorHelper2D->GetMapEditorSystem2D()->GrabMapNames(ZShadeSandbox::ConvertToStandardString(m_ActiveWorldName), map_names);

			if (map_names.size() > 0)
			{
				WorldTabMapNameComboBox->Items->Clear();

				string first_name = map_names[0];
				for (int i = 0; i < map_names.size(); i++)
				{
					string map_name = map_names[i];

					WorldTabMapNameComboBox->Items->Add(gcnew String(map_name.c_str()));
				}

				WorldTabMapNameComboBox->Text = gcnew String(first_name.c_str());
			}
		}
	}
	private: System::Void MapTabPlatformerUpdateMapNameButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Update the name of the map and update contents in world tab that use the map name
		if (PlatformerMapEditNameTextbox->Text != "" && PlatformerMapEditNameTextbox->Text != m_ActiveMapName)
		{
			string mapName = ZShadeSandbox::ConvertToStandardString(PlatformerMapEditNameTextbox->Text);

			// Need to update map names for the world
			mEditorHelper2D->GetMapEditorSystem2D()->UpdateNewMapName(
				ZShadeSandbox::ConvertToStandardString(m_ActiveWorldName),
				ZShadeSandbox::ConvertToStandardString(m_ActiveMapName),
				mapName
			);

			mEditorHelper2D->GetMapEditorSystem2D()->SetMapNameAttribute(mapName);

			m_ActiveMapName = gcnew String(mapName.c_str());

			vector<string> map_names;
			mEditorHelper2D->GetMapEditorSystem2D()->GrabMapNames(ZShadeSandbox::ConvertToStandardString(m_ActiveWorldName), map_names);

			if (map_names.size() > 0)
			{
				WorldTabMapNameComboBox->Items->Clear();

				string first_name = map_names[0];
				for (int i = 0; i < map_names.size(); i++)
				{
					string map_name = map_names[i];

					WorldTabMapNameComboBox->Items->Add(gcnew String(map_name.c_str()));
				}

				WorldTabMapNameComboBox->Text = gcnew String(first_name.c_str());
			}
		}
	}
	private: System::Void TopdownMapEnableFOWMapTabCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		MapTabUpdateTopdownCreateFOWButtons();

		// Update whether the map has FOW or not
		if (mEditorHelper2D && bWorldCreated && bMapCreated)
		{
			bool fow = TopdownMapEnableFOWMapTabCheckBox->Checked;
			mEditorHelper2D->GetMapEditorSystem2D()->SetMapFOWAttribute(fow);
		}
	}
	private: System::Void TopdownMapHardnessCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Toggle display of hardness in the map
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleMapHardness(TopdownMapHardnessCheckbox->Checked);
	}
	private: System::Void PlatformerMapHardnessCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Toggle display of hardness in the map
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleMapHardness(TopdownMapHardnessCheckbox->Checked);
	}
	private: System::Void TopdownMapVisionComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Update the vision in the topdown map
		if (mEditorHelper2D && bWorldCreated && bMapCreated)
		{
			int vision = ZShadeSandboxGlobal::Convert::ConvertStringToInteger(ZShadeSandbox::ConvertToStandardString(TopdownMapVisionComboBox->SelectedItem->ToString()));
			mEditorHelper2D->GetMapEditorSystem2D()->SetMapVisionAttribute(vision);
		}
	}
	private: System::Void TopdownMapMusicTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Update the main music name for the map
		string musicName = ZShadeSandbox::ConvertToStandardString(TopdownMapMusicTextbox->Text);
		mEditorHelper2D->GetMapEditorSystem2D()->SetMapMusicAttribute(musicName);
	}
	private: System::Void TopdownMapFOWRadiusMapTabCB_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// If FOW is enabled for topdown map then update the FOW radius value
		if (mEditorHelper2D && bWorldCreated && bMapCreated)
		{
			int fowRadius = ZShadeSandboxGlobal::Convert::ConvertStringToInteger(ZShadeSandbox::ConvertToStandardString(TopdownMapFOWRadiusMapTabCB->SelectedItem->ToString()));
			mEditorHelper2D->GetMapEditorSystem2D()->SetMapFOWRadiusAttribute(fowRadius);
		}
	}
	private: System::Void TopdownMapFOWFlashlightMapTabCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// If FOW is enabled it will use a flash light style and the map will not be uncovered but the sprite will only be able to see
		// the extent of the FOW radius from the sprite at all times.
		bool fowFlashlight = TopdownMapFOWFlashlightMapTabCheckBox->Checked;
		mEditorHelper2D->GetMapEditorSystem2D()->SetMapFOWFlashlightAttribute(fowFlashlight);
	}
	private: System::Void TopdownMapFOWUncoveredMapTabCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->SetMapFOWUncoveredAttribute(TopdownMapFOWUncoveredMapTabCheckBox->Checked);
	}
	private: System::Void PlatformerMapEditMusicTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Update the main music name for the map
		string musicName = ZShadeSandbox::ConvertToStandardString(PlatformerMapEditMusicTextbox->Text);
		mEditorHelper2D->GetMapEditorSystem2D()->SetMapMusicAttribute(musicName);
	}
	#pragma endregion

	//
	// Tile Tab Functionality
	//
	
	#pragma region "Tile Tab Functionality"
	
	#pragma endregion
	
	
	//
	// Sprite Tab Functionality
	//
	
	#pragma region "Sprite Tab Functionality"
	private: System::Void SpriteTabRepositionRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabResizeRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabNoUpdateRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabGoFastCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabInstantCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabResizeUpButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabResizeLeftButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabResizeRightButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}
	private: System::Void SpriteTabResizeDownButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}
	
	// Topdown Map Sprite Updating
	private: System::Void SpriteTabTopdownTeleportSpriteYesRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		CheckSpriteTabTeleportSpriteStatus();
	}
	private: System::Void SpriteTabTopdownTeleportSpriteNoRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		CheckSpriteTabTeleportSpriteStatus();
	}
	private: System::Void SpriteTabTopdownBrowseTextureNameButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_sprites_path;
			gameFolder += "\\";
			openFileDialog1->Filter = "PNG Files|*.png";
			openFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string textureNamePath = ZShadeSandbox::ConvertToStandardString(openFileDialog1->FileName);

				string::size_type pos = string(textureNamePath).find_last_of("\\/");

				BetterString str(textureNamePath);
				int index = str.get_index('.');
				string name = (str.substring(pos + 1, index)).toString();
				
				// Find the sprite that was clicked from select sprite mode
				mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTextureName(string(name).append(".png"));

				SpriteTabTopdownTextureNameTextbox->Text = gcnew String(string(name).append(".png").c_str());

				// Update the mesh texture for the sprite
				mEditorHelper2D->GetMapEditorSystem2D()->UpdateSelectedSpriteMeshTexture();
			}
		}
	}
	private: System::Void SpriteTabTopdownUpdateAnimationButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->UpdateSelectedSpriteAnimation();
	}
	private: System::Void SpriteTabTopdownRevertAnimationButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SpriteTabTopdownAnimationNameTextbox->Text = "NONE";
		mEditorHelper2D->GetMapEditorSystem2D()->RevertSelectedSpriteAnimation();
	}
	private: System::Void SpriteTabTopdownScriptOpenButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->OpenSelectedSpriteScript(ZShadeSandbox::ConvertToStandardString(SpriteTabTopdownScriptTextbox->Text));
	}
	private: System::Void SpriteTabTopdownBackgroundRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteBackground(SpriteTabTopdownBackgroundRadioButton->Checked);
	}
	private: System::Void SpriteTabTopdownEntityRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteEntity(SpriteTabTopdownEntityRadioButton->Checked);
	}
	private: System::Void SpriteTabTopdownInvisibleRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteInvisible(SpriteTabTopdownInvisibleRadioButton->Checked);
	}
	private: System::Void SpriteTabTopdownStaticRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpritePhysicsType(SpriteTabTopdownStaticRadioButton->Checked);
	}
	private: System::Void SpriteTabTopdownDynamicRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpritePhysicsType(SpriteTabTopdownDynamicRadioButton->Checked);
	}
	private: System::Void SpriteTabTopdownScriptBrowseButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_scripts_path;
			gameFolder += "\\";
			openFileDialog1->Filter = "LUA Files|*.lua";
			openFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string textureNamePath = ZShadeSandbox::ConvertToStandardString(openFileDialog1->FileName);

				string::size_type pos = string(textureNamePath).find_last_of("\\/");

				BetterString str(textureNamePath);
				int index = str.get_index('.');
				string name = (str.substring(pos + 1, index)).toString();

				// Find the sprite that was clicked from select sprite mode
				mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteScriptName(string(name).append(".lua"));

				SpriteTabTopdownScriptTextbox->Text = gcnew String(string(name).append(".lua").c_str());
			}
		}
	}
	private: System::Void SpriteTabTopdownHardCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteHard(SpriteTabTopdownHardCheckbox->Checked);
	}
	private: System::Void SpriteTabTopdownNoHitCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteNoHit(SpriteTabTopdownNoHitCheckbox->Checked);
	}
	private: System::Void SpriteTabTopdownTouchCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteTouch(SpriteTabTopdownTouchCheckbox->Checked);
	}
	private: System::Void SpriteTabTopdownVisionComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (mEditorHelper2D && bWorldCreated && bMapCreated)
		{
			System::String^ vision_str = SpriteTabTopdownVisionComboBox->SelectedItem->ToString();
			string vision_str_str = ZShadeSandbox::ConvertToStandardString(vision_str);
			mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteVision(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(vision_str_str));
		}
	}
	private: System::Void SpriteTabTopdownDepthTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownDepthTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteDepth(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownAnimationNameTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownAnimationNameTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteAnimationName(str_str);
	}
	private: System::Void SpriteTabTopdownDefaultSequenceTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownDefaultSequenceTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteDefaultSequence(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownAnimationSpeedTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownAnimationSpeedTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteAnimationSpeed(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownCollideMsgTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownCollideMsgTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteCollideMsg(str_str);
	}
	private: System::Void SpriteTabTopdownHPMaxTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownHPMaxTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteHPMax(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownHPTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownHPTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteHP(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownManaTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownManaTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteMana(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownGoldTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownGoldTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteGold(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownStrengthTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownStrengthTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteStrength(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownSpeedTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownSpeedTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		int speed = ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str);
		// In a topdown map x and y are the same speed
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteSpeedX(speed);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteSpeedY(speed);
	}
	/*
	private: System::Void SpriteTabTopdownMaxSpeedTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownMaxSpeedTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		int speed = ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str);
		// In a topdown map x and y have the same max speed
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteMaxSpeedX(speed);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteMaxSpeedY(speed);
	}
	*/
	private: System::Void SpriteTabTopdownDefenseTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownDefenseTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteDefense(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownExperienceTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownExperienceTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteExperience(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownLevelTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownLevelTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteLevel(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownScriptTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownScriptTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteScriptName(str_str);
	}
	private: System::Void SpriteTabTopdownTeleportSpriteMapNameTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownTeleportSpriteMapNameTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTeleportMapName(str_str);
	}
	private: System::Void SpriteTabTopdownTeleportSpriteXTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownTeleportSpriteXTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTeleportMapX(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownTeleportSpriteYTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabTopdownTeleportSpriteYTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTeleportMapY(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabTopdownAlwaysSeenByPlayerCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteAlwaysSeenByPlayer(SpriteTabTopdownAlwaysSeenByPlayerCheckbox->Checked);
	}
	
	// Platformer Map Sprite Updating
	private: System::Void SpriteTabPlatformerScriptOpenButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->OpenSelectedSpriteScript(ZShadeSandbox::ConvertToStandardString(SpriteTabPlatformerScriptTextbox->Text));
	}
	private: System::Void SpriteTabPlatformerTeleportSpriteYesRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		CheckSpriteTabTeleportSpriteStatus();
	}
	private: System::Void SpriteTabPlatformerTeleportSpriteNoRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		CheckSpriteTabTeleportSpriteStatus();
	}
	private: System::Void SpriteTabPlatformerBrowseTextureNameButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_sprites_path;
			gameFolder += "\\";
			openFileDialog1->Filter = "PNG Files|*.png";
			openFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string textureNamePath = ZShadeSandbox::ConvertToStandardString(openFileDialog1->FileName);

				string::size_type pos = string(textureNamePath).find_last_of("\\/");

				BetterString str(textureNamePath);
				int index = str.get_index('.');
				string name = (str.substring(pos + 1, index)).toString();

				// Find the sprite that was clicked from select sprite mode
				mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTextureName(string(name).append(".png"));

				SpriteTabPlatformerTextureNameTextbox->Text = gcnew String(string(name).append(".png").c_str());
			}
		}
	}
	private: System::Void SpriteTabPlatformerUpdateAnimationButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->UpdateSelectedSpriteAnimation();
	}
	private: System::Void SpriteTabPlatformerRevertAnimationButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SpriteTabPlatformerAnimationNameTextbox->Text = "NONE";
		mEditorHelper2D->GetMapEditorSystem2D()->RevertSelectedSpriteAnimation();
	}
	private: System::Void SpriteTabPlatformerBackgroundRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteBackground(SpriteTabPlatformerBackgroundRadioButton->Checked);
	}
	private: System::Void SpriteTabPlatformerEntityRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteEntity(SpriteTabPlatformerEntityRadioButton->Checked);
	}
	private: System::Void SpriteTabPlatformerInvisibleRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteInvisible(SpriteTabPlatformerInvisibleRadioButton->Checked);
	}
	private: System::Void SpriteTabPlatformerStaticRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpritePhysicsType(SpriteTabTopdownStaticRadioButton->Checked);
	}
	private: System::Void SpriteTabPlatformerDynamicRadioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpritePhysicsType(SpriteTabTopdownDynamicRadioButton->Checked);
	}
	private: System::Void SpriteTabPlatformerScriptBrowseButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (bWorldCreated)
		{
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += mEngineOptions->m_GD2D->m_scripts_path;
			gameFolder += "\\";
			openFileDialog1->Filter = "LUA Files|*.lua";
			openFileDialog1->InitialDirectory = gcnew String(gameFolder);

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				string textureNamePath = ZShadeSandbox::ConvertToStandardString(openFileDialog1->FileName);

				string::size_type pos = string(textureNamePath).find_last_of("\\/");

				BetterString str(textureNamePath);
				int index = str.get_index('.');
				string name = (str.substring(pos + 1, index)).toString();

				// Find the sprite that was clicked from select sprite mode
				mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteScriptName(string(name).append(".lua"));

				SpriteTabPlatformerScriptTextbox->Text = gcnew String(string(name).append(".lua").c_str());
			}
		}
	}
	private: System::Void SpriteTabPlatformerHardCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteHard(SpriteTabPlatformerHardCheckbox->Checked);
	}
	private: System::Void SpriteTabPlatformerNoHitCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteNoHit(SpriteTabPlatformerNoHitCheckbox->Checked);
	}
	private: System::Void SpriteTabPlatformerTouchCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteTouch(SpriteTabPlatformerTouchCheckbox->Checked);
	}
	private: System::Void SpriteTabPlatformerAnimationNameTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerAnimationNameTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteAnimationName(str_str);
	}
	private: System::Void SpriteTabPlatformerDefaultSequenceTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerDefaultSequenceTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteDefaultSequence(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerAnimationSpeedTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerAnimationSpeedTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteAnimationSpeed(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerCollideMsgTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerCollideMsgTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteCollideMsg(str_str);
	}
	private: System::Void SpriteTabPlatformerHPMaxTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerHPMaxTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteHPMax(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerHPTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerHPTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteHP(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerManaTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerManaTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteMana(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerGoldTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerGoldTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteGold(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerStrengthTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerStrengthTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteStrength(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerSpeedTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerSpeedTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		int speed = ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteSpeedX(speed);
	}
	/*
	private: System::Void SpriteTabPlatformerMaxSpeedTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerMaxSpeedTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		int speed = ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteMaxSpeedX(speed);
	}
	*/
	private: System::Void SpriteTabPlatformerDefenseTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerDefenseTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteDefense(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerExperienceTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerExperienceTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteExperience(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerLevelTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerLevelTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteLevel(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerScriptTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerScriptTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteScriptName(str_str);
	}
	private: System::Void SpriteTabPlatformerTeleportSpriteMapNameTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerTeleportSpriteMapNameTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTeleportMapName(str_str);
	}
	private: System::Void SpriteTabPlatformerTeleportSpriteXTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerTeleportSpriteXTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTeleportMapX(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerTeleportSpriteYTextbox_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::String^ str = SpriteTabPlatformerTeleportSpriteYTextbox->Text;
		string str_str = ZShadeSandbox::ConvertToStandardString(str);
		mEditorHelper2D->GetMapEditorSystem2D()->SetSelectedSpriteTeleportMapY(ZShadeSandboxGlobal::Convert::ConvertStringToInteger(str_str));
	}
	private: System::Void SpriteTabPlatformerAlwaysSeenByPlayerCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		mEditorHelper2D->GetMapEditorSystem2D()->ToggleSelectedSpriteAlwaysSeenByPlayer(SpriteTabPlatformerAlwaysSeenByPlayerCheckbox->Checked);
	}
	#pragma endregion
	
	//
	// Tile Cache Functionality
	//
	
	private: System::Void TileCacheTabUpdateCacheButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		FillTileCache();
	}
	
	//
	// Sprite Cache Functionality
	//
	
	private: System::Void SpriteCacheTabUpdateCacheButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		FillSpriteCache();
	}
};
}
