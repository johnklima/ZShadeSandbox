#include "ProceduralAdditionHelper.h"
#include "ProceduralGenerator.h"
#include "ZMath.h"
using ZShadeSandboxTerrain::ProceduralAdditionHelper;
//===============================================================================================================================
//===============================================================================================================================
ProceduralAdditionHelper::ProceduralAdditionHelper(ZShadeSandboxTerrain::HeightmapContainer heightmapInput, ZShadeSandboxTerrain::ProceduralParameters pp)
:	mProceduralParameters(pp)
{
	mProceduralHeightmap.HeightmapSize() = mProceduralParameters.terrainSize;
	mProceduralHeightmap.SeaLevel() = mProceduralParameters.seaLevel;
	mProceduralHeightmap.Init();
	
	mHeightMapInput.HeightmapSize() = mProceduralParameters.terrainSize;
	mHeightMapInput.SeaLevel() = mProceduralParameters.seaLevel;
	mHeightMapInput.CopyHeight(heightmapInput);
}
//===============================================================================================================================
ProceduralAdditionHelper::~ProceduralAdditionHelper()
{
}
//===============================================================================================================================
void ProceduralAdditionHelper::AddRandomHeightmap()
{
	XMFLOAT2 point(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight);
	
	ZShadeSandboxMath::ZMath::RandomSeed();
	
	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			float height = mHeightMapInput.ReadHeight(x, z) + ZShadeSandboxMath::ZMath::RandF<float>(point.x, point.y) * 0.01f;
			
			mProceduralHeightmap.UpdateHeight(x, z, height);
		}
	}
}
//===============================================================================================================================
void ProceduralAdditionHelper::AddPerlinNoiseHeightmap()
{
	ZShadeSandboxMath::ZMath::RandomSeed();
	
	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			float height = mHeightMapInput.ReadHeight(x, z) + ProceduralGenerator::PerlinValue(x, z, 1000) * 5.0f;
			
			mProceduralHeightmap.UpdateHeight(x, z, height);
		}
	}
}
//===============================================================================================================================
void ProceduralAdditionHelper::AddFieldNoiseHeightmap()
{
	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			float height = mHeightMapInput.ReadHeight(x, z) + (float)sin(x);
			
			mProceduralHeightmap.UpdateHeight(x, z, height);
		}
	}
}
//===============================================================================================================================
void ProceduralAdditionHelper::RadiateHeightFromOrigin(int centerX, int centerZ, int radius, float minHeight, float maxHeight, bool addToInput)
{
	float realMaxHeight = maxHeight - minHeight;
	
	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			float distance = (float)sqrt(SQR((float)abs(x - centerX)) + SQR((float)abs(z - centerZ)));
			
			if (distance < (float)radius)
			{
				float height = minHeight + realMaxHeight * distance / (float)radius;
				
				if (addToInput)
				{
					height += mHeightMapInput.ReadHeight(x, z);
				}
				
				mProceduralHeightmap.UpdateHeight(x, z, height);
			}
			else
			{
				float height = minHeight + realMaxHeight;
				
				if (addToInput)
				{
					height += mHeightMapInput.ReadHeight(x, z);
				}
				
				mProceduralHeightmap.UpdateHeight(x, z, height);
			}
		}
	}
}
//===============================================================================================================================
