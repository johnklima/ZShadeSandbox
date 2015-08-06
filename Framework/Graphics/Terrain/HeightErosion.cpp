#include "HeightErosion.h"
#include "CGlobal.h"
using ZShadeSandboxTerrain::HeightErosion;
//===============================================================================================================================
//===============================================================================================================================
HeightErosion::HeightErosion(ZShadeSandboxTerrain::HeightmapContainer heightMapInput, int terrainSize, float erosionValue)
:	iTerrainSize(terrainSize)
,	fErosionValue(erosionValue)
{
	mErosionMap.HeightmapSize() = iTerrainSize;
	mErosionMap.SeaLevel() = 0;
	mErosionMap.Init();
	
	mHeightMap.HeightmapSize() = iTerrainSize;
	mHeightMap.SeaLevel() = 0;
	mHeightMap.CopyHeight(heightMapInput);
	
	Erode();
}
//===============================================================================================================================
HeightErosion::~HeightErosion()
{
}
//===============================================================================================================================
void HeightErosion::Erode()
{
	/*
		What you are doing here is basically interpolating between the height in the previous array element with the current array element.
		Whatever percentage the ErosionValue is, you will use that percentage of the PreviousValue and add that to the inverse percentage
		of the current value. So, let's say the erosion value is 0.3. That represents 30%. So the formula becomes "take 30% of the
		previous value and combine that with 70% of the current value". A value of 1.0 would be 100% and you would get a flat plain
		because it would "smear" the first value 100% into all the other values into the row or column. A value of 0.0 would be 0% and
		the result would be 0% of the previous value and 100% of the current value. The final result would be absolutely no change to
		the row or column, and ultimately no change to the shape of the terrain. So, you can hopefully see that this ErosionValue is a
		percentage of how flat you want the terrain to be.

		Believe it or not, that's all there is to this algorithm. You simply process every row in the array from left to right using this
		formula, then do it again from right to left, then do it again from top to bottom using columns instead of rows, and then do it
		again from bottom to top using columns instead of rows. And your terrain will be "eroded" by whatever percentage you used for
		the ErosionValue.
	*/
	
	// Process the values from east to west
	UpdateErosion(-1, 0, 0, 0);
	RebuildHeightmap();
	UpdateErosion( 1, 0, 0, 0);
	RebuildHeightmap();
	
	// Process the values from west to east
	UpdateErosion(0, -1, 0, 0);
	RebuildHeightmap();
	UpdateErosion(0,  1, 0, 0);
	RebuildHeightmap();
	
	// Process the values from north to south
	UpdateErosion(0, 0, -1, 0);
	RebuildHeightmap();
	UpdateErosion(0, 0,  1, 0);
	RebuildHeightmap();
	
	// Process the values from south to north
	UpdateErosion(0, 0, 0, -1);
	RebuildHeightmap();
	UpdateErosion(0, 0, 0,  1);
	RebuildHeightmap();
}
//===============================================================================================================================
void HeightErosion::UpdateErosion(int xUpdate0, int xUpdate1, int zUpdate0, int zUpdate1)
{
	float currHeight;
	float prevHeight;
	float erosion;
	
	for (int z = 0; z < iTerrainSize; z++)
	{
		for (int x = 0; x < iTerrainSize; x++)
		{
			currHeight = mHeightMap.ReadHeight(x + xUpdate0, z + zUpdate0);
			prevHeight = mHeightMap.ReadHeight(x + xUpdate1, z + zUpdate1);
			
			// Could be that the next or prev was not on the map
			if (currHeight == 0) currHeight = mHeightMap.ReadHeight(x, z);
			if (prevHeight == 0) prevHeight = mHeightMap.ReadHeight(x, z);
			
			erosion = CalculateErosionValue(currHeight, prevHeight);
			
			mErosionMap.UpdateHeight(x, z, erosion);
		}
	}
}
//===============================================================================================================================
void HeightErosion::RebuildHeightmap()
{
	for (int z = 0; z < iTerrainSize; z++)
	{
		for (int x = 0; x < iTerrainSize; x++)
		{
			mHeightMap.UpdateHeight(x, z, mErosionMap.ReadHeight(x, z));
		}
	}
}
//===============================================================================================================================
float HeightErosion::CalculateErosionValue(float currHeight, float prevHeight)
{
	return fErosionValue * prevHeight + (1 - fErosionValue) * currHeight;
}
//===============================================================================================================================