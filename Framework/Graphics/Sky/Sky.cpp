#include "Sky.h"
#include <fstream>
#include "Camera.h"
#include "Vertex.h"
#include "CGlobal.h"
#include "TextureManager.h"
#include "MaterialManager.h"
using namespace std;
//==============================================================================================================================
//==============================================================================================================================
Sky::Sky(D3D* d3d, const std::string& basePath, const std::string& cubemapFilename, float skySphereRadius)
{
	shader = new SkyShader(d3d);

	MaterialManager::Instance()->AddDiffuse(d3d, basePath, cubemapFilename, cubemapFilename);

	ZShadeSandboxMesh::MeshParameters mp;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(skySphereRadius, skySphereRadius, skySphereRadius);
	mp.in2D = false;
	mp.useCustomShader = true;
	mp.shader = shader;
	mp.material = MaterialManager::Instance()->GetMaterial(cubemapFilename);
	
	mesh = new ZShadeSandboxMesh::SkydomeMesh(d3d, mp);
}
//==============================================================================================================================
Sky::~Sky()
{
}
//==============================================================================================================================
void Sky::SetWireframe(bool wire)
{
	shader->Wireframe() = wire;
}
//==============================================================================================================================
ID3D11ShaderResourceView* Sky::CubeMapSRV()
{
	return mCubeMapSRV;
}
//==============================================================================================================================
void Sky::Render(D3D* d3d, Camera* camera)
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.centerCam = true;
	mesh->Render(mrp);
}
//==============================================================================================================================
void Sky::RenderWithReflection(D3D* d3d, Camera* camera, float planeHeight)
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	mrp.reflection = true;
	mrp.centerCam = false;
	mrp.seaLevel = planeHeight;
	mesh->Render(mrp);
}
//==============================================================================================================================
