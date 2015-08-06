//==============================================================================================================================
// LakeBed.h
//
//==============================================================================================================================
// History
//
// -Created on 5/29/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __LAKEBED_H
#define __LAKEBED_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "ZMath.h"
#include "LightCamera.h"
#include "LakeBedShader.h"
#include "ShadowMapBuildShader.h"
#include "TextureAnimation3D.h"

//==============================================================================================================================
//==============================================================================================================================

class LakeBed
{
public:
	
	LakeBed(D3D* d3d, float width = 512.0f, float height = 512.0f, float depth = 512.0f);
	
	void Update(float dt);
	void Render(Camera* camera, LightCamera* lightCamera, bool renderDeferred = false, bool shadow = false, XMFLOAT4 clipplane = XMFLOAT4(0, 0, 0, 0), bool reflect = false);
	void RenderBuffers();
	
	void Initialize();
	void CreateBuffers();
	
	int TriangleCount() const { return mTriangleCount; }
	void SetWireframe(bool wire);
	
	void LoadDiffuseMap(string filename);
	void LoadWaveMap0(string filename);
	void LoadWaveMap1(string filename);
	void LoadMicroBump0(string filename);
	void LoadMicroBump1(string filename);
	void LoadDepthMap(string filename);
	
	void SetDiffuseMap(ID3D11ShaderResourceView* srv) 	{ m_DiffuseMap = srv; }
	void SetWaveMap0(ID3D11ShaderResourceView* srv) 	{ m_WaveMap0 = srv; }
	void SetWaveMap1(ID3D11ShaderResourceView* srv) 	{ m_WaveMap1 = srv; }
	void SetMicroBump0(ID3D11ShaderResourceView* srv) 	{ g_MicroBump0 = srv; }
	void SetMicroBump1(ID3D11ShaderResourceView* srv) 	{ g_MicroBump1 = srv; }
	void SetDepthMap(ID3D11ShaderResourceView* srv) 	{ g_DepthMap = srv; }
	
private:
	
	D3D* m_D3DSystem;
	
	TextureAnimation3D* mCausticAnimation;

	ZShadeSandboxShader::LakeBedShader* mShader;
	ShadowMapBuildShader* mShadowMapShader;
	
	float fWidth;
	float fHeight;
	float fDepth;
	float fCausticTimer;
	
	// Mesh attributes
	XMFLOAT3				  mPosition;
	XMFLOAT3				  mScale;
	XMFLOAT3				  mRotation;
	int 					  mVertexCount;
	int 					  mIndexCount;
	int			 			  mTriangleCount;
	UINT 					  mVertexByteWidth;
	vector<ZShadeSandboxMesh::VertexTex>		  mVerticesTex;
	vector<UINT>              mIndices;
	ID3D11Buffer*             pVB;
	ID3D11Buffer*             pIB;
	
	ID3D11ShaderResourceView* m_DiffuseMap;
	ID3D11ShaderResourceView* m_WaveMap0;
	ID3D11ShaderResourceView* m_WaveMap1;
	ID3D11ShaderResourceView* g_MicroBump0;
	ID3D11ShaderResourceView* g_MicroBump1;
	ID3D11ShaderResourceView* g_DepthMap;
};

//==============================================================================================================================
//==============================================================================================================================
#endif//__LAKEBED_H