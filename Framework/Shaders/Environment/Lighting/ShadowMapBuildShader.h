//===============================================================================================================================
// ShadowMapBuildShader
//
//===============================================================================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __SHADOWMAPBUILDSHADER_H
#define __SHADOWMAPBUILDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "LightCamera.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
// To use this supply a render target to get the shader resource view from the shadow map rendered onto the render target in the scene
class ShadowMapBuildShader : public ZShadeSandboxShader::Shader
{
	struct cbShadowMapConst
	{
		XMFLOAT4X4	g_World;
		XMFLOAT4X4	g_LightView;
		XMFLOAT4X4  g_LightProj;
	};
	
public:
	
	ShadowMapBuildShader(D3D* d3d);
	ShadowMapBuildShader(const ShadowMapBuildShader& other);
	~ShadowMapBuildShader();

	bool Initialize();
	void Shutdown();
	
	bool Render(int indexCount, XMMATRIX world, LightCamera* lightCamera);

private:
	
	ID3D11Buffer* m_pShadowMapCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__SHADOWMAPBUILDSHADER_H