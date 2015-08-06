#include "BSPPortalGenerator.h"
#include "BSPArrayData.h"
using ZShadeSandboxBSP::BSPPortalGenerator;
//==================================================================================================================================
//==================================================================================================================================
BSPPortalGenerator::BSPPortalGenerator()
{
}
//==================================================================================================================================
void BSPPortalGenerator::Build()
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	long stackPointer = 0;
	NodeStack* nodeStack = new NodeStack[arrayData->lNumberOfNodes + 1];
	int portalIndex;
	nodeStack[stackPointer].node = 0;
	nodeStack[stackPointer].jumpBackPoint = 0;
	
	// Using goto statements here, the reason is that while recursing the tree this function call 'ClipPortal' which also recurses the tree.
	// Recursing the tree while recursing the tree is a sure way to get a stack overflow on even a moderately sized level which is
	// why the goto statements are used in this function which will help to stop a stack overflow on a large level.

START:

	ZShadeSandboxBSP::BSPPortal* initialPortal = CalculateInitialPortal(nodeStack[stackPointer].node);
	ZShadeSandboxBSP::BSPPortal* portalList = ClipPortal(0, initialPortal);
	ZShadeSandboxBSP::BSPPortal* iter = portalList;

	while (iter != 0)
	{
		if (iter->LeafCount() != 2)
		{
			ZShadeSandboxBSP::BSPPortal* temp = iter->NextPortal();
			RemovePortalFromList(iter);
			iter = temp;
		}
		else
		{
			// If there is a portal already in the portal array this is larger than the current one we are testing
			// then the function 'CheckDuplicatePortal' returns true
			if (CheckDuplicatePortal(iter, portalIndex))
			{
				ZShadeSandboxBSP::BSPPortal* temp = iter->NextPortal();
				RemovePortalFromList(iter);
				iter = temp;
			}
			else
			{
				arrayData->portalArray[portalIndex] = iter;
				
				if (portalIndex == arrayData->lNumberOfPortals)
				{
					for (int a = 0; a < iter->LeafCount(); a++)
					{
						long index = iter->LeafOwnerArray(a);

						arrayData->leafArray[index].portalIndexList[arrayData->leafArray[index].numberOfPortals] = arrayData->lNumberOfPortals;
						arrayData->leafArray[index].numberOfPortals++;
					}

					arrayData->IncreasePortalCount();
				}

				iter = iter->NextPortal();
			}
		}
	}

	if (arrayData->nodeArray[nodeStack[stackPointer].node].isLeaf == 0)
	{
		nodeStack[stackPointer + 1].node = arrayData->nodeArray[nodeStack[stackPointer].node].frontID;
		nodeStack[stackPointer + 1].jumpBackPoint = 1;
		stackPointer++;
		goto START;
	}

BACK:

	if (arrayData->nodeArray[nodeStack[stackPointer].node].backID != -1)
	{
		nodeStack[stackPointer + 1].node = arrayData->nodeArray[nodeStack[stackPointer].node].backID;
		nodeStack[stackPointer + 1].jumpBackPoint = 2;
		stackPointer++;
		goto START;
	}

