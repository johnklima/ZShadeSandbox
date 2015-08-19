//===============================================================================================================================
// TreeShader
//
//===============================================================================================================================
// History
//
// -Created on 8/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TREESHADER_H
#define __TREESHADER_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include "TreeParameters.h"

//==============================================================================================================================
//==============================================================================================================================
class TreeShader : public ZShadeSandboxShader::Shader
{
	struct cbTreeBuffer
	{
		XMFLOAT4	g_EyePos;
		float 		g_TreeBillWidth;
		float		g_TreeBillHeight;
		float		padding1;
		float		padding2;
		XMFLOAT4	g_DiffuseColor;
		int			g_HasTexture;
		int			g_HasNormalMap;
		int			g_IsInstance;
		int			g_IsLeaf;
	};
	
	struct cbLeafBuffer
	{
		// This only needs to be loaded once into the constant buffer
		XMFLOAT4X4  g_LeafOnTree[NUM_LEAVES_PER_TREE];
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
	};
	
public:
	
	TreeShader(D3D* d3d);
	TreeShader(const TreeShader& other);
	~TreeShader();
	
	bool Initialize();
	void Shutdown();
	
	// This should only be called once after the leaf matrices have been calculated
	void InitializeLeafBuffer(InternalTreeRenderParameters trp);
	
	void SetFarPlane(float farPlane) { fFarPlane = farPlane; }
	
	void RenderLeaves
	(	int indexCount
	,	int instanceCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ID3D11ShaderResourceView* texture
	);
	
private:
	
	//
	// Variables
	//
	
	float fFarPlane;
	
	ID3D11Buffer* m_pTreeBufferCB;
	ID3D11Buffer* m_pLeafBufferCB;
	ID3D11Buffer* m_pMatrixBufferCB;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__WATERSHADER_H