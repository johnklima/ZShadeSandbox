#include "AnimationFrame2D.h"
#include "MaterialManager.h"
//-----------------------------------------------------------------------------------------------------------------
AnimationFrame2D::AnimationFrame2D(D3D* d3d)
:	m_FrameTexture(NULL),
	m_D3DSystem(d3d)
{
}
//-----------------------------------------------------------------------------------------------------------------
AnimationFrame2D::AnimationFrame2D(const AnimationFrame2D& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
AnimationFrame2D::~AnimationFrame2D()
{
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationFrame2D::SetFrameName(string name)
{
	mFrameName = name;
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationFrame2D::SetFrameBasePath(string frameBasePath)
{
	mFrameBasePath = frameBasePath;
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationFrame2D::SetFramePath(string path)
{
	mFramePath = path;
}
//-----------------------------------------------------------------------------------------------------------------
ZShadeSandboxMesh::QuadMesh* AnimationFrame2D::GetFrameTexture()
{
	return m_FrameTexture;
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationFrame2D::SetFrameBox(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;

	width = w;
	height = h;
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationFrame2D::SetDirection(int dir)
{
	direction = dir;
}
//-----------------------------------------------------------------------------------------------------------------
string AnimationFrame2D::GetFrameName()
{
	return mFrameName;
}
//-----------------------------------------------------------------------------------------------------------------
float AnimationFrame2D::GetX()
{
	return x;
}
//-----------------------------------------------------------------------------------------------------------------
float AnimationFrame2D::GetY()
{
	return y;
}
//-----------------------------------------------------------------------------------------------------------------
float AnimationFrame2D::GetWidth()
{
	return width;
}
//-----------------------------------------------------------------------------------------------------------------
float AnimationFrame2D::GetHeight()
{
	return height;
}
//-----------------------------------------------------------------------------------------------------------------
int AnimationFrame2D::GetDirection()
{
	return direction;
}
//-----------------------------------------------------------------------------------------------------------------
void AnimationFrame2D::LoadFrameTexture()
{
	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, mFrameBasePath,
		mFramePath, mFramePath, mFramePath, m_D3DSystem->GetEngineOptions()->m_TransparencyKey);

	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.in2D = true;
	mp.shader = 0;
	mp.material = MaterialManager::Instance()->GetMaterial(mFramePath);
	m_FrameTexture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, width, height, true);
}
//-----------------------------------------------------------------------------------------------------------------