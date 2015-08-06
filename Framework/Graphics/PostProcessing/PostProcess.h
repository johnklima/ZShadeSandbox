//==================================================================================================================================
// PostProcess.h
//
// http://gamedev.stackexchange.com/questions/773/what-are-some-good-resources-for-learning-hlsl
// https://digitalerr0r.wordpress.com/tutorials/
//==================================================================================================================================
// History
//
// -Created on 4/20/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __POSTPROCESS_H
#define __POSTPROCESS_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "Shader.h"
#include "PostProcessTypes.h"
//==================================================================================================================================
//==================================================================================================================================
class PostProcess
{
public:
	
	PostProcess(D3D* d3d);
	~PostProcess();
	
	void AddInputTexture(ID3D11ShaderResourceView* srv);
	void AddOutputTarget(ID3D11RenderTargetView* rtv);
	void AddOutputViewport(D3D11_VIEWPORT viewport);
	void AddOutputUAV(ID3D11UnorderedAccessView* uav);
	
	void PopInputTextures();
	void PopOutputTargets();
	void PopOutputUAVs();
	void PopOutputViewports();
	
	EPostProcessTypes::Type Type();
	
	virtual void UnloadResources();
	
	// Override this to set the constant buffers and internal stuff for the post process
	virtual void ProcessInternal() {}
	
	// Processes the post process's textures and render targets
	void Process(string pixelShaderName);
	void ProcessCompute(string computeShaderName, int threadX, int threadY, int threadZ);
	
protected:
	
	D3D* mD3DSystem;
	
	bool bPixelShaderSet;
	
	// What type of post process is this
	EPostProcessTypes::Type mType;
	
	// Textures sent to the shader
	vector<ID3D11ShaderResourceView*> mInputTextures;
	
	// Output targets to draw to
	vector<ID3D11RenderTargetView*> mOutputTargets;
	vector<D3D11_VIEWPORT> mOutputViewports;
	vector<ID3D11UnorderedAccessView*> mOutputUAVs;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__POSTPROCESS_H