//===============================================================================================================================
// DeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __DEFERREDSHADER_H
#define __DEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class DeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbPackBuffer
	{
		XMFLOAT2 g_SpecularPowerRange;
		float g_SpecularIntensity;
		float g_SpecularPower;
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
	};
	
	struct cbMatrixBuffer2
	{
		XMFLOAT4X4	g_matWVP;
	};
	
public:
	
	DeferredShader(D3D* d3d);
	DeferredShader(const DeferredShader& other);
	~DeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	Camera* camera
	,	XMMATRIX world
	,	XMFLOAT2 specularPowerRange
	,	float specularIntensity
	,	float specularPower
	,	ID3D11ShaderResourceView* texture
	);
	
private:
	
	ID3D11Buffer* m_pPackCB;
	ID3D11Buffer* m_pMatrixCB;
	ID3D11Buffer* m_pMatrix2CB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__DEFERREDSHADER_H