//===============================================================================================================================
// WaterErosion
//
//===============================================================================================================================
// History
//
// -Created on 6/17/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __WATEREROSION_H
#define __WATEREROSION_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "HeightmapData.h"
#include "HeightmapContainer.h"
#include "ProceduralParameters.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
struct VelocityVector
{
	float x;
	float y;

	VelocityVector()
	{
		x = y = 0;
	}

	VelocityVector(float px, float py)
	{
		x = px;
		y = py;
	}
};

struct OutflowValues
{
	float left;
	float right;
	float top;
	float bottom;

	OutflowValues()
	{
		left = right = top = bottom = 0;
	}

	OutflowValues(float l, float r, float t, float b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}
};

struct ThermalWeatheringValues
{
	float topLeft;
	float top;
	float topRight;
	float right;
    float bottomRight;
    float bottom;
    float bottomLeft;
    float left;
};

//===============================================================================================================================
class WaterErosion
{
public:
	
	WaterErosion(ZShadeSandboxTerrain::HeightmapContainer heightMapInput, WaterErosionParameters wep, bool useThermalWeathering);
	~WaterErosion();
	
	void Erode();
	void ThermalWeathering();
	
	// Returns the generated erosion map
	ZShadeSandboxTerrain::HeightmapContainer GetErosionMap() { return mErosionMap; }
	
private:
	
	void ThermalErosion();
	void NormalErosion();
	void BuildVelocityVectorMap();
	void SetWaterSource();
	void ApplyEvaporation();
	void ApplyErosion();
	void ApplyThermalWeathering();
	
	OutflowValues ReadOutflowFluxMap(int x, int z);
	VelocityVector ReadVelocityVectorMap(int x, int z);
	
	void UpdateOutflowFluxMap(int x, int z, OutflowValues value);
	void UpdateVelocityVectorMap(int x, int z, VelocityVector value);
	
	float FlowValue(float direction, float heightDifference);
	
private:
	
	WaterErosionParameters mWaterErosionParameters;
	
	ZShadeSandboxTerrain::HeightmapContainer mHeightMapInput;
	ZShadeSandboxTerrain::HeightmapContainer mWaterMap;
	ZShadeSandboxTerrain::HeightmapContainer mSedimentationMap;
	ZShadeSandboxTerrain::HeightmapContainer mErosionMap;
	
	vector<OutflowValues> mOutflowFluxMap;
	vector<VelocityVector> mVelocityVectorMap;
};
}
//===============================================================================================================================
#endif//__WATEREROSION_H