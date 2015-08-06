#include "WaterErosion.h"
using ZShadeSandboxTerrain::WaterErosion;
//===============================================================================================================================
//===============================================================================================================================
WaterErosion::WaterErosion(ZShadeSandboxTerrain::HeightmapContainer heightMapInput, ZShadeSandboxTerrain::WaterErosionParameters wep, bool useThermalWeathering)
:	mWaterErosionParameters(wep)
{
	mErosionMap.HeightmapSize() = mWaterErosionParameters.terrainSize;
	mErosionMap.SeaLevel() = mWaterErosionParameters.seaLevel;
	mErosionMap.Init();
	
	mHeightMapInput.HeightmapSize() = mWaterErosionParameters.terrainSize;
	mHeightMapInput.SeaLevel() = mWaterErosionParameters.seaLevel;
	mHeightMapInput.Init();
	mHeightMapInput.CopyHeight(heightMapInput);
	
	mWaterMap.HeightmapSize() = mWaterErosionParameters.terrainSize;
	mWaterMap.SeaLevel() = mWaterErosionParameters.seaLevel;
	mWaterMap.Init();
	
	mSedimentationMap.HeightmapSize() = mWaterErosionParameters.terrainSize;
	mSedimentationMap.SeaLevel() = mWaterErosionParameters.seaLevel;
	mSedimentationMap.Init();
	
	mOutflowFluxMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	mVelocityVectorMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	
	if (useThermalWeathering)
		ThermalWeathering();
	else
		Erode();
}
//===============================================================================================================================
WaterErosion::~WaterErosion()
{
}
//===============================================================================================================================
void WaterErosion::Erode()
{
	switch (mWaterErosionParameters.erosionType)
	{
		case ZShadeSandboxTerrain::EWaterErosion::Type::eNormal:
		{
			NormalErosion();
		}
		break;
		case ZShadeSandboxTerrain::EWaterErosion::Type::eThermal:
		{
			ThermalErosion();
		}
		break;
	}
}
//===============================================================================================================================
void WaterErosion::ThermalErosion()
{
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateOutflowFluxMap(x, z, OutflowValues(0, 0, 0, 0));
		}
	}
	
	mWaterErosionParameters.waterSourceHeight *= 0.05f;
	
	mWaterErosionParameters.erosionDuration = (1.0 + mWaterErosionParameters.erosionDuration) * 0.7f;

	for (float time = mWaterErosionParameters.erosionDuration; time > 0; time -= mWaterErosionParameters.deltaT)
	{
		SetWaterSource();

		BuildVelocityVectorMap();

		ApplyErosion();

		ApplyThermalWeathering();

		ApplyEvaporation();

		float maxLength = 0;
		for (int i = 0; i < mWaterErosionParameters.terrainSize; i++)
		{
			float length = sqrt(mVelocityVectorMap[i].x * mVelocityVectorMap[i].x + mVelocityVectorMap[i].y * mVelocityVectorMap[i].y);
			if (length > maxLength)
			{
				maxLength = length;
			}
		}

		mWaterErosionParameters.deltaT = 1 / (1.5 * maxLength);
		mWaterErosionParameters.deltaT = min(mWaterErosionParameters.deltaT, 0.05);
	}
	
	// Get the final erosion map from the affected height map
	mErosionMap.CopyHeight(mHeightMapInput);
}
//===============================================================================================================================
void WaterErosion::ThermalWeathering()
{
	mWaterErosionParameters.deltaT = 0.1f;
	
	for (float time = mWaterErosionParameters.erosionDuration; time > 0; time -= mWaterErosionParameters.deltaT)
	{
		ApplyThermalWeathering();
	}
	
	// Get the final erosion map from the affected height map
	mErosionMap.CopyHeight(mHeightMapInput);
}
//===============================================================================================================================
void WaterErosion::NormalErosion()
{
	for (int round = 0; round < mWaterErosionParameters.normalErosionRounds * mWaterErosionParameters.terrainSize; round++)
	{
		float x = ZShadeSandboxMath::ZMath::RandF<float>(0, mWaterErosionParameters.terrainSize - 1);
		float z = ZShadeSandboxMath::ZMath::RandF<float>(0, mWaterErosionParameters.terrainSize - 1);
		
		int currentIndex = (z * mWaterErosionParameters.terrainSize) + x;
		
		int currentCarriedSediment = 0;
		
		// Do not check any spots under water
		if (mHeightMapInput.ReadHeight(currentIndex) <= mWaterErosionParameters.seaLevel)
		{
			continue;
		}
		
		// Advance until water level is reached
		while (mHeightMapInput.ReadHeight(currentIndex) > mWaterErosionParameters.seaLevel)
		{
			int lowestNeighborX = x;
			int lowestNeighborZ = z;
			
			int lowestNeighborHeight = mHeightMapInput.ReadHeight(currentIndex);
			
			// Try to look at 5 random points in the neighborhood (to add a little randomness into the flow)
			for (int i = 0; i < 5; i++)
			{
				int currentNeighborX = x + ZShadeSandboxMath::ZMath::RandF<float>(-1, 1);
				int currentNeighborZ = z + ZShadeSandboxMath::ZMath::RandF<float>(-1, 1);
				
				// Ignore the neighbor if it is outside the map
				if (currentNeighborX < 0 || currentNeighborX == mWaterErosionParameters.terrainSize || lowestNeighborZ < 0 || currentNeighborZ == mWaterErosionParameters.terrainSize)
				{
					continue;
				}

				if (mHeightMapInput.ReadHeight(currentNeighborX + mWaterErosionParameters.terrainSize * currentNeighborZ) < lowestNeighborHeight)
				{
					lowestNeighborX = currentNeighborX;
					lowestNeighborZ = currentNeighborZ;
					lowestNeighborHeight = mHeightMapInput.ReadHeight(currentNeighborX + mWaterErosionParameters.terrainSize * currentNeighborZ);
				}
			}
			
			// If no lower neighbour was found, try to lift the tile with the suspended sediment.
			if (lowestNeighborX == x && lowestNeighborZ == z)
			{
				// If we are out of sediment, terminate the flow.
				if(currentCarriedSediment <= mWaterErosionParameters.seaLevel) break;
				
				mHeightMapInput.UpdateHeight(currentIndex, mHeightMapInput.ReadHeight(currentIndex) + mWaterErosionParameters.normalErosionFactor);
				
				// Consume the suspended sediment at twice the erosion rate (to prevent infinite cycles of erosion and sedimentation).
				currentCarriedSediment -= 2;
			}
			// A lower neighbour was found, lower the current tile and move the cursor to that neighbour.
			else
			{
				mHeightMapInput.UpdateHeight(currentIndex, mHeightMapInput.ReadHeight(currentIndex) - mWaterErosionParameters.normalErosionFactor);
				
				if(mWaterErosionParameters.enableNormalErosionSedimentation)
				{
					currentCarriedSediment += 1;
				}
				
				x = lowestNeighborX;
				z = lowestNeighborZ;
				
				currentIndex = (z * mWaterErosionParameters.terrainSize) + x;
			}
		}
	}
	
	// Get the final erosion map from the affected height map
	mErosionMap.CopyHeight(mHeightMapInput);
}
//===============================================================================================================================
void WaterErosion::SetWaterSource()
{
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			mWaterMap.UpdateHeight(x, z, mWaterMap.ReadHeight(x, z) + (mWaterErosionParameters.waterSourceHeight * mWaterErosionParameters.deltaT));
		}
	}
}
//===============================================================================================================================
void WaterErosion::ApplyEvaporation()
{
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			mWaterMap.UpdateHeight(x, z, mWaterMap.ReadHeight(x, z) * 0.985f);
		}
	}
}
//===============================================================================================================================
void WaterErosion::BuildVelocityVectorMap()
{
	ZShadeSandboxTerrain::HeightmapContainer waterMapCopy(mWaterErosionParameters.terrainSize, mWaterErosionParameters.seaLevel);
	waterMapCopy.CopyHeight(mWaterMap);
	
	float currentHeight;
	int currentIndex;
	
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;
			
			currentHeight = mHeightMapInput.ReadHeight(currentIndex);
			
			// Calculate outflow values for individual directions.
			OutflowValues ov;
			
			if (x > 0)
			{
				float heightDifference = currentHeight + waterMapCopy.ReadHeight(currentIndex) - mHeightMapInput.ReadHeight(currentIndex - 1) - waterMapCopy.ReadHeight(currentIndex - 1);
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).left, heightDifference);
				ov.left = max(0, flowValue);
			}
			
			if (x + 1 < mWaterErosionParameters.terrainSize)
			{
				float heightDifference = currentHeight + waterMapCopy.ReadHeight(currentIndex) - mHeightMapInput.ReadHeight(currentIndex + 1) - waterMapCopy.ReadHeight(currentIndex + 1);
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).right, heightDifference);
				ov.right = max(0, flowValue);
			}

			if (z > 0)
			{
				float heightDifference = currentHeight + waterMapCopy.ReadHeight(currentIndex) - mHeightMapInput.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize) - waterMapCopy.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize);
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).top, heightDifference);
				ov.top = max(0, flowValue);
			}

			if (z + 1 < mWaterErosionParameters.terrainSize)
			{
				float heightDifference = currentHeight + waterMapCopy.ReadHeight(currentIndex) - mHeightMapInput.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize) - waterMapCopy.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize);
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).bottom, heightDifference);
				ov.bottom = max(0, flowValue);
			}
			
			UpdateOutflowFluxMap(x, z, ov);
			
			// Scale the outflow values so sum(outflow) < amount of water here
			OutflowValues outflow = ReadOutflowFluxMap(x, z);
			float sumOutflow = outflow.left + outflow.right + outflow.top + outflow.bottom;
			
			if (sumOutflow > mWaterMap.ReadHeight(x, z))
			{
				float factor = min(1, mWaterMap.ReadHeight(currentIndex) * mWaterErosionParameters.pipeLength * mWaterErosionParameters.pipeLength / (sumOutflow * mWaterErosionParameters.deltaT));
				
				outflow.left *= factor;
				outflow.right *= factor;
				outflow.top *= factor;
				outflow.bottom *= factor;
				
				UpdateOutflowFluxMap(x, z, outflow);
				
				outflow = ReadOutflowFluxMap(x, z);
				sumOutflow = outflow.left + outflow.right + outflow.top + outflow.bottom;
			}
		}
	}
	
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;
			
			OutflowValues outflow = ReadOutflowFluxMap(x, z);
			float sumOutflow = outflow.left + outflow.right + outflow.top + outflow.bottom;
			float sumInflow = 0;
			
			if (x > 0)
			{
				sumInflow += mOutflowFluxMap[currentIndex - 1].right;
			}

			if (x + 1 < mWaterErosionParameters.terrainSize)
			{
				sumInflow += mOutflowFluxMap[currentIndex + 1].left;
			}

			if (z > 0)
			{
				sumInflow += mOutflowFluxMap[currentIndex - mWaterErosionParameters.terrainSize].bottom;
			}
			
			if (z + 1 < mWaterErosionParameters.terrainSize)
			{
				sumInflow += mOutflowFluxMap[currentIndex + mWaterErosionParameters.terrainSize].top;
			}
			
			float oldWaterLevel = waterMapCopy.ReadHeight(currentIndex);
			float newWaterLevel = oldWaterLevel + (mWaterErosionParameters.deltaT * (sumInflow - sumOutflow)) / SQR(mWaterErosionParameters.pipeLength);
			
			mWaterMap.UpdateHeight(currentIndex, max(0, newWaterLevel));
			
			float waterAverage = (oldWaterLevel + mWaterMap.ReadHeight(currentIndex)) / 2;
			
			// The velocity field must be updated
			VelocityVector vvField;
			
			// Horizontal (x-axis) velocity field vector component
			if (x == 0)
			{
				// Left border.
				vvField.x = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x + 1].left;
			}
			else if (x + 1 == mWaterErosionParameters.terrainSize)
			{
				// Right border.
				vvField.x = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x - 1].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].left;
			}
			else
			{
				// The rest.
				vvField.x = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x - 1].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].left +
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x + 1].left;
				
				vvField.x /= 2;
			}
			
			// Vertical (y-axis) velocity field vector component
			if (z == 0)
			{
				// Top border.
				vvField.y = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z + 1)) + x + 1].top;
			}
			else if (z + 1 == mWaterErosionParameters.terrainSize)
			{
				// Bottom border.
				vvField.y = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z - 1)) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z)) + x].top;
			}
			else
			{
				// The rest.
				vvField.y = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z - 1)) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].top +
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z + 1)) + x].top;
				
				vvField.y /= 2;
			}
			
			UpdateVelocityVectorMap(x, z, vvField);
		}
	}
	
	waterMapCopy.Clear();
}
//===============================================================================================================================
void WaterErosion::ApplyErosion()
{
	ZShadeSandboxTerrain::HeightmapContainer sedimentToMoveMap(mWaterErosionParameters.terrainSize, mWaterErosionParameters.seaLevel);
	sedimentToMoveMap.Init();
	
	int currentIndex;

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;

			sedimentToMoveMap.UpdateHeight(currentIndex, 0);

			VelocityVector currentVelocityVector = ReadVelocityVectorMap(x, z);

			signed int coordinateOffsetX = currentVelocityVector.x < 0 ? -1 : 0;
			signed int coordinateOffsetY = currentVelocityVector.y < 0 ? -1 : 0;
			signed int baseCoordinateX = x + coordinateOffsetX;
			signed int baseCoordinateY = z + coordinateOffsetY;

			float coordinatePartX = currentVelocityVector.x * mWaterErosionParameters.deltaT - coordinateOffsetX;
			float coordinatePartY = currentVelocityVector.y * mWaterErosionParameters.deltaT - coordinateOffsetY;

			if (coordinatePartX < 0)
				coordinatePartX += 1;

			if (coordinatePartY < 0)
				coordinatePartY += 1;

			//
			// Calculate current height of the target point (it will likely have to be interpolated from 4 surrounding points).
			//

			float heightSum = 0;
			float heightWeightSum = 0;

			// Top Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * (1 - coordinatePartY);
				heightSum += mHeightMapInput.ReadHeight(baseCoordinateX + mWaterErosionParameters.terrainSize * baseCoordinateY) * currentWeight;
				heightWeightSum += currentWeight;
			}

			// Top Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * (1 - coordinatePartY);
				heightSum += mHeightMapInput.ReadHeight(baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * baseCoordinateY) * currentWeight;
				heightWeightSum += currentWeight;
			}

			// Bottom Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * coordinatePartY;
				heightSum += mHeightMapInput.ReadHeight(baseCoordinateX + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)) * currentWeight;
				heightWeightSum += currentWeight;
			}

			// Bottom Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * coordinatePartY;
				heightSum += mHeightMapInput.ReadHeight(baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)) * currentWeight;
				heightWeightSum += currentWeight;
			}

			float targetPointHeightDiff = heightSum / heightWeightSum - mHeightMapInput.ReadHeight(currentIndex);

			float velocityVectorLength = sqrt(SQR(ReadVelocityVectorMap(x, z).x) + SQR(ReadVelocityVectorMap(x, z).y));

			float surfaceTilt;

			if (velocityVectorLength > 0)
			{
				surfaceTilt = atan2(targetPointHeightDiff, velocityVectorLength);
			}
			else
			{
				surfaceTilt = 0;
			}

			surfaceTilt = max(0.2f, surfaceTilt);

			float sedimentCapacity = mWaterErosionParameters.sedimentCapacityConstant * surfaceTilt * velocityVectorLength;

			float sedimentToMove = 0;

			if (sedimentCapacity < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Invalid sediment capacity.");
				return;
			}

			if (mSedimentationMap.ReadHeight(currentIndex) == 0 && sedimentCapacity <= 0)
			{
				sedimentToMoveMap.UpdateHeight(currentIndex, 0);
				continue;
			}

			float originalHeight = mHeightMapInput.ReadHeight(currentIndex);

			if (mSedimentationMap.ReadHeight(currentIndex) < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Negative sediment before update.");
				return;
			}

			float sedimentChange;

			if (sedimentCapacity > mSedimentationMap.ReadHeight(currentIndex))
			{
				// The water can carry more sediment - some sediment will be picked up
				sedimentChange = mWaterErosionParameters.dissolvingConstant * mWaterErosionParameters.deltaT * (sedimentCapacity - mSedimentationMap.ReadHeight(currentIndex));

				if (sedimentChange > 1 || sedimentChange != sedimentChange)
				{
					ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Excessive negative sediment change.");
					return;
				}
				
				float currHeightMapHeight = mHeightMapInput.ReadHeight(currentIndex);
				float currWaterMapHeight = mWaterMap.ReadHeight(currentIndex);
				
				mHeightMapInput.UpdateHeight(currentIndex, currHeightMapHeight - sedimentChange);
				mWaterMap.UpdateHeight(currentIndex, currWaterMapHeight + sedimentChange);
				sedimentToMove = mSedimentationMap.ReadHeight(currentIndex) + sedimentChange;
			}
			else if (sedimentCapacity < mSedimentationMap.ReadHeight(currentIndex))
			{
				// The water is over saturated with sediment - some sediment will be deposited
				sedimentChange = mWaterErosionParameters.depositionConstant * mWaterErosionParameters.deltaT * (mSedimentationMap.ReadHeight(currentIndex) - sedimentCapacity);

				if (sedimentChange > 1 || sedimentChange != sedimentChange)
				{
					ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Excessive positive sediment change.");
					return;
				}

				float currHeightMapHeight = mHeightMapInput.ReadHeight(currentIndex);
				float currWaterMapHeight = mWaterMap.ReadHeight(currentIndex);
				
				mHeightMapInput.UpdateHeight(currentIndex, currHeightMapHeight + sedimentChange);
				mWaterMap.UpdateHeight(currentIndex, currWaterMapHeight - sedimentChange);
				
				if (mWaterMap.ReadHeight(currentIndex) < 0)
				{
					mWaterMap.UpdateHeight(currentIndex, 0);
				}

				sedimentToMove = mSedimentationMap.ReadHeight(currentIndex) - sedimentChange;
			}

			if (sedimentToMove < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Negative sediment to move.");
				return;
			}

			sedimentToMoveMap.UpdateHeight(currentIndex, sedimentToMove);
		}
	}

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;

			VelocityVector currentVelocityVector = ReadVelocityVectorMap(x, z);

			// Move the sediment according to the velocity field map
			if (sedimentToMoveMap.ReadHeight(currentIndex) == 0) continue;

			// we are doing a step backwards in time, so inverse vector has to be used
			currentVelocityVector.x *= -1;
			currentVelocityVector.y *= -1;

			signed int coordinateOffsetX = currentVelocityVector.x < 0 ? -1 : 0;
			signed int coordinateOffsetY = currentVelocityVector.y < 0 ? -1 : 0;
			signed int baseCoordinateX = x + coordinateOffsetX;
			signed int baseCoordinateY = z + coordinateOffsetY;

			float coordinatePartX = currentVelocityVector.x * mWaterErosionParameters.deltaT - coordinateOffsetX;
			float coordinatePartY = currentVelocityVector.y * mWaterErosionParameters.deltaT - coordinateOffsetY;

			if (coordinatePartX > 1 || coordinatePartY > 1 || coordinatePartX < 0 || coordinatePartY < 0)
			{
			}

			//
			// Calculate current height of the target point (it will likely have to be interpolated from 4 surrounding points).
			//

			float sedimentSum = 0;
			float sedimentWeightSum = 0;

			// Top Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * (1 - coordinatePartY);
				sedimentSum += sedimentToMoveMap.ReadHeight(baseCoordinateX + mWaterErosionParameters.terrainSize * baseCoordinateY) * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			// Top Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * (1 - coordinatePartY);
				sedimentSum += sedimentToMoveMap.ReadHeight(baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * baseCoordinateY) * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			// Bottom Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * coordinatePartY;
				sedimentSum += sedimentToMoveMap.ReadHeight(baseCoordinateX + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)) * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			// Bottom Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * coordinatePartY;
				sedimentSum += sedimentToMoveMap.ReadHeight(baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)) * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			if (sedimentSum < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Negative interpolated sediment value.");
				return;
			}

			mSedimentationMap.UpdateHeight(currentIndex, sedimentWeightSum > 0 ? sedimentSum / sedimentWeightSum : 0);
		}
	}
	
	sedimentToMoveMap.Clear();
}
//===============================================================================================================================
void WaterErosion::ApplyThermalWeathering()
{
	float currentIndex;
	
	ZShadeSandboxTerrain::HeightmapContainer heightMapCopy(mWaterErosionParameters.terrainSize, mWaterErosionParameters.seaLevel);
	heightMapCopy.CopyHeight(mHeightMapInput);
	
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;

			float currentHeight = mHeightMapInput.ReadHeight(currentIndex);

			double heightDiffTopLeft = 0;
			double heightDiffTop = 0;
			double heightDiffTopRight = 0;
			double heightDiffRight = 0;
			double heightDiffBottomLeft = 0;
			double heightDiffBottom = 0;
			double heightDiffBottomRight = 0;
			double heightDiffLeft = 0;

			if (z > 0)
			{
				if (x > 0)
				{
					heightDiffTopLeft = currentHeight - heightMapCopy.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize - 1);
				}

				heightDiffTop = currentHeight - heightMapCopy.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize);

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					heightDiffTopRight = currentHeight - heightMapCopy.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize + 1);
				}
			}

			if (x < mWaterErosionParameters.terrainSize - 1)
			{
				heightDiffRight = currentHeight - heightMapCopy.ReadHeight(currentIndex + 1);
			}

			if (z < mWaterErosionParameters.terrainSize - 1)
			{
				if (x > 0)
				{
					heightDiffBottomLeft = currentHeight - heightMapCopy.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize - 1);
				}

				heightDiffBottom = currentHeight - heightMapCopy.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize);

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					heightDiffBottomRight = currentHeight - heightMapCopy.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize + 1);
				}
			}

			if (x > 0)
			{
				heightDiffLeft = currentHeight - heightMapCopy.ReadHeight(currentIndex - 1);
			}

			double maxHeightDiff = 0;
			maxHeightDiff = max(maxHeightDiff, heightDiffTopLeft);
			maxHeightDiff = max(maxHeightDiff, heightDiffTop);
			maxHeightDiff = max(maxHeightDiff, heightDiffTopRight);
			maxHeightDiff = max(maxHeightDiff, heightDiffRight);
			maxHeightDiff = max(maxHeightDiff, heightDiffBottomLeft);
			maxHeightDiff = max(maxHeightDiff, heightDiffBottom);
			maxHeightDiff = max(maxHeightDiff, heightDiffBottomRight);
			maxHeightDiff = max(maxHeightDiff, heightDiffLeft);

			if (maxHeightDiff < mWaterErosionParameters.talusAngle)
			{
				continue;
			}

			double amountToTransport = maxHeightDiff / 2 * mWaterErosionParameters.deltaT * mWaterErosionParameters.thermalPowerMultiplier;

			double totalTransportableAmount = 0;

			if (heightDiffTopLeft >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffTopLeft;

			if (heightDiffTop >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffTop;

			if (heightDiffTopRight >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffTopRight;

			if (heightDiffRight >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffRight;

			if (heightDiffBottomLeft >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffBottomLeft;

			if (heightDiffBottom >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffBottom;

			if (heightDiffBottomRight >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffBottomRight;

			if (heightDiffLeft >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffLeft;

			mHeightMapInput.UpdateHeight(currentIndex, mHeightMapInput.ReadHeight(currentIndex) - amountToTransport);

			if (z > 0)
			{
				if (x > 0)
				{
					if (heightDiffTopLeft >= mWaterErosionParameters.talusAngle)
						mHeightMapInput.UpdateHeight(currentIndex - mWaterErosionParameters.terrainSize - 1, mHeightMapInput.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize - 1) + amountToTransport * heightDiffTopLeft / totalTransportableAmount);
				}

				if (heightDiffTop >= mWaterErosionParameters.talusAngle)
					mHeightMapInput.UpdateHeight(currentIndex - mWaterErosionParameters.terrainSize, mHeightMapInput.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize) + amountToTransport * heightDiffTop / totalTransportableAmount);

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					if (heightDiffTopRight >= mWaterErosionParameters.talusAngle)
						mHeightMapInput.UpdateHeight(currentIndex - mWaterErosionParameters.terrainSize + 1, mHeightMapInput.ReadHeight(currentIndex - mWaterErosionParameters.terrainSize + 1) + amountToTransport * heightDiffTopRight / totalTransportableAmount);
				}
			}

			if (x < mWaterErosionParameters.terrainSize - 1)
			{
				if (heightDiffRight >= mWaterErosionParameters.talusAngle)
					mHeightMapInput.UpdateHeight(currentIndex + 1, mHeightMapInput.ReadHeight(currentIndex + 1) + amountToTransport * heightDiffRight / totalTransportableAmount);
			}

			if (z < mWaterErosionParameters.terrainSize - 1)
			{
				if (x > 0)
				{
					if (heightDiffBottomLeft >= mWaterErosionParameters.talusAngle)
						mHeightMapInput.UpdateHeight(currentIndex + mWaterErosionParameters.terrainSize - 1, mHeightMapInput.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize - 1) + amountToTransport * heightDiffBottomLeft / totalTransportableAmount);
				}

				if (heightDiffBottom >= mWaterErosionParameters.talusAngle)
					mHeightMapInput.UpdateHeight(currentIndex + mWaterErosionParameters.terrainSize, mHeightMapInput.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize) + amountToTransport * heightDiffBottom / totalTransportableAmount);

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					if (heightDiffBottomRight >= mWaterErosionParameters.talusAngle)
						mHeightMapInput.UpdateHeight(currentIndex + mWaterErosionParameters.terrainSize + 1, mHeightMapInput.ReadHeight(currentIndex + mWaterErosionParameters.terrainSize + 1) + amountToTransport * heightDiffBottomRight / totalTransportableAmount);
				}
			}

			if (x > 0)
			{
				if (heightDiffLeft >= mWaterErosionParameters.talusAngle)
					mHeightMapInput.UpdateHeight(currentIndex - 1, mHeightMapInput.ReadHeight(currentIndex - 1) + amountToTransport * heightDiffLeft / totalTransportableAmount);
			}
		}
	}

	heightMapCopy.Clear();
}
//===============================================================================================================================
float WaterErosion::FlowValue(float direction, float heightDifference)
{
	return direction + mWaterErosionParameters.deltaT * mWaterErosionParameters.pipeCrossectionArea * (mWaterErosionParameters.graviationalAcceleration * heightDifference) / mWaterErosionParameters.pipeLength;
}
//===============================================================================================================================
ZShadeSandboxTerrain::OutflowValues WaterErosion::ReadOutflowFluxMap(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mOutflowFluxMap[(z * mWaterErosionParameters.terrainSize) + x];
	}
	
	OutflowValues ov;
	ov.left = ov.right = ov.top = ov.bottom = 0;
	return ov;
}
//===============================================================================================================================
ZShadeSandboxTerrain::VelocityVector WaterErosion::ReadVelocityVectorMap(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mVelocityVectorMap[(z * mWaterErosionParameters.terrainSize) + x];
	}
	
	VelocityVector vv;
	vv.x = vv.y = 0;
	return vv;
}
//===============================================================================================================================
void WaterErosion::UpdateOutflowFluxMap(int x, int z, ZShadeSandboxTerrain::OutflowValues value)
{
	mOutflowFluxMap[(z * mWaterErosionParameters.terrainSize) + x] = value;
}
//===============================================================================================================================
void WaterErosion::UpdateVelocityVectorMap(int x, int z, ZShadeSandboxTerrain::VelocityVector value)
{
	mVelocityVectorMap[(z * mWaterErosionParameters.terrainSize) + x] = value;
}
//===============================================================================================================================