//==================================================================================================================================
// PostProcessManager.h
//
//==================================================================================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __POSTPROCESSINGMANAGER_H
#define __POSTPROCESSINGMANAGER_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "PostProcess.h"
//==================================================================================================================================
class PostProcessManager
{
public:
	
	struct RenderTarget
	{
		UINT Width;
		UINT Height;
		DXGI_FORMAT Format;
		ID3D11Texture2D* DSTexture;
		ID3D11Texture2D* Texture;
		ID3D11ShaderResourceView* SRView;
		ID3D11RenderTargetView* RTView;
		ID3D11DepthStencilView* DSView;
		ID3D11UnorderedAccessView* UAView;
		D3D11_VIEWPORT viewport;
		bool bInUse;
		
		void Set();
		void Clear(float red, float green, float blue, float alpha);
	};
	
	PostProcessManager(D3D* d3d);
	~PostProcessManager();
	
	void Init();
	
	void AddPostProcess(PostProcess* pp);
	void ClearCurrentPostProcesses();
	
	// Render a post process onto the existing scene
	void Render(ID3D11ShaderResourceView* sceneSRV, ID3D11RenderTargetView*& outputRTV);
	
	static RenderTarget* CreateRenderTarget
	(	int textureWidth
	,	int textureHeight
	,	float screenDepth
	,	float screenNear
	,	DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT
	,	int multiSamples = 1
	,	int msQuality = 0
	,	int mipLevels = 1
	,	bool generateMipMaps = false
	,	bool useAsUAV = false
	);
	
	static void ClearRenderTargets();
	
	// The shader that presents the post process to the screen
	static ZShadeSandboxShader::Shader* mShader;

private:
	
	// Need to add a constant buffer for post processing parameters
	// Need to handle input textures and output textures for rendering the effects
	// Need a SceneSRV input, RenderTarget output to the scene and update the scene depth buffer when DOF is used
	
	static D3D* mD3DSystem;
	
	// List of current post processes to render
	std::vector<PostProcess*> m_CurrentPostProcesses;
	
	// Save a copy of a render target so that lookup is faster any time a render target needs to be used
	static vector<RenderTarget*> mCreatedTargets;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__POSTPROCESSINGMANAGER_H