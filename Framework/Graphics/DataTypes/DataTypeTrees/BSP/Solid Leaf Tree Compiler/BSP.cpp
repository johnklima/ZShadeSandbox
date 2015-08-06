#include "BSP.h"
#include "BSPArrayData.h"
using ZShadeSandboxBSP::BSP;
//==================================================================================================================================
//==================================================================================================================================
BSP::BSP()
{
	// Allocate the array data
	BSPArrayData::Instance();
}
//==================================================================================================================================
void BSP::Load(string levelFilename)
{
	// Load the polygons in the level
	BSPArrayData::Instance()->polygonList = 0;
	BSPArrayData::Instance()->polygonList = LoadBSP(levelFilename);
	
	// Create the BSP
	Build(0, BSPArrayData::Instance()->polygonList);
	
	// Create the portals in the level
	mBSPPortalGenerator = new BSPPortalGenerator();
	mBSPPortalGenerator->Build();
	
	// Calculate the PVS for rendering
	mBSPPVSGenerator = new BSPPVSGenerator();
	bytesPerSet = (BSPArrayData::Instance()->lNumberOfLeaves + 7) >> 3;
	BSPArrayData::Instance()->pvsData = (BYTE*)malloc(BSPArrayData::Instance()->lNumberOfLeaves * bytesPerSet);
	ZeroMemory(BSPArrayData::Instance()->pvsData, sizeof(BSPArrayData::Instance()->lNumberOfLeaves * bytesPerSet));
	BSPArrayData::Instance()->PVSCompressedSize = mBSPPVSGenerator->CalculatePVS();
	BSPArrayData::Instance()->pvsData = (BYTE*)realloc(BSPArrayData::Instance()->pvsData, BSPArrayData::Instance()->PVSCompressedSize);
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPolygon* BSP::LoadBSP(string levelFilename)
{
	FILE* stream;
	BSPPolygon* root = 0;
	BSPPolygon* child = 0;
	WORD polyCount, vertexCount;
	ZShadeSandboxMesh::VertexTex xyzBuffer[50];
	string materialName;
	
	stream = fopen(levelFilename.c_str(), "rb");
	
	fread(&polyCount, sizeof(WORD), 1, stream);
	
	for (int i = 0; i < polyCount; i++)
	{
		fread(&vertexCount, sizeof(WORD), 1, stream);
		
		for (int v = 0; v < vertexCount; v++)
		{
			fread(&xyzBuffer[v].position.x, sizeof(float), 1, stream);
			fread(&xyzBuffer[v].position.y, sizeof(float), 1, stream);
			fread(&xyzBuffer[v].position.z, sizeof(float), 1, stream);
			fread(&xyzBuffer[v].texture.x, sizeof(float), 1, stream);
			fread(&xyzBuffer[v].texture.y, sizeof(float), 1, stream);
		}
		
		fread(&materialName, sizeof(string), 1, stream);
		
		if (i == 0)
		{
			root = AddPolygon(NULL, &xyzBuffer[0], vertexCount);
			child = root;
		}
		else
		{
			child = AddPolygon(child, &xyzBuffer[0], vertexCount);
		}
		
		// Add the material to the polygon
		child->MaterialName() = materialName;
	}
	
	fclose(stream);
	
	return root;
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPolygon* BSP::AddPolygon(ZShadeSandboxBSP::BSPPolygon* parent, ZShadeSandboxMesh::VertexTex* vertices, int vertexCount)
{
	ZShadeSandboxBSP::BSPPolygon* child = new ZShadeSandboxBSP::BSPPolygon();
	
	vector<ZShadeSandboxMesh::VertexTex> verts(vertexCount);
	
	for (int i = 0; i < vertexCount; i++)
	{
		verts[i] = vertices[i];
	}
	
	child->Create(verts);
	
	if (parent != NULL)
	{
		parent->SetNextPolygon(child);
	}
	
	return child;
}
//==================================================================================================================================
void BSP::Build(long node, ZShadeSandboxBSP::BSPPolygon* polygonList)
{
	float result;
	
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	ZShadeSandboxBSP::BSPPolygon* polygonTest = 0;
	ZShadeSandboxBSP::BSPPolygon* frontList = 0;
	ZShadeSandboxBSP::BSPPolygon* backList = 0;
	ZShadeSandboxBSP::BSPPolygon* nextPolygon = 0;
	ZShadeSandboxBSP::BSPPolygon* frontSplit = 0;
	ZShadeSandboxBSP::BSPPolygon* backSplit = 0;
	
	ZShadeSandboxMath::XMMath3 vec1, vec2, a, b;
	
	mNodeArray[node].planeID = LocateSplitter(polygonList);
	
	polygonTest = polygonList;
	
	// Create dummy bounding boxes for the node so it can be altered
	arrayData->nodeArray[node].boundingBox.vMax = XMFLOAT3(-40000, -40000, -40000);
	arrayData->nodeArray[node].boundingBox.vMin = XMFLOAT3( 40000,  40000,  40000);
	
	while (polygonTest != 0)
	{
		nextPolygon = polygonTest->NextPolygon();
		
		switch (ClassifyPolygon(&arrayData->planeArray[arrayData->nodeArray[node].planeID], polygonTest))
		{
			case ZShadeSandboxBSP::EPointPosition::Type::eOnPlane:
			{
				a = arrayData->planeArray[arrayData->nodeArray[node].planeID].normal;
				b = polygonTest->Normal();
				
				result = ZShadeSandboxMath::ZMath::Abs((a.x - b.x) + (a.y - b.y) + (a.z - b.z));
				
				if (result < 0.1f)
				{
					polygonTest->SetNextPolygon(frontList);
					frontList = polygonTest;
				}
				else
				{
					polygonTest->SetNextPolygon(backList);
					backList = polygonTest;
				}
			}
			break;
			case ZShadeSandboxBSP::EPointPosition::Type::eFront:
			{
				polygonTest->SetNextPolygon(frontList);
				frontList = polygonTest;
			}
			break;
			case ZShadeSandboxBSP::EPointPosition::Type::eBack:
			{
				polygonTest->SetNextPolygon(backList);
				backList = polygonTest;
			}
			break;
			case ZShadeSandboxBSP::EPointPosition::Type::eSpanning:
			{
				frontSplit = new ZShadeSandboxBSP::BSPPolygon();
				backSplit = new ZShadeSandboxBSP::BSPPolygon();
				
				ZeroMemory(frontSplit, sizeof(ZShadeSandboxBSP::BSPPolygon));
				ZeroMemory(backSplit, sizeof(ZShadeSandboxBSP::BSPPolygon));
				
				SplitPolygon(polygonTest, &arrayData->planeArray[arrayData->nodeArray[node].planeID], frontSplit, backSplit);
				
				frontSplit->UsedAsSplitter() = polygonTest->UsedAsSplitter();
				backSplit->UsedAsSplitter() = polygonTest->UsedAsSplitter();
				
				frontSplit->MaterialName() = polygonTest->MaterialName();
				backSplit->MaterialName() = polygonTest->MaterialName();
				
				DeletePolygon(polygonTest);
				
				frontSplit->SetNextPolygon(frontList);
				frontList = frontSplit;
				
				backSplit->SetNextPolygon(backList);
				backList = backSplit;
			}
			break;
			default: break;
		}
		
		polygonTest = nextPolygon;
	}
	
	CalculateAABB(&arrayData->nodeArray[node].boundingBox, frontList);
	
	ZShadeSandboxMath::AABB leafAABB = arrayData->nodeArray[node].boundingBox;
	
	CalculateAABB(&arrayData->nodeArray[node].boundingBox, backList);
	
	int count = 0;
	
	ZShadeSandboxBSP::BSPPolygon* tempf = frontList;
	
	while (tempf != 0)
	{
		if (!tempf->UsedAsSplitter())
		{
			count++;
		}
		
		tempf = tempf->NextPolygon();
	}
	
	if (count == 0)
	{
		ZShadeSandboxBSP::BSPPolygon* iter = frontList;
		ZShadeSandboxBSP::BSPPolygon* temp;
		
		arrayData->leafArray[lNumberOfLeaves].startPolygon = arrayData->lNumberOfPolygons;
		
		while (iter != 0)
		{
			arrayData->polygonArray[lNumberOfPolygons] = *iter;
			
			arrayData->IncreasePolygonCount();
			
			temp = iter;
			
			iter = iter->NextPolygon();
			
			delete temp;
		}
		
		arrayData->leafArray[arrayData->lNumberOfLeaves].endPolygon = arrayData->lNumberOfPolygons;
		arrayData->leafArray[arrayData->lNumberOfLeaves].endPolygon = arrayData->lNumberOfPolygons;
		arrayData->leafArray[arrayData->lNumberOfLeaves].boundingBox = leafAABB;
		
		arrayData->nodeArray[node].frontID = arrayData->lNumberOfLeaves;
		arrayData->nodeArray[node].isLeaf = 1;
		
		arrayData->IncreaseLeafCount();
	}
	else
	{
		arrayData->nodeArray[node].isLeaf = 0;
		arrayData->nodeArray[node].frontID = arrayData->lNumberOfNodes + 1;
		
		arrayData->IncreaseNodeCount();
		
		Build(arrayData->lNumberOfNodes, frontList);
	}
	
	if (backList == 0)
	{
		arrayData->nodeArray[node].backID = -1;
	}
	else
	{
		arrayData->nodeArray[node].frontID = arrayData->lNumberOfNodes + 1;
		
		arrayData->IncreaseNodeCount();
		
		Build(arrayData->lNumberOfNodes, backList);
	}
}
//==================================================================================================================================
long BSP::LocateSplitter(ZShadeSandboxBSP::BSPPolygon* polygonList)
{
	ZShadeSandboxBSP::BSPPolygon* splitter = polygonList;
	
	ZShadeSandboxBSP::BSPPolygon* currentPolygon = 0;
	
	unsigned long bestScore = 1000000;
	
	ZShadeSandboxBSP::BSPPolygon* selectedPolygon = 0;
	
	while (splitter != 0)
	{
		if (!splitter->UsedAsSplitter())
		{
			BSPPlane splitterPlane;
			
			splitterPlane.normal = splitter->Normal();
			splitterPlane.pointOnPlane = splitter->GetVertexPosition(0);
			
			currentPolygon = polygonList;
			
			unsigned long score = 0, splits = 0, backfaces = 0, frontfaces = 0;
			
			while (currentPolygon != 0)
			{
				switch (ClassifyPolygon(&splitterPlane, currentPolygon))
				{
					case ZShadeSandboxBSP::EPointPosition::Type::eOnPlane: break;
					case ZShadeSandboxBSP::EPointPosition::Type::eFront: frontfaces++; break;
					case ZShadeSandboxBSP::EPointPosition::Type::eBack: backfaces++; break;
					case ZShadeSandboxBSP::EPointPosition::Type::eSpanning: splits++; break;
					default: break;
				}
				
				currentPolygon = currentPolygon->NextPolygon();
			}
			
			score = ZShadeSandboxMath::ZMath::Abs((frontfaces - backfaces) + (splits * 3));
			
			if (score < bestScore)
			{
				bestScore = score;
				selectedPolygon = splitter;
			}
			
			splitter = splitter->NextPolygon();
		}
	}
	
	selectedPolygon->UsedAsSplitter() = true;
	
	BSPArrayData::Instance()->planeArray[BSPArrayData::Instance()->lNumberOfPlanes].pointOnPlane = selectedPolygon->GetVertexPosition(0);
	BSPArrayData::Instance()->planeArray[BSPArrayData::Instance()->lNumberOfPlanes].normal = selectedPolygon->Normal();
	
	BSPArrayData::Instance()->IncreasePlaneCount();
	
	return BSPArrayData::Instance()->lNumberOfPlanes - 1;
}
//==================================================================================================================================
void BSP::CalculateAABB(ZShadeSandboxMath::AABB* box, ZShadeSandboxBSP::BSPPolygon* polygonList)
{
	ZShadeSandboxBSP::BSPPolygon* polygonPointer = polygonList;
	
	while (polygonPointer != 0)
	{
		for (int i = 0; i < polygonPointer->GetVertexCount(); i++)
		{
			// Check minimum bounds
			if (polygonPointer->GetVertexPosition(i).x < box->vMin.x) box->vMin.x = polygonPointer->GetVertexPosition(i).x;
			if (polygonPointer->GetVertexPosition(i).y < box->vMin.y) box->vMin.y = polygonPointer->GetVertexPosition(i).y;
			if (polygonPointer->GetVertexPosition(i).z < box->vMin.z) box->vMin.z = polygonPointer->GetVertexPosition(i).z;
			
			// Check maximum bounds
			if (polygonPointer->GetVertexPosition(i).x < box->vMax.x) box->vMax.x = polygonPointer->GetVertexPosition(i).x;
			if (polygonPointer->GetVertexPosition(i).y < box->vMax.y) box->vMax.y = polygonPointer->GetVertexPosition(i).y;
			if (polygonPointer->GetVertexPosition(i).z < box->vMax.z) box->vMax.z = polygonPointer->GetVertexPosition(i).z;
		}
		
		polygonPointer = polygonPointer->NextPolygon();
	}
}
//==================================================================================================================================
void BSP::DeletePolygon(ZShadeSandboxBSP::BSPPolygon* polygon)
{
	polygon->ClearVertices();
	polygon->ClearIndices();
	
	delete polygon;
}
//==================================================================================================================================
ZShadeSandboxBSP::EPointPosition::Type BSP::ClassifyPoint(ZShadeSandboxMath::XMMath3 pos, ZShadeSandboxBSP::BSPPlane* plane)
{
	ZShadeSandboxMath::XMMath3 vec1 = plane->pointOnPlane;
	ZShadeSandboxMath::XMMath3 dir = vec1 - pos;
	
	float dot = dir.Dot(plane->normal);
	
	if (dot < -0.001f)
		return ZShadeSandboxBSP::EPointPosition::Type::eFront;
	
	if (dot > 0.001f)
		return ZShadeSandboxBSP::EPointPosition::Type::eBack;
	
	return ZShadeSandboxBSP::EPointPosition::Type::eOnPlane;
}
//==================================================================================================================================
ZShadeSandboxBSP::EPointPosition::Type BSP::ClassifyPolygon(ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPolygon* polygon)
{
	int inFront = 0, behind = 0, onPlane = 0;
	
	ZShadeSandboxMath::XMMath3 vec1 = plane->pointOnPlane;
	
	for (int a = 0; a < polygon->GetVertexCount(); a++)
	{
		ZShadeSandboxMath::XMMath3 vec2 = polygon->GetVertexPosition(a);
		
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
	
	if (onPlane == polygon->GetVertexCount())
	{
		return ZShadeSandboxBSP::EPointPosition::Type::eOnPlane;
	}
	
	if (behind == polygon->GetVertexCount())
	{
		return ZShadeSandboxBSP::EPointPosition::Type::eBack;
	}
	
	if (inFront == polygon->GetVertexCount())
	{
		return ZShadeSandboxBSP::EPointPosition::Type::eFront;
	}
	
	return ZShadeSandboxBSP::EPointPosition::Type::eSpanning;
}
//==================================================================================================================================
void BSP::SplitPolygon(ZShadeSandboxBSP::BSPPolygon* polygon, ZShadeSandboxBSP::BSPPlane* plane, ZShadeSandboxBSP::BSPPolygon* frontSplit, ZShadeSandboxBSP::BSPPolygon* backSplit)
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
	firstVertex = polygon->GetVertex(0);
	
	switch (ClassifyPoint(firstVertex.position, plane))
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
	
	for (loop = 1; loop < polygon->GetVertexCount() + 1; loop++)
	{
		if (loop == polygon->GetVertexCount())
		{
			currentVertex = 0;
		}
		else
		{
			currentVertex = loop;
		}
		
		pointA = polygon->GetVertexPosition(loop - 1);
		pointB = polygon->GetVertexPosition(currentVertex);
		
		planeNormal = plane->normal;
		
		int result = ClassifyPoint(pointB, plane);
		
		if (result == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
		{
			backList[backCounter++] = polygon->GetVertex(currentVertex);
			frontList[frontCounter++] = polygon->GetVertex(currentVertex);
		}
		else
		{
			if (ZShadeSandboxMath::ZMath::GetIntersect(pointA, pointB, pointOnPlane, planeNormal, intersectionPoint, percent))
			{
				float deltax, deltay, texx, texy;

				deltax = polygon->GetVertexTexture(currentVertex).x - polygon->GetVertexTexture(loop - 1).x;
				deltay = polygon->GetVertexTexture(currentVertex).y - polygon->GetVertexTexture(loop - 1).y;

				texx = polygon->GetVertexTexture(loop - 1).x + (deltax * percent);
				texy = polygon->GetVertexTexture(loop - 1).y + (deltay * percent);

				ZShadeSandboxMesh::VertexTex copy = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, texx, texy);

				if (result == ZShadeSandboxBSP::EPointPosition::Type::eFront)
				{
					backList[backCounter++] = copy;
					frontList[frontCounter++] = copy;

					if (currentVertex != 0)
					{
						frontList[frontCounter++] = polygon->GetVertex(currentVertex);
					}
				}

				if (result == ZShadeSandboxBSP::EPointPosition::Type::eBack)
				{
					backList[backCounter++] = copy;
					frontList[frontCounter++] = copy;

					if (currentVertex != 0)
					{
						backList[backCounter++] = polygon->GetVertex(currentVertex);
					}
				}
			}
			else
			{
				if (result == ZShadeSandboxBSP::EPointPosition::Type::eFront)
				{
					if (currentVertex != 0)
					{
						frontList[frontCounter++] = polygon->GetVertex(currentVertex);
					}
				}

				if (result == ZShadeSandboxBSP::EPointPosition::Type::eBack)
				{
					if (currentVertex != 0)
					{
						backList[backCounter++] = polygon->GetVertex(currentVertex);
					}
				}
			}
		}
	}

	frontSplit->CreateSplitPolygon(frontList, frontCounter);
	backSplit->CreateSplitPolygon(backList, backCounter);
	
	frontSplit->Normal() = polygon->Normal();
	backSplit->Normal() = polygon->Normal();
}
//==================================================================================================================================