#include "Light.h"
#include "SphereMesh.h"
#include "MaterialManager.h"
#include "LightShadingBuffers.h"
using ZShadeSandboxLighting::Light;
//==============================================================================================================================
//==============================================================================================================================
Light::Light()
{
	mMesh = NULL;
	mLightCamera = new LightCamera();
	fIntensity = 2.8f;
	bToggleLight = true;
}
//==============================================================================================================================
void Light::BuildSphereMesh(D3D* d3d)
{
	float size = 0.5;
	//float r = size / 100;
	
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = mPosition;
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(size, size, size);
	mp.material = MaterialManager::Instance()->GetMaterial("Stone");
	mp.in2D = false;
	mp.shader = 0;
	
	//mMesh = new ZShadeSandboxMesh::SphereMesh(r, size, size, d3d, mp);
	mMesh = new ZShadeSandboxMesh::SphereMesh(d3d, mp, "Models\\sphere.txt");
}
//==============================================================================================================================
void Light::UpdateLVP()
{
	mLightCamera->Update(mDirection);
}
//==============================================================================================================================
void Light::Update()
{
	mLightCamera->Position() = mDirection;
	mLightCamera->Update();
}
//==============================================================================================================================
void Light::SetLens(float fnear, float ffar)
{
	mLightCamera->Near() = fnear;
	mLightCamera->Far() = ffar;
	mLightCamera->LookAt() = XMFLOAT3(0, 0, 0);
	mLightCamera->SetLens();
}
//==============================================================================================================================
void Light::UpdateMeshPosition(XMFLOAT3 pos)
{
	mPosition = pos;
	if (mMesh != NULL) mMesh->Position() = pos;
}
//==============================================================================================================================
void Light::ScaleMesh(XMFLOAT3 scale)
{
	if (mMesh != NULL) mMesh->Scale() = scale;
}
//==============================================================================================================================
int Light::TriangleCount() const { return mMesh->TriangleCount(); }
//==============================================================================================================================
void Light::RenderSphereMesh(ZShadeSandboxLighting::LightRenderParameters lrp)
{
	if (mMesh == NULL) return;

	if (lrp.toggleMesh)
	{
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = lrp.camera;
		mrp.light = this;
		mrp.reflection = lrp.reflect;
		mrp.clipplane = lrp.clipplane;
		mrp.renderDeferred = lrp.renderDeferred;
		mMesh->SetWireframe(lrp.toggleWireframe);
		mMesh->Render(mrp);
	}
}
//==============================================================================================================================
void Light::SetMesh(ZShadeSandboxMesh::CustomMesh* mesh)
{
	mMesh = mesh;
}
//==============================================================================================================================
int Light::FetchLightTypeIndex()
{
	switch (mLightType)
	{
		case ZShadeSandboxLighting::ELightType::eAmbient: return 0;
		case ZShadeSandboxLighting::ELightType::eDirectional: return 1;
		case ZShadeSandboxLighting::ELightType::eSpot: return 2;
		case ZShadeSandboxLighting::ELightType::ePoint: return 3;
		case ZShadeSandboxLighting::ELightType::eCapsule: return 4;
		default: break;
	}

	return -1;
}
//==============================================================================================================================