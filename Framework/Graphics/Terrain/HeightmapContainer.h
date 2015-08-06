//===============================================================================================================================
// HeightmapContainer
//
// Height is returned in a safe way
//===============================================================================================================================
// History
//
// -Created on 6/18/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __HEIGHTMAPCONTAINER_H
#define __HEIGHTMAPCONTAINER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "HeightmapData.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
class HeightmapContainer
{
public:
	
	HeightmapContainer();
	HeightmapContainer(int heightmapSize, float seaLevel);
	~HeightmapContainer();
	
	void Init();
	void Clear(bool resize = true);
	void Resize();
	bool InBounds(int x, int z);
	float ReadHeight(int x, int z, ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Type::Axis_Y);
	float ReadHeight(int index, ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Type::Axis_Y);
	void UpdateHeight(int x, int z, float value, ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Type::Axis_Y);
	void UpdateHeight(int index, float value, ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Type::Axis_Y);
	void UpdateHeight(int x, int z, ZShadeSandboxTerrain::HeightData hd);
	void UpdateHeight(int index, ZShadeSandboxTerrain::HeightData hd);
	void CopyHeight(HeightmapContainer hmap);
	void AddHeight(HeightmapContainer hmap);
	void SubtractHeight(HeightmapContainer hmap);
	void ScaleHeight(HeightmapContainer hmap);
	void NormalizeHeight(HeightmapContainer hmap);
	void FillHeight(float height);
	void AddHeight(float height);
	void SubtractHeight(float height);
	void ScaleHeight(float height);
	void NormalizeHeight(float height);
	void FlattenToSeaLevel();
	
	int& HeightmapSize() { return iHeightmapSize; }
	float& SeaLevel() { return fSeaLevel; }
	
private:
	
	float fSeaLevel;
	int iHeightmapSize;
	
	vector<ZShadeSandboxTerrain::HeightData> mHeightMap;
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__HEIGHTMAPCONTAINER_H