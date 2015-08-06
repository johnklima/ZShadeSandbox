//===============================================================================================================================
// Heightmap Editor Helper
//
// Interfaces with the terrain heightmap for the editor
//===============================================================================================================================
// History
//
// -Created on 6/19/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __HEIGHTMAPEDITORHELPER_H
#define __HEIGHTMAPEDITORHELPER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <set>
#include "D3D.h"
#include "Heightmap.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
namespace EBrushType
{
	enum Type
	{
		eRaise,
		eLower,
		eFlatten,
		eSmooth,
		eNone
	};
}
//===============================================================================================================================
class HeightmapEditorHelper
{
public:
	
	HeightmapEditorHelper();
	HeightmapEditorHelper(ZShadeSandboxTerrain::Heightmap* heightmap);
	~HeightmapEditorHelper();
	
	void Init();
	
	void SaveToFile(const char* filename);
	
	// Updates the selected point based on the brush type
	void UpdateBrush(XMFLOAT3 point);
	
	void FlattenHeightmap();
	void SmoothHeightmap();
	void ErodeHeightmap();
	void NormalizeHeightmap();
	
public:
	
	float& ErosionFactor()			{ return fErosionFactor; }
	float& NormizingFactor()		{ return fNormizingFactor; }
	float& SmoothingCount()			{ return fSmoothingCount; }
	float& SeaLevel()				{ return fSeaLevel; }
	float& FlatHeight()				{ return fFlatHeight; }
	float& MaxHeight()				{ return fMaxHeight; }
	float& BrushSize()				{ return fBrushSize; }
	float& BrushStrength()			{ return fBrushStrength; }
	EBrushType::Type& BrushType()	{ return mBrushType; }
	
	void SetHeightmap(ZShadeSandboxTerrain::Heightmap* heightmap) { mHeightmap = heightmap; }
	
private:
	
	void Raise(XMFLOAT3 point);
	void Lower(XMFLOAT3 point);
	void Flatten(XMFLOAT3 point);
	void Smooth(XMFLOAT3 point);
	void AddToHeightmapUpdateList(XMFLOAT3 point);
	void MoveVertices(XMFLOAT3 point, float height);
	void UpdateHeightmapAfterMove();
	
protected:
	
	ZShadeSandboxTerrain::Heightmap* mHeightmap;
	
	EBrushType::Type mBrushType;
	
	float fErosionFactor;
	float fNormizingFactor;
	float fSmoothingCount;
	float fSeaLevel;
	float fFlatHeight;
	float fMaxHeight;
	float fBrushSize;
	float fBrushStrength;
	
	// List of vertices to update from the heightmap
	std::set<HeightData> heightmapUpdateList;
};
}
//===============================================================================================================================
#endif//__HEIGHTMAPEDITORHELPER_H