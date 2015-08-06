#include "InventoryEditorSystem2D.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "Environment2D.h"
//==================================================================================================================================
//==================================================================================================================================
InventoryEditorSystem2D::InventoryEditorSystem2D(Environment2D* env2D)
:	m_Env2D(env2D)
{
	m_mainGameDirectory = m_Env2D->GetGD2D();
	m_D3DSystem = m_Env2D->GetD3D();
	m_EngineOptions = m_Env2D->GetEngineOptions();
	
	m_Updated = false;
	
	iToolWindowWidth = 0;
	
	Init();
}
//==================================================================================================================================
void InventoryEditorSystem2D::Init()
{
	sBackgroundImageName = "";
	iItemSize = 0;
	iItemRowSizeRegular = 0;
	iItemColSizeRegular = 0;
	iItemRowSizeMagic = 0;
	iItemColSizeMagic = 0;
	iItemRowPaddingRegular = 0;
	iItemColPaddingRegular = 0;
	iItemRowPaddingMagic = 0;
	iItemColPaddingMagic = 0;
	vStartLocationRegular = XMFLOAT2(0, 0);
	vStartLocationMagic = XMFLOAT2(0, 0);
}
//==================================================================================================================================
string InventoryEditorSystem2D::UpdateTitleBarText()
{
	if (m_Updated)
	{
		//Place an [*] on the title bar of the window
		string path = "ZShadeSandbox [*]";
		
		if (m_mainGameDirectory->GAME_FOLDER != "")
		{
			path = "ZShadeSandbox (";
			path.append(m_mainGameDirectory->GAME_FOLDER);
			path.append(") [*]");
		}

		return path;
	}
	else
	{
		string path = "ZShadeSandbox";
		
		if (m_mainGameDirectory->GAME_FOLDER != "")
		{
			path = "ZShadeSandbox (";
			path.append(m_mainGameDirectory->GAME_FOLDER);
			path.append(")");
		}

		return path;
	}
}
//==================================================================================================================================
void InventoryEditorSystem2D::Create()
{
	if (m_Env2D == 0) return;
	if (m_Env2D->PlayerInventory() == 0) return;
	if (sBackgroundImageName == "") return;
	
	m_Env2D->PlayerInventory()->Init(
		m_mainGameDirectory->m_required_textures,// path of the stamp tool
		m_mainGameDirectory->m_sprites_path,// path of the background image
		sBackgroundImageName,
		m_EngineOptions->m_screenWidth,
		m_EngineOptions->m_screenHeight,
		iItemSize,
		iItemSize,
		iItemSize,
		iItemRowSizeRegular, iItemColSizeRegular,
		iItemRowSizeMagic, iItemColSizeMagic,
		iItemRowPaddingRegular, iItemColPaddingRegular,
		iItemRowPaddingMagic, iItemColPaddingMagic,
		vStartLocationRegular,
		vStartLocationMagic
	);
	
	m_Updated = true;
}
//==================================================================================================================================
void InventoryEditorSystem2D::FillWithDefaultItems()
{
	for (int i = 0; i < m_Env2D->PlayerInventory()->MaxAllowedItemsRegular(); i++)
	{
		string str = "RegularItem";
		str += ZShadeSandboxGlobal::Convert::ConvertToString<int>(i + 1);
		
		m_Env2D->PlayerInventory()->AddItemRegular(m_mainGameDirectory->m_required_textures, "DefaultRegularItem.png", str);
	}
	
	for (int i = 0; i < m_Env2D->PlayerInventory()->MaxAllowedItemsMagic(); i++)
	{
		string str = "MagicItem";
		str += ZShadeSandboxGlobal::Convert::ConvertToString<int>(i + 1);
		
		m_Env2D->PlayerInventory()->AddItemMagic(m_mainGameDirectory->m_required_textures, "DefaultMagicItem.png", str);
	}
	
	m_Env2D->PlayerInventory()->PositionItemsOnGrid();
	
	m_Updated = true;
}
//==================================================================================================================================
void InventoryEditorSystem2D::RemoveAllItems()
{
	for (int i = 0; i < m_Env2D->PlayerInventory()->MaxAllowedItemsRegular(); i++)
	{
		m_Env2D->PlayerInventory()->RemoveItemRegular(i);
	}
	
	for (int i = 0; i < m_Env2D->PlayerInventory()->MaxAllowedItemsMagic(); i++)
	{
		m_Env2D->PlayerInventory()->RemoveItemMagic(i);
	}
	
	m_Updated = true;
}
//==================================================================================================================================
void InventoryEditorSystem2D::PositionItemsOnGrid()
{
	m_Env2D->PlayerInventory()->PositionItemsOnGrid();
}
//==================================================================================================================================
void InventoryEditorSystem2D::ToggleInventoryActive()
{
	m_Env2D->InventoryActive() = !m_Env2D->InventoryActive();
}
//==================================================================================================================================
void InventoryEditorSystem2D::LoadInventory(string filename)
{
	ZShadeXMLLoader::LoadInventoryXML(filename, m_Env2D->GetGD2D(), m_EngineOptions, m_Env2D->PlayerInventory());
	
	sBackgroundImageName = m_Env2D->PlayerInventory()->BackgroundImageName();
	iItemSize = m_Env2D->PlayerInventory()->ItemSize();
	iItemRowSizeRegular = m_Env2D->PlayerInventory()->MaxItemsXRegular();
	iItemColSizeRegular = m_Env2D->PlayerInventory()->MaxItemsYRegular();
	iItemRowSizeMagic = m_Env2D->PlayerInventory()->MaxItemsXMagic();
	iItemColSizeMagic = m_Env2D->PlayerInventory()->MaxItemsYMagic();
	iItemRowPaddingRegular = m_Env2D->PlayerInventory()->XPaddingRegular();
	iItemColPaddingRegular = m_Env2D->PlayerInventory()->YPaddingRegular();
	iItemRowPaddingMagic = m_Env2D->PlayerInventory()->XPaddingMagic();
	iItemColPaddingMagic = m_Env2D->PlayerInventory()->YPaddingMagic();
	vStartLocationRegular = m_Env2D->PlayerInventory()->GridStartLocationRegular();
	vStartLocationMagic = m_Env2D->PlayerInventory()->GridStartLocationMagic();
}
//==================================================================================================================================
void InventoryEditorSystem2D::SaveInventory(string filename)
{
	ZShadeXMLSaver::SaveInventoryXML(filename, m_Env2D->PlayerInventory());
	
	m_Updated = false;
}
//==================================================================================================================================
