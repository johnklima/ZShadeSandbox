#include "SkyPlaneShader.h"
#include "ConstantBuffer.h"
#include "TextureManager.h"
//==============================================================================================================================
SkyPlaneShader::SkyPlaneShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
SkyPlaneShader::SkyPlaneShader(const SkyPlaneShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
SkyPlaneShader::~SkyPlaneShader()
{
}
//==============================================================================================================================
bool SkyPlaneShader::Initialize()
{
	ConstantBuffer<cbSkyPlaneBuffer> skyPlaneCB(m_pD3DSystem);
	skyPlaneCB.Initialize(PAD16(sizeof(cbSkyPlaneBuffer)));
	m_pSkyPlaneCB = skyPlaneCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("SkyPlaneShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	LoadVertexShader("skyPlaneVS");
	LoadPixelShader("skyPlanePS");
	LoadPixelShader("skyPlaneWireframePS");
	AssignVertexShaderLayout("SkyPlaneShader");
	
	return true;
}
//==============================================================================================================================
void SkyPlaneShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	//ShutdownShader();
}
//==============================================================================================================================
/*void SkyPlaneShader::RenderFunc(int indexCount, Camera* pCamera, XMMATRIX view, XMMATRIX world, XMFLOAT4 clipplane,
	ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
	SkyPlaneParameters spp, bool useReflection)
{
	XMFLOAT3 eyeVertex = pCamera->Position();
	//Invert the Y coordinate of the camera around the water plane height for the reflected camera position
	eyeVertex.y = -eyeVertex.y + (0 * 2.0f);
	
	if (!useReflection)
		Render11(indexCount, pCamera, cloudTexture, perturbTexture, spp);
	else
		Render11Reflection(indexCount, pCamera, eyeVertex, view, cloudTexture, perturbTexture, spp);
}*/
//==============================================================================================================================
bool SkyPlaneShader::Render11
(	int indexCount
,	XMMATRIX world
,	ZShadeSandboxMath::XMMath4 clipplane
,	Camera* camera
,	ID3D11ShaderResourceView* cloudTexture
,	ID3D11ShaderResourceView* perturbTexture
,	float translation
,	float scale
,	float brightness
)
{
	cbSkyPlaneBuffer per_frame;
	per_frame.g_Translation = translation;
	per_frame.g_Scale = scale;
	per_frame.g_Brightness = brightness;
	per_frame.padding = 0.0f;
	
	//per_frame.g_ClipPlane = XMFLOAT4(clipplane.x, clipplane.y, clipplane.z, clipplane.w);

	//cbMatrixBuffer per_object;

	//if (m_UseCustomWorld)
	//{
	//	XMFLOAT4X4 world = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	//	per_object.g_matWorld = world;
	//}
	//else
	//	per_object.g_matWorld = camera->World4x4();

	//if (m_UseCustomView)
	//{
	//	//XMFLOAT4X4 view = GMathMF(XMMatrixTranspose(mView.Get()));
	//	per_object.g_matView = mView;
	//}
	//else
	//	per_object.g_matView = camera->View4x4();

	//if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
	//	per_object.g_matProj = camera->Ortho4x4();
	//else if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
	//	per_object.g_matProj = camera->Proj4x4();

	//if (m_UseOrtho)
	//{
	//	per_object.g_matProj = camera->Ortho4x4();
	//}

	// Consts
	D3D11_MAPPED_SUBRESOURCE mapped_res;
	m_pD3DSystem->GetDeviceContext()->Map(m_pSkyPlaneCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	{
		assert(mapped_res.pData);
		*(cbSkyPlaneBuffer*)mapped_res.pData = per_frame;
	}
	m_pD3DSystem->GetDeviceContext()->Unmap(m_pSkyPlaneCB, 0);

	/*D3D11_MAPPED_SUBRESOURCE mapped_res2;
	m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
	{
		assert(mapped_res2.pData);
		*(cbMatrixBuffer*)mapped_res2.pData = per_object;
	}
	m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);*/

	camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, world, false);

	ID3D11Buffer* vs_cbs[2] = { m_pSkyPlaneCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);

	ID3D11Buffer* ps_cbs[2] = { m_pSkyPlaneCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 2, ps_cbs);

	ID3D11ShaderResourceView* ps_srvs[2] = { cloudTexture, perturbTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);

		SwitchTo("skyPlanePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("skyPlaneWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SwitchTo("skyPlaneVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	SetVertexShader();
	SetPixelShader();

	SetInputLayout("SkyPlaneShader");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);
	}

	return true;
}
//==============================================================================================================================
/*bool SkyPlaneShader::Render11(int indexCount, Camera* camera, ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
	SkyPlaneParameters spp)
{
	// 2D Should be using a sky shader, does not need it
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		return false;
	
	cbSkyPlaneBuffer cSPB;
	cSPB.g_Translation = spp.g_Translation;
	cSPB.g_Scale = spp.g_Scale;
	cSPB.g_Brightness = spp.g_Brightness;
	cSPB.padding = 0;
	// Create the constant sky plane buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pSkyPlaneCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbSkyPlaneBuffer*)mapped_res.pData = cSPB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pSkyPlaneCB, 0);
	}

	cbMatrixBuffer cMB;

	/*XMFLOAT3 eyeVertex = camera->Position();
	XMMATRIX translate = XMMatrixTranslation(eyeVertex.x, eyeVertex.y, eyeVertex.z);
	XMMATRIX world = mWorld;
	//world = XMMatrixMultiply(world, translate);
	cMB.g_World = GMathMF(XMMatrixTranspose(world));
	*

	if (m_UseCustomWorld)
		cMB.g_WorldMatrix = GMathMF(XMMatrixTranspose(mWorld.Get()));
	else
		cMB.g_WorldMatrix = camera->World4x4();

	if (m_UseCustomView)
		cMB.g_ViewMatrix = GMathMF(XMMatrixTranspose(mView.Get()));
	else
		cMB.g_ViewMatrix = camera->View4x4();

	// Center Sky about eye in world space
	//XMFLOAT3 eyeVertex = camera->Position();
	//XMMATRIX T = XMMatrixTranslation(eyeVertex.x, eyeVertex.y, eyeVertex.z);
	//XMMATRIX mv(1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1);
	
	//XMMATRIX W = mWorld;
	//cMB.g_World = GMathMF(XMMatrixTranspose(W));
	//cMB.g_View = camera->View4x4();
	cMB.g_ProjMatrix = camera->Proj4x4();
	{
		// Create the constant matrix buffer
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);
	}

	ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pSkyPlaneCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[2] = { cloudTexture, perturbTexture };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);

		SwitchTo("SkyPlanePS", ST_PIXEL);
	}
	else
	{
		SwitchTo("SkyPlaneWireframePS", ST_PIXEL);
	}

	SetInputLayout("SkyPlaneShader");

	SetVertexShader();
	SetPixelShader();

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);
	}

	return true;
}*/
//==============================================================================================================================
/*bool SkyPlaneShader::Render11Reflection(int indexCount, Camera* camera, XMFLOAT3 eye, XMMATRIX reflection,
	ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture,
	SkyPlaneParameters spp)
{
	// 2D Should be using a sky shader, does not need it
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		return false;
	
	cbSkyPlaneBuffer cSPB;
	cSPB.g_Translation = spp.g_Translation;
	cSPB.g_Scale = spp.g_Scale;
	cSPB.g_Brightness = spp.g_Brightness;
	cSPB.padding = 0;
	// Create the constant sky plane buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pSkyPlaneCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbSkyPlaneBuffer*)mapped_res.pData = cSPB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pSkyPlaneCB, 0);
	}

	//XMMATRIX mv(1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1);
	XMMATRIX T = XMMatrixTranslation(eye.x, eye.y, eye.z);
	XMMATRIX RP = XMMatrixMultiply(reflection, camera->Proj());
	cbMatrixBuffer cMB;
	XMMATRIX W = mWorld;
	cMB.g_World = GMathMF(XMMatrixTranspose(W));
	cMB.g_View = GMathMF(XMMatrixTranspose(reflection));
	cMB.g_Proj = camera->Proj4x4();
	// Create the constant matrix buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);
	}

	ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pSkyPlaneCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[2] = { cloudTexture, perturbTexture };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);

		SwitchTo("SkyPlanePS", ST_PIXEL);
	}
	else
	{
		SwitchTo("SkyPlaneWireframePS", ST_PIXEL);
	}

	//SwitchTo("SkyPlaneVS", ST_VERTEX);

	SetInputLayout("SkyPlaneShader");

	SetVertexShader();
	SetPixelShader();

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);
	}

	return true;
}
//==============================================================================================================================
*/