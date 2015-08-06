#include "PlayerInventory2D.h"
//==================================================================================================================================
//==================================================================================================================================
PlayerInventory2D::PlayerInventory2D(D3D* d3d)
:	m_D3DSystem(d3d)
,	iGridSize(64)
,	iCountRegular(0)
,	bSelectedRegular(false)
,	iSelectedIDRegular(-1)
,	iXPaddingRegular(0)
,	iYPaddingRegular(0)
,	iMaxItemsXRegular(0)
,	iMaxItemsYRegular(0)
,	iMaxItemsAllowedRegular(0)
,	vGridStartLocationRegular(0, 0)
,	mCursorNormalRegular(0)
,	mCursorHightlightRegular(0)
,	iCountMagic(0)
,	bSelectedMagic(false)
,	iSelectedIDMagic(-1)
,	iXPaddingMagic(0)
,	iYPaddingMagic(0)
,	iMaxItemsXMagic(0)
,	iMaxItemsYMagic(0)
,	iMaxItemsAllowedMagic(0)
,	vGridStartLocationMagic(0, 0)
,	mCursorNormalMagic(0)
,	mCursorHightlightMagic(0)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
bool PlayerInventory2D::RegularInventoryFull()
{
	return iCountRegular >= iMaxItemsAllowedRegular;
}
//==================================================================================================================================
bool PlayerInventory2D::MagicInventoryFull()
{
	return iCountMagic >= iMaxItemsAllowedMagic;
}
//==================================================================================================================================
bool PlayerInventory2D::CursorInsideRegularGrid(int x, int y)
{
	if ((x >= vGridStartLocationRegular.x &&  x < vGridStartLocationRegular.x + (iMaxItemsXRegular * (iGridSize + iXPaddingRegular)))
	&&  (y >= vGridStartLocationRegular.y &&  y < vGridStartLocationRegular.y + (iMaxItemsYRegular * (iGridSize + iYPaddingRegular))))
	{
		return true;
	}
	
	return false;
}
//==================================================================================================================================
bool PlayerInventory2D::CursorInsideMagicGrid(int x, int y)
{
	if ((x >= vGridStartLocationMagic.x &&  x < vGridStartLocationMagic.x + (iMaxItemsXMagic * (iGridSize + iXPaddingMagic)))
	&&  (y >= vGridStartLocationMagic.y &&  y < vGridStartLocationMagic.y + (iMaxItemsYMagic * (iGridSize + iYPaddingMagic))))
	{
		return true;
	}
	
	return false;
}
//==================================================================================================================================
void PlayerInventory2D::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		if (CursorInsideRegularGrid(x, y))
		{
			bSelectedRegular = true;
			
			// Locate the item that was selected
			Sprite* spr;
			for (int i = 0; i < mItemsRegular.size(); i++)
			{
				spr = mItemsRegular[i];
				
				if (spr->Body().Inside(x, y))
				{
					iSelectedIDRegular = spr->ID();
					break;
				}
			}
		}
		
		if (CursorInsideMagicGrid(x, y))
		{
			bSelectedMagic = true;
			
			// Locate the item that was selected
			Sprite* spr;
			for (int i = 0; i < mItemsMagic.size(); i++)
			{
				spr = mItemsMagic[i];
				
				if (spr->Body().Inside(x, y))
				{
					iSelectedIDMagic = spr->ID();
					break;
				}
			}
		}
	}
}
//==================================================================================================================================
void PlayerInventory2D::OnMouseMove(WPARAM btnState, int x, int y)
{
	float newX;
	float newY;
	
	SnapRegular(x, y, newX, newY);
	
	// Clip the selection tool to the inventory
	if (CursorInsideRegularGrid(newX, newY))
	{
		mCursorNormalRegular->TopLeftPosition() = XMFLOAT3(newX, newY, 0);
		mCursorHightlightRegular->TopLeftPosition() = XMFLOAT3(newX, newY, 0);
	}
	
	SnapMagic(x, y, newX, newY);
	
	// Clip the selection tool to the inventory
	if (CursorInsideMagicGrid(newX, newY))
	{
		mCursorNormalMagic->TopLeftPosition() = XMFLOAT3(newX, newY, 0);
		mCursorHightlightMagic->TopLeftPosition() = XMFLOAT3(newX, newY, 0);
	}
}
//==================================================================================================================================
void PlayerInventory2D::OnMouseUp(WPARAM btnState, int x, int y)
{
	bSelectedRegular = false;
	bSelectedMagic = false;
}
//==================================================================================================================================
void PlayerInventory2D::Render(Camera* camera)
{
	mBackground->Render(camera);
	
	RenderRegularItems(camera);
	RenderMagicItems(camera);
}
//==================================================================================================================================
void PlayerInventory2D::RenderRegularItems(Camera* camera)
{
	Sprite* spr;
	for (int i = 0; i < mItemsRegular.size(); i++)
	{
		spr = mItemsRegular[i];
		
		//Update the animation if it exists
		spr->UpdateAnimation();
		
		m_D3DSystem->TurnOnAlphaBlending();
		{
			spr->Render(camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
	
	m_D3DSystem->TurnOnAlphaBlending();
	{
		if (bSelectedRegular)
			mCursorHightlightRegular->Render(camera, 0);
		else
			mCursorNormalRegular->Render(camera, 0);
	}
	m_D3DSystem->TurnOffAlphaBlending();
}
//==================================================================================================================================
void PlayerInventory2D::RenderMagicItems(Camera* camera)
{
	Sprite* spr;
	for (int i = 0; i < mItemsMagic.size(); i++)
	{
		spr = mItemsMagic[i];
		
		//Update the animation if it exists
		spr->UpdateAnimation();
		
		m_D3DSystem->TurnOnAlphaBlending();
		{
			spr->Render(camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
	
	m_D3DSystem->TurnOnAlphaBlending();
	{
		if (bSelectedMagic)
			mCursorHightlightMagic->Render(camera, 0);
		else
			mCursorNormalMagic->Render(camera, 0);
	}
	m_D3DSystem->TurnOffAlphaBlending();
}
//==================================================================================================================================
void PlayerInventory2D::Init(string base_path
,	string background_path
,	string background_texture
,	int backgroundWidth
,	int backgroundHeight
,	int cursorWidth
,	int cursorHeight
,	int gridSize
,	int maxItemsXRegular
,	int maxItemsYRegular
,	int maxItemsXMagic
,	int maxItemsYMagic
,	int xPaddingRegular
,	int yPaddingRegular
,	int xPaddingMagic
,	int yPaddingMagic
,	XMFLOAT2 gridStartLocationRegular
,	XMFLOAT2 gridStartLocationMagic
)
{
	vGridStartLocationRegular = gridStartLocationRegular;
	vGridStartLocationMagic = gridStartLocationMagic;
	
	iGridSize = gridSize;
	iMaxItemsXRegular = maxItemsXRegular;
	iMaxItemsYRegular = maxItemsYRegular;
	iMaxItemsXMagic = maxItemsXMagic;
	iMaxItemsYMagic = maxItemsYMagic;
	iXPaddingRegular = xPaddingRegular;
	iYPaddingRegular = yPaddingRegular;
	iXPaddingMagic = xPaddingMagic;
	iYPaddingMagic = yPaddingMagic;
	
	iMaxItemsAllowedRegular = iMaxItemsXRegular * iMaxItemsYRegular;
	iMaxItemsAllowedMagic = iMaxItemsXMagic * iMaxItemsYMagic;
	
	XMFLOAT3 startLocation(vGridStartLocationRegular.x, vGridStartLocationRegular.y, 0);
	mCursorNormalRegular = new Sprite(m_D3DSystem);
	mCursorNormalRegular->Initialize(base_path, "stamp_normal.png", startLocation, cursorWidth, cursorHeight, STATIC);
	mCursorHightlightRegular = new Sprite(m_D3DSystem);
	mCursorHightlightRegular->Initialize(base_path, "stamp_highlight.png", startLocation, cursorWidth, cursorHeight, STATIC);
	
	startLocation = XMFLOAT3(vGridStartLocationMagic.x, vGridStartLocationMagic.y, 0);
	mCursorNormalMagic = new Sprite(m_D3DSystem);
	mCursorNormalMagic->Initialize(base_path, "stamp_normal.png", startLocation, cursorWidth, cursorHeight, STATIC);
	mCursorHightlightMagic = new Sprite(m_D3DSystem);
	mCursorHightlightMagic->Initialize(base_path, "stamp_highlight.png", startLocation, cursorWidth, cursorHeight, STATIC);
	
	mBackground = new Sprite(m_D3DSystem);
	mBackground->Initialize(background_path, background_texture, XMFLOAT3(0, 0, 0), backgroundWidth, backgroundHeight, STATIC);
	
	sBackgroundImageName = background_texture;
}
//==================================================================================================================================
void PlayerInventory2D::SnapRegular(int x, int y, float& rx, float& ry)
{
	rx = (x / (iGridSize + iXPaddingRegular)) * (iGridSize + iXPaddingRegular);
	ry = (y / (iGridSize + iYPaddingRegular)) * (iGridSize + iYPaddingRegular);
}
//==================================================================================================================================
void PlayerInventory2D::SnapMagic(int x, int y, float& rx, float& ry)
{
	rx = (x / (iGridSize + iXPaddingMagic)) * (iGridSize + iXPaddingMagic);
	ry = (y / (iGridSize + iYPaddingMagic)) * (iGridSize + iYPaddingMagic);
}
//==================================================================================================================================
string PlayerInventory2D::SelectedInventoryMessageRegular()
{
	Sprite* spr;
	for (int i = 0; i < mItemsRegular.size(); i++)
	{
		spr = mItemsRegular[i];

		if (spr->ID() == iSelectedIDRegular)
		{
			return spr->InventoryMessage();
		}
	}
	
	return "NONE";
}
//==================================================================================================================================
string PlayerInventory2D::SelectedInventoryMessageMagic()
{
	Sprite* spr;
	for (int i = 0; i < mItemsMagic.size(); i++)
	{
		spr = mItemsMagic[i];

		if (spr->ID() == iSelectedIDMagic)
		{
			return spr->InventoryMessage();
		}
	}
	
	return "NONE";
}
//==================================================================================================================================
void PlayerInventory2D::PositionItemsOnGrid()
{
	PositionItemsOnGridRegular();
	PositionItemsOnGridMagic();
}
//==================================================================================================================================
void PlayerInventory2D::PositionItemsOnGridRegular()
{
	float gridStartX = vGridStartLocationRegular.x;
	float gridStartY = vGridStartLocationRegular.y;
	
	Sprite* spr;
	for (int i = 0; i < mItemsRegular.size(); i++)
	{
		spr = mItemsRegular[i];
		
		spr->TopLeftPosition() = XMFLOAT3(gridStartX, gridStartY, 0);
		
		gridStartX += iGridSize;
		gridStartX += iXPaddingRegular;
		
		if (gridStartX > vGridStartLocationRegular.x + (iMaxItemsXRegular * (iGridSize + iXPaddingRegular)))
		{
			gridStartX = vGridStartLocationRegular.x;
			gridStartY += iGridSize;
			gridStartY += iYPaddingRegular;
		}
	}
}
//==================================================================================================================================
void PlayerInventory2D::PositionItemsOnGridMagic()
{
	float gridStartX = vGridStartLocationMagic.x;
	float gridStartY = vGridStartLocationMagic.y;
	
	Sprite* spr;
	for (int i = 0; i < mItemsMagic.size(); i++)
	{
		spr = mItemsMagic[i];
		
		spr->TopLeftPosition() = XMFLOAT3(gridStartX, gridStartY, 0);
		
		gridStartX += iGridSize;
		gridStartX += iXPaddingMagic;
		
		if (gridStartX > vGridStartLocationMagic.x + (iMaxItemsXMagic * (iGridSize + iXPaddingMagic)))
		{
			gridStartX = vGridStartLocationMagic.x;
			gridStartY += iGridSize;
			gridStartY += iYPaddingMagic;
		}
	}
}
//==================================================================================================================================
void PlayerInventory2D::AddItemRegular(string base_item_path, string item_texture, string inventoryMessage)
{
	if (!RegularInventoryFull())
	{
		Sprite* newItem = new Sprite(m_D3DSystem);
		newItem->Initialize(base_item_path, item_texture, XMFLOAT3(0, 0, 0), iGridSize, iGridSize, STATIC);
		newItem->InventoryMessage() = inventoryMessage;
		AddItemRegular(newItem);
	}
}
//==================================================================================================================================
void PlayerInventory2D::AddItemRegular(Sprite* item)
{
	if (!RegularInventoryFull())
	{
		item->ID() = iCountRegular;
		mItemsRegular.push_back(item);
		iCountRegular++;
	}
}
//==================================================================================================================================
void PlayerInventory2D::RemoveItemRegular(int itemID)
{
	Sprite* spr = 0;
	auto current = mItemsRegular.begin();
	
	while (current != mItemsRegular.end())
	{
		spr = (*current);
		if (spr != 0 && spr->ID() == itemID)
		{
			current = mItemsRegular.erase(current);
			iCountRegular--;
		}
		else
		{
			++current;
		}
	}
	
	// Refractor the ids
	for (int i = 0; i < mItemsRegular.size(); i++)
	{
		spr = mItemsRegular[i];

		spr->ID() = i;
	}
	
	PositionItemsOnGridRegular();
}
//==================================================================================================================================
void PlayerInventory2D::RemoveItemRegular(Sprite* item)
{
	Sprite* spr = 0;
	auto current = mItemsRegular.begin();
	
	while (current != mItemsRegular.end())
	{
		spr = (*current);
		if (spr != 0 && spr->ID() == item->ID())
		{
			current = mItemsRegular.erase(current);
			iCountRegular--;
		}
		else
		{
			++current;
		}
	}
	
	// Refractor the ids
	for (int i = 0; i < mItemsRegular.size(); i++)
	{
		spr = mItemsRegular[i];

		spr->ID() = i;
	}
	
	PositionItemsOnGridRegular();
}
//==================================================================================================================================
void PlayerInventory2D::AddItemMagic(string base_item_path, string item_texture, string inventoryMessage)
{
	if (!MagicInventoryFull())
	{
		Sprite* newItem = new Sprite(m_D3DSystem);
		newItem->Initialize(base_item_path, item_texture, XMFLOAT3(0, 0, 0), iGridSize, iGridSize, STATIC);
		newItem->InventoryMessage() = inventoryMessage;
		AddItemMagic(newItem);
	}
}
//==================================================================================================================================
void PlayerInventory2D::AddItemMagic(Sprite* item)
{
	if (!MagicInventoryFull())
	{
		item->ID() = iCountMagic;
		mItemsMagic.push_back(item);
		iCountMagic++;
	}
}
//==================================================================================================================================
void PlayerInventory2D::RemoveItemMagic(int itemID)
{
	Sprite* spr = 0;
	auto current = mItemsMagic.begin();
	
	while (current != mItemsMagic.end())
	{
		spr = (*current);
		if (spr != 0 && spr->ID() == itemID)
		{
			current = mItemsMagic.erase(current);
			iCountMagic--;
		}
		else
		{
			++current;
		}
	}
	
	// Refractor the ids
	for (int i = 0; i < mItemsMagic.size(); i++)
	{
		spr = mItemsMagic[i];

		spr->ID() = i;
	}
	
	PositionItemsOnGridMagic();
}
//==================================================================================================================================
void PlayerInventory2D::RemoveItemMagic(Sprite* item)
{
	Sprite* spr = 0;
	auto current = mItemsMagic.begin();
	
	while (current != mItemsMagic.end())
	{
		spr = (*current);
		if (spr != 0 && spr->ID() == item->ID())
		{
			current = mItemsMagic.erase(current);
			iCountMagic--;
		}
		else
		{
			++current;
		}
	}
	
	// Refractor the ids
	for (int i = 0; i < mItemsMagic.size(); i++)
	{
		spr = mItemsMagic[i];

		spr->ID() = i;
	}
	
	PositionItemsOnGridMagic();
}
//==================================================================================================================================