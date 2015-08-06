//===============================================================================================================================
// D3D.h
//
//===============================================================================================================================
// History
//
// -Created in Fall 2013 by Dustin Watson
//===============================================================================================================================
#ifndef __D3D_H
#define __D3D_H
//===============================================================================================================================
//===============================================================================================================================

#pragma managed(push, off)

//
//Includes
//

#include "GDI.h"
#include <vector>
#include <map>
#include <unordered_map>
#include "InterfacePointers.h"
#include "EngineOptions.h"
#include "DepthStencilBuffer.h"
#include "RenderTarget2D.h"
#include "D3DIncludes.h"
//#include "GBuffer.h"

using namespace std;

class GBuffer;
class DepthStencilBuffer;
class RenderTarget2D;
class DCRenderTexture;
//class DummyTargetShader;

namespace ZShadeSandboxShader {
	class Shader;
}

//===============================================================================================================================
//===============================================================================================================================
class D3D
{
public:
	D3D();
	D3D(const D3D& other);
	~D3D();

	//
	// Initialization
	//

	bool Initialize11(EngineOptions* options);
	bool InitializeAdapterAndModes();
	bool InitializeDeviceAndSwapChain11();
	bool InitializeBackbuffers11();
	
	void InitRenderStates();

	//
	// Shutdown
	//

	void Shutdown();

	//
	// Scene rendering
	//

	bool BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	//
	// Viewport adjustment
	//

	void ResetViewport();
	// Sets the viewport for a given render target size
	void SetViewport(UINT rtWidth, UINT rtHeight);

	//
	// Reset for Post processing buffers
	//

	void AfterReset(int width, int height);

	//
	// Getters for the device's
	//

	ID3D10Device* GetDevice10() { return m_device10; }
	ID3D11DevicePtr GetDevice11() { return m_device11; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }
	
	//
	// Device info
	//

	void GetVideoCardInfo(char* cardName, int& memory);

	void SetBackBufferRenderTarget();
	//void SetColorRenderTarget();
	void ClearBackbufferStencil(float red, float green, float blue, float alpha);

	//
	// Custom states for turning items on or off
	//
	
	
	//
	// Getters for backbuffer
	//
	
	//ID3D11DepthStencilView* GetDepthStencilView();
	//ID3D11RenderTargetView* GetBackBufferRTV() { return m_renderTargetViewBackbuffer; }
	//ID3D11ShaderResourceView* GetDepthStencilSRV() { return m_pDepthStencilSRV; }
	RenderTarget2D* GetBackbufferRenderTarget() { return m_pBackbufferRT; }
	DepthStencilBuffer* GetBackbufferDepthStencilBuffer() { return m_pBackbufferDS; }
	
	//
	// GBuffer rendering
	//
	
	void GBufferBegin();
	void GBufferEnd();
	
	RenderTarget2D* GBufferColorTarget();
	RenderTarget2D* GBufferNormalTarget();
	RenderTarget2D* GBufferDepthTarget();
	
	RenderTarget2D*& GBufferColorTargetRef();
	RenderTarget2D*& GBufferNormalTargetRef();
	RenderTarget2D*& GBufferDepthTargetRef();
	
	//
	// Getters
	//

	EngineOptions* GetEngineOptions() { return mOptions; }

	//
	// Available device states, sampler states and rasterizer states
	//

	void SetDepthStencilState(ID3D11DepthStencilState* dst);
	void SetBlendState(ID3D11BlendState* bld);
	void SetRasterizerState(ID3D11RasterizerState* rs);
	
	ID3D11BlendState* BlendDisabled()           { return m_pBlendDisabled; }
    ID3D11BlendState* AdditiveBlend()           { return m_pAdditiveBlend; }
    ID3D11BlendState* AlphaBlend()              { return m_pAlphaBlend; }
    ID3D11BlendState* PreMultipliedAlphaBlend() { return m_pPMAlphaBlend; }
    ID3D11BlendState* ColorWriteDisabled()      { return m_pNoColor; }
	
	ID3D11RasterizerState* DisableDepthClip()     { return m_pDisableDepthClip; }
	ID3D11RasterizerState* NoCull()               { return m_pNoCull; }
    ID3D11RasterizerState* BackFaceCull()         { return m_pCullBackfaces; }
    ID3D11RasterizerState* BackFaceCullScissor()  { return m_pCullBackfacesScissor; }
    ID3D11RasterizerState* FrontFaceCull()        { return m_pCullFrontfaces; }
    ID3D11RasterizerState* FrontFaceCullScissor() { return m_pCullFrontfacesScissor; }
    ID3D11RasterizerState* NoCullNoMS()           { return m_pNoCullNoMS; }
    ID3D11RasterizerState* NoCullScissor()        { return m_pNoCullScissor; }
    ID3D11RasterizerState* Wireframe()            { return m_pWireframe; }
	ID3D11RasterizerState* RSDepth()			  { return m_pDepth; }

	ID3D11DepthStencilState* DepthDisabled()            { return m_pDepthDisabled; }
    ID3D11DepthStencilState* DepthEnabled()             { return m_pDepthEnabled; }
    ID3D11DepthStencilState* ReverseDepthEnabled()      { return m_pRevDepthEnabled; }
    ID3D11DepthStencilState* DepthWriteEnabled()        { return m_pDepthWriteEnabled; }
    ID3D11DepthStencilState* ReverseDepthWriteEnabled() { return m_pRevDepthWriteEnabled; }

