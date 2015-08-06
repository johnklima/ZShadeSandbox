//==============================================================================
// SkyPlane.h
//
// http://www.rastertek.com/tertut12.html
//==============================================================================
// History
//
// -Created on 4/13/2015 by Dustin Watson
//==============================================================================
#ifndef __SKYPLANE_H
#define __SKYPLANE_H
//==============================================================================================================================
//==============================================================================================================================
#include <string>
#include "D3D.h"
#include "SkyPlaneParameters.h"
#include "SkyPlaneShader.h"
#include "Texture.h"
#include "MaterialShader.h"
using std::string;
//==============================================================================================================================
//==============================================================================================================================
class SkyPlane
{
	D3D* m_D3DSystem;
	SkyPlaneShader* mSkyPlaneShader;
	MaterialShader* mTextureShader;
	
	// Configurable parameters
	SkyPlaneParameters mSkyPlaneParameters;
	//SkyPlaneMeshParameters mSkyPlaneMeshParameters;
	float m_Translation;
	
	// Mesh attributes
	bool 					  m_Wireframe;
	Texture* 				  mCloudTexture;
	Texture* 				  mCloudPerturbTexture;
	XMFLOAT4X4 				  mWorld;
	int 					  mVertexCount;
	int 					  mIndexCount;
	int			 			  mTriangleCount;
	UINT 					  mVertexByteWidth;
	vector<ZShadeSandboxMesh::VertexTex>		  mVerticesTex;
	vector<UINT>              mIndices;
	ID3D11Buffer*             pVB;
	ID3D11Buffer*             pIB;
	ZShadeSandboxMesh::VertexTex*				  mSkyPlane;
	
public:
	
	SkyPlane(D3D* d3d, SkyPlaneParameters spp);
	
	void Update(float dt);
	
	void Render(D3D* d3d, Camera* camera, float planeHeight);
	void RenderWithReflection(D3D* d3d, Camera* camera, float planeHeight);
	void RenderBuffers();

	void BuildSkyPlane();
	void Initialize();
	void CreateBuffers();
	
	int TriangleCount() const { return mTriangleCount; }
	void SetWireframe(bool wire);
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__SKYPLANE_H