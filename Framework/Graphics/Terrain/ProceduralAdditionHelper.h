//===============================================================================================================================
// ProceduralAdditionHelper
//
// This class will add things to a heightmap
//===============================================================================================================================
// History
//
// -Created on 6/18/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __PROCEDURALADDITIONHELPER_H
#define __PROCEDURALADDITIONHELPER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "HeightmapContainer.h"
#include "ProceduralParameters.h"

//===============================================================================================================================
namespace ZShadeSandboxTerrain {
class ProceduralAdditionHelper
{
public:
	
	ProceduralAdditionHelper(ZShadeSandboxTerrain::HeightmapContainer heightmapInput, ZShadeSandboxTerrain::ProceduralParameters pp);
	~ProceduralAdditionHelper();
	
	void AddRandomHeightmap();
	void AddPerlinNoiseHeightmap();
	void AddFieldNoiseHeightmap();
	
	// Updates the heightmap with new values radiating from an origin point
	void RadiateHeightFromOrigin(int centerX, int centerZ, int radius, float minHeight, float maxHeight, bool addToInput);
	
	// Need to be able to add random stuff to parts of a heightmap,
	// like smoothing part of a heightmap, adding noise to part of a heightmap, flattening part of a heightmap
	
	
	ZShadeSandboxTerrain::HeightmapContainer ProceduralMap() { return mProceduralHeightmap; }
	
private:
	
	ZShadeSandboxTerrain::ProceduralParameters mProceduralParameters;
	
	// Input heightmap to start from
	ZShadeSandboxTerrain::HeightmapContainer mHeightMapInput;
	
	// The resulting height map after it has been affected
	ZShadeSandboxTerrain::HeightmapContainer mProceduralHeightmap;
};
}
//===============================================================================================================================
#endif//__PROCEDURALADDITIONHELPER_H