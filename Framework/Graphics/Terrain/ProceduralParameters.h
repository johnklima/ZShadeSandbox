//===============================================================================================================================
// Procedural Parameters
//
//===============================================================================================================================
// History
//
// -Created on 6/18/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __PROCEDURALPARAMETERS_H
#define __PROCEDURALPARAMETERS_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxTerrain {
namespace EProceduralType
{
	enum Type
	{
		eRandom,
		ePerlinNoise,
		eFieldNoise,
		eDiamondSquare
	};
}

namespace EErosionType
{
	enum Type
	{
		eHeight,
		eWater
	};
}

namespace EWaterErosion
{
	enum Type
	{
		eThermal,
		eNormal
	};
}

struct WaterErosionParameters
{
	int		terrainSize;
	float	seaLevel;
	int 	waterSourceHeight;
	float	thermalPowerMultiplier;
	float	deltaT;
	float	pipeLength;
	float	pipeCrossectionArea;
	float	graviationalAcceleration;
	float	sedimentCapacityConstant;
	float	dissolvingConstant;
	float	depositionConstant;
	float	minimumComputedSurfaceTilt;
	float	talusAngle;
	float	erosionDuration;
	int		normalErosionRounds;
	float	normalErosionFactor;
	bool	enableNormalErosionSedimentation;

	ZShadeSandboxTerrain::EWaterErosion::Type erosionType;

	WaterErosionParameters()
	{
		terrainSize = 0;
		seaLevel = 0;
		waterSourceHeight = 0;
		thermalPowerMultiplier = 4.0f;
		deltaT = 0.005f;
		pipeLength = 1.0f;
		pipeCrossectionArea = 20.0f;
		graviationalAcceleration = 9.7f;
		sedimentCapacityConstant = 1.0f;
		dissolvingConstant = 0.5f;
		depositionConstant = 1.0f;
		minimumComputedSurfaceTilt = 0.1f;
		talusAngle = 0.5f;
		erosionDuration = 1.0f;
		normalErosionRounds = 1;
		normalErosionFactor = 5;
		enableNormalErosionSedimentation = true;
		erosionType = ZShadeSandboxTerrain::EWaterErosion::Type::eNormal;
	}
};

namespace EDiamondSquareType
{
	enum Type
	{
		eNormal,
		eBicubic
	};
}

struct ProceduralParameters
{
	int terrainSize;
	float seaLevel;
	float maxHeight;
	float erosionValue;
	int smoothingPassCount;
	float normalizeFactor;
	int diamondSquareFeatureSize;
	// Make sure the diamond square scale is not 0 or the height will be 0
	int diamondSquareScale;
	EProceduralType::Type proceduralType;
	EErosionType::Type erosionType;
	EDiamondSquareType::Type diamondSquareType;
	bool useErosion;
	bool useSmoothing;
	bool normalize;
	bool useThermalWeathering;

	ZShadeSandboxTerrain::WaterErosionParameters waterErosionParameters;
	
	ProceduralParameters()
	{
		terrainSize = 256;
		seaLevel = 0;
		maxHeight = 1024;
		erosionValue = 0.3f;
		smoothingPassCount = 1.0f;
		normalizeFactor = 1.0f;
		diamondSquareFeatureSize = 16;
		diamondSquareScale = 1;
		proceduralType = EProceduralType::Type::eRandom;
		erosionType = EErosionType::Type::eHeight;
		diamondSquareType = EDiamondSquareType::Type::eNormal;
		useErosion = false;
		useSmoothing = false;
		normalize = false;
		useThermalWeathering = false;
	}
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__PROCEDURALPARAMETERS_H