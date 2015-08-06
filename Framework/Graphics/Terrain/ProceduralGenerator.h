//===============================================================================================================================
// Procedural Generator
//
//===============================================================================================================================
// History
//
// -Created on 6/16/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __PROCEDURALGENERATOR_H
#define __PROCEDURALGENERATOR_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "HeightErosion.h"
#include "WaterErosion.h"
#include "HeightmapContainer.h"
#include "ProceduralParameters.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
class ProceduralGenerator
{
public:
	
	ProceduralGenerator(ZShadeSandboxTerrain::ProceduralParameters pp);
	~ProceduralGenerator();
	
	void BuildRandomHeightmap();
	void BuildPerlinNoiseHeightmap();
	void BuildFieldNoiseHeightmap();
	
	// This function uses the diamond-square mid-point displacement algorithm with bicubic interpolation
	void BuildDiamondSquare();
	
	// Returns the generated procedural map
	ZShadeSandboxTerrain::HeightmapContainer ProceduralMap() { return mProceduralHeightmap; }
	
	static float PerlinValue(int x, int y, int random);
	static float BicubicInterpolation(float p[4][4]);
	
private:
	
	float ReadOverflowProceduralHeight(int x, int z);
	
private:
	
	// The resulting height map after it has been affected procedurally
	ZShadeSandboxTerrain::HeightmapContainer mProceduralHeightmap;
	
	vector<ZShadeSandboxTerrain::HeightData> mVerticalOverflow;
	vector<ZShadeSandboxTerrain::HeightData> mHorizontalOverflow;
	
	ZShadeSandboxTerrain::ProceduralParameters mProceduralParameters;
};
}
//===============================================================================================================================
#endif//__PROCEDURALGENERATOR_H