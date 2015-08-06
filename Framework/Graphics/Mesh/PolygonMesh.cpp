#include "PolygonMesh.h"
#include "Shaders.h"
#include "MaterialShader.h"
#include "MaterialManager.h"
using ZShadeSandboxMesh::PolygonMesh;
//==================================================================================================================================
//==================================================================================================================================
PolygonMesh::PolygonMesh(ZShadeSandboxMath::Polygon* poly, ZShadeSandboxLighting::ShaderMaterial* material)
:	mPolygon(poly)
,	mMaterial(material)
,	m_Wireframe(false)
{
}
//==================================================================================================================================
XMFLOAT3& PolygonMesh::Position()
{
	return mPolygon->Attributes()->mPosition;
}
//==================================================================================================================================
XMFLOAT3& PolygonMesh::Scale()
{
	return mPolygon->Attributes()->mScale;
}
//==================================================================================================================================
XMFLOAT3& PolygonMesh::Rotate()
{
	return mPolygon->Attributes()->mRotation;
}
//==================================================================================================================================
void PolygonMesh::BuildIndexBuffer()
{
	mPolygon->Attributes()->BuildIndexBuffer();
}
//==================================================================================================================================
void PolygonMesh::BuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type type)
{
	mPolygon->Attributes()->BuildVertexBuffer(type);
}
//==================================================================================================================================
void PolygonMesh::ReBuildVertexBuffer(ZShadeSandboxMesh::EVertexType::Type type)
{
	mPolygon->Attributes()->ReBuildVertexBuffer(type);
}
//==================================================================================================================================
void PolygonMesh::Render(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	// Cannot do anything if there is no camera
	if (mrp.camera == 0) return;
	
	mPolygon->Attributes()->SetBuffers(mrp.renderType);
	
	//mrp.world = mPolygon->Attributes()->WorldXM();
	mrp.world = XMMatrixIdentity();
	
	if (mrp.reflection)
		mrp.view = mrp.camera->ReflectionView4x4();
	else
		mrp.view = mrp.camera->View4x4();
	
	ZShadeSandboxShader::Shaders::pMaterialShader->Wireframe() = m_Wireframe;
	ZShadeSandboxShader::Shaders::pMaterialShader->Render11(mPolygon->GetIndexCount(), 0, mrp, mMaterial);
}
//==================================================================================================================================