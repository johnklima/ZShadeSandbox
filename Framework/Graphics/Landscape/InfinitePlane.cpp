#include "InfinitePlane.h"
//===============================================================================================================================
InfinitePlane::InfinitePlane(float planeSize, float planeHeight, float gridSpacing, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   fGridSpacing(gridSpacing)
{
	mMesh = new ZShadeSandboxMesh::PlaneMesh(planeSize, planeHeight, d3d, mp);
	mWorld = XMMatrixIdentity();
}
//===============================================================================================================================
InfinitePlane::~InfinitePlane()
{
}
//===============================================================================================================================
void InfinitePlane::SetWireframe(float wire)
{
	mMesh->SetWireframe(wire);
}
//===============================================================================================================================
void InfinitePlane::Render(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	XMFLOAT3 eye = mrp.camera->Position();
	
	XMMATRIX world, scale, translate;
	
	// scale to meet the grid spacing and translate only in exact steps of the grid spacing
	scale = XMMatrixScaling(fGridSpacing, fGridSpacing, fGridSpacing);
	translate = XMMatrixTranslation((eye.x / fGridSpacing) * fGridSpacing, 0.0f, (eye.z / fGridSpacing) * fGridSpacing);
	
	world = XMMatrixMultiply( mWorld, scale );
	world = XMMatrixMultiply( world, translate );
	
	mrp.specifyWorld = true;
	mrp.world = world;
	
	mMesh->Render(mrp);
}
//===============================================================================================================================
void InfinitePlane::Update(float planeHeight)
{
	mMesh->UpdateBuffers(planeHeight);
}
//===============================================================================================================================