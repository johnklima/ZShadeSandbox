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
void BSPRenderer::SaveBSP(string filename)
{
	FILE* stream = fopen(filename.c_str(), "w+b");
	long a;
	
	//
	// Save the node array
	//
	
	BSPArrayData::Instance()->lNumberOfNodes++;
	
	fwrite(&BSPArrayData::Instance()->lNumberOfNodes, sizeof(long), 1, stream);
	
	ZShadeSandboxBSP::BSPNode* n = BSPArrayData::Instance()->nodeArray;
	
	for (a = 0; a < BSPArrayData::Instance()->lNumberOfNodes; a++)
	{
		fwrite(&n->isLeaf, sizeof(unsigned char), 1, stream);
		fwrite(&n->planeID, sizeof(unsigned long), 1, stream);
		fwrite(&n->frontID, sizeof(unsigned long), 1, stream);
		fwrite(&n->backID, sizeof(unsigned long), 1, stream);
		n++;
	}
	
	//
	// Save the plane array
	//
	
	fwrite(&BSPArrayData::Instance()->lNumberOfPlanes, sizeof(long), 1, stream);
	
	fwrite(BSPArrayData::Instance()->planeArray, sizeof(ZShadeSandboxBSP::BSPPlane), BSPArrayData::Instance()->lNumberOfPlanes, stream);
	
	//
	// Save the leaf array
	//
	
	fwrite(&BSPArrayData::Instance()->lNumberOfLeaves, sizeof(long), 1, stream);
	
	ZShadeSandboxBSP::BSPLeaf* l = BSPArrayData::Instance()->leafArray;
	
	for (a = 0; a < BSPArrayData::Instance()->lNumberOfLeaves; a++)
	{
		fwrite(&l->startPolygon, sizeof(unsigned char), 1, stream);
		fwrite(&l->endPolygon, sizeof(unsigned long), 1, stream);
		fwrite(&l->pvsIndex, sizeof(unsigned long), 1, stream);
		fwrite(&l->boundingBox, sizeof(ZShadeSandboxMath::AABB), 1, stream);
		l++;
	}
	
	//
	// Save the polygon array
	//
	
	fwrite(&BSPArrayData::Instance()->lNumberOfPolygons, sizeof(long), 1, stream);
	
	ZShadeSandboxBSP::BSPPolygon* p = BSPArrayData::Instance()->polygonArray;
	
	for (a = 0; a < BSPArrayData::Instance()->lNumberOfLeaves; a++)
	{
		int vertexCount = p->GetVertexCount();
		int indexCount = p->GetIndexCount();
		ZShadeSandboxMesh::VertexTex* verts = new ZShadeSandboxMesh::VertexTex[vertexCount];
		UINT* inds = new UINT[indexCount];
		XMFLOAT3 normal = p->Normal();
		
		for (int i = 0; i < vertexCount; i++)
		{
			verts[i] = p->GetVertex(i);
		}
		
		for (int i = 0; i < indexCount; i++)
		{
			inds[i] = p->GetIndex(i);
		}
		
		fwrite(&vertexCount, sizeof(int), 1, stream);
		fwrite(verts, sizeof(ZShadeSandboxMesh::VertexTex), vertexCount, stream);
		fwrite(&indexCount, sizeof(int), 1, stream);
		fwrite(inds, sizeof(UINT), indexCount, stream);
		fwrite(&normal, sizeof(XMFLOAT3), 1, stream);
		
		p++;
	}
	
	//
	// Save the pvs array data
	//
	
	fwrite(&BSPArrayData::Instance()->PVSCompressedSize, sizeof(long), 1, stream);
	
	fwrite(BSPArrayData::Instance()->pvsData, sizeof(BYTE), BSPArrayData::Instance()->PVSCompressedSize, stream);
	
	//
	// Close the stream after finished
	//
	
	fclose(stream);
}
//==================================================================================================================================
void BSPRenderer::LoadBSP(string filename)
{
	FILE* stream = fopen(filename.c_str(), "rb");
	long a;
	
	//
	// Read the node array
	//
	
	fread(&BSPArrayData::Instance()->lNumberOfNodes, sizeof(long), 1, stream);
	
	BSPArrayData::Instance()->nodeArray = (ZShadeSandboxBSP::BSPNode*)malloc(BSPArrayData::Instance()->lNumberOfNodes * sizeof(ZShadeSandboxBSP::BSPNode));
	
	ZShadeSandboxBSP::BSPNode* n = BSPArrayData::Instance()->nodeArray;
	
	for (a = 0; a < BSPArrayData::Instance()->lNumberOfNodes; a++)
	{
		fread(&n->isLeaf, sizeof(unsigned char), 1, stream);
		fread(&n->planeID, sizeof(unsigned long), 1, stream);
		fread(&n->frontID, sizeof(unsigned long), 1, stream);
		fread(&n->backID, sizeof(unsigned long), 1, stream);
		n++;
	}
	
	//
	// Read the plane array
	//
	
	fread(&BSPArrayData::Instance()->lNumberOfPlanes, sizeof(long), 1, stream);
	
	BSPArrayData::Instance()->planeArray = (ZShadeSandboxBSP::BSPPlane*)malloc(BSPArrayData::Instance()->lNumberOfPlanes * sizeof(ZShadeSandboxBSP::BSPPlane));
	
	fread(BSPArrayData::Instance()->planeArray, sizeof(ZShadeSandboxBSP::BSPPlane), BSPArrayData::Instance()->lNumberOfPlanes, stream);
	
	//
	// Read the leaf array
	//
	
	fread(&BSPArrayData::Instance()->lNumberOfLeaves, sizeof(long), 1, stream);
	
	BSPArrayData::Instance()->leafArray = (ZShadeSandboxBSP::BSPLeaf*)malloc(BSPArrayData::Instance()->lNumberOfLeaves * sizeof(ZShadeSandboxBSP::BSPLeaf));
	
	//fread(BSPArrayData::Instance()->leafArray, sizeof(ZShadeSandboxBSP::BSPLeaf), BSPArrayData::Instance()->lNumberOfLeaves, stream);
	
	ZShadeSandboxBSP::BSPLeaf* l = BSPArrayData::Instance()->leafArray;
	
	for (a = 0; a < BSPArrayData::Instance()->lNumberOfLeaves; a++)
	{
		fread(&l->startPolygon, sizeof(unsigned char), 1, stream);
		fread(&l->endPolygon, sizeof(unsigned long), 1, stream);
		fread(&l->pvsIndex, sizeof(unsigned long), 1, stream);
		fread(&l->boundingBox, sizeof(ZShadeSandboxMath::AABB), 1, stream);
		l++;
	}
	
	//
	// Read the polygon array
	//
	
	fread(&BSPArrayData::Instance()->lNumberOfPolygons, sizeof(long), 1, stream);
	
	BSPArrayData::Instance()->polygonArray = (ZShadeSandboxBSP::BSPPolygon*)malloc(BSPArrayData::Instance()->lNumberOfPolygons * sizeof(ZShadeSandboxBSP::BSPPolygon));
	
	ZShadeSandboxBSP::BSPPolygon* p = BSPArrayData::Instance()->polygonArray;
	
	for (a = 0; a < BSPArrayData::Instance()->lNumberOfPolygons; a++)
	{
		ZShadeSandboxMesh::VertexTex* verts;
		UINT* inds;
		int vertexCount, indexCount;
		XMFLOAT3 normal;
		
		fread(&vertexCount, sizeof(int), 1, stream);
		verts = new ZShadeSandboxMesh::VertexTex[vertexCount];
		fread(&verts, sizeof(ZShadeSandboxMesh::VertexTex), vertexCount, stream);
		fread(&indexCount, sizeof(int), 1, stream);
		inds = new UINT[indexCount];
		fread(&verts, sizeof(UINT), indexCount, stream);
		fread(&normal, sizeof(XMFLOAT3), 1, stream);
		
		vector<ZShadeSandboxMesh::VertexTex> vertices(vertexCount);
		vector<UINT> indices(indexCount);
		
		for (int i = 0; i < vertexCount; i++)
		{
			vertices[i] = verts[i];
		}
		
		for (int i = 0; i < indexCount; i++)
		{
			indices[i] = inds[i];
		}
		
		p->Create(vertices);
		p->CreateIndices(indices);
		
		p->Normal() = ZShadeSandboxMath::XMMath3(normal);
		
		delete verts;
		delete inds;
	}
	
	//
	// Read the PVS array data
	//
	
	fread(&BSPArrayData::Instance()->PVSCompressedSize, sizeof(long), 1, stream);
	
	BSPArrayData::Instance()->pvsData = (BYTE*)malloc(BSPArrayData::Instance()->PVSCompressedSize * sizeof(BYTE));
	
	fread(BSPArrayData::Instance()->pvsData, sizeof(BYTE), BSPArrayData::Instance()->PVSCompressedSize, stream);
	
	//
	// Close the stream after finished
	//
	
	fclose(stream);
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
bool BSPRenderer::LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end)
{
	return LineOfSight(start, end, 0);
}
//==================================================================================================================================
bool BSPRenderer::LineOfSight(ZShadeSandboxMath::XMMath3 start, ZShadeSandboxMath::XMMath3 end, long node)
{
	BSPArrayData* arrayData = BSPArrayData::Instance();
	
	float temp;
	ZShadeSandboxMath::XMMath3 intersection;
	ZShadeSandboxBSP::BSPNode* currentNode = &arrayData->nodeArray[node];
	ZShadeSandboxBSP::BSPPlane* plane = &arrayData->planeArray[currentNode->planeID];
	
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
bool BSPRenderer::CollideSphere(ZShadeSandboxMath::XMMath3 center, float radius)
{
	return CollideSphere(center, radius, 0);
}
//==================================================================================================================================
bool BSPRenderer::CollideSphere(ZShadeSandboxMath::XMMath3 center, float radius, long node)
{
	BSPArrayData* arrayData = BSPArrayData::Instance();

	float temp;
	ZShadeSandboxMath::XMMath3 intersection, sphereRadiusEnd;
	ZShadeSandboxBSP::BSPNode* currentNode = &arrayData->nodeArray[node];
	ZShadeSandboxBSP::BSPPlane* plane = &arrayData->planeArray[currentNode->planeID];

	ZShadeSandboxBSP::EPointPosition::Type pointA = ZShadeSandboxBSP::BSP::ClassifyPoint(center, plane);

	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eBack)
	{
		sphereRadiusEnd = center + (plane->normal * radius);
	}
	else
	{
		ZShadeSandboxMath::XMMath3 n(plane->normal);
		n = -n;
		sphereRadiusEnd = center + (n * radius);
	}

	ZShadeSandboxBSP::EPointPosition::Type pointB = ZShadeSandboxBSP::BSP::ClassifyPoint(sphereRadiusEnd, plane);

	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane && pointB == ZShadeSandboxBSP::EPointPosition::Type::eOnPlane)
	{
		if (currentNode->isLeaf == 0)
		{
			return CollideSphere(center, radius, currentNode->frontID);
		}
		else
		{
			return false;
		}
	}

	// Spanning front to back
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eFront && pointB == ZShadeSandboxBSP::EPointPosition::Type::eBack)
	{
		if (currentNode->backID == -1)
		{
			return true;
		}

		ZShadeSandboxMath::ZMath::GetIntersect(center, sphereRadiusEnd, plane->pointOnPlane, plane->normal, intersection, temp);

		if (currentNode->isLeaf == 0)
		{
			// Not a leaf
			return CollideSphere(center, ZShadeSandboxMath::XMMath3(intersection - center).Length(), currentNode->frontID)
				|| CollideSphere(sphereRadiusEnd, ZShadeSandboxMath::XMMath3(intersection - sphereRadiusEnd).Length(), currentNode->backID);
		}
		else
		{
			// Front leafs are empty
			return CollideSphere(sphereRadiusEnd, ZShadeSandboxMath::XMMath3(intersection - sphereRadiusEnd).Length(), currentNode->backID);
		}
	}

	// Spanning back to front
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eBack && pointB == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		if (currentNode->backID == -1)
		{
			return true;
		}

		ZShadeSandboxMath::ZMath::GetIntersect(sphereRadiusEnd, center, plane->pointOnPlane, plane->normal, intersection, temp);

		if (currentNode->isLeaf == 0)
		{
			// Not a leaf
			return CollideSphere(sphereRadiusEnd, ZShadeSandboxMath::XMMath3(intersection - sphereRadiusEnd).Length(), currentNode->frontID)
				|| CollideSphere(center, ZShadeSandboxMath::XMMath3(intersection - center).Length(), currentNode->backID);
		}
		else
		{
			// Front leafs are empty
			return CollideSphere(center, ZShadeSandboxMath::XMMath3(intersection - center).Length(), currentNode->backID);
		}
	}

	// One of the points is on the plane
	if (pointA == ZShadeSandboxBSP::EPointPosition::Type::eFront && pointB == ZShadeSandboxBSP::EPointPosition::Type::eFront)
	{
		if (currentNode->isLeaf == 0)
		{
			return CollideSphere(center, radius, currentNode->frontID);
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (currentNode->backID == -1)
		{
			return true;
		}
		else
		{
			return CollideSphere(center, radius, currentNode->backID);
		}
	}

	return false;
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
		switch (ZShadeSandboxBSP::BSP::ClassifyPoint(ZShadeSandboxMath::XMMath3(mrp.camera->Position()), &arrayData->planeArray[arrayData->nodeArray[node].planeID]))
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
							currentPolygon = &arrayData->polygonArray[count];
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