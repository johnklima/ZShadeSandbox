//===============================================================================================================================
// CSDesaturation
//
//===============================================================================================================================
// History
//
// -Created on 6/25/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __CSDESATURATION_H
#define __CSDESATURATION_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class CSDesaturation : public ZShadeSandboxShader::Shader
{
public:
	
	CSDesaturation(D3D* d3d, string imageName);
	CSDesaturation(const CSDesaturation& other);
	~CSDesaturation();

	ID3D11ShaderResourceView* GetGPUSRV() { return p_textureSRV; }
	byte* imageCopyFromGPU();

	bool Initialize();
	void Shutdown();
	
	void Render11(int threadX, int threadY, int threadZ);

private:
	
	//This will go to the pixel shader. It is the texture from the GPU.
	ID3D11ShaderResourceView* p_textureSRV;

	string m_ImageName;
	int m_TextureDataSize;
	byte* p_TextureData;
	ID3D11Texture2D* p_srcTexture;
	ID3D11Texture2D* p_dstTexture;

	ID3D11ShaderResourceView* p_srcData;
	ID3D11UnorderedAccessView* p_dstData;

	ID3D11Buffer* p_srcGPUBuffer;
	ID3D11Buffer* p_dstGPUBuffer;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__CSDESATURATION_H