#include "BSPPVSGenerator.h"
#include "BSPArrayData.h"
#include "BSPPortalGenerator.h"
#include "BSP.h"
using ZShadeSandboxBSP::BSPPVSGenerator;
//==================================================================================================================================
//==================================================================================================================================
BSPPVSGenerator::BSPPVSGenerator()
{
}
//==================================================================================================================================
long BSPPVSGenerator::CalculatePVS()
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	BYTE* leafPVS = new BYTE[arrayData->bytesPerSet];
	long PVSMasterWritePointer = 0;
	
	for (long leaf = 0; leaf < arrayData->lNumberOfLeaves; leaf++)
	{
		arrayData->leafArray[leaf].PVSIndex = PVSMasterWritePointer;
		
		ZeroMemory(leafPVS, arrayData->bytesPerSet);
		
		SetPVSBit(leafPVS, leaf);
		
		for (long sourcePortalIndex = 0; sourcePortalIndex < arrayData->leafArray[leaf].numberOfPortals; sourcePortalIndex++)
		{
			ZShadeSandboxBSP::BSPPortal* sourcePortal = arrayData->portalArray[arrayData->leafArray[leaf].portalIndexList[sourcePortalIndex]];
			
			long targetLeaf = sourcePortal->LeafOwnerArray(0);
			
			if (targetLeaf == leaf)
			{
				targetLeaf = sourcePortal->LeafOwnerArray(1);
			}
			
			SetPVSBit(leafPVS, targetLeaf);
			
			for (long destPortalIndex = 0; destPortalIndex < arrayData->leafArray[targetLeaf].numberOfPortals; destPortalIndex++)
			{
				ZShadeSandboxBSP::BSPPortal* targetPortal = arrayData->portalArray[arrayData->leafArray[targetLeaf].portalIndexList[destPortalIndex]];
				
				ZShadeSandboxBSP::EPointPosition::Type targetOnSourcePlane = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&GetPortalPlane(sourcePortal), targetPortal);
				
				// Make sure the destination portal is not the source portal and not on the same plane as the source portal
				if (sourcePortal != targetPortal && targetOnSourcePlane != ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
				{
					CalculatePVS(leaf, sourcePortal, targetPortal, targetLeaf, leafPVS);
				}
			}
		}
		
		// Compress the buffer and copy it to pvs data array and return how many bytes were written after compression
		PVSMasterWritePointer += CompressLeafSet(leafPVS, PVSMasterWritePointer);
	}
	
	delete [] leafPVS;
	
	return PVSMasterWritePointer;
}
//==================================================================================================================================
void BSPPVSGenerator::CalculatePVS(long sourceLeaf, ZShadeSandboxBSP::BSPPortal* srcPortal, ZShadeSandboxBSP::BSPPortal* targetPortal, long targetLeaf, BYTE* leafPVS)
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	// Get the leaf on the opposing side of the target portal
	long generatorLeaf = targetPortal->LeafOwnerArray(0);
	
	if (generatorLeaf == targetLeaf)
	{
		generatorLeaf = targetPortal->LeafOwnerArray(1);
	}
	
	SetPVSBit(leafPVS, generatorLeaf);
	
	// Find which side of the source portal the source leaf is on and which side of the target portal the target leaf is on
	ZShadeSandboxMath::XMMath3 sourceLeafCenter = (arrayData->leafArray[sourceLeaf].boundingBox.vMax + arrayData->leafArray[sourceLeaf].boundingBox.vMin) * 0.5f;
	ZShadeSandboxMath::XMMath3 targetLeafCenter = (arrayData->leafArray[targetLeaf].boundingBox.vMax + arrayData->leafArray[targetLeaf].boundingBox.vMin) * 0.5f;
	
	int sourceLeafLocation = BSP::ClassifyPoint(sourceLeafCenter, GetPortalPlane(srcPortal));
	int targetLeafLocation = BSP::ClassifyPoint(targetLeafCenter, GetPortalPlane(targetPortal));
	
	ZShadeSandboxBSP::BSPPortal* generatorPortal = 0;
	ZShadeSandboxBSP::BSPPortal* sourcePortal = 0;
	
	for (long generaterPortalIndex = 0; generaterPortalIndex < arrayData->leafArray[generatorLeaf].numberOfPortals; generaterPortalIndex++)
	{
		if (arrayData->portalArray[arrayData->leafArray[generatorLeaf].portalIndexList[generaterPortalIndex]] == targetPortal)
		{
			delete generatorPortal;
			delete sourcePortal;
			continue;
		}
		
		sourcePortal = new ZShadeSandboxBSP::BSPPortal();
		*sourcePortal = *srcPortal;
		
		generatorPortal = new ZShadeSandboxBSP::BSPPortal();
		
		*generatorPortal = *arrayData->portalArray[arrayData->leafArray[generatorLeaf].portalIndexList[generaterPortalIndex]];
		
		ZShadeSandboxBSP::EPointPosition::Type generatorLocation = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&GetPortalPlane(targetPortal), generatorPortal);
		
		if (generatorLocation == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane || generatorLocation == sourceLeafLocation)
		{
			delete generatorPortal;
			delete sourcePortal;
			continue;
		}
		
		generatorLocation = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&GetPortalPlane(targetPortal), generatorPortal);
		
		if (generatorLocation == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane || generatorLocation == targetLeafLocation)
		{
			delete generatorPortal;
			delete sourcePortal;
			continue;
		}
		
		// Test the generator portal against the anti penumbra planes to see if it can be seen
		generatorPortal = ClipToAntiPenumbra(sourcePortal, targetPortal, generatorPortal);
		
		if (generatorPortal == 0)
		{
			if (sourcePortal)
			{
				delete sourcePortal;
				continue;
			}
			
			continue;
		}
		
		sourcePortal = ClipToAntiPenumbra(generatorPortal, targetPortal, sourcePortal);
		
		if (sourcePortal == 0)
		{
			if (generatorPortal)
			{
				delete generatorPortal;
				continue;
			}
			
			continue;
		}
		
		CalculatePVS(sourceLeaf, sourcePortal, generatorPortal, generatorLeaf, leafPVS);
		
		delete generatorPortal;
		delete sourcePortal;
	}
}
//==================================================================================================================================
struct ClipPlanes
{
	int numberOfPlanes;
	ZShadeSandboxBSP::BSPPlane* planes;
};
ZShadeSandboxBSP::BSPPortal* BSPPVSGenerator::ClipToAntiPenumbra(ZShadeSandboxBSP::BSPPortal* srcPortal, ZShadeSandboxBSP::BSPPortal* targetPortal, ZShadeSandboxBSP::BSPPortal* generatorPortal)
{
	ZShadeSandboxMath::XMMath3 edgeVector1, edgeVector2, normal;
	ZShadeSandboxBSP::EPointPosition::Type portalLocation;
	ZShadeSandboxBSP::BSPPortal *frontSplit, *backSplit, *tempSource, *tempTarget;
	ClipPlanes clipPlanes;
	ZShadeSandboxBSP::BSPPlane tempPlane;
	int nextVertex = 0;
	
	clipPlanes.numberOfPlanes = 0;
	clipPlanes.planes = new ZShadeSandboxBSP::BSPPlane[srcPortal->GetVertexCount() * targetPortal->GetVertexCount() * 2];
	
	//
	// Create the planes
	//
	
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			tempSource = srcPortal;
			tempTarget = targetPortal;
		}
		else
		{
			tempSource = targetPortal;
			tempTarget = srcPortal;
		}
		
		for (int sourceVertex = 0; sourceVertex < tempSource->GetVertexCount(); sourceVertex++)
		{
			portalLocation = ZShadeSandboxBSP::BSP::ClassifyPoint(ZShadeSandboxMath::XMMath3(tempSource->GetVertexPosition(sourceVertex)), GetPortalPlane(tempTarget));
			
			// Since it is 'L' shaped skip it
			if (portalLocation == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
			{
				continue;
			}
			
			for (int targetVertex = 0; targetVertex < tempTarget->GetVertexCount(); targetVertex++)
			{
				if (targetVertex == tempTarget->GetVertexCount() - 1)
				{
					nextVertex = 0;
				}
				else
				{
					nextVertex = targetVertex + 1;
				}
				
				edgeVector1 = ZShadeSandboxMath::XMMath3(tempSource->GetVertexPosition(sourceVertex)) - ZShadeSandboxMath::XMMath3(tempTarget->GetVertexPosition(targetVertex));
				edgeVector2 = ZShadeSandboxMath::XMMath3(tempSource->GetVertexPosition(nextVertex)) - ZShadeSandboxMath::XMMath3(tempTarget->GetVertexPosition(targetVertex));
				normal = edgeVector1.Cross(edgeVector2);
				normal.Normalize();
				
				tempPlane.pointOnPlane = ZShadeSandboxMath::XMMath3(tempSource->GetVertexPosition(sourceVertex));
				tempPlane.normal = normal;
				
				// tempPlane is only an anti-penumbra plane if it divides the source portal and target portal so that each
				// portal lays on opposing sides of the plane.
				
				ZShadeSandboxBSP::EPointPosition::Type sourceLocation = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&tempPlane, tempSource);
				ZShadeSandboxBSP::EPointPosition::Type targetLocation = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&tempPlane, tempTarget);
		
				if (sourceLocation == ZShadeSandboxBSP::EPointPosition::Type::eFront)
				{
					if (targetLocation == ZShadeSandboxBSP::EPointPosition::Type::eBack)
					{
						clipPlanes.planes[clipPlanes.numberOfPlanes] = tempPlane;
						clipPlanes.numberOfPlanes++;
					}
				}
				else if (sourceLocation == ZShadeSandboxBSP::EPointPosition::Type::eBack)
				{
					if (targetLocation == ZShadeSandboxBSP::EPointPosition::Type::eFront)
					{
						clipPlanes.planes[clipPlanes.numberOfPlanes] = tempPlane;
						clipPlanes.numberOfPlanes++;
					}
				}
			}
		}
	}
	
	//
	// Clipping the generator portal to the planes
	//
	
	for (int i = 0; i < clipPlanes.numberOfPlanes; i++)
	{
		portalLocation = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&clipPlanes.planes[i], generatorPortal);
		ZShadeSandboxBSP::EPointPosition::Type sourceLocation = ZShadeSandboxBSP::BSPPortalGenerator::ClassifyPortal(&clipPlanes.planes[i], srcPortal);
		
		// Lies on the outside of the anti penumbra so lose it
		if (portalLocation == sourceLocation || portalLocation == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
		{
			delete generatorPortal;
			delete clipPlanes.planes;
			return 0;
		}
		
		// Lies inside the anti penumbra so keep it
		if ((portalLocation == ZShadeSandboxBSP::EPointPosition::Type::eBack && sourceLocation == ZShadeSandboxBSP::EPointPosition::Type::eFront)
		||	(portalLocation == ZShadeSandboxBSP::EPointPosition::Type::eFront && sourceLocation == ZShadeSandboxBSP::EPointPosition::Type::eBack))
		{
			continue;
		}
		
		// Lies on the anti penumbra so clip it
		if (portalLocation == ZShadeSandboxBSP::EPointPosition::Type::eSpanning)
		{
			frontSplit = new ZShadeSandboxBSP::BSPPortal();
			backSplit = new ZShadeSandboxBSP::BSPPortal();
			
			ZeroMemory(frontSplit, sizeof(ZShadeSandboxBSP::BSPPortal));
			ZeroMemory(backSplit, sizeof(ZShadeSandboxBSP::BSPPortal));
			
			ZShadeSandboxBSP::BSPPortalGenerator::SplitPortal(generatorPortal, &clipPlanes.planes[i], frontSplit, backSplit);
			
			if (sourceLocation == ZShadeSandboxBSP::EPointPosition::Type::eFront)
			{
				delete frontSplit;
				delete generatorPortal;
				generatorPortal = backSplit;
			}
			else if (sourceLocation == ZShadeSandboxBSP::EPointPosition::Type::eBack)
			{
				delete backSplit;
				delete generatorPortal;
				generatorPortal = frontSplit;
			}
		}
	}
	
	delete clipPlanes.planes;
	
	return generatorPortal;
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPlane BSPPVSGenerator::GetPortalPlane(ZShadeSandboxBSP::BSPPortal* portal)
{
	ZShadeSandboxBSP::BSPPlane plane;
	plane.pointOnPlane = portal->GetVertexPosition(0);
	plane.normal = portal->Normal();
	return plane;
}
//==================================================================================================================================
void BSPPVSGenerator::SetPVSBit(BYTE*& visArray, long destLeaf)
{
	// To find the correct byte set divide by 8 (>> 3)
	long byteToSet = destLeaf >> 3;
	// Then multiply by 8 (<< 3)
	BYTE bitToSet = (BYTE)(destLeaf - (byteToSet << 3);
	// OR the bit so that if it is not set then it gets set
	visArray[byteToSet] |= 1 << bitToSet;
}
//==================================================================================================================================
long BSPPVSGenerator::CompressLeafSet(BYTE* visArray, long writePos)
{
	int j;
	int rep;
	BYTE* dest = &BSPArrayData::Instance()->pvsData[writePos];
	BYTE* dest_p = dest;
	
	for (j = 0; j < BSPArrayData::Instance()->bytesPerSet; j++)
	{
		*dest_p++ = visArray[j];
		
		if (visArray[j])
		{
			continue;
		}
		
		rep = 1;
		
		for (j++; j < BSPArrayData::Instance()->bytesPerSet; j++)
		{
			if (visArray[j] || rep == 255)
			{
				break;
			}
			else
			{
				rep++;
			}
		}
		
		*dest_p++ = rep;
			
		j--;
	}
	
	return dest_p - dest;
}
//==================================================================================================================================