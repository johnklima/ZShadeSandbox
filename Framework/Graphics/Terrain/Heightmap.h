//===================================================================================================================
// Heightmap
//
//===================================================================================================================
// History
//
// -Created on 6/18/2014 by Dustin Watson
//===================================================================================================================
#ifndef __HEIGHTMAP_H
#define __HEIGHTMAP_H
//===================================================================================================================
//===================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "HeightmapData.h"
#include "ProceduralGenerator.h"
#include "HeightmapContainer.h"
#include "ProceduralParameters.h"

//===================================================================================================================
//===================================================================================================================
namespace ZShadeSandboxTerrain {
class Heightmap
{
public:
	
	Heightmap(string heightmap, ZShadeSandboxTerrain::ProceduralParameters pp, float heightScale);
	Heightmap(ZShadeSandboxTerrain::ProceduralParameters pp, float heightScale);
	Heightmap(const Heightmap& o);
	~Heightmap();
	
	bool LoadElevation(string heightmap);
	void LoadProceduralElevation();
	
	bool InBounds(int x, int z);
	bool InBounds(int index);
	float ReadHeight(int x, int z);
	float ReadHeight(int index);

	float ReadX(int x, int z);
	float ReadX(int index);
	float ReadZ(int x, int z);
	float ReadZ(int index);

	void UpdateHeight(int x, int z, float value) { m_heightmap.UpdateHeight(x, z, value); }
	void UpdateHeight(int index, float value) { m_heightmap.UpdateHeight(index, value); }

	int HeightmapSize() { return mProceduralParameters.terrainSize; }
	
	ZShadeSandboxTerrain::HeightmapContainer GetHeightmap() { return m_heightmap; }
	
	void CopyToHeightMapData();

	//
	// Procedural Generation for the heightmap
	//
	
	/*
	Body of a simple script generating a hilly map with roughly radial lake in the middle:
	
	// load map parameters
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");

	// create a radial gradient with height 1 in the center and height 1200 on the outer rim
	local base = GGen_Data_2D(width, height, 0);
	base.RadialGradient(width / 2, height / 2, width > height ? height / 2 : width / 2, 1, 1200, true);

	// create a separate noise map using default noise settings
	local noise = GGen_Data_2D(width, height);
	noise.Noise(2, width > height ? height / 8 : width / 8, GGEN_STD_NOISE);

	// adjust the range of the noise
	noise.ScaleValuesTo(-500, 500);

	// combine the maps
	base.AddMap(noise);

	// raise the water level so 9% of the map is under level 0
	base.Flood(0.91);

	return base;
	*/
	
	void BuildRandomHeightmap();
	void BuildPerlinNoiseHeightmap();
	void BuildFieldNoiseHeightmap();
	void BuildDiamondSquare();
	void AddRandomHeightmap();
	void AddPerlinNoiseHeightmap();
	void AddFieldNoiseHeightmap();

	// Perform a smoothing of the map so it is not choppy
	void Smooth(int smoothingPassCount);
	void Smooth();
	
	// Normalize the map so that it is not too tall
	void Normalize(float normalizeFactor);
	
	// Perform height erosion in four directions by using a linear interpolation from an erosion value
	// erosionValue if between 0 and 1 inclusive
	void ErodeHeight(float erosionValue);
	
	void ErodeWater(ZShadeSandboxTerrain::WaterErosionParameters wep);
	
	void ThermalWeathering(ZShadeSandboxTerrain::WaterErosionParameters wep);
	
	void Flatten(float flatHeight);
	
private:
	
	EHeightExtension::Type m_ext;
	
	float fHeightScale;
	
	ZShadeSandboxTerrain::ProceduralParameters mProceduralParameters;
	
	vector<HeightData> mHeightData;

	ZShadeSandboxTerrain::HeightmapContainer m_heightmap;
};
}
//===================================================================================================================
//===================================================================================================================
#endif//__HEIGHTMAP_H