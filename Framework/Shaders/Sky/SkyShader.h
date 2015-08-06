//=====================================================================================
//SkydomeShader.h
//=====================================================================================
#ifndef __SKYSHADER_H
#define __SKYSHADER_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "CustomMeshParameters.h"
//==============================================================================================================================
//==============================================================================================================================
//RasterizerState NoCull
//{
//    CullMode = None;
//};

//DepthStencilState LessEqualDSS
//{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
//    DepthFunc = LESS_EQUAL;
//};
//SetRasterizerState(NoCull);
//SetDepthStencilState(LessEqualDSS, 0);
//==============================================================================================================================
//==============================================================================================================================
class SkyShader : public ZShadeSandboxShader::Shader
{
	struct Const_Per_Frame
	{
		//XMFLOAT4X4 g_WorldViewProj;
		XMFLOAT4X4 g_World;
		XMFLOAT4X4 g_View;
		XMFLOAT4X4 g_Proj;
	};
	
public:
	SkyShader(D3D* d3d);
	SkyShader(const SkyShader& other);
	~SkyShader();

	bool Initialize();
	void Shutdown();
	
	void RenderFunc(int indexCount, ZShadeSandboxMesh::MeshRenderParameters mrp, ZShadeSandboxLighting::ShaderMaterial* material);
	
	//void RenderFunc(int indexCount, Camera* pCamera, XMFLOAT4X4 view, XMMATRIX world, XMFLOAT4 clipplane, ID3D11ShaderResourceView* cubemap, bool useReflection);
	//bool Render11(int indexCount, Camera* camera, ID3D11ShaderResourceView* cubemap);
	//bool Render11Reflection(int indexCount, Camera* camera, XMFLOAT3 eye, XMFLOAT4X4 reflection, ID3D11ShaderResourceView* cubemap);

private:
	
	ID3D11Buffer* m_pPerFrameCB;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__SKYSHADER_H