END:

	// This is like returning from a function
	stackPointer--;

	if (stackPointer > -1)
	{
		if (nodeStack[stackPointer + 1].jumpBackPoint == 1)
		{
			goto BACK;
		}
		else if (nodeStack[stackPointer + 1].jumpBackPoint == 2)
		{
			goto END;
		}
	}

	delete[] nodeStack;
}
//==================================================================================================================================
void BSPPortalGenerator::RemovePortalFromList(ZShadeSandboxBSP::BSPPortal* portal)
{
	ZShadeSandboxBSP::BSPPortal* temp = portal;
	ZShadeSandboxBSP::BSPPortal *prev, *next;

	if (portal->PrevPortal() != 0)
	{
		prev = portal->PrevPortal();

		if (portal->NextPortal() != 0)
		{
			prev->NextPortal() = portal->NextPortal();
		}
		else
		{
			prev->NextPortal() = 0;
		}
	}
	
	if (portal->NextPortal() != 0)
	{
		next = portal->NextPortal();

		if (portal->PrevPortal() != 0)
		{
			next->PrevPortal() = portal->PrevPortal();
		}
		else
		{
			next->PrevPortal() = 0;
		}
	}

	DeletePortal(temp);
}
//==================================================================================================================================
bool BSPPortalGenerator::CheckDuplicatePortal(ZShadeSandboxBSP::BSPPortal* portal, int& index)
{
	long checkPortalLeaf1 = portal->LeafOwnerArray(0);
	long checkPortalLeaf2 = portal->LeafOwnerArray(1);
	long PALeaf1 = 0;
	long PALeaf2 = 0;

	for (long i = 0; i < lNumberOfPortals; i++)
	{
		PALeaf1 = mPortalArray[i]->LeafOwnerArray(0);
		PALeaf2 = mPortalArray[i]->LeafOwnerArray(1);

		if ((checkPortalLeaf1 == PALeaf1 && checkPortalLeaf2 == PALeaf2) || (checkPortalLeaf1 == PALeaf2 && checkPortalLeaf2 == PALeaf1))
		{
			ZShadeSandboxMath::XMMath3 max1, min1, max2, min2;

			GetPolygonBounds(portal, min1, max1);
			GetPolygonBounds(portal, min2, max2);

			float newSize = (max1 - min1).Length();
			float oldSize = (max2 - min2).Length();

			if (ZShadeSandboxMath::ZMath::Fabs(newSize) > ZShadeSandboxMath::ZMath::Fabs(oldSize))
			{
				ZShadeSandboxBSP::BSPPortal* temp = mPortalArray[i];

				DeletePortal(temp);

				index = i;

				return false;
			}
			else
			{
				// Portal already in the array
				return true;
			}
		}
	}

	index = lNumberOfPortals;

	// Not found in the array
	return false;
}
//==================================================================================================================================
void BSPPortalGenerator::GetPolygonBounds(ZShadeSandboxBSP::BSPPortal* portal, ZShadeSandboxMath::XMMath3& bmin, ZShadeSandboxMath::XMMath3& bmax)
{
	// tests each vertex in the polygon and returns a Minimum Point and a Maximum Point that bounds the Polygon
	XMFLOAT3 minVertex(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; portal->GetVertexCount(); i++)
	{
		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, portal->GetVertex(i).position.x);    // Find smallest x value in model
		minVertex.y = min(minVertex.y, portal->GetVertex(i).position.y);    // Find smallest y value in model
		minVertex.z = min(minVertex.z, portal->GetVertex(i).position.z);    // Find smallest z value in model

		//Get the largest vertex 
		maxVertex.x = max(maxVertex.x, portal->GetVertex(i).position.x);    // Find largest x value in model
		maxVertex.y = max(maxVertex.y, portal->GetVertex(i).position.y);    // Find largest y value in model
		maxVertex.z = max(maxVertex.z, portal->GetVertex(i).position.z);    // Find largest z value in model
	}

	bmin = minVertex;
	bmax = maxVertex;
}
//==================================================================================================================================
ZShadeSandboxBSP::EPointPosition::Type BSPPortalGenerator::ClassifyPortal(ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPortal* portal)
{
	int inFront = 0, behind = 0, onPlane = 0;
	
	ZShadeSandboxMath::XMMath3 vec1 = plane->pointOnPlane;

	for (int a = 0; a < portal->GetVertexCount(); a++)
	{
		ZShadeSandboxMath::XMMath3 vec2 = portal->GetVertexPosition(a);

		ZShadeSandboxMath::XMMath3 dir = vec1 - vec2;

		float dot = dir.Dot(plane->normal);

		if (dot > 0.001f)
		{
			behind++;
		}
		else if (dot < -0.001f)
		{
			inFront++;
		}
		else
		{
			onPlane++;
			inFront++;
			behind++;
		}
	}

	if (onPlane == portal->GetVertexCount())
	{
		return ZShadeSandboxBSP::EPointPosition::Type::eOnPlane;
	}

	if (behind == portal->GetVertexCount())
	{
		return ZShadeSandboxBSP::EPointPosition::Type::eBack;
	}

	if (inFront == portal->GetVertexCount())
	{
		return ZShadeSandboxBSP::EPointPosition::Type::eFront;
	}

	return ZShadeSandboxBSP::EPointPosition::Type::eSpanning;
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPortal* BSPPortalGenerator::CalculateInitialPortal(long node)
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	ZShadeSandboxMath::XMMath3 maxP, minP, boxCenter, boxCenterPoint;

	maxP = ZShadeSandboxMath::XMMath3(arrayData->nodeArray[node].boundingBox.vMax);
	maxP = ZShadeSandboxMath::XMMath3(arrayData->nodeArray[node].boundingBox.vMin);

	ZShadeSandboxMath::XMMath3 planeNormal = arrayData->planeArray[arrayData->nodeArray[node].planeID].normal;

	boxCenter = (maxP + minP) / 2;

	float distanceToPlane = (arrayData->planeArray[arrayData->nodeArray[node].planeID].pointOnPlane - boxCenter).Dot(planeNormal);

	boxCenterPoint = boxCenter + (planeNormal * distanceToPlane);

	ZShadeSandboxMath::XMMath3 A(0, 0, 0);

	if (ZShadeSandboxMath::ZMath::Fabs(planeNormal.y) > ZShadeSandboxMath::ZMath::Fabs(planeNormal.z))
	{
		if (ZShadeSandboxMath::ZMath::Fabs(planeNormal.z) < ZShadeSandboxMath::ZMath::Fabs(planeNormal.x))
		{
			A.z = 1;
		}
		else
		{
			A.x = 1;
		}
	}
	else
	{
		if (ZShadeSandboxMath::ZMath::Fabs(planeNormal.y) <= ZShadeSandboxMath::ZMath::Fabs(planeNormal.x))
		{
			A.y = 1;
		}
		else
		{
			A.x = 1;
		}
	}

	ZShadeSandboxMath::XMMath3 U, V;

	U = A.Cross(planeNormal);
	U.Normalize();

	V = U.Cross(planeNormal);
	V.Normalize();

	ZShadeSandboxMath::XMMath3 boxHalfLength(maxP - boxCenter);

	float length = boxHalfLength.Length();

	U = U * length;
	V = V * length;

	//
	// Create four vertices for the portal
	//

	ZShadeSandboxMath::XMMath3 P[4];

	// Bottom Right
	P[0] = boxCenterPoint + U - V;

	// Top Right
	P[1] = boxCenterPoint + U + V;

	// Top Left
	P[2] = boxCenterPoint - U + V;

	// Bottom Left
	P[3] = boxCenterPoint - U - V;

	//
	// Now fill the portal structure with vertices and indices
	//

	ZShadeSandboxBSP::BSPPortal* portal = new ZShadeSandboxBSP::BSPPortal();

	ZeroMemory(portal, sizeof(ZShadeSandboxBSP::BSPPortal));

	vector<ZShadeSandboxMesh::VertexTex> vertices;
	vector<UINT> indices;

	for (int i = 0; i < 4; i++)
	{
		vertices[i].position.x = P[i].x;
		vertices[i].position.y = P[i].y;
		vertices[i].position.z = P[i].z;
		vertices[i].texture.x = 0;
		vertices[i].texture.y = 0;
	}

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;

	portal->Create(vertices);
	portal->CreateIndices(indices);

	portal->NextPortal() = 0;
	portal->PrevPortal() = 0;

	portal->LeafCount() = 0;

	portal->Normal() = planeNormal;

	return portal;
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPortal* BSPPortalGenerator::ClipPortal(long node, ZShadeSandboxBSP::BSPPortal* portal)
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	ZShadeSandboxBSP::BSPPortal *portalList = 0, *frontPortalList = 0, *backPortalList, *iter = 0, *frontSplit = 0, *backSplit = 0;

	switch (ClassifyPortal(&arrayData->planeArray[arrayData->nodeArray[node].planeID], portal))
	{
		case ZShadeSandboxBSP::EPointPosition::Type::eFront:
		{
			if (arrayData->nodeArray[node].isLeaf == 0)
			{
				portalList = ClipPortal(arrayData->nodeArray[node].frontID, portal);
				return portalList;
			}
			else
			{
				portal->LeafOwnerArray(portal->LeafCount()) = arrayData->nodeArray[node].frontID;
				portal->IncLeafCount();
				portal->NextPortal() = 0;
				portal->PrevPortal() = 0;
				return portal;
			}
		}
		break;
		case ZShadeSandboxBSP::EPointPosition::Type::eBack:
		{
			if (arrayData->nodeArray[node].backID != -1)
			{
				portalList = ClipPortal(arrayData->nodeArray[node].backID, portal);
				return portalList;
			}
			else
			{
				// Solid Space
				DeletePortal(portal);
				return 0;
			}
		}
		break;
		case ZShadeSandboxBSP::EPointPosition::Type::eSpanning:
		{
			frontSplit = new ZShadeSandboxBSP::BSPPortal();
			backSplit = new ZShadeSandboxBSP::BSPPortal();

			ZeroMemory(frontSplit, sizeof(ZShadeSandboxBSP::BSPPortal));
			ZeroMemory(backSplit, sizeof(ZShadeSandboxBSP::BSPPortal));

			SplitPortal(portal, &arrayData->planeArray[arrayData->nodeArray[node].planeID], frontSplit, backSplit);

			DeletePortal(portal);

			// There is another front node
			if (arrayData->nodeArray[node].isLeaf == 0)
			{
				frontPortalList = ClipPortal(arrayData->nodeArray[node].frontID, frontSplit);
			}
			else
			{
				// About to get pushed into another leaf
				frontSplit->LeafOwnerArray(portal->LeafCount()) = arrayData->nodeArray[node].frontID;
				frontSplit->IncLeafCount();
				frontSplit->NextPortal() = 0;
				frontSplit->PrevPortal() = 0;
				frontPortalList = frontSplit;
			}

			if (arrayData->nodeArray[node].backID != -1)
			{
				backPortalList = ClipPortal(arrayData->nodeArray[node].backID, backSplit);
			}
			else
			{
				// In solid space so delete it
				DeletePortal(backSplit);
			}

			// Something is in the front list
			if (frontPortalList != 0)
			{
				iter = frontPortalList;

				while (iter->NextPortal() != 0)
				{
					iter = iter->NextPortal();
				}

				if (backPortalList != 0)
				{
					iter->NextPortal() = backPortalList;
					backPortalList->PrevPortal() = iter;
				}

				return frontPortalList;
			}
			else
			{
				// Nothing in the front list
				if (backPortalList != 0)
				{
					return backPortalList;
				}

				return 0;
			}

			return 0;
		}
		break;
		case ZShadeSandboxBSP::EPointPosition::Type::eOnPlane:
		{
			// Leaf node
			if (arrayData->nodeArray[node].isLeaf != 0)
			{
				portal->LeafOwnerArray(portal->LeafCount()) = arrayData->nodeArray[node].frontID;
				portal->IncLeafCount();
				portal->NextPortal() = 0;
				portal->PrevPortal() = 0;
				frontPortalList = portal;
			}
			else
			{
				frontPortalList = ClipPortal(arrayData->nodeArray[node].frontID, portal);
			}

			if (frontPortalList == 0)
			{
				return 0;
			}

			if (arrayData->nodeArray[node].backID == -1)
			{
				return frontPortalList;
			}

			while (frontPortalList != 0)
			{
				ZShadeSandboxBSP::BSPPortal* tempnext = frontPortalList->NextPortal();
				backPortalList = 0;

				// Fragment this fragment into the back tree
				backPortalList = ClipPortal(arrayData->nodeArray[node].backID, frontPortalList);

				if (backPortalList != 0)
				{
					iter = backPortalList;

					while (iter->NextPortal() != 0)
					{
						iter = iter->NextPortal();
					}

					// Attach the last fragment to the first fragment from a previous loop

					iter->NextPortal() = portalList;

					if (portalList != 0)
					{
						portalList->PrevPortal() = iter;
					}

					// Portal List now points at the current complete list of fragment collected from each loop
					portalList = backPortalList;
				}

				portalList = backPortalList;
			}

			return portalList;
		}
		break;
	}
}
//==================================================================================================================================
void BSPPortalGenerator::SplitPortal(ZShadeSandboxBSP::BSPPortal* portal, ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPortal* frontSplit, ZShadeSandboxBSP::BSPPortal* backSplit)
{
	SplitPortalPolygon(portal, plane, frontSplit, backSplit);

	frontSplit->LeafCount() = portal->LeafCount();
	backSplit->LeafCount() = portal->LeafCount();

	memcpy(frontSplit->LeafOwnerArray(), portal->LeafOwnerArray(), sizeof(long)* portal->LeafCount());
	memcpy(backSplit->LeafOwnerArray(), portal->LeafOwnerArray(), sizeof(long)* portal->LeafCount());
}
//==================================================================================================================================
void BSPPortalGenerator::SplitPortalPolygon(ZShadeSandboxBSP::BSPPortal* portal, ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPortal* frontSplit, ZShadeSandboxBSP::BSPPortal* backSplit)
{
	ZShadeSandboxMesh::VertexTex firstVertex;
	vector<ZShadeSandboxMesh::VertexTex> frontList, backList;
	ZShadeSandboxMath::XMMath3 planeNormal, intersectionPoint, pointOnPlane, pointA, pointB;
	int frontCounter = 0, backCounter = 0, loop = 0, currentVertex = 0;
	float percent;

	frontList.resize(40);
	backList.resize(40);

	// Find any vertex on the plane to use later in plane intersect routine
	pointOnPlane = plane->pointOnPlane;

	// First we find out if the first vertex belongs in front or back list
	firstVertex = portal->GetVertex(0);

	switch (ZShadeSandboxBSP::BSP::ClassifyPoint(firstVertex.position, plane))
	{
		case ZShadeSandboxBSP::EPointPosition::Type::eFront:
		{
			frontList[frontCounter++] = firstVertex;
		}
		break;
		case ZShadeSandboxBSP::EPointPosition::Type::eBack:
		{
			backList[backCounter++] = firstVertex;
		}
		break;
		case ZShadeSandboxBSP::EPointPosition::Type::eOnPlane:
		{
			backList[backCounter++] = firstVertex;
			frontList[frontCounter++] = firstVertex;
		}
		break;
		default: break;
	}

	for (loop = 1; loop < portal->GetVertexCount() + 1; loop++)
	{
		if (loop == portal->GetVertexCount())
		{
			currentVertex = 0;
		}
		else
		{
			currentVertex = loop;
		}

		pointA = portal->GetVertexPosition(loop - 1);
		pointB = portal->GetVertexPosition(currentVertex);

		planeNormal = plane->normal;

		int result = ZShadeSandboxBSP::BSP::ClassifyPoint(pointB, plane);

		if (result == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
		{
			backList[backCounter++] = portal->GetVertex(currentVertex);
			frontList[frontCounter++] = portal->GetVertex(currentVertex);
		}
		else
		{
			if (ZShadeSandboxMath::ZMath::GetIntersect(pointA, pointB, pointOnPlane, planeNormal, intersectionPoint, percent))
			{
				float deltax, deltay, texx, texy;

				deltax = portal->GetVertexTexture(currentVertex).x - portal->GetVertexTexture(loop - 1).x;
				deltay = portal->GetVertexTexture(currentVertex).y - portal->GetVertexTexture(loop - 1).y;

				texx = portal->GetVertexTexture(loop - 1).x + (deltax * percent);
				texy = portal->GetVertexTexture(loop - 1).y + (deltay * percent);

				ZShadeSandboxMesh::VertexTex copy = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, texx, texy);

				if (result == ZShadeSandboxBSP::EPointPosition::Type::eFront)
				{
					backList[backCounter++] = copy;
					frontList[frontCounter++] = copy;

					if (currentVertex != 0)
					{
						frontList[frontCounter++] = portal->GetVertex(currentVertex);
					}
				}

				if (result == ZShadeSandboxBSP::EPointPosition::Type::eBack)
				{
					backList[backCounter++] = copy;
					frontList[frontCounter++] = copy;

					if (currentVertex != 0)
					{
						backList[backCounter++] = portal->GetVertex(currentVertex);
					}
				}
			}
			else
			{
				if (result == ZShadeSandboxBSP::EPointPosition::Type::eFront)
				{
					if (currentVertex != 0)
					{
						frontList[frontCounter++] = portal->GetVertex(currentVertex);
					}
				}

				if (result == ZShadeSandboxBSP::EPointPosition::Type::eBack)
				{
					if (currentVertex != 0)
					{
						backList[backCounter++] = portal->GetVertex(currentVertex);
					}
				}
			}
		}
	}

	frontSplit->CreateSplitPolygon(frontList, frontCounter);
	backSplit->CreateSplitPolygon(backList, backCounter);

	frontSplit->Normal() = portal->Normal();
	backSplit->Normal() = portal->Normal();
}
//==================================================================================================================================