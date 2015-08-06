#include "OceanPatchTree.h"
#include "OceanParameters.h"
//==============================================================================================================================
//==============================================================================================================================
OceanPatchTree::OceanPatchTree(D3D* d3d, int oceanSize, int patchSize, float seaLevel)
:   mD3DSystem(d3d)
,   fSeaLevel(seaLevel)
,   iOceanSize(oceanSize)
,   iPatchSize(patchSize + 1)
,   iPatchCount(0)
,   iTotalPatches(0)
,   iTotalPatchID(0)
,   mPatches(0)
{
	InitializeTree();
	CreateTree();
}
//==============================================================================================================================
void OceanPatchTree::InitializeTree()
{
	iTotalPatchID = 0;
	iTotalPatches = (iOceanSize / (iPatchSize - 1)) * (iOceanSize / (iPatchSize - 1));
	iPatchCount = numberOfPatches(iTotalPatches, iPatchSize - 1);
}
//==============================================================================================================================
void OceanPatchTree::CreateTree()
{
	XMFLOAT3 bounds[4];

	bounds[0] = XMFLOAT3(0, fSeaLevel, 0);
	bounds[1] = XMFLOAT3(iOceanSize, fSeaLevel, 0);
	bounds[2] = XMFLOAT3(0, fSeaLevel, iOceanSize);
	bounds[3] = XMFLOAT3(iOceanSize, fSeaLevel, iOceanSize);

	ZShadeSandboxMath::AABB aabb(XMFLOAT3(0, fSeaLevel, 0), XMFLOAT3(iOceanSize, fSeaLevel, iOceanSize));

	mPatches = 0;

	CreatePatch(mPatches, bounds, iOceanSize * iPatchSize, aabb, NULL, 0, 0);
}
//==============================================================================================================================
int OceanPatchTree::numberOfPatches(int leaves, int leafWidth)
{
	int ctr = 0, val = 0;

	while (val == 0)
	{
		ctr += leaves;
		leaves /= leafWidth;

		if (leaves == 0)
			break;

		if (leaves == 1)
			val = 1;
	}

	ctr++;

	return ctr;
}
//==============================================================================================================================
void OceanPatchTree::CreatePatch(OceanPatch*& patch, XMFLOAT3 bounds[4], float width, ZShadeSandboxMath::AABB aabb, OceanPatch* parentPatch, int parentID, int nodeID)
{
	patch = new OceanPatch();

	patch->children[0] = 0;
	patch->children[1] = 0;
	patch->children[2] = 0;
	patch->children[3] = 0;

	float xDiff = bounds[0].x - bounds[1].x;
	float zDiff = bounds[0].z - bounds[2].z;

	int pwidth = (int)abs(xDiff);
	int pdepth = (int)abs(zDiff);

	if (pwidth == iPatchSize - 1)
		patch->isLeaf = true;
	else
		patch->isLeaf = false;

	patch->parent = parentPatch;
	patch->boundary = aabb;
	patch->nodeID = nodeID;
	patch->parentID = parentID;
	patch->width = width;

	// Lower Left
	patch->boundingCoord[0] = XMFLOAT3(bounds[0].x, fSeaLevel, bounds[0].z);
	// Lower Right
	patch->boundingCoord[1] = XMFLOAT3(bounds[1].x, fSeaLevel, bounds[1].z);
	// Upper Left
	patch->boundingCoord[2] = XMFLOAT3(bounds[2].x, fSeaLevel, bounds[2].z);
	// Upper Right
	patch->boundingCoord[3] = XMFLOAT3(bounds[3].x, fSeaLevel, bounds[3].z);

	patch->center = XMFLOAT3(
		((patch->boundingCoord[2].x - patch->boundingCoord[1].x) / 2),
		fSeaLevel,
		((patch->boundingCoord[3].z - patch->boundingCoord[1].z) / 2));

	patch->center.x = abs(patch->center.x);
	patch->center.y = abs(patch->center.y);
	patch->center.z = abs(patch->center.z);

	float halfWidth = ((float)iPatchSize - 1.0f) / 2.0f;

	if (patch->isLeaf)
	{
		patch->mesh = new OceanPatch::MeshAttributes();
		patch->mesh->seaLevel = fSeaLevel;
		patch->mesh->vertexCount = patch->mesh->indexCount = 4;
		int realVertexCount = iPatchSize * iPatchSize;
		patch->mesh->triangleCount = realVertexCount / 3;
		patch->mesh->vertices.resize(patch->mesh->vertexCount);
		patch->mesh->indices.resize(patch->mesh->indexCount);
		patch->mesh->vertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);

		for (int i = 0; i < 4; i++)
		{
			patch->mesh->vertices[i].position = XMFLOAT3(patch->boundingCoord[i].x - halfWidth, patch->boundingCoord[i].y, patch->boundingCoord[i].z - halfWidth);
			patch->mesh->vertices[i].texture = XMFLOAT2(patch->boundingCoord[i].x * (1.0f / (iOceanSize - 1)), patch->boundingCoord[i].z * (1.0f / (iOceanSize - 1)));
		}

		patch->mesh->indices[0] = 1;
		patch->mesh->indices[1] = 0;
		patch->mesh->indices[2] = 3;
		patch->mesh->indices[3] = 2;

		HRESULT result;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		//Setup the description of the static vertex buffer
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = patch->mesh->vertexByteWidth * patch->mesh->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource a pointer to the vertex data
		vertexData.pSysMem = &patch->mesh->vertices[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Create the vertex buffer
		result = mD3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &patch->mesh->vb);
		if (FAILED(result)) return;

		//
		// Create Index Buffer
		//

		//Setup the description of the static index buffer
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.ByteWidth = sizeof(UINT) * patch->mesh->indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		//Give the subresource a pointer to the index data
		indexData.pSysMem = &patch->mesh->indices[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		//Create the index buffer
		result = mD3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &patch->mesh->ib);
		if (FAILED(result)) return;
	}
	else
	{
		//
		// Patch 1
		//

		iTotalPatchID++;
		patch->branches[0] = iTotalPatchID;
		XMFLOAT3 bounds1[4];

		bounds1[0] = XMFLOAT3(bounds[0].x, fSeaLevel, bounds[0].z);
		bounds1[1] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[1].z);
		bounds1[2] = XMFLOAT3(bounds[2].x, fSeaLevel, bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2));
		bounds1[3] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2));

		ZShadeSandboxMath::AABB aabb1(XMFLOAT3(bounds1[0].x - halfWidth, fSeaLevel, bounds1[0].z - halfWidth), XMFLOAT3(bounds1[3].x - halfWidth, fSeaLevel, bounds1[3].z - halfWidth));

		CreatePatch(patch->children[0], bounds1, width / 2, aabb1, patch, nodeID, iTotalPatchID);

		//
		// Patch 2
		//

		iTotalPatchID++;
		patch->branches[1] = iTotalPatchID;
		XMFLOAT3 bounds2[4];

		bounds2[0] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[1].z);
		bounds2[1] = XMFLOAT3(bounds[1].x, fSeaLevel, bounds[1].z);
		bounds2[2] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2));
		bounds2[3] = XMFLOAT3(bounds[1].x, fSeaLevel, bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2));

		ZShadeSandboxMath::AABB aabb2(XMFLOAT3(bounds2[0].x - halfWidth, fSeaLevel, bounds2[0].z - halfWidth), XMFLOAT3(bounds2[3].x - halfWidth, fSeaLevel, bounds2[3].z - halfWidth));

		CreatePatch(patch->children[1], bounds2, width / 2, aabb2, patch, nodeID, iTotalPatchID);

		//
		// Patch 3
		//

		iTotalPatchID++;
		patch->branches[2] = iTotalPatchID;
		XMFLOAT3 bounds3[4];

		bounds3[0] = XMFLOAT3(bounds[0].x, fSeaLevel, bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2));
		bounds3[1] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[0].z + abs((bounds[0].z - bounds[2].z) / 2));
		bounds3[2] = XMFLOAT3(bounds[2].x, fSeaLevel, bounds[2].z);
		bounds3[3] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[3].z);

		ZShadeSandboxMath::AABB aabb3(XMFLOAT3(bounds3[0].x - halfWidth, fSeaLevel, bounds3[0].z - halfWidth), XMFLOAT3(bounds3[3].x - halfWidth, fSeaLevel, bounds3[3].z - halfWidth));

		CreatePatch(patch->children[2], bounds3, width / 2, aabb3, patch, nodeID, iTotalPatchID);

		//
		// Patch 4
		//

		iTotalPatchID++;
		patch->branches[3] = iTotalPatchID;
		XMFLOAT3 bounds4[4];

		bounds4[0] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2));
		bounds4[1] = XMFLOAT3(bounds[3].x, fSeaLevel, bounds[1].z + abs((bounds[1].z - bounds[3].z) / 2));
		bounds4[2] = XMFLOAT3(bounds[0].x + abs((bounds[0].x - bounds[1].x) / 2), fSeaLevel, bounds[3].z);
		bounds4[3] = XMFLOAT3(bounds[3].x, fSeaLevel, bounds[3].z);

		ZShadeSandboxMath::AABB aabb4(XMFLOAT3(bounds4[0].x - halfWidth, fSeaLevel, bounds4[0].z - halfWidth), XMFLOAT3(bounds4[3].x - halfWidth, fSeaLevel, bounds4[3].z - halfWidth));

		CreatePatch(patch->children[3], bounds4, width / 2, aabb4, patch, nodeID, iTotalPatchID);
	}
}
//==============================================================================================================================