	ID3D11SamplerState* Mirror()       { return m_pMirror; }
	ID3D11SamplerState* Linear()       { return m_pLinear; }
	ID3D11SamplerState* LinearClamp()  { return m_pLinearClamp; }
    ID3D11SamplerState* Point()        { return m_pPoint; }
    ID3D11SamplerState* Anisotropic()  { return m_pAnisotropic; }
    ID3D11SamplerState* ShadowMap()    { return m_pShadowMap; }
    ID3D11SamplerState* ShadowMapPCF() { return m_pShadowMapPCF; }

	void TurnOnZBuffer();
	void TurnOffZBuffer();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	
	void TurnOnAdditiveBlending();
	void TurnOffAdditiveBlending();
	
	void TurnOnCulling();
	void TurnOffCulling();

	void TurnOnWireframe();
	//void SetSolidMode();
	
	/*
	void ClearRenderTargets();

	void SetRenderTargetNotInUse(RenderTarget2D* rt);
	
	//
	// Create a render target
	//

	RenderTarget2D* CreateRenderTarget
	(
		int textureWidth,
		int textureHeight,
		float screenDepth,
		float screenNear,
		DXGI_FORMAT format,
		UINT numMipLevels = 1,
		UINT multiSamples = 1,
		UINT msQuality = 0,
		bool autoGenMipMaps = false,
        bool createUAV = false,
        UINT arraySize = 1
	);
	*/
	
	void PrepareFullScreenSettings();

	void SetBackbufferSize(int w, int h);

	void ResizeDevice();
	void GetBackbufferSize(int & w, int & h);
	void SetFullscreen(bool fullscreen);

	GDI* GDIPlus() { return m_gdi; }

private:

	GDI* m_gdi;
	EngineOptions* mOptions;
	
	//
	// DX10 Device and backbuffer
	//

	ID3D10Device* m_device10;
	ID3D10RenderTargetView* m_renderTargetView10;
	ID3D10Texture2D* m_depthStencilBuffer10;
	ID3D10DepthStencilState* m_depthStencilState10;
	ID3D10DepthStencilView* m_depthStencilView10;
	ID3D10RasterizerState* m_rasterState10;
	ID3D10DepthStencilState* m_depthDisabledStencilState10;
	ID3D10RasterizerState* m_wireframeRS10;
	ID3D10RasterizerState* m_solidRS10;

	//
	// Device attributes
	//

	int m_screenWidth;
	int m_screenHeight;
	bool m_isFullscreen;
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	//vector<D3DDISPLAYMODE> listOfModes;
	//unsigned int numerator;
	//unsigned int denominator;

	//
	// DX11 Device
	//

	ID3D11DevicePtr m_device11;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	D3D11_VIEWPORT m_viewport;

	DXGI_RATIONAL refreshRate;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;

	//
	// Backbuffer 
	//
	
	//ID3D11RenderTargetView* m_renderTargetViewBackbuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	//ID3D11Texture2D* m_depthStencilBackBuffer;
	//ID3D11DepthStencilView* m_pDepthStencilBackBufferView;
	//ID3D11ShaderResourceView* m_pDepthStencilSRV;
	DXGI_FORMAT m_backBufferFormat;
	ID3D11Texture2D* backBufferPtr;
	DepthStencilBuffer* m_pBackbufferDS;
	RenderTarget2D* m_pBackbufferRT;
	GBuffer* mGBuffer;
	
	//
	// Available Blend States
	//

	ID3D11BlendState* m_pBlendDisabled;
	ID3D11BlendState* m_pAdditiveBlend;
	ID3D11BlendState* m_pAlphaBlend;
	ID3D11BlendState* m_pPMAlphaBlend;
	ID3D11BlendState* m_pNoColor;

	//
	// Available Rasterizer States
	//
	
	ID3D11RasterizerState* m_pDisableDepthClip;
	ID3D11RasterizerState* m_pNoCull;
	ID3D11RasterizerState* m_pCullBackfaces;
	ID3D11RasterizerState* m_pCullBackfacesScissor;
	ID3D11RasterizerState* m_pCullFrontfaces;
	ID3D11RasterizerState* m_pCullFrontfacesScissor;
	ID3D11RasterizerState* m_pNoCullNoMS;
	ID3D11RasterizerState* m_pNoCullScissor;
	ID3D11RasterizerState* m_pWireframe;
	ID3D11RasterizerState* m_pDepth;
	
	//
	// Available Depth Stencil States
	//

	ID3D11DepthStencilState* m_pDepthDisabled;
	ID3D11DepthStencilState* m_pDepthEnabled;
	ID3D11DepthStencilState* m_pRevDepthEnabled;
	ID3D11DepthStencilState* m_pDepthWriteEnabled;
	ID3D11DepthStencilState* m_pRevDepthWriteEnabled;

	//
	// Available Sampler States
	//

	ID3D11SamplerState* m_pMirror;
	ID3D11SamplerState* m_pLinear;
	ID3D11SamplerState* m_pLinearClamp;
	ID3D11SamplerState* m_pPoint;
	ID3D11SamplerState* m_pAnisotropic;
	ID3D11SamplerState* m_pShadowMap;
	ID3D11SamplerState* m_pShadowMapPCF;
};

#pragma managed(pop)

//===============================================================================================================================
//===============================================================================================================================
#endif//__D3D_H