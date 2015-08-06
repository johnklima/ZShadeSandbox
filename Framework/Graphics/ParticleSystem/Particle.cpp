#include "Particle.h"
#include "MaterialManager.h"
//==================================================================================================================================
//==================================================================================================================================
Particle::Particle(D3D* d3d)
:   m_D3DSystem(d3d)
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	m_position = XMFLOAT3(0, 0, 0);
	m_velocity = XMFLOAT3(0, 0, 0);
	m_acceleration = XMFLOAT3(0, 0, 0);
	m_size = XMFLOAT3(0, 0, 0);
	m_transparency = 255.0f;
	m_color = XMFLOAT4(0, 0, 0, m_transparency);
	m_textureName = "";
	m_shape = PS_Circle;
	m_lifetime = 255;
	m_totalLifetime = 255;
	m_alive = false;
	m_texturePath = "";
	m_blendAmount = 0;
	m_in2D = false;
	m_textureType = DDS;
	m_mass = 1;
}
//==================================================================================================================================
void Particle::AddForce(XMFLOAT3 force)
{
	// Newtons 2nd law: force = mass * acceleration
	
	ZShadeSandboxMath::XMMath3 f(force.x, force.y, force.z);
	
	f /= m_mass;
	
	m_acceleration.x += f.x;
	m_acceleration.y += f.y;
	m_acceleration.z += f.z;
}
//==================================================================================================================================
void Particle::CreateMesh()
{
	MaterialManager::Instance()->AddDiffuse(m_D3DSystem, m_texturePath, m_textureName, m_textureName);

	ZShadeSandboxMesh::MeshParameters mp;
	mp.pos = m_position;
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = m_size;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = false;
	mp.material = MaterialManager::Instance()->GetMaterial(m_textureName);
	mp.in2D = m_in2D;

	if (BetterString(m_textureName) == "")
	{
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Color;
	}
	
	// Currently not using rotation
	switch (m_shape)
	{
		case PS_Circle: m_CircleMesh = new ZShadeSandboxMesh::CircleMesh(m_D3DSystem, m_size.x, mp); break;
		case PS_Spherical: m_SphereMesh = new ZShadeSandboxMesh::SphereMesh(m_size.x, 20, 20, m_D3DSystem, mp); break;
		case PS_Rectangular: m_RectangleMesh = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, m_size.x, m_size.y, true); break;
	}
}
//==================================================================================================================================
void Particle::Update()
{
	if (Dead()) return;
	
	if (m_lifetime <= 0)
	{
		m_alive = false;
		
		// The particle just died so do nothing
		return;
	}
	
	// Apply the acceleration of gravity to the particle if applicable
	m_velocity.y += m_acceleration.y;
	
	// Accelerate x/z of the particle if applicable
	// used to speed up or slow down the particle
	m_velocity.x += m_acceleration.x;
	m_velocity.z += m_acceleration.z;
	
	// Calculate the final position of the particle
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
	m_position.z += m_velocity.z;
	
	// Clear the acceleration
	m_acceleration.x *= 0.0f;
	m_acceleration.y *= 0.0f;
	m_acceleration.z *= 0.0f;
	
	// Update the position of the shape
	switch (m_shape)
	{
		case PS_Circle: m_CircleMesh->Position() = m_position; break;
		case PS_Spherical: m_SphereMesh->Position() = m_position; break;
		case PS_Rectangular: m_RectangleMesh->Position() = m_position; break;
	}
	
	// Decrease the lifetime for this particle
	m_lifetime--;
}
//==================================================================================================================================
bool Particle::Dead()
{
	return !m_alive;
}
//==================================================================================================================================
void Particle::Render(Camera* camera)
{
	if (Dead()) return;
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = camera;
	//mrp.blendAmount = m_blendAmount;

	// Need to be able to generate color vertices in a mesh class, currently it just generates texture vertices
	// This will default to rendering with just color
	switch (m_shape)
	{
		case PS_Circle: m_CircleMesh->Render(mrp); break;
		case PS_Spherical: m_SphereMesh->Render(mrp); break;
		case PS_Rectangular: m_RectangleMesh->Render(mrp); break;
	}
}
//==================================================================================================================================
Particle Particle::Clone()
{
	Particle p(m_D3DSystem);
	
	Clone( p );
	
	return p;
}
//==================================================================================================================================
void Particle::Clone(Particle &c)
{
	c.m_position = m_position;
	c.m_velocity = m_velocity;
	c.m_acceleration = m_acceleration;
	c.m_size = m_size;
	c.m_transparency = m_transparency;
	c.m_color = m_color;
	c.m_textureName = m_textureName;
	c.m_shape = m_shape;
	c.m_lifetime = m_lifetime;
	c.m_totalLifetime = m_totalLifetime;
	c.m_alive = m_alive;
	c.m_texturePath = m_texturePath;
	c.m_in2D = m_in2D;
	c.m_textureType = m_textureType;
}
//==================================================================================================================================