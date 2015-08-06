#include "FogData.h"
//-----------------------------------------------------------------------------------------------------------------
/*ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_CNW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_CNE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_CSW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_CSE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_JNW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_JNE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_JSW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_JSE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_NNN_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_SSS_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_EEE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_shadow_WWW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_all_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CNW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CNE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CSW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CSE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JNW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JNE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JSW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JSE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_NNN_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_SSS_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_EEE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_WWW_texture = NULL;*/
//-----------------------------------------------------------------------------------------------------------------
D3D* FogData::m_D3DSystem = 0;
FogData* FogData::instance = 0;
//-----------------------------------------------------------------------------------------------------------------
signed char* FogData::m_fow_frame_table = 0;
//-----------------------------------------------------------------------------------------------------------------
FogData::FogData(D3D* d3d)
{
	FogData::m_D3DSystem = d3d;
}
//-----------------------------------------------------------------------------------------------------------------
FogData::FogData(const FogData& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
FogData::~FogData()
{
}
//-----------------------------------------------------------------------------------------------------------------
FogData* FogData::NewInstance(D3D* d3d)
{
	instance = new FogData(d3d);
	return instance;
}
//-----------------------------------------------------------------------------------------------------------------
FogData* FogData::Instance()
{
	return instance;
}
//-----------------------------------------------------------------------------------------------------------------
unsigned short FogData::FindValidConnectorFromNeighbors(
	unsigned short neighborBitNorth,
	unsigned short neighborBitSouth,
	unsigned short neighborBitEast,
	unsigned short neighborBitWest
)
{
	if (neighborBitNorth == fow_n_non || neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_CSE || neighborBitEast == fow_n_SSS
	&&	neighborBitWest == fow_n_CSW || neighborBitWest == fow_n_SSS)
	{
		return fow_n_SSS;
	}
	
	if (neighborBitNorth == fow_n_CNW || neighborBitNorth == fow_n_WWW
	&&	neighborBitSouth == fow_n_CSW || neighborBitSouth == fow_n_WWW
	&&	neighborBitEast == fow_n_non || neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_all)
	{
		return fow_n_WWW;
	}
	
	if (neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_non || neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_CNE || neighborBitEast == fow_n_NNN
	&&	neighborBitWest == fow_n_CNW || neighborBitWest == fow_n_NNN)
	{
		return fow_n_NNN;
	}
	
	if (neighborBitNorth == fow_n_CNE || neighborBitNorth == fow_n_EEE
	&&	neighborBitSouth == fow_n_CSE || neighborBitSouth == fow_n_EEE
	&&	neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_non || neighborBitWest == fow_n_all)
	{
		return fow_n_EEE;
	}
	
	if (neighborBitNorth == fow_n_non || neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_CSW
	&&	neighborBitEast == fow_n_non || neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_CSW)
	{
		return fow_n_JSW;
	}
	
	if (neighborBitNorth == fow_n_CNW
	&&	neighborBitSouth == fow_n_non || neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_non || neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_CNW)
	{
		return fow_n_JNW;
	}
	
	if (neighborBitNorth == fow_n_CNE
	&&	neighborBitSouth == fow_n_non || neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_CNE
	&&	neighborBitWest == fow_n_non || neighborBitWest == fow_n_all)
	{
		return fow_n_JNE;
	}
	
	if (neighborBitNorth == fow_n_non || neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_CSE
	&&	neighborBitEast == fow_n_CSE
	&&	neighborBitWest == fow_n_non || neighborBitWest == fow_n_all)
	{
		return fow_n_JSE;
	}
	
	if (neighborBitNorth == fow_n_JSW
	&&	neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_JSW
	&&	neighborBitWest == fow_n_all)
	{
		return fow_n_CSW;
	}
	
	if (neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_JNW
	&&	neighborBitEast == fow_n_JNW
	&&	neighborBitWest == fow_n_all)
	{
		return fow_n_CNW;
	}
	
	if (neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_JNE
	&&	neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_JNE)
	{
		return fow_n_CNE;
	}
	
	if (neighborBitNorth == fow_n_JSE
	&&	neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_JSE)
	{
		return fow_n_CSE;
	}
	
	if (neighborBitNorth == fow_n_all
	&&	neighborBitSouth == fow_n_all
	&&	neighborBitEast == fow_n_all
	&&	neighborBitWest == fow_n_all)
	{
		return fow_n_all;
	}
	
	if (neighborBitNorth == fow_n_non
	&&	neighborBitSouth == fow_n_non
	&&	neighborBitEast == fow_n_non
	&&	neighborBitWest == fow_n_non)
	{
		return fow_n_non;
	}

	return -1;
}
//-----------------------------------------------------------------------------------------------------------------