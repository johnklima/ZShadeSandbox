#include "LakeBed.h"
#include "TextureManager.h"
#include "BetterString.h"
//==============================================================================================================================
//==============================================================================================================================
LakeBed::LakeBed(D3D* d3d, float width, float height, float depth)
:   m_D3DSystem(d3d)
,	fWidth(width)
,   fHeight(height)
,   fDepth(depth)
{
	Initialize();
	CreateBuffers();
	
	mShader = new ZShadeSandboxShader::LakeBedShader(d3d);
	mShadowMapShader = new ShadowMapBuildShader(d3d);
	
	vector<string> names;
	names.push_back("CausticsRender_001.png");
	names.push_back("CausticsRender_002.png");
	names.push_back("CausticsRender_003.png");
	names.push_back("CausticsRender_004.png");
	names.push_back("CausticsRender_005.png");
	names.push_back("CausticsRender_006.png");
	names.push_back("CausticsRender_007.png");
	names.push_back("CausticsRender_008.png");
	names.push_back("CausticsRender_009.png");
	names.push_back("CausticsRender_010.png");
	names.push_back("CausticsRender_011.png");
	names.push_back("CausticsRender_012.png");
	names.push_back("CausticsRender_013.png");
	names.push_back("CausticsRender_014.png");
	names.push_back("CausticsRender_015.png");
	names.push_back("CausticsRender_016.png");
	mCausticAnimation = new TextureAnimation3D(d3d, "Textures\\CausticAnimation", names);

	mPosition = XMFLOAT3(0, -30, 0);
	mRotation = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
}
//==============================================================================================================================
void LakeBed::LoadDiffuseMap(string filename)
{
	m_DiffuseMap = TextureManager::Instance()->GetTexture(BetterString(filename));
}
//==============================================================================================================================
void LakeBed::LoadWaveMap0(string filename)
{
	m_WaveMap0 = TextureManager::Instance()->GetTexture(BetterString(filename));
}
//==============================================================================================================================
void LakeBed::LoadWaveMap1(string filename)
{
	m_WaveMap1 = TextureManager::Instance()->GetTexture(BetterString(filename));
}
//==============================================================================================================================
void LakeBed::LoadMicroBump0(string filename)
{
	g_MicroBump0 = TextureManager::Instance()->GetTexture(BetterString(filename));
}
//==============================================================================================================================
void LakeBed::LoadMicroBump1(string filename)
{
	g_MicroBump1 = TextureManager::Instance()->GetTexture(BetterString(filename));
}
//==============================================================================================================================
void LakeBed::LoadDepthMap(string filename)
{
	g_DepthMap = TextureManager::Instance()->GetTexture(BetterString(filename));
}
//==============================================================================================================================
void LakeBed::Update(float dt)
{
	mCausticAnimation->Update(0.00005f * dt);

	fCausticTimer += 0.005f * dt;
	//
	//if (fCausticTimer > 1.0f)
	//{
	//	fCausticTimer -= 1.0f;
	//}
}
//==============================================================================================================================
void LakeBed::Render(Camera* camera, LightCamera* lightCamera, bool renderDeferred, bool shadow, XMFLOAT4 clipplane, bool reflect)
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
	
	RenderBuffers();
	
	if (shadow)
	{
		mShadowMapShader->Render(mIndexCount, world, lightCamera);
	}
	else
	{
		// Set shader attributes
		mShader->SetDiffuseMap(m_DiffuseMap);
		mShader->SetWaveMap0(m_WaveMap0);
		mShader->SetWaveMap1(m_WaveMap1);
		mShader->SetMicroBump0(g_MicroBump0);
		mShader->SetMicroBump1(g_MicroBump1);
		mShader->SetDepthMap(g_DepthMap);
		
		// Get the current caustic animation texture
		mShader->SetCausticTexture(mCausticAnimation->CurrentTextureSRV());

		mShader->Render(mIndexCount, fCausticTimer, camera, lightCamera, world, clipplane, renderDeferred, reflect);
	}
}
//==============================================================================================================================
void LakeBed::RenderBuffers()
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
void LakeBed::Initialize()
{
	UINT vertexCount = fWidth * fHeight;
	UINT faceCount = (fWidth - 1) * (fHeight - 1) * 2;
	
	float halfWidth = 0.5f * fWidth;
	float halfDepth = 0.5f * fDepth;
	
	float dx = fWidth / (fWidth - 1);
	float dz = fDepth / (fHeight - 1);
	
	float du = 1.0f / (fWidth - 1);
	float dv = 1.0f / (fHeight - 1);
	
	//
	// Load Vertices
	//
	
	mVertexCount = vertexCount;
	mTriangleCount = mVertexCount / 3;
	
	mVerticesTex.resize(mVertexCount);
	
	for (UINT i = 0; i < fHeight; ++i)
	{
		float z = halfDepth - i * dz;
		
		for (UINT j = 0; j < fWidth; ++j)
		{
			float x = -halfWidth + j * dx;
			
			mVerticesTex[i * fWidth + j].position = XMFLOAT3(x, 0, z);
			
			// Stretch the texture over the grid
			mVerticesTex[i * fWidth + j].texture.x = j * du;
			mVerticesTex[i * fWidth + j].texture.y = i * du;
		}
	}
	
	mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);
	
	//
	// Load Indices
	//
	
	mIndexCount = faceCount * 3;
	mIndices.resize( mIndexCount );
	
	// Iterate over each quad and compute indices.
	UINT k = 0;
	
	for (UINT i = 0; i < fHeight - 1; ++i)
	{
		for (UINT j = 0; j < fWidth - 1; ++j)
		{
			mIndices[ k ]     = i * fWidth + j;
			mIndices[ k + 1 ] = i * fWidth + j + 1;
			mIndices[ k + 2 ] = (i + 1) * fWidth + j;
			
			mIndices[ k + 3 ] = (i + 1) * fWidth + j;
			mIndices[ k + 4 ] = i * fWidth + j + 1;
			mIndices[ k + 5 ] = (i + 1) * fWidth + j + 1;

			k += 6; // next quad
		}
	}
}
//==============================================================================================================================
void LakeBed::CreateBuffers()
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
void LakeBed::SetWireframe(bool wire)
{
	mShader->Wireframe() = wire;
}
//==============================================================================================================================