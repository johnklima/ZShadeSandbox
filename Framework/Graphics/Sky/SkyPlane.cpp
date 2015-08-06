#include "SkyPlane.h"
//==============================================================================================================================
//==============================================================================================================================
SkyPlane::SkyPlane(D3D* d3d, SkyPlaneParameters spp)
:   m_D3DSystem(d3d)
,   m_Translation(0.0f)
,   m_Wireframe(false)
,   mTriangleCount(0)
{
	mSkyPlaneShader = new SkyPlaneShader(m_D3DSystem);
	
	mSkyPlaneParameters = spp;
	
	mCloudTexture = new Texture(m_D3DSystem);
	mCloudTexture->Initialize(spp.cloudTexturePath, spp.cloudTextureName);
	
	mCloudPerturbTexture = new Texture(m_D3DSystem);
	mCloudPerturbTexture->Initialize(spp.cloudTexturePath, spp.cloudPerturbTextureName);
	
	BuildSkyPlane();
	Initialize();
	CreateBuffers();
	
	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
}
//==============================================================================================================================
void SkyPlane::Update(float dt)
{
	// Increment the texture translation value each frame.
	m_Translation += 0.0005f;
	if (m_Translation > 1.0f)
	{
		m_Translation -= 1.0f;
	}
}
//==============================================================================================================================
void SkyPlane::SetWireframe(bool wire)
{
	m_Wireframe = wire;
}
//==============================================================================================================================
void SkyPlane::BuildSkyPlane()
{
	float quadSize, radius, constant, textureDelta;
	int i, j, index;
	float positionX, positionY, positionZ, tu, tv;
	float skyPlaneRes = mSkyPlaneParameters.g_skyPlaneResolution;

	int size = (skyPlaneRes + 1) * (skyPlaneRes + 1);

	mSkyPlane = new ZShadeSandboxMesh::VertexTex[size];

	// Determine the size of each quad on the sky plane.
	quadSize = mSkyPlaneParameters.g_skyPlaneWidth / (float)skyPlaneRes;

	// Calculate the radius of the sky plane based on the width.
	radius = mSkyPlaneParameters.g_skyPlaneWidth / 2.0f;

	// Calculate the height constant to increment by.
	constant = (mSkyPlaneParameters.g_skyPlaneTop - mSkyPlaneParameters.g_skyPlaneBottom) / (radius * radius);

	// Calculate the texture coordinate increment value.
	textureDelta = (float)mSkyPlaneParameters.g_textureRepeat / (float)skyPlaneRes;

	// Loop through the sky plane and build the coordinates based on the increment values given.
	for (j = 0; j <= skyPlaneRes; j++)
	{
		for (i = 0; i <= skyPlaneRes; i++)
		{
			// Calculate the vertex coordinates.
			positionX = (-0.5f * mSkyPlaneParameters.g_skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * mSkyPlaneParameters.g_skyPlaneWidth) + ((float)j * quadSize);
			positionY = mSkyPlaneParameters.g_skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// Calculate the texture coordinates.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// Calculate the index into the sky plane array to add this coordinate.
			index = j * (skyPlaneRes + 1) + i;

			// Add the coordinates to the sky plane array.
			mSkyPlane[index].position.x = positionX;
			mSkyPlane[index].position.y = positionY;
			mSkyPlane[index].position.z = positionZ;
			mSkyPlane[index].texture.x = tu;
			mSkyPlane[index].texture.y = tv;
		}
	}
}
//==============================================================================================================================
void SkyPlane::Initialize()
{
	int index, index1, index2, index3, index4;
	float skyPlaneRes = mSkyPlaneParameters.g_skyPlaneResolution;

	mVertexCount = (skyPlaneRes + 1) * (skyPlaneRes + 1) * 6;
	mIndexCount = mVertexCount;
	
	mTriangleCount = mVertexCount / 3;
	
	mIndices.resize(mIndexCount);

	index = 0;

	mVerticesTex.resize(mVertexCount);
	mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);

	for (int j = 0; j < skyPlaneRes; j++)
	{
		for (int i = 0; i < skyPlaneRes; i++)
		{
			index1 = j * (skyPlaneRes + 1) + i;
			index2 = j * (skyPlaneRes + 1) + (i + 1);
			index3 = (j + 1) * (skyPlaneRes + 1) + i;
			index4 = (j + 1) * (skyPlaneRes + 1) + (i + 1);

			ZShadeSandboxMesh::VertexTex v1 = mSkyPlane[index1];
			ZShadeSandboxMesh::VertexTex v2 = mSkyPlane[index2];
			ZShadeSandboxMesh::VertexTex v3 = mSkyPlane[index3];
			ZShadeSandboxMesh::VertexTex v4 = mSkyPlane[index4];

			// Upper Left Triangle
			mVerticesTex[index] = v1;
			mIndices[index] = index;
			index++;

			// Upper Right Triangle
			mVerticesTex[index] = v2;
			mIndices[index] = index;
			index++;

			// Bottom Left Triangle
			mVerticesTex[index] = v3;
			mIndices[index] = index;
			index++;

			// Bottom Left Triangle
			mVerticesTex[index] = v3;
			mIndices[index] = index;
			index++;

			// Upper Right Triangle
			mVerticesTex[index] = v2;
			mIndices[index] = index;
			index++;

			// Bottom Right Triangle
			mVerticesTex[index] = v4;
			mIndices[index] = index;
			index++;
		}
	}
}
//==============================================================================================================================
void SkyPlane::CreateBuffers()
{
	//
	// Create Vertex Buffer
	//

	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = mVertexByteWidth * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the vertex data
	vertexData.pSysMem = &mVerticesTex[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = m_D3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &pVB);
	if (FAILED(result)) return;

	//
	// Create Index Buffer
	//

	mIndexCount = mIndices.size();

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
	result = m_D3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &pIB);
	if (FAILED(result)) return;
}
//==============================================================================================================================
void SkyPlane::Render(D3D* d3d, Camera* camera, float planeHeight)
{
	// Enable additive blending so the clouds blend with the sky dome color.
	
	RenderBuffers();
	
	XMFLOAT3 eyeVertex = camera->Position();
	XMMATRIX translate = XMMatrixTranslation(0.0f, 200.0f, 0.0f);
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	world = XMMatrixMultiply(world, translate);
	mSkyPlaneShader->Wireframe() = m_Wireframe;
	//mSkyPlaneShader->SetWireframe(m_Wireframe);
	//mSkyPlaneShader->UseCustomWorld(true);
	//mSkyPlaneShader->SetCustomWorld(world);
	
	mSkyPlaneShader->Render11(
		mIndexCount,
		world,
		ZShadeSandboxMath::XMMath4(0, 0, 0, 0),
		camera,
		mCloudTexture->getTexture11(),
		mCloudPerturbTexture->getTexture11(),
		m_Translation,
		mSkyPlaneParameters.g_Scale,
		mSkyPlaneParameters.g_Brightness
	);
}
//==============================================================================================================================
void SkyPlane::RenderWithReflection(D3D* d3d, Camera* camera, float planeHeight)
{
	/*
	RenderBuffers();

	XMFLOAT3 eyeVertex = camera->Position();
	//Invert the Y coordinate of the camera around the water plane height for the reflected camera position
	eyeVertex.y = eyeVertex.y + (planeHeight * 2.0f);
	XMMATRIX translate = XMMatrixTranslation(eyeVertex.x, eyeVertex.y + 4.0f, eyeVertex.z);
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	world = XMMatrixMultiply(world, translate);
	mSkyPlaneShader->SetWireframe(m_Wireframe);
	mSkyPlaneShader->UseCustomWorld(true);
	mSkyPlaneShader->SetCustomWorld(world);
	mSkyPlaneShader->UseCustomView(true);
	mSkyPlaneShader->SetCustomView(camera->ReflectionView4x4());

	mSkyPlaneShader->Render11(mIndexCount, XMMath4(0, 0, 0, 0), camera, mCloudTexture->getTexture11(), mCloudPerturbTexture->getTexture11(), m_Translation);
	*/
}
//==============================================================================================================================
void SkyPlane::RenderBuffers()
{
	unsigned int stride;
	unsigned int offset;

	//Set the vertex buffer stride and offset
	stride = mVertexByteWidth;
	offset = 0;

	m_D3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	m_D3DSystem->GetDeviceContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

	m_D3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
//==============================================================================================================================