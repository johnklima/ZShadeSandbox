#include "Tree.h"
#include "TextureManager.h"
//==================================================================================================================================
//==================================================================================================================================
Tree::Tree(D3D* d3d, TreeParameters tp)
:	mD3DSystem(d3d)
{
	mTreeParameters = new TreeParameters();
	mTreeParameters->treeCount = tp.treeCount;
	mTreeParameters->leafTextureName = tp.leafTextureName;
	mTreeParameters->treeModelName = tp.treeModelName;
	mTreeParameters->gameDirectory3D = tp.gameDirectory3D;
	
	Init();
}
//==================================================================================================================================
void Tree::Init()
{
	mShader = new TreeShader(mD3DSystem);
	
	CreateLeaves();
	CreateMesh();
}
//==================================================================================================================================
void Tree::CreateLeaves()
{
	mLeafAttributes = new ZShadeSandboxMesh::MeshAttributes();
	mLeafAttributes->mD3D = mD3DSystem;
	mLeafAttributes->mVertexCount = 4;
	mLeafAttributes->mIndexCount = 6;
	mLeafAttributes->mTriangleCount = 2;
	mLeafAttributes->mVerticesNormalTexTan.resize(mLeafAttributes->mVertexCount);
	mLeafAttributes->mVerticesNormalTexTan[0] = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	mLeafAttributes->mVerticesNormalTexTan[1] = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	mLeafAttributes->mVerticesNormalTexTan[2] = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	mLeafAttributes->mVerticesNormalTexTan[3] = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	mLeafAttributes->mIndices.resize(mLeafAttributes->mIndexCount);
	mLeafAttributes->mIndices[0] = 0;
	mLeafAttributes->mIndices[1] = 1;
	mLeafAttributes->mIndices[2] = 2;
	mLeafAttributes->mIndices[3] = 0;
	mLeafAttributes->mIndices[4] = 2;
	mLeafAttributes->mIndices[5] = 3;
	mLeafAttributes->mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTexTan);
	mLeafAttributes->BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTan);
	mLeafAttributes->BuildIndexBuffer();
	
	mLeafTexture = TextureManager::Instance()->GetTexture(mTreeParameters->leafTextureName);
	
	// Here we create the leaf world matrices, that will be the leafs
	// position and orientation on the tree each individual tree. We will create an array of matrices
	// for the leaves that we will send to the shaders in the cbPerInstance constant buffer
	// This matrix array is used "per tree", so that each tree gets the exact same number of leaves,
	// with the same orientation, position, and scale as all of the other trees
	// Start by initializing the matrix array
	ZShadeSandboxMath::ZMath::RandomSeed();
	XMFLOAT3 fTPos;
	XMMATRIX rotationMatrix;
	XMMATRIX tempMatrix, scale, translation;
	XMVECTOR tempPos;
	InternalTreeRenderParameters itrp;
	
	for (int i = 0; i < NUM_LEAVES_PER_TREE; i++)
	{
		float rotX = (rand() % 2000) / 500.0f;
		float rotY = (rand() % 2000) / 500.0f;
		float rotZ = (rand() % 2000) / 500.0f;
		
		// the rand() function is slightly more biased towards lower numbers, which would make the center of
		// the leaf "mass" be more dense with leaves than the outside of the "sphere" of leaves we are making.
		// We want the outside of the "sphere" of leaves to be more dense than the inside, so the way we do this
		// is getting a distance value between 0 and 4, we then subtract that value from 6, so that the very center
		// does not have any leaves. then below you can see we are checking to see if the distance is greater than 4
		// (because the tree branches are approximately 4 units radius from the center of the tree). If the distance
		// is greater than 4, then we set it at 4, which will make the edge of the "sphere" of leaves more densly
		// populated than the center of the leaf mass
		float distFromCenter = 6.0f - ((rand() % 1000) / 250.0f);	
		
		if (distFromCenter > 4.0f)
		{
			distFromCenter = 4.0f;
		}
		
		// Now we create a vector with the length of distFromCenter, by simply setting it's x component as distFromCenter.
		// We will now rotate the vector, which will give us the "sphere" of leaves after we have rotated all the leaves.
		// We do not want a perfect sphere, more like a half sphere to cover the branches, so we check to see if the y
		// value is less than -1.0f (giving us slightly more than half a sphere), and if it is, negate it so it is reflected
		// across the xz plane
		tempPos = XMVectorSet(distFromCenter, 0.0f, 0.0f, 0.0f);
		rotationMatrix = XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ);
		tempPos = XMVector3TransformCoord(tempPos, rotationMatrix );
		
		if (XMVectorGetY(tempPos) < -1.0f)
		{
			tempPos = XMVectorSetY(tempPos, -XMVectorGetY(tempPos));
		}
		
		// Now we create our leaves "tree" matrix (this is not the leaves "world matrix", because we are not
		// defining the leaves position, orientation, and scale in world space, but instead in "tree" space
		XMStoreFloat3(&fTPos, tempPos);
		
		scale = XMMatrixScaling( 0.25f, 0.25f, 0.25f );
		translation = XMMatrixTranslation(fTPos.x, fTPos.y + 8.0f, fTPos.z );
		tempMatrix = scale * rotationMatrix * translation;
		
		// To make things simple, we just store the matrix directly into our cbPerInst structure
		itrp.g_LeafOnTree[i] = tempMatrix;
	}
	
	// Finally create the leaf space matrix array
	mShader->InitializeLeafBuffer(itrp);
}
//==================================================================================================================================
void Tree::CreateMesh()
{
	mTree = new ZShadeSandboxMesh::OBJMesh(mD3DSystem, mTreeParameters->gameDirectory3D);
	mTree->Load(mTreeParameters->treeModelName, false, false);
	
	// Setup the tree instance positions
	std::vector<XMFLOAT3> inst(mTreeParameters->treeCount);
	XMVECTOR tempPos;
	ZShadeSandboxMath::ZMath::RandomSeed();
	
	// We are just creating random positions for the trees, between the positions of (-100, 0, -100) to (100, 0, 100)
	// then storing the position in our instanceData array
	for(int i = 0; i < mTreeParameters->treeCount; i++)
	{
		float randX = ((float)(rand() % 2000) / 10) - 100;
		float randZ = ((float)(rand() % 2000) / 10) - 100;
		
		tempPos = XMVectorSet(randX, 0.0f, randZ, 0.0f);
		
		XMStoreFloat3(&inst[i], tempPos);
	}
	
	// Set these instance positions into the obj model for the tree
	mTree->AddInstancePositions(inst);
	mLeafAttributes->AddInstancePositions(inst);
}
//==================================================================================================================================
void Tree::Render(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	if (mrp.camera == NULL) return;
	
	mrp.useInstancing = true;
	
	//
	// Draw the leaves
	//
	
	if (mrp.useInstancing)
	{
		mLeafAttributes->SetBuffersInstanced(mrp.renderType);
		mrp.world = XMMatrixIdentity();
	}
	else
	{
		mLeafAttributes->SetBuffers(mrp.renderType);
		mrp.world = mLeafAttributes->WorldXM();
	}
	
	mShader->RenderLeaves(mLeafAttributes->mIndexCount, NUM_LEAVES_PER_TREE * mTreeParameters->treeCount, mrp, mLeafTexture);
	
	//
	// Draw the tree model
	//
	
	mTree->Render(mrp);
}
//==================================================================================================================================