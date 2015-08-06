//==================================================================================================================================
// PlayerInventory2D.h
//
//==================================================================================================================================
// History
//
// -Created on 7/24/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLAYERINVENTORY2D_H
#define __PLAYERINVENTORY2D_H
//==================================================================================================================================
//==================================================================================================================================

//
// Includes
//

#include <vector>
#include "D3D.h"
#include "ZMath.h"
#include "Sprite.h"

//==================================================================================================================================
//==================================================================================================================================
class PlayerInventory2D
{
public:
	
	PlayerInventory2D(D3D* d3d);
	
	string SelectedInventoryMessageRegular();
	string SelectedInventoryMessageMagic();
	
	bool RegularInventoryFull();
	bool MagicInventoryFull();
	
	void Render(Camera* camera);
	void RenderRegularItems(Camera* camera);
	void RenderMagicItems(Camera* camera);
	void Init(
		string base_path,
		string background_path,
		string background_texture,
		int backgroundWidth,
		int backgroundHeight,
		int cursorWidth,
		int cursorHeight,
		int gridSize,
		int maxItemsXRegular,
		int maxItemsYRegular,
		int maxItemsXMagic,
		int maxItemsYMagic,
		int xPaddingRegular,
		int yPaddingRegular,
		int xPaddingMagic,
		int yPaddingMagic,
		XMFLOAT2 gridStartLocationRegular,
		XMFLOAT2 gridStartLocationMagic
	);
	
	bool CursorInsideRegularGrid(int x, int y);
	bool CursorInsideMagicGrid(int x, int y);
	
	void AddItemRegular(string base_item_path, string item_texture, string inventoryMessage);
	void AddItemRegular(Sprite* item);
	void RemoveItemRegular(int itemID);
	void RemoveItemRegular(Sprite* item);
	
	void AddItemMagic(string base_item_path, string item_texture, string inventoryMessage);
	void AddItemMagic(Sprite* item);
	void RemoveItemMagic(int itemID);
	void RemoveItemMagic(Sprite* item);
	
	void PositionItemsOnGrid();
	void PositionItemsOnGridRegular();
	void PositionItemsOnGridMagic();
	
	void SnapRegular(int x, int y, float& rx, float& ry);
	void SnapMagic(int x, int y, float& rx, float& ry);
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
public:
	
	string ItemRegularImageName(int i) { return mItemsRegular[i]->BaseTextureFilename(); }
	string ItemRegularInventoryMessage(int i) { return mItemsRegular[i]->InventoryMessage(); }
	string ItemMagicImageName(int i) { return mItemsMagic[i]->BaseTextureFilename(); }
	string ItemMagicInventoryMessage(int i) { return mItemsMagic[i]->InventoryMessage(); }
	int MaxAllowedItemsRegular() { return iMaxItemsAllowedRegular; }
	int MaxAllowedItemsMagic() { return iMaxItemsAllowedMagic; }
	string& BackgroundImageName() { return sBackgroundImageName; }
	string BackgroundImageName() const { return sBackgroundImageName; }
	int& ItemSize() { return iGridSize; }
	int ItemSize() const { return iGridSize; }
	int& MaxItemsXRegular() { return iMaxItemsXRegular; }
	int MaxItemsXRegular() const { return iMaxItemsXRegular; }
	int& MaxItemsYRegular() { return iMaxItemsYRegular; }
	int MaxItemsYRegular() const { return iMaxItemsYRegular; }
	int& MaxItemsXMagic() { return iMaxItemsXMagic; }
	int MaxItemsXMagic() const { return iMaxItemsXMagic; }
	int& MaxItemsYMagic() { return iMaxItemsYMagic; }
	int MaxItemsYMagic() const { return iMaxItemsYMagic; }
	int& XPaddingRegular() { return iXPaddingRegular; }
	int XPaddingRegular() const { return iXPaddingRegular; }
	int& YPaddingRegular() { return iYPaddingRegular; }
	int YPaddingRegular() const { return iYPaddingRegular; }
	int& XPaddingMagic() { return iXPaddingMagic; }
	int XPaddingMagic() const { return iXPaddingMagic; }
	int& YPaddingMagic() { return iYPaddingMagic; }
	int YPaddingMagic() const { return iYPaddingMagic; }
	XMFLOAT2& GridStartLocationRegular() { return vGridStartLocationRegular; }
	XMFLOAT2 GridStartLocationRegular() const { return vGridStartLocationRegular; }
	XMFLOAT2& GridStartLocationMagic() { return vGridStartLocationMagic; }
	XMFLOAT2 GridStartLocationMagic() const { return vGridStartLocationMagic; }
	
private:
	
	int iGridSize;
	
	Sprite* mBackground;
	string sBackgroundImageName;
	
	// Regular Items Info
	int iCountRegular;
	bool bSelectedRegular;
	int iSelectedIDRegular;
	int iXPaddingRegular;
	int iYPaddingRegular;
	int iMaxItemsXRegular;
	int iMaxItemsYRegular;
	int iMaxItemsAllowedRegular;
	XMFLOAT2 vGridStartLocationRegular;
	Sprite* mCursorNormalRegular;
	Sprite* mCursorHightlightRegular;
	vector<Sprite*> mItemsRegular;
	
	// Magic Items Info
	int iCountMagic;
	bool bSelectedMagic;
	int iSelectedIDMagic;
	int iXPaddingMagic;
	int iYPaddingMagic;
	int iMaxItemsXMagic;
	int iMaxItemsYMagic;
	int iMaxItemsAllowedMagic;
	XMFLOAT2 vGridStartLocationMagic;
	Sprite* mCursorNormalMagic;
	Sprite* mCursorHightlightMagic;
	vector<Sprite*> mItemsMagic;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLAYERINVENTORY2D_H