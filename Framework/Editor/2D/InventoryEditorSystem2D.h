//==================================================================================================================================
// InventoryEditorSystem2D.h
//
//==================================================================================================================================
// History
//
// -Created on 7/24/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __INVENTORYEDITORSYSTEM2D_H
#define __INVENTORYEDITORSYSTEM2D_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "ZMath.h"

class Environment2D;

//==================================================================================================================================
//==================================================================================================================================
/*
	Will be able to edit:
		background image
		item size
		amount of items per row/col
		start location of the first item
		
	In this mode press a button to get to it and it will
	render the inventory as it normally would in environment2D
	with the mouse buttons.
*/
class InventoryEditorSystem2D
{
public:
	
	InventoryEditorSystem2D(Environment2D* env2D);
	
	void Init();
	
	// Update the text in the title bar
	string UpdateTitleBarText();
	
	// Creates the inventory with the specified attributes in the environment2D class
	void Create();
	
	void FillWithDefaultItems();
	void RemoveAllItems();
	void PositionItemsOnGrid();
	void ToggleInventoryActive();
	
	void LoadInventory(string filename);
	void SaveInventory(string filename);
	
public:
	
	GameDirectory2D* GetGD() { return m_mainGameDirectory; }
	D3D* D3DSystem() { return m_D3DSystem; }
	int& ToolWindowWidth() { return iToolWindowWidth; }
	int ToolWindowWidth() const { return iToolWindowWidth; }
	string& BackgroundImageName() { return sBackgroundImageName; }
	string BackgroundImageName() const { return sBackgroundImageName; }
	int& ItemSize() { return iItemSize; }
	int ItemSize() const { return iItemSize; }
	int& ItemRowSizeRegular() { return iItemRowSizeRegular; }
	int ItemRowSizeRegular() const { return iItemRowSizeRegular; }
	int& ItemColSizeRegular() { return iItemColSizeRegular; }
	int ItemColSizeRegular() const { return iItemColSizeRegular; }
	int& ItemRowSizeMagic() { return iItemRowSizeMagic; }
	int ItemRowSizeMagic() const { return iItemRowSizeMagic; }
	int& ItemColSizeMagic() { return iItemColSizeMagic; }
	int ItemColSizeMagic() const { return iItemColSizeMagic; }
	int& ItemRowPaddingRegular() { return iItemRowPaddingRegular; }
	int ItemRowPaddingRegular() const { return iItemRowPaddingRegular; }
	int& ItemColPaddingRegular() { return iItemColPaddingRegular; }
	int ItemColPaddingRegular() const { return iItemColPaddingRegular; }
	int& ItemRowPaddingMagic() { return iItemRowPaddingMagic; }
	int ItemRowPaddingMagic() const { return iItemRowPaddingMagic; }
	int& ItemColPaddingMagic() { return iItemColPaddingMagic; }
	int ItemColPaddingMagic() const { return iItemColPaddingMagic; }
	XMFLOAT2& StartLocationRegular() { return vStartLocationRegular; }
	XMFLOAT2 StartLocationRegular() const { return vStartLocationRegular; }
	XMFLOAT2& StartLocationMagic() { return vStartLocationMagic; }
	XMFLOAT2 StartLocationMagic() const { return vStartLocationMagic; }
	
private:
	
	Environment2D* m_Env2D;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	GameDirectory2D* m_mainGameDirectory;
	
	int iToolWindowWidth;
	bool m_Updated;
	
	string sBackgroundImageName;
	int iItemSize;
	int iItemRowSizeRegular;
	int iItemColSizeRegular;
	int iItemRowSizeMagic;
	int iItemColSizeMagic;
	int iItemRowPaddingRegular;
	int iItemColPaddingRegular;
	int iItemRowPaddingMagic;
	int iItemColPaddingMagic;
	XMFLOAT2 vStartLocationRegular;
	XMFLOAT2 vStartLocationMagic;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__INVENTORYEDITORSYSTEM2D_H