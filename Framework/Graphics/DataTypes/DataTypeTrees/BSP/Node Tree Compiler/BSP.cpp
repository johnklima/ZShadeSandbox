#include "BSP.h"
#include "MemoryMgr.h"
#include "XMMath.h"
#include "ZMath.h"
using ZShadeSandboxBSP::BSP;
//==================================================================================================================================
//==================================================================================================================================
BSP::BSP()
{
	mPolygonList = 0;
	InitPolygons();
}
//==================================================================================================================================
void BSP::InitPolygons()
{
	//ZShadeSandboxMesh::VertexTex vertexList[4][4];
	
	SmartArray2D<ZShadeSandboxMesh::VertexTex> vertexList(4, 4);
	
	ZShadeSandboxBSP::BSPPolygon* child = 0;
	
	int direction[4];
	int offset;
	
	for (int z = 0; z < 40; z++)
	{
		for (int x = 0; x < 20; x++)
		{
			ZeroMemory(direction, sizeof(int) * 4);
			
			offset = (z * 20) + x;
			
			if (BSPMap[offset] != 0)
			{
				// NE Wall
				if (BSPMap[offset] == 2)
				{
					//vertexList[0][0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) - 0.5f, 0, 0);
					//vertexList[0][1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) + 0.5f, 1, 0);
					//vertexList[0][2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) + 0.5f, 1, 1);
					//vertexList[0][3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) - 0.5f, 0, 1);
					vertexList.insert(0, 0, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) - 0.5f, 0, 0));
					vertexList.insert(0, 1, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 3.0f, (20.0f - z) + 0.5f, 1, 0));
					vertexList.insert(0, 2, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 0.0f, (20.0f - z) + 0.5f, 0, 1));
					vertexList.insert(0, 3, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) - 0.5f, 1, 1));
					direction[0] = 1;
				}
				
				// NW Wall
				if (BSPMap[offset] == 3)
				{
					//vertexList[0][0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) + 0.5f, 0, 0);
					//vertexList[0][1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) - 0.5f, 1, 0);
					//vertexList[0][2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) - 0.5f, 1, 1);
					//vertexList[0][3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) + 0.5f, 0, 1);
					vertexList.insert(0, 0, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) + 0.5f, 0, 0));
					vertexList.insert(0, 1, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 3.0f, (20.0f - z) - 0.5f, 1, 0));
					vertexList.insert(0, 2, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 0.0f, (20.0f - z) - 0.5f, 0, 1));
					vertexList.insert(0, 3, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) + 0.5f, 1, 1));
					direction[0] = 1;
				}
				
				// Standard Wall
				if (BSPMap[offset] == 1)
				{
					if (x > 0)
					{
						// Add a left facing wall if Nothing to the left
						if (BSPMap[offset - 1] == 0)
						{
							//vertexList[0][0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) + 0.5f, 0, 0);
							//vertexList[0][1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) - 0.5f, 1, 0);
							//vertexList[0][2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) - 0.5f, 1, 1);
							//vertexList[0][3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) + 0.5f, 0, 1);
							vertexList.insert(0, 0, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) + 0.5f, 0, 0));
							vertexList.insert(0, 1, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) - 0.5f, 1, 0));
							vertexList.insert(0, 2, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) - 0.5f, 0, 1));
							vertexList.insert(0, 3, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) + 0.5f, 1, 1));
							direction[0] = 1;
						}
					}
					
					if (x > 19)
					{
						// Add a right facing wall if Nothing to the right
						if (BSPMap[offset + 1] == 0)
						{
							//vertexList[1][0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) - 0.5f, 0, 0);
							//vertexList[1][1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) + 0.5f, 1, 0);
							//vertexList[1][2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) + 0.5f, 1, 1);
							//vertexList[1][3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) - 0.5f, 0, 1);
							vertexList.insert(1, 0, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) - 0.5f, 0, 0));
							vertexList.insert(1, 1, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) + 0.5f, 1, 0));
							vertexList.insert(1, 2, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) + 0.5f, 0, 1));
							vertexList.insert(1, 3, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) - 0.5f, 1, 1));
							direction[1] = 1;
						}
					}
					
					if (z > 0)
					{
						// Add a south facing wall if Nothing to the south
						if (BSPMap[offset - 20] == 0)
						{
							//vertexList[2][0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) + 0.5f, 0, 0);
							//vertexList[2][1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) + 0.5f, 1, 0);
							//vertexList[2][2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) + 0.5f, 1, 1);
							//vertexList[2][3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) + 0.5f, 0, 1);
							vertexList.insert(2, 0, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 3.0f, (20.0f - z) + 0.5f, 0, 0));
							vertexList.insert(2, 1, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) + 0.5f, 1, 0));
							vertexList.insert(2, 2, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) + 0.5f, 0, 1));
							vertexList.insert(2, 3, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 0.0f, (20.0f - z) + 0.5f, 1, 1));
							direction[2] = 1;
						}
					}
					
					if (z > 39)
					{
						// Add a north facing wall if Nothing to the north
						if (BSPMap[offset + 20] == 0)
						{
							//vertexList[3][0] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) - 0.5f, 0, 0);
							//vertexList[3][1] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 3.0f, (20.0f - z) - 0.5f, 1, 0);
							//vertexList[3][2] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 9.5f, 0.0f, (20.0f - z) - 0.5f, 1, 1);
							//vertexList[3][3] = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) - 0.5f, 0, 1);
							vertexList.insert(3, 0, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 3.0f, (20.0f - z) - 0.5f, 0, 0));
							vertexList.insert(3, 1, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 3.0f, (20.0f - z) - 0.5f, 1, 0));
							vertexList.insert(3, 2, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x -  9.5f, 0.0f, (20.0f - z) - 0.5f, 0, 1));
							vertexList.insert(3, 3, ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x - 10.5f, 0.0f, (20.0f - z) - 0.5f, 1, 1));
							direction[3] = 1;
						}
					}
				}
				
				// Build the polygons
				for (int a = 0; a < 4; a++)
				{
					if (direction[a] != 0)
					{
						if (mPolygonList == NULL)
						{
							mPolygonList = AddPolygon(NULL, vertexList.InnerVector(a));
							child = mPolygonList;
						}
						else
						{
							child = AddPolygon(child, vertexList.InnerVector(a));
						}
					}
				}
			}
		}
	}
	
	//
	// Build the BSP Tree
	//
	
	mBSPRoot = new ZShadeSandboxBSP::BSPNode();
	
	Build(mBSPRoot, mPolygonList);
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPolygon* BSP::AddPolygon(ZShadeSandboxBSP::BSPPolygon* parent, vector<ZShadeSandboxMesh::VertexTex> vertices)
{
	ZShadeSandboxBSP::BSPPolygon* child = new ZShadeSandboxBSP::BSPPolygon();
	
	child->Create(vertices);
	
	if (parent != NULL)
	{
		parent->SetNextPolygon(child);
	}
	
	return child;
}
//==================================================================================================================================
/*BSPPolygon* BSP::AddPolygon(BSPPolygon* parent, ZShadeSandboxMesh::VertexTex* vertices, int vertexCount)
{
	BSPPolygon* child = new BSPPolygon();
	
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
}*/
//==================================================================================================================================
void BSP::Build(ZShadeSandboxBSP::BSPNode* node, ZShadeSandboxBSP::BSPPolygon* polygonList)
{
	ZShadeSandboxBSP::BSPPolygon* polygonTest = 0;
	ZShadeSandboxBSP::BSPPolygon* frontList = 0;
	ZShadeSandboxBSP::BSPPolygon* backList = 0;
	ZShadeSandboxBSP::BSPPolygon* nextPolygon = 0;
	ZShadeSandboxBSP::BSPPolygon* frontSplit = 0;
	ZShadeSandboxBSP::BSPPolygon* backSplit = 0;
	
	ZShadeSandboxMath::XMMath3 vec1, vec2;
	
	node->splitterPolygon = LocateSplitter(polygonList);
	
	polygonTest = polygonList;
	
	while (polygonTest != 0)
	{
		nextPolygon = polygonTest->NextPolygon();
		
		if (polygonTest != node->splitterPolygon)
		{
			switch (ClassifyPolygon(node->splitterPolygon, polygonTest))
			{
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
					frontSplit = new BSPPolygon();
					backSplit = new BSPPolygon();
					
					ZeroMemory(frontSplit, sizeof(BSPPolygon));
					ZeroMemory(backSplit, sizeof(BSPPolygon));
					
					SplitPolygon(polygonTest, node->splitterPolygon, frontSplit, backSplit);
					
					delete polygonTest;
					
					frontSplit->SetNextPolygon(frontList);
					frontList = frontSplit;
					
					backSplit->SetNextPolygon(backList);
					backList = backSplit;
				}
				break;
				default: break;
			}
		}
		
		polygonTest = nextPolygon;
	}
	
	if (frontList == 0)
	{
		ZShadeSandboxBSP::BSPNode* leafNode = new ZShadeSandboxBSP::BSPNode();
		ZeroMemory(leafNode, sizeof(ZShadeSandboxBSP::BSPNode));
		leafNode->isLeaf = true;
		leafNode->isSolid = false;
		node->frontChild = leafNode;
	}
	else
	{
		ZShadeSandboxBSP::BSPNode* newNode = new ZShadeSandboxBSP::BSPNode();
		ZeroMemory(newNode, sizeof(ZShadeSandboxBSP::BSPNode));
		newNode->isLeaf = false;
		node->frontChild = newNode;
		Build(newNode, frontList);
	}
	
	if (backList == 0)
	{
		ZShadeSandboxBSP::BSPNode* leafNode = new ZShadeSandboxBSP::BSPNode();
		ZeroMemory(leafNode, sizeof(ZShadeSandboxBSP::BSPNode));
		leafNode->isLeaf = true;
		leafNode->isSolid = true;
		node->backChild = leafNode;
	}
	else
	{
		ZShadeSandboxBSP::BSPNode* newNode = new ZShadeSandboxBSP::BSPNode();
		ZeroMemory(newNode, sizeof(ZShadeSandboxBSP::BSPNode));
		newNode->isLeaf = false;
		node->backChild = newNode;
		Build(newNode, backList);
	}
}
//==================================================================================================================================
ZShadeSandboxBSP::EPointPosition::Type BSP::ClassifyPoint(ZShadeSandboxMath::XMMath3 pos, ZShadeSandboxBSP::BSPPolygon* plane)
{
	ZShadeSandboxMath::XMMath3 vec1 = plane->GetVertexPosition(0);
	ZShadeSandboxMath::XMMath3 dir = vec1 - pos;
	
	float dot = dir.Dot(plane->Normal());
	
	if (dot < -0.001f)
		return ZShadeSandboxBSP::EPointPosition::Type::eFront;
	
	if (dot > 0.001f)
		return ZShadeSandboxBSP::EPointPosition::Type::eBack;
	
	return ZShadeSandboxBSP::EPointPosition::Type::eOnPlane;
}
//==================================================================================================================================
ZShadeSandboxBSP::EPointPosition::Type BSP::ClassifyPolygon(ZShadeSandboxBSP::BSPPolygon* plane, ZShadeSandboxBSP::BSPPolygon* polygon)
{
	int inFront = 0, behind = 0, onPlane = 0;
	
	ZShadeSandboxMath::XMMath3 vec1 = plane->GetVertexPosition(0);
	
	for (int a = 0; a < polygon->GetVertexCount(); a++)
	{
		ZShadeSandboxMath::XMMath3 vec2 = polygon->GetVertexPosition(a);
		
		ZShadeSandboxMath::XMMath3 dir = vec1 - vec2;
		
		float dot = dir.Dot(plane->Normal());
		
		if (dot < -0.001f)
		{
			inFront++;
		}
		else
		{
			onPlane++;
			inFront++;
			behind++;
		}
		
		if (dot > 0.001f)
		{
			behind++;
		}
	}
	
	if (onPlane == polygon->GetVertexCount())
	{
		// Would normally be eOnPlane for normal BSP
		return ZShadeSandboxBSP::EPointPosition::Type::eFront;
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
bool BSP::GetIntersect
(	ZShadeSandboxMath::XMMath3& linestart
,	ZShadeSandboxMath::XMMath3& lineend
,	ZShadeSandboxMath::XMMath3& vertex
,	ZShadeSandboxMath::XMMath3& normal
,	ZShadeSandboxMath::XMMath3& intersection
,	float& percentage
)
{
	ZShadeSandboxMath::XMMath3 direction, L1;
	float lineLength, distFromPlane;
	
	direction.x = lineend.x - linestart.x;
	direction.y = lineend.y - linestart.y;
	direction.z = lineend.z - linestart.z;
	
	lineLength = direction.Dot(normal);
	
	if (fabsf(lineLength) < 0.0001f)
	{
		return false;
	}
	
	L1.x = vertex.x - linestart.x;
	L1.y = vertex.y - linestart.y;
	L1.z = vertex.z - linestart.z;
	
	distFromPlane = L1.Dot(normal);
	
	percentage = distFromPlane / lineLength;
	
	if (percentage < 0.0f)
	{
		return false;
	}
	else if (percentage > 1.0f)
	{
		return false;
	}
	
	intersection.x = linestart.x + direction.x * percentage;
	intersection.y = linestart.y + direction.y * percentage;
	intersection.z = linestart.z + direction.z * percentage;
	
	return true;
}
//==================================================================================================================================
void BSP::SplitPolygon(ZShadeSandboxBSP::BSPPolygon* polygon, ZShadeSandboxBSP::BSPPolygon* plane, ZShadeSandboxBSP::BSPPolygon* frontSplit, ZShadeSandboxBSP::BSPPolygon* backSplit)
{
	ZShadeSandboxMesh::VertexTex firstVertex;
	vector<ZShadeSandboxMesh::VertexTex> frontList, backList;
	ZShadeSandboxMath::XMMath3 planeNormal, intersectionPoint, pointOnPlane, pointA, pointB;
	int frontCounter = 0, backCounter = 0, loop = 0, currentVertex = 0;
	float percent;
	
	frontList.resize(20);
	backList.resize(20);

	// Find any vertex on the plane to use later in plane intersect routine
	pointOnPlane = plane->GetVertexPosition(0);
	
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
		
		planeNormal = plane->Normal();

		int result = ClassifyPoint(pointB, plane);
		
		if (result == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
		{
			backList[backCounter++] = polygon->GetVertex(currentVertex);
			frontList[frontCounter++] = polygon->GetVertex(currentVertex);
		}
		else
		{
			if (GetIntersect(pointA, pointB, pointOnPlane, planeNormal, intersectionPoint, percent))
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
}
//==================================================================================================================================
ZShadeSandboxBSP::BSPPolygon* BSP::LocateSplitter(ZShadeSandboxBSP::BSPPolygon* polygonList)
{
	ZShadeSandboxBSP::BSPPolygon* splitter = polygonList;
	
	ZShadeSandboxBSP::BSPPolygon* currentPolygon = 0;
	
	unsigned long bestScore = 100000;
	
	ZShadeSandboxBSP::BSPPolygon* selectedPolygon = 0;
	
	while (splitter != 0)
	{
		currentPolygon = polygonList;
		
		unsigned long score = 0, splits = 0, backfaces = 0, frontfaces = 0;
		
		while (currentPolygon != 0)
		{
			if (currentPolygon != splitter)
			{
				EPointPosition::Type result = ClassifyPolygon(splitter, currentPolygon);
				
				switch (result)
				{
					case ZShadeSandboxBSP::EPointPosition::Type::eOnPlane: break;
					case ZShadeSandboxBSP::EPointPosition::Type::eFront: frontfaces++; break;
					case ZShadeSandboxBSP::EPointPosition::Type::eBack: backfaces++; break;
					case ZShadeSandboxBSP::EPointPosition::Type::eSpanning: splits++; break;
					default: break;
				}
			}
			
			currentPolygon = currentPolygon->NextPolygon();
		}
		
		score = ZShadeSandboxMath::ZMath::Abs((frontfaces - backfaces) + (splits * 8));
		
		if (score < bestScore)
		{
			bestScore = score;
			selectedPolygon = splitter;
		}
		
		splitter = splitter->NextPolygon();
	}
	
	return selectedPolygon;
}
//==================================================================================================================================
bool BSP::LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end)
{
	return LineOfSight(start, end, mBSPRoot);
}
//==================================================================================================================================
bool BSP::LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end, ZShadeSandboxBSP::BSPNode* node)
{
	if (node->isLeaf)
	{
		return !node->isSolid;
	}
	
	ZShadeSandboxBSP::EPointPosition::Type pointA = ClassifyPoint(start, node->splitterPolygon);
	ZShadeSandboxBSP::EPointPosition::Type pointB = ClassifyPoint(end, node->splitterPolygon);
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane && pointB == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
	{
		return LineOfSight(start, end, node->frontChild);
	}
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eFront && pointB == ZShadeSandboxBSP::EPointPosition::Type::eBack)
	{
		float temp;
		
		ZShadeSandboxMath::XMMath3 vertex, normal, intersection;
		
		vertex = node->splitterPolygon->GetVertexPosition(0);
		normal = node->splitterPolygon->Normal();
		
		GetIntersect(start, end, vertex, normal, intersection, temp);
		
		return LineOfSight(start, intersection, node->frontChild) && LineOfSight(end, intersection, node->backChild);
	}
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eBack && pointB == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		float temp;
		
		ZShadeSandboxMath::XMMath3 vertex, normal, intersection;
		
		vertex = node->splitterPolygon->GetVertexPosition(0);
		normal = node->splitterPolygon->Normal();
		
		GetIntersect(start, end, vertex, normal, intersection, temp);
		
		return LineOfSight(end, intersection, node->frontChild) && LineOfSight(start, intersection, node->backChild);
	}
	
	//
	// If we get here then one of the points is on the plane
	//
	
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eFront && pointB == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		return LineOfSight(start, end, node->frontChild);
	}
	else
	{
		return LineOfSight(start, end, node->backChild);
	}
	
	return true;
}
//==================================================================================================================================