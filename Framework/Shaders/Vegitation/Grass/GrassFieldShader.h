//===============================================================================================================================
// GrassFieldShader
//
//===============================================================================================================================
// History
//
// -Created on 7/13/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __GRASSFIELDSHADER_H
#define __GRASSFIELDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class GrassFieldShader : public ZShadeSandboxShader::Shader
{
public:
	GrassFieldShader(D3D* d3d);
	GrassFieldShader(const GrassFieldShader& other);
	~GrassFieldShader();

	bool Initialize(D3D* d3d);
	void Shutdown();
	
	void RenderFunc(Camera* pCamera, XMMATRIX view, XMMATRIX world, XMFLOAT4 clipplane);

	bool Render11(D3D* d3d, int vertexCount);

	/*
	void SetWorld(CXMMATRIX m)                    { pWorld->SetMatrix(reinterpret_cast<const float*>(&m)); }
	void SetView(CXMMATRIX m)                     { pView->SetMatrix(reinterpret_cast<const float*>(&m)); }
	void SetProj(CXMMATRIX m)                     { pProj->SetMatrix(reinterpret_cast<const float*>(&m)); }

	void SetNumVerts(int verts) { m_NumVerts = verts; }

	void SetGrassAlphaTopSRV(ID3D11ShaderResourceView* srv) { pGrassAlphaMapTop->SetResource(srv); }
	void SetGrassDiffuseTopSRV(ID3D11ShaderResourceView* srv) { pGrassDiffuseMapTop->SetResource(srv); }
	void SetGrassAlphaBodySRV(ID3D11ShaderResourceView* srv) { pGrassAlphaMapBody->SetResource(srv); }
	void SetGrassDiffuseBodySRV(ID3D11ShaderResourceView* srv) { pGrassDiffuseMapBody->SetResource(srv); }

	void SetHalfScale(float halfScale) { pHalfScale->SetFloat(halfScale); }
	void SetKHalfPi(float kHalfPi) { pKHalfPi->SetFloat(kHalfPi); }
	void SetPix2(float fPIx2) { pfPIx2->SetFloat(fPIx2); }
	void SetWindVec(XMFLOAT2 windVec) { pWindVec->SetRawValue(&windVec, 0, sizeof(XMFLOAT2)); }
	void SetWindCoeff(float windCoeff) { pWindCoeff->SetFloat(windCoeff); }
	void SetWaveDistortX(XMFLOAT4 waveDistX) { pWaveDistortX->SetRawValue(&waveDistX, 0, sizeof(XMFLOAT4)); }
	void SetWaveDistortY(XMFLOAT4 waveDistY) { pWaveDistortY->SetRawValue(&waveDistY, 0, sizeof(XMFLOAT4)); }
	void SetWaveDistortZ(XMFLOAT4 waveDistZ) { pWaveDistortZ->SetRawValue(&waveDistZ, 0, sizeof(XMFLOAT4)); }
	void SetLightingWaveScale(XMFLOAT4 lightingWaveScale) { pLightingWaveScale->SetRawValue(&lightingWaveScale, 0, sizeof(XMFLOAT4)); }
	void SetLightingScaleBias(XMFLOAT4 lightingScaleBias) { pLightingScaleBias->SetRawValue(&lightingScaleBias, 0, sizeof(XMFLOAT4)); }
	void SetTime(float time) { pTime->SetFloat(time); }
	void SetWaveSpeed(XMFLOAT4 waveSpeed) { pWaveSpeed->SetRawValue(&waveSpeed, 0, sizeof(XMFLOAT4)); }
	void SetEyeVertex(XMFLOAT3 eye) { pEyeVertex->SetRawValue(&eye, 0, sizeof(XMFLOAT3)); }
	*/

private:
	int m_NumVerts;

	//bool InitializeShader11(D3D* d3d);
	
	//void ShutdownShader();
	
	//void RenderShader(D3D* d3d, int vertexCount);

	//ID3D11InputLayout* m_layout11;

	/*
	ID3DX11EffectTechnique* pColorTech;

	ID3DX11EffectScalarVariable* pHalfScale;
	ID3DX11EffectScalarVariable* pKHalfPi;
	ID3DX11EffectScalarVariable* pfPIx2;
	ID3DX11EffectVectorVariable* pWindVec;
	ID3DX11EffectScalarVariable* pWindCoeff;
	ID3DX11EffectVectorVariable* pWaveDistortX;
	ID3DX11EffectVectorVariable* pWaveDistortY;
	ID3DX11EffectVectorVariable* pWaveDistortZ;
	ID3DX11EffectVectorVariable* pLightingWaveScale;
	ID3DX11EffectVectorVariable* pLightingScaleBias;
	ID3DX11EffectScalarVariable* pTime;
	ID3DX11EffectVectorVariable* pWaveSpeed;
	ID3DX11EffectVectorVariable* pEyeVertex;

	//Alpha transparency and Color diffuse
	ID3DX11EffectShaderResourceVariable* pGrassAlphaMapTop;
	ID3DX11EffectShaderResourceVariable* pGrassDiffuseMapTop;
	ID3DX11EffectShaderResourceVariable* pGrassAlphaMapBody;
	ID3DX11EffectShaderResourceVariable* pGrassDiffuseMapBody;

	ID3DX11EffectMatrixVariable* pWorld;
	ID3DX11EffectMatrixVariable* pView;
	ID3DX11EffectMatrixVariable* pProj;
	*/
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__GRASSFIELDSHADER_H