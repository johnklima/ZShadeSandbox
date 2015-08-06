#include "BSPRenderer.h"
#include "BSPArrayData.h"
using ZShadeSandboxBSP::BSPRenderer;
//==================================================================================================================================
//==================================================================================================================================
BSPRenderer::BSPRenderer(D3D* d3d)
{
	bDontFrustumReject = false;
	
	mBSPMesh = new ZShadeSandboxBSP::BSPMesh(d3d);
}
//==================================================================================================================================
bool BSPRenderer::LeafInFrustum(long leaf, XMFLOAT4 frustumPlanes[6])
{
	ZShadeSandboxMath::XMMath3 bMax = BSPArrayData::Instance()->leafArray[leaf].boundingBox.vMax;
	ZShadeSandboxMath::XMMath3 bMin = BSPArrayData::Instance()->leafArray[leaf].boundingBox.vMin;
	ZShadeSandboxMath::XMMath3 nearPoint;
	
	for (int i = 0; i < 6; i++)
	{
		ZShadeSandboxMath::XMMath3 normal = ZShadeSandboxMath::XMMath3(frustumPlanes[i].x, frustumPlanes[i].y, frustumPlanes[i].z);
		
		if (normal.x > 0.0f)
		{
			if (normal.y > 0.0f)
			{
				if (normal.z > 0.0f)
				{
					nearPoint.x = bMin.x;
					nearPoint.y = bMin.y;
					nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMin.x;
					nearPoint.y = bMin.y;
					nearPoint.z = bMax.z;
				}
			}
			else
			{
				if (normal.z > 0.0f)
				{
					nearPoint.x = bMin.x;
					nearPoint.y = bMax.y;
					nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMin.x;
					nearPoint.y = bMax.y;
					nearPoint.z = bMax.z;
				}
			}
		}
		else
		{
			if (normal.y > 0.0f)
			{
				if (normal.z > 0.0f)
				{
					nearPoint.x = bMax.x;
					nearPoint.y = bMin.y;
					nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMax.x;
					nearPoint.y = bMin.y;
					nearPoint.z = bMax.z;
				}
			}
			else
			{
				if (normal.z > 0.0f)
				{
					nearPoint.x = bMax.x;
					nearPoint.y = bMax.y;
					nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMax.x;
					nearPoint.y = bMax.y;
					nearPoint.z = bMax.z;
				}
			}
		}
	
		// near extreme point is outside, and thus the AABB is Totally outside the polyhedron
		
		if ((normal.Dot(nearPoint) + frustumPlanes[i].w) > 0)
		{
			return false;
		}
	}
	
	return true;
}
//==================================================================================================================================
bool BSPRenderer::LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end, long node)
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	float temp;
	ZShadeSandboxMath::XMMath3 intersection;
	ZShadeSandboxBSP::BSPNode* currentNode = arrayData->nodeArray[node];
	ZShadeSandboxBSP::BSPPlane* plane = arrayData->planeArray[currentNode->planeID];
	
	ZShadeSandboxBSP::EPointPosition::Type pointA = ZShadeSandboxBSP::BSP::ClassifyPoint(start, plane);
	ZShadeSandboxBSP::EPointPosition::Type pointB = ZShadeSandboxBSP::BSP::ClassifyPoint(end, plane);
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane && pointB == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
	{
		if (currentNode->isLeaf == 0)
		{
			// Not a leaf
			return LineOfSight(start, end, currentNode->frontID);
		}
		else
		{
			// Front leafs are empty
			return true;
		}
	}
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eFront && pointB == ZShadeSandboxBSP::EPointPosition::Type::eBack)
	{
		if (currentNode->backID == -1)
		{
			return false;
		}
		
		ZShadeSandboxMath::ZMath::GetIntersect(start, end, plane->pointOnPlane, plane->normal, intersection, temp);
		
		if (currentNode->isLeaf == 0)
		{
			// Not a leaf
			return LineOfSight(start, intersection, currentNode->frontID) && LineOfSight(end, intersection, currentNode->backID);
		}
		else
		{
			// Front leafs are empty
			return true && LineOfSight(end, intersection, currentNode->backID);
		}
	}
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eBack && pointB == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		if (currentNode->backID == -1)
		{
			return false;
		}
		
		ZShadeSandboxMath::ZMath::GetIntersect(start, end, plane->pointOnPlane, plane->normal, intersection, temp);
		
		if (currentNode->isLeaf == 0)
		{
			// Not a leaf
			return LineOfSight(end, intersection, currentNode->frontID) && LineOfSight(start, intersection, currentNode->backID);
		}
		else
		{
			// Front leafs are empty
			return true && LineOfSight(start, intersection, currentNode->backID);
		}
	}
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eFront && pointB == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		if (currentNode->isLeaf == 0)
		{
			return LineOfSight(start, end, currentNode->frontID);
		}
		else
		{
			return true;
		}
	}
	else
	{
		if (currentNode->backID == -1)
		{
			return false;
		}
		else
		{
			return LineOfSight(start, end, currentNode->backID);
		}
	}
	
	return true;
}
//==================================================================================================================================
void BSPRenderer::RenderBSP(ZShadeSandboxMesh::MeshRenderParameters mrp, XMFLOAT4 frustumPlanes[6])
{
	// Cannot do anything if there is no camera
	if (mrp.camera == 0) return;
	
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	int node = 0;
	int leaf = 0;
	
	// Loop until we find the leaf that contains the camera
	while (true)
	{
		switch (ZShadeSandboxBSP::BSP::ClassifyPoint(ZShadeSandboxMath::XMMath3(mrp.camera->Position()), arrayData->planeArray[arrayData->nodeArray[node].planeID]))
		{
			case ZShadeSandboxBSP::EPointPosition::Type::eFront:
			{
				if (arrayData->nodeArray[node].isLeaf != 0)
				{
					leaf = arrayData->nodeArray[node].frontID;
					
					RenderBSP(leaf, mrp, frustumPlanes);
					
					return;
				}
				else
				{
					node = arrayData->nodeArray[node].frontID;
				}
			}
			break;
			case ZShadeSandboxBSP::EPointPosition::Type::eBack:
			{
				if (arrayData->nodeArray[node].backID == -1)
				{
					// Solid Space
					return;
				}
				else
				{
					node = arrayData->nodeArray[node].backID;
				}
			}
			break;
			case ZShadeSandboxBSP::EPointPosition::Type::eOnPlane:
			{
				if (arrayData->nodeArray[node].isLeaf != 0)
				{
					leaf = arrayData->nodeArray[node].frontID;
					
					RenderBSP(leaf, mrp, frustumPlanes);
					
					return;
				}
				else
				{
					node = arrayData->nodeArray[node].frontID;
				}
			}
			break;
			default: break;
		}
	}
}
//==================================================================================================================================
void BSPRenderer::RenderBSP(long leaf, ZShadeSandboxMesh::MeshRenderParameters mrp, XMFLOAT4 frustumPlanes[6])
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	vector<ZShadeSandboxBSP::BSPPolygon*> polygonRenderList;
	ZShadeSandboxBSP::BSPPolygon* currentPolygon;
	int i;
	long PVSOffset = arrayData->leafArray[leaf].pvsIndex;
	BYTE* pvsPointer = arrayData->pvsData;
	pvsPointer += PVSOffset;
	long currentLeaf = 0;
	
	while (currentLeaf < arrayData->lNumberOfLeaves)
	{
		if (*pvsPointer != 0)
		{
			// Fond out which of the 8 bits are visible
			// One byte holds 8 leafs visibility bits
			for (i = 0; i < 8; i++)
			{
				BYTE mask = 1 << i;
				BYTE pvs = *pvsPointer;
				
				// AND the mask byte with the pvs byte we can see
				// if that bit is set to one or zero
				if (pvs & mask)
				{
					// This leaf has its bit set so it is potentially visible
					if (LeafInFrustum(currentLeaf, frustumPlanes) || bDontFrustumReject)
					{
						unsigned long start = arrayData->leafArray[currentLeaf].startPolygon;
						unsigned long end = arrayData->leafArray[currentLeaf].endPolygon;
						unsigned long count = 0;
						
						for (count = start; count < end; count++)
						{
							currentPolygon = arrayData->polygonArray[count];
							polygonRenderList.push_back(currentPolygon);
						}
					}
				}
				
				currentLeaf++;
			}
			
			pvsPointer++;
		}
		else
		{
			pvsPointer++;
			
			BYTE runLength = *pvsPointer;
			
			pvsPointer++;
			
			currentLeaf += runLength * 8;
		}
	}
	
	for (i = 0; i < polygonRenderList.size(); i++)
	{
		polygonRenderList[i]->Mesh()->Render(mrp);
	}
}
//==================================================================================================================================