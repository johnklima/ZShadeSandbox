//===============================================================================================================================
// FontShader
//
//===============================================================================================================================
// History
//
// -Created on 1/14/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __FONTSHADER_H
#define __FONTSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "GDIFont.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class FontShader : public ZShadeSandboxShader::Shader
{
	struct VSPerBatchCB
    {
        XMFLOAT2 TextureSize;
        XMFLOAT2 ViewportSize;
    };
	
	struct SpriteDrawData
    {
        XMMATRIX Transform;
        XMFLOAT4 Color;
        XMFLOAT4 DrawRect;
    };
	
	static const UINT64 MaxBatchSize = 1000;
	
public:
	FontShader(D3D* d3d);
	FontShader(const FontShader& other);
	~FontShader();

	bool Initialize();
	void Shutdown();
	
	void SetVertexBuffer(ID3D11Buffer* vb) { vertexBuffer = vb; }
	
	void RenderBatch11(ID3D11ShaderResourceView* texture, const SpriteDrawData* drawData, UINT64 numSprites);
	bool Render11(const GDIFont& font, const BetterString text, const XMMATRIX& transform, const XMFLOAT4& color);
	
private:
	bool InitializeShader11();
	
	void ShutdownShader();
	
	ID3D11Buffer* m_pPerBatchCB;
	ID3D11Buffer* m_pInstanceData;
	ID3D11Buffer* vertexBuffer;
	
	SpriteDrawData textDrawData[MaxBatchSize];
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__FONTSHADER_H