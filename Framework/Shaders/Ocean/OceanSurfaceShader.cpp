#include "OceanSurfaceShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
#include "TextureManager.h"
//==============================================================================================================================
OceanSurfaceShader::OceanSurfaceShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OceanSurfaceShader::OceanSurfaceShader(const OceanSurfaceShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OceanSurfaceShader::~OceanSurfaceShader()
{
}
//==============================================================================================================================
bool OceanSurfaceShader::Initialize()
{
	float g_SkyBlending = 16.0f;
	DWORD* buffer = new DWORD[FRESNEL_TEX_SIZE];
	
	//<JPK> this is dumbass, but XMFresnelTerm is not a DWORD
	// Using water's refraction index 1.33
	DWORD fresnel[FRESNEL_TEX_SIZE] = {
		255, 248, 242, 236, 231, 225, 220, 214, 209, 204, 199, 194, 190, 185,
		181, 176, 172, 168, 164, 160, 157, 153, 149, 146, 142, 139, 136, 132,
		129, 126, 123, 121, 118, 115, 112, 110, 107, 105, 102, 100, 98, 95, 93,
		91, 89, 87, 85, 83, 81, 79, 77, 76, 74, 72, 71, 69, 67, 66, 64, 63, 62,
		60, 59, 57, 56, 55, 54, 53, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
		40, 39, 38, 38, 37, 36, 35, 34, 34, 33, 32, 32, 31, 30, 30, 29, 28, 28,
		27, 27, 26, 26, 25, 24, 24, 23, 23, 23, 22, 22, 21, 21, 20, 20, 20, 19,
		19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 14,
		13, 13, 13, 13, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 10, 10, 10, 10,
		10, 10, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5
	};
	
	for (int i = 0; i < FRESNEL_TEX_SIZE; i++)
	{
		float cos_a = i / (FLOAT)FRESNEL_TEX_SIZE;
		DWORD sky_blend = (DWORD)(powf(1 / (1 + cos_a), g_SkyBlending) * 255);
		buffer[i] = (sky_blend << 8) | fresnel[i];
	}
	
	TextureManager::Instance()->Create1DTexture(g_pFresnelMap, g_pSRV_Fresnel, FRESNEL_TEX_SIZE, buffer);
	
	SAFE_DELETE_ARRAY(buffer);
	
	ConstantBuffer<cbPerFrameBuffer> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(cbPerFrameBuffer)));
	m_pPerFrameCB = perFrameCB.Buffer();
	
	ConstantBuffer<cbTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(cbTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("OceanSurfaceShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	LoadVertexShader("OceanSurfaceVS");
	LoadHullShader("OceanSurfaceHS");
	LoadDomainShader("OceanSurfaceDS");
	LoadPixelShader("OceanSurfacePS");
	LoadPixelShader("OceanSurfaceWireframePS");
	AssignVertexShaderLayout("OceanSurfaceShader");
	
	mTexDisplacement = 0;
	mTexPerlin = 0;
	mTexGradient = 0;
	mWaveMap0 = 0;
	mWaveMap1 = 0;
	mReflectionMap = 0;
	mRefractionMap = 0;
	
	// This should read the texture filename elsewhere so that it can be updated by an editor
	m_pSRV_ReflectCube = TextureManager::Instance()->GetTexture("Textures\\reflect_cube.dds");
	assert(m_pSRV_ReflectCube);
	
	return true;
}
//==============================================================================================================================
void OceanSurfaceShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OceanSurfaceShader::Render11( int indexCount, Camera* camera, int frustumCull, XMMATRIX world, OceanParameters ocean_params, XMFLOAT2 bottom_left)
{
	cbPerFrameBuffer cFB;
	cbTessellationBuffer cTB;
	cbMatrixBuffer cMB;
	
	cTB.g_EyePos = camera->Position();
	cTB.g_FrustumCull = frustumCull;
	cTB.g_MinDist = ocean_params.g_MinDist;
	cTB.g_MaxDist = ocean_params.g_MaxDist;
	cTB.g_MinTess = ocean_params.g_MinTess;
	cTB.g_MaxTess = ocean_params.g_MaxTess;
	// Map tessellation constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pTessellationCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbTessellationBuffer*)mapped_res.pData = cTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pTessellationCB, 0);
	}
	
	cFB.g_padding = XMFLOAT2(0, 0);
	cFB.g_SeaLevel = ocean_params.g_SeaLevel;
	cFB.g_WaveHeight = 1.0f;// ocean_params.g_WaveHeight;
	cFB.g_LightDirection = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	cFB.g_SpecularShininess = ocean_params.g_SpecularShininess;
	cFB.g_CamPos = camera->Position();
	cFB.g_Time = ocean_params.g_Time;
	cFB.g_RefractionTint = ocean_params.g_RefractionTint;
	cFB.g_DullColor = ocean_params.g_DullColor;
	
	//<JPK> add perlin
	// Constant m_PerlinSpeed need to be adjusted mannually
	XMFLOAT2 perlin_move = ZShadeSandboxMath::XMMath2(ocean_params.g_WindDir) * -1.0f * ocean_params.g_Time * ocean_params.g_PerlinSpeed;
	cFB.g_PerlinMovement = perlin_move;

	// Texcoord for perlin noise
	XMFLOAT2 uv_base = ZShadeSandboxMath::XMMath2(bottom_left) / ocean_params.g_PatchLength * ocean_params.g_PerlinSize;
	cFB.g_UVBase = uv_base;

	//additional perlin params
	cFB.g_PerlinSize = ocean_params.g_PerlinSize;
	cFB.g_PerlinSpeed = ocean_params.g_PerlinSpeed;
	cFB.g_PerlinAmplitude = ocean_params.g_PerlinAmplitude;
	cFB.g_PerlinGradient = ocean_params.g_PerlinGradient;
	cFB.g_PerlinOctave = ocean_params.g_PerlinOctave;
	cFB.g_TexelLength_x2 = ocean_params.g_PatchLength / ocean_params.g_MapDimension * 2;
	cFB.g_UVScale = 1.0f / ocean_params.g_PatchLength;
	cFB.g_UVOffset = 0.5f / ocean_params.g_MapDimension;
	
	// Map the per frame constants
	{
	D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPerFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbPerFrameBuffer*)mapped_res.pData = cFB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerFrameCB, 0);
	}
	
	cMB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	cMB.g_matView = camera->View4x4();
	cMB.g_matProj = camera->Proj4x4();
	cMB.g_ReflectionView = camera->ReflectionView4x4();
	// Map the matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbMatrixBuffer*)mapped_res2.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);
	}
	
	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(0, 1, hs_cbs);
	
	// Set the matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[2] = { m_pPerFrameCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 2, ds_cbs);
	
	// Set the shading constant buffer into the Pixel Shader
	ID3D11Buffer* ps_cbs[1] = { m_pPerFrameCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	ID3D11ShaderResourceView* ds_srvs[2] = { mTexDisplacement, mTexPerlin };
	ID3D11SamplerState* ds_samp[2] = { m_pD3DSystem->Point(), m_pD3DSystem->Anisotropic() };
	
	ID3D11ShaderResourceView* ps_srvs[9] = { mTexDisplacement, mTexPerlin, mTexGradient, mWaveMap0, mWaveMap1, mReflectionMap, mRefractionMap, g_pSRV_Fresnel, m_pSRV_ReflectCube };
	ID3D11SamplerState* ps_samp[4] = { m_pD3DSystem->Mirror(), m_pD3DSystem->Linear(), m_pD3DSystem->Point(), m_pD3DSystem->Anisotropic() };
	
	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 2, ds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 2, ds_samp);

		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 9, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 4, ps_samp);

		SwitchTo("OceanSurfacePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		//<JPK> need this on the domain for wireframe
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 2, ds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 2, ds_samp);
		
		SwitchTo("OceanSurfaceWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	SetInputLayout("OceanSurfaceShader");
	
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();
	
	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		for (int i = 0; i < 2; i++) ds_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 2, ds_srvs);
		for (int i = 0; i < 2; i++) ds_samp[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 2, ds_samp);

		for (int i = 0; i < 9; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 9, ps_srvs);
		for (int i = 0; i < 4; i++) ps_samp[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 4, ps_samp);
	}
	
	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);

	return true;
}
//==============================================================================================================================