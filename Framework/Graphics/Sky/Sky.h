//=====================================================================================
//Sky.h
//=====================================================================================
#ifndef __SKY_H
#define __SKY_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Camera.h"
#include "SkyShader.h"
#include "SkydomeMesh.h"
//==============================================================================================================================
//==============================================================================================================================
class Sky
{
public:
	
	Sky(D3D* d3d, const std::string& basePath, const std::string& cubemapFilename, float skySphereRadius);
	Sky(const Sky& rhs);
	~Sky();

	Sky& operator=(const Sky& rhs);

	ID3D11ShaderResourceView* CubeMapSRV();

	void Render(D3D* d3d, Camera* camera);
	void RenderWithReflection(D3D* d3d, Camera* camera, float planeHeight);

	void SetWireframe(bool wire);
	
	int TriangleCount() const { return mesh->TriangleCount(); }

private:
	//ID3D11Buffer* mVB;
	//ID3D11Buffer* mIB;
	//UINT mIndexCount;

	ID3D11ShaderResourceView* mCubeMapSRV;
	
	ZShadeSandboxMesh::SkydomeMesh* mesh;
	SkyShader* shader;
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__SKY_H
