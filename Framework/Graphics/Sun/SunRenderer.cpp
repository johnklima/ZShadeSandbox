#include "SunRenderer.h"
#include "XMMath.h"
using ZShadeSandboxLighting::SunRenderer;
using ZShadeSandboxLighting::Sun;
//===============================================================================================================================
//===============================================================================================================================
SunRenderer::SunRenderer(Sun* sun)
{
	mSun = sun;
	mD3DSystem = sun->GetD3D();
	
	// Load the shader
	mShader = new ZShadeSandboxShader::SunShader(mD3DSystem);
	
	// Create the material for the sun
	mMaterial = new ZShadeSandboxLighting::ShaderMaterial(mD3DSystem, "Sun");
	mMaterial->AddDiffuseTexture("Textures", "lens01.dds");
	mMaterial->AddDiffuseTexture("Textures", "lens02.dds");
	mMaterial->AddDiffuseTexture("Textures", "lens03.dds");
	mMaterial->AddDiffuseTexture("Textures", "lens04.dds");

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	//for (UINT i = 0; i < 8; ++i)
	{
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
	}
	//blendDesc.RenderTarget[0] = rtbd;
	mD3DSystem->GetDevice11()->CreateBlendState(&blendDesc, &mTransparency);
}
//===============================================================================================================================
void SunRenderer::SetWireframe(bool wire)
{
	mShader->Wireframe() = wire;
}
//===============================================================================================================================
void SunRenderer::RenderMeshBuffers(int renderType, int sunQuadID)
{
	unsigned int stride;
	unsigned int offset;
	
	SunQuad* quad = mSun->GetQuad(sunQuadID);
	ID3D11Buffer* vb = quad->VB();

	//Set the vertex buffer stride and offset
	stride = quad->VertexByteWidth();
	offset = 0;

	mD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	
	//Set the index buffer to active in the input assembler so it can be rendered
	mD3DSystem->GetDeviceContext()->IASetIndexBuffer(quad->IB(), DXGI_FORMAT_R32_UINT, 0);
	
	mD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
//===============================================================================================================================
void SunRenderer::Render(Camera* camera, int renderType)
{
	if (!mShader->Wireframe())
		mD3DSystem->TurnOffCulling();

	ZShadeSandboxMath::XMMath3 cameraPosition(camera->Position());
	ZShadeSandboxMath::XMMath3 cameraTarget(camera->Target());
	ZShadeSandboxMath::XMMath3 sunDirection(mSun->Direction());
	ZShadeSandboxMath::XMMath3 normalizedTarget = cameraPosition - cameraTarget;
	normalizedTarget.NormalizeMe();
	float dot = sunDirection.Dot(normalizedTarget);
	
	if (mSun->Alpha() != 0.0f)
	{
		XMMATRIX world = mSun->GetQuad(0)->World();
		RenderMeshBuffers(renderType, 0);
		//mShader->UseCustomWorld(true);
		//mShader->SetCustomWorld(world);
		mShader->Render11(mSun->GetQuad(0)->IndexCount(), mSun->Alpha(), mSun->GlowFactor(), camera, mMaterial->GetTexture(0)->getTexture11());
	}
	
	if (mSun->Alpha() > 0.7f && dot < -0.8f)
	{
		for (int i = 1; i < 4; i++)
		{
			RenderMeshBuffers(renderType, i);
			//mShader->UseCustomWorld(true);
			//mShader->SetCustomWorld(mSun->GetQuad(i)->CalculateWorldMatrix());
			mShader->Render11(mSun->GetQuad(i)->IndexCount(), mSun->Alpha(), mSun->GlowFactor(), camera, mMaterial->GetTexture(i)->getTexture11());
		}
	}

	if (!mShader->Wireframe())
		mD3DSystem->TurnOnCulling();
}
//===============================================================================================================================