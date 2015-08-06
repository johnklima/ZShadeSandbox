//===============================================================================================================================
// TextureShader
//
//===============================================================================================================================
// History
//
// -Created on 6/18/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __TEXTURESHADER_H
#define __TEXTURESHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class TextureShader : public ZShadeSandboxShader::Shader
{
	struct cbShadingBuffer
	{
		XMFLOAT4 g_ClipPlane;
		XMFLOAT2 padding;
		int	     g_FlipTextureH;
		int	     g_FlipTextureV;
	};
	
public:
	
	TextureShader(D3D* d3d);
	TextureShader(const TextureShader& other);
	~TextureShader();
	
	bool Initialize();
	void Shutdown();
	
	// true - flips the texture Horizontally
	// false - flips the texture Vertically
	void FlipTextureHorizontally(bool flip);
	
	bool Render11
	(	int indexCount
	,	ZShadeSandboxMath::XMMath4 clipplane
	,	XMMATRIX world
	,	bool reflection
	,	Camera* camera
	,	ID3D11ShaderResourceView* texture
	);
	
private:
	
	bool bFlipHorizontally;
	bool bFlipVertically;
	
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pMatrixCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__TEXTURESHADER_H