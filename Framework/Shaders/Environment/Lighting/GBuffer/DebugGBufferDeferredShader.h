//===============================================================================================================================
// DebugGBufferDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/11/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __DEBUGGBUFFERDEFERREDSHADER_H
#define __DEBUGGBUFFERDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
class DebugGBufferDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbInvMatrixBuffer
	{
		XMFLOAT4X4 g_InvViewProj;
	};

public:
	
	DebugGBufferDeferredShader(D3D* d3d);
	DebugGBufferDeferredShader(const DebugGBufferDeferredShader& other);
	~DebugGBufferDeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11(ID3D11ShaderResourceView* texture);
	bool RenderDepth11(ID3D11ShaderResourceView* texture, Camera* camera);

private:

	ID3D11Buffer* m_pMatrixCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__DEBUGGBUFFERDEFERREDSHADER_H