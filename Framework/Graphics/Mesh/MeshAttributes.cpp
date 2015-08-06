#include "MeshAttributes.h"
using ZShadeSandboxMesh::MeshAttributes;
//===============================================================================================================================
//===============================================================================================================================
MeshAttributes::MeshAttributes()
{
	Init();
}
//===============================================================================================================================
void MeshAttributes::Release()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
	SAFE_RELEASE(mInstanceBuffer);
}
//===============================================================================================================================
void MeshAttributes::Init()
{
	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
	mRotation = XMFLOAT3(0, 0, 0);
	mVertexCount = 0;
	mIndexCount = 0;
	mTriangleCount = 0;
	mInstanceCount = 0;
	mVertexByteWidth = 0;
	mInstanceByteWidth = 0;
	mVertexBuffer = 0;
	mIndexBuffer = 0;
	mInstanceBuffer = 0;
	mD3D = 0;
}
//===============================================================================================================================
void MeshAttributes::Subdivide(ZShadeSandboxMesh::EVertexType::Type type)
{
	switch (type)
	{
		case ZShadeSandboxMesh::EVertexType::Type::VT_Pos: SubdivideVertex<ZShadeSandboxMesh::VertexPos>(mVerticesPos); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_Color: SubdivideVertex<ZShadeSandboxMesh::VertexColor>(mVerticesColor); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_Tex: SubdivideVertex<ZShadeSandboxMesh::VertexTex>(mVerticesTex); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTex: SubdivideVertex<ZShadeSandboxMesh::VertexNormalTex>(mVerticesNormalTex); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTan: SubdivideVertex<ZShadeSandboxMesh::VertexNormalTexTan>(mVerticesNormalTexTan); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTanBi: SubdivideVertex<ZShadeSandboxMesh::VertexNormalTexTanBiTan>(mVerticesNormalTexTanBi); break;
		default:
		{
		   //The shader was not defined
		   ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Vertex Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
void MeshAttributes::BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type type)
{
	switch (type)
	{
		case ZShadeSandboxMesh::EVertexType::Type::VT_Pos: BuildVertexBuffer<ZShadeSandboxMesh::VertexPos>(mVerticesPos); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_Color: BuildVertexBuffer<ZShadeSandboxMesh::VertexColor>(mVerticesColor); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_Tex: BuildVertexBuffer<ZShadeSandboxMesh::VertexTex>(mVerticesTex); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTex: BuildVertexBuffer<ZShadeSandboxMesh::VertexNormalTex>(mVerticesNormalTex); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTan: BuildVertexBuffer<ZShadeSandboxMesh::VertexNormalTexTan>(mVerticesNormalTexTan); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTanBi: BuildVertexBuffer<ZShadeSandboxMesh::VertexNormalTexTanBiTan>(mVerticesNormalTexTanBi); break;
		default:
		{
		   //The shader was not defined
		   ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Vertex Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
void MeshAttributes::ReBuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type type)
{
	switch (type)
	{
		case ZShadeSandboxMesh::EVertexType::Type::VT_Pos: ReBuildVertexBuffer<ZShadeSandboxMesh::VertexPos>(mVerticesPos); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_Color: ReBuildVertexBuffer<ZShadeSandboxMesh::VertexColor>(mVerticesColor); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_Tex: ReBuildVertexBuffer<ZShadeSandboxMesh::VertexTex>(mVerticesTex); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTex: ReBuildVertexBuffer<ZShadeSandboxMesh::VertexNormalTex>(mVerticesNormalTex); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTan: ReBuildVertexBuffer<ZShadeSandboxMesh::VertexNormalTexTan>(mVerticesNormalTexTan); break;
		case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTanBi: ReBuildVertexBuffer<ZShadeSandboxMesh::VertexNormalTexTanBiTan>(mVerticesNormalTexTanBi); break;
		default:
		{
		   //The shader was not defined
		   ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Vertex Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
XMMATRIX MeshAttributes::WorldXM()
{
	XMMATRIX scale, rx, ry, rz, translate;

	scale = XMMatrixScaling( mScale.x, mScale.y, mScale.z );
	rx = XMMatrixRotationX( mRotation.x );
	ry = XMMatrixRotationY( mRotation.y );
	rz = XMMatrixRotationZ( mRotation.z );
	translate = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	XMMATRIX world = XMMatrixIdentity();
	
	world = XMMatrixMultiply( world, scale );
	world = XMMatrixMultiply( world, rx );
	world = XMMatrixMultiply( world, ry );
	world = XMMatrixMultiply( world, rz );
	world = XMMatrixMultiply( world, translate );

	if (mInstancePos.size() > 0)
	{
		world = XMMatrixIdentity();
		world = XMMatrixMultiply(world, scale);
		world = XMMatrixMultiply(world, rx);
		world = XMMatrixMultiply(world, ry);
		world = XMMatrixMultiply(world, rz);
		world = XMMatrixMultiply(world, XMMatrixTranslation(0, 0, 0));
	}

	return world;
}
//===============================================================================================================================
XMMATRIX MeshAttributes::BillboardWorldXM(XMFLOAT3 eye)
{
	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	float angle = atan2(mPosition.x - eye.x, mPosition.z - eye.z) * (180.0 / PI);
	
	// ZShadeSandboxGlobal::Convert rotation into radians.
	float rotation = (float)angle * 0.0174532925f;
	
	//
	//Use the rotation to first rotate the world matrix accordingly, and then translate to the position of the billboard in the world.
	//
	
	// Setup the rotation the billboard at the origin using the world matrix.
	//D3DXMatrixRotationY(&worldMatrix, rotation);
	XMMATRIX billboardWorld = XMMatrixRotationY( rotation );
	
	// Setup the translation matrix from the billboard model.
	//D3DXMatrixTranslation(&translateMatrix, modelposition.x, modelposition.y, modelposition.z);
	XMMATRIX mBillboardTrans = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	
	//Perform a scale for the billboard matrix
	XMMATRIX scale = XMMatrixScaling( mScale.x, mScale.y, mScale.z );
	
	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	//D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);
	return scale * billboardWorld * mBillboardTrans;
}
//===============================================================================================================================
void MeshAttributes::ReplyMeshIndices(vector<UINT> ind)
{
	mIndexCount = ind.size();
	
	for (UINT i = 0; i < mIndexCount; i++)
	{
		mIndices.push_back(ind[i]);
	}
}
//===============================================================================================================================
void MeshAttributes::SetBuffers(ZShadeSandboxMesh::ERenderType::Type renderType, bool useIndexBuffer)
{
	unsigned int stride;
	unsigned int offset;
	
	//Set the vertex buffer stride and offset for the vertex buffer and instance buffer
	stride = mVertexByteWidth;
	offset = 0;
	
	mD3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	
	if (useIndexBuffer)
	{
		//Set the index buffer to active in the input assembler so it can be rendered
		mD3D->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}
	
	//Set the type of primitive that should be rendered from this vertex buffer
	switch (renderType)
	{
		case ZShadeSandboxMesh::ERenderType::eTriangleList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
		case ZShadeSandboxMesh::ERenderType::ePointList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST); break;
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST); break;
		default:
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Render Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
void MeshAttributes::SetBuffersInstanced(ZShadeSandboxMesh::ERenderType::Type renderType, bool useIndexBuffer)
{
	unsigned int strides[2];
	unsigned int offsets[2];

	//Set the vertex buffer stride and offset for the vertex buffer and instance buffer
	strides[0] = mVertexByteWidth;
	offsets[0] = 0;
	strides[1] = mInstanceByteWidth;
	offsets[1] = 0;

	ID3D11Buffer* buf_arr[2] = { mVertexBuffer, mInstanceBuffer };

	mD3D->GetDeviceContext()->IASetVertexBuffers(0, 2, buf_arr, strides, offsets);

	if (useIndexBuffer)
	{
		//Set the index buffer to active in the input assembler so it can be rendered
		mD3D->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	//Set the type of primitive that should be rendered from this vertex buffer
	switch (renderType)
	{
		case ZShadeSandboxMesh::ERenderType::eTriangleList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
		case ZShadeSandboxMesh::ERenderType::ePointList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST); break;
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList: mD3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST); break;
		default:
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Render Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
void MeshAttributes::AddInstancePositions(vector<XMFLOAT3> v)
{
	bool used = false;
	
	if (mInstancePos.size() > 0)
	{
		used = true;
		mInstancePos.clear();
	}
	
	mInstanceCount = v.size();
	mInstancePos.resize(mInstanceCount);
	mInstanceByteWidth = sizeof(ZShadeSandboxMesh::InstancePos);
	
	for (int i = 0; i < mInstanceCount; i++)
	{
		mInstancePos[i].position = v[i];
	}
	
	(used) ? ReBuildInstanceBuffer() : BuildInstanceBuffer();
}
//===============================================================================================================================
void MeshAttributes::BuildIndexBuffer()
{
	HRESULT result;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	//Give the subresource a pointer to the index data
	indexData.pSysMem = &mIndices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	//Create the index buffer
	result = mD3D->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result)) return;
}
//===============================================================================================================================
void MeshAttributes::BuildInstanceBuffer()
{
	if (mInstancePos.size() > 0)
	{
		HRESULT result;
		D3D11_BUFFER_DESC instanceBufferDesc;
		D3D11_SUBRESOURCE_DATA instanceData;
		
		mInstanceByteWidth = sizeof(ZShadeSandboxMesh::InstancePos);
		
		// Setup the description of the instance buffer
		instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		instanceBufferDesc.ByteWidth = mInstanceByteWidth * mInstanceCount;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = 0;
		instanceBufferDesc.MiscFlags = 0;
		instanceBufferDesc.StructureByteStride = 0;
		
		// Give the subresource a pointer to the instance data
		instanceData.pSysMem = &mInstancePos[0];
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;
		
		// Create the instance buffer
		result = mD3D->GetDevice11()->CreateBuffer(&instanceBufferDesc, &instanceData, &mInstanceBuffer);
		if (FAILED(result)) return;
	}
}
//===============================================================================================================================
void MeshAttributes::ReBuildInstanceBuffer()
{
	D3D11_MAPPED_SUBRESOURCE resource;
	mD3D->GetDeviceContext()->Map(mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &mInstancePos[0], mInstanceByteWidth * mInstanceCount);
	mD3D->GetDeviceContext()->Unmap(mInstanceBuffer, 0);
}
//===============================================================================================================================