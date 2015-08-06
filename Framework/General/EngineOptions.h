//================================================================================================================
// EngineOptions.h
//
// This was initially created for 2D maps
//================================================================================================================
// History
//
// Updated on 5/19/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENGINEOPTIONS_H
#define __ENGINEOPTIONS_H
//================================================================================================================
//================================================================================================================
#include "GameDirectory.h"
#include <Windows.h>
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
//================================================================================================================
//================================================================================================================
enum D3D_VERSION
{
	DIRECTX10,
	DIRECTX11
};
//================================================================================================================
enum DimType
{
	ZSHADE_2D,
	ZSHADE_3D
};
//================================================================================================================
enum Map2DType
{
	REGULAR,
	ISOMETRIC
};
//================================================================================================================
enum GameType2D
{
	TOPDOWN,
	PLATFORMER
};
//================================================================================================================
struct EngineOptions
{
	HWND hwnd;
	HWND toolHwnd;
	HINSTANCE hinstance;
	D3D_VERSION m_d3dVersion;
	DimType m_DimType;
	GameType2D m_GameType2D;
	GameDirectory2D* m_GD2D;
	GameDirectory3D* m_GD3D;
	string m_Env3DType;
	string m_GameFolderName;
	XMFLOAT3 m_TransparencyKey;
	
	float fFarPlane;
	float fNearPlane;
	
	bool m_inEditor;
	bool m_vsync;
	bool fullscreen;
	int m_screenWidth;
	int m_screenHeight;
	//int m_renderDistance2D;

	int TILE_SECTION_MAX;// = 768
	int SECTION_ROW_SIZE;// = 32
	int SECTION_COL_SIZE;// = 24
	int ROW_SIZE;// = 12
	int COL_SIZE;// = 8
	int TILE_SIZE;// = 64

	//Added for Isometric maps
	int ISO_ROW_SIZE;// = 8
	int ISO_COL_SIZE;// = 8
	int ISO_TILE_WIDTH;// = 64
	int ISO_TILE_HEIGHT;// = 32

	void InitializeDefaults()
	{
		m_d3dVersion = DIRECTX11;
		m_DimType = ZSHADE_2D;
		m_Env3DType = "";
		m_inEditor = false;
		fullscreen = false;
		m_vsync = false;
		m_screenWidth = 800;
		m_screenHeight = 600;
		hwnd = nullptr;
		m_GD3D = NULL;
		m_GameType2D = TOPDOWN;
		m_GD2D = NULL;
		SECTION_ROW_SIZE = 3;//y
		SECTION_COL_SIZE = 3;//x
		TILE_SECTION_MAX = SECTION_ROW_SIZE * SECTION_COL_SIZE;
		ROW_SIZE = 12;
		COL_SIZE = 8;
		TILE_SIZE = 64;
		ISO_ROW_SIZE = 8;
		ISO_COL_SIZE = 8;
		ISO_TILE_WIDTH = 64;
		ISO_TILE_HEIGHT = 32;
		//m_renderDistance2D = 2;
	}
	
	void SetTileMax()
	{
		TILE_SECTION_MAX = SECTION_ROW_SIZE * SECTION_COL_SIZE;
	}
};
//================================================================================================================
//================================================================================================================
#endif//__ENGINEOPTIONS_H