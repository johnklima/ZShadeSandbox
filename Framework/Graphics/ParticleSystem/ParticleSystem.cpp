#include "ParticleSystem.h"
//==================================================================================================================================
//==================================================================================================================================
ParticleSystem::ParticleSystem(D3D* d3d, ParticleSystemParameters psp)
:   m_parameters(psp)
,   m_D3DSystem(d3d)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void ParticleSystem::Generate()
{
	for (uint32 i = 0; i < m_parameters.maxParticles; i++)
	{
		GenerateParticle();
	}
}
//==================================================================================================================================
void ParticleSystem::Generate(vector<XMFLOAT3> knownPositions)
{
	if (knownPositions.size() != m_parameters.maxParticles) return;
	
	for (uint32 i = 0; i < m_parameters.maxParticles; i++)
	{
		GenerateParticle(knownPositions[i]);
	}
}
//==================================================================================================================================
void ParticleSystem::Generate(vector<XMFLOAT3> knownPositions, vector<uint32> knownLifetime)
{
	if (knownPositions.size() != m_parameters.maxParticles && knownLifetime.size() != m_parameters.maxParticles) return;
	
	for (uint32 i = 0; i < m_parameters.maxParticles; i++)
	{
		GenerateParticle(knownPositions[i], knownLifetime[i]);
	}
}
//==================================================================================================================================
void ParticleSystem::GenerateParticle()
{
	Particle p(m_D3DSystem);// = new Particle();
	
	p.m_position = m_parameters.systemOrigin;
	p.m_velocity = m_parameters.systemVelocity;
	p.m_acceleration = m_parameters.systemAcceleration;
	p.m_size = m_parameters.systemSize;
	p.m_color = XMFLOAT4(m_parameters.systemColor.x, m_parameters.systemColor.y, m_parameters.systemColor.z,
		m_parameters.systemTransparency);
	p.m_transparency = m_parameters.systemTransparency;
	p.m_shape = m_parameters.systemShape;
	p.m_lifetime = m_parameters.systemLifetime;
	p.m_totalLifetime = m_parameters.systemLifetime;
	p.m_textureName = m_parameters.particleTextureName;
	p.m_texturePath = m_parameters.particleTexturePath;
	p.m_blendAmount = m_parameters.particleBlendAmount;
	p.m_in2D = m_parameters.in2D;
	p.m_textureType = m_parameters.particleTextureType;
	p.m_alive = true;
	
	// Create the shape mesh for the particle
	p.CreateMesh();
	
	m_particles.push_back(p);
}
//==================================================================================================================================
void ParticleSystem::GenerateParticle(Particle p)
{
	m_particles.push_back(p.Clone());
}
//==================================================================================================================================
void ParticleSystem::GenerateParticle(XMFLOAT3 position)
{
	Particle p(m_D3DSystem);// = new Particle();
	
	p.m_position = position;
	p.m_velocity = m_parameters.systemVelocity;
	p.m_acceleration = m_parameters.systemAcceleration;
	p.m_size = m_parameters.systemSize;
	p.m_color = XMFLOAT4(m_parameters.systemColor.x, m_parameters.systemColor.y, m_parameters.systemColor.z,
		m_parameters.systemTransparency);
	p.m_transparency = m_parameters.systemTransparency;
	p.m_shape = m_parameters.systemShape;
	p.m_lifetime = m_parameters.systemLifetime;
	p.m_totalLifetime = m_parameters.systemLifetime;
	p.m_textureName = m_parameters.particleTextureName;
	p.m_texturePath = m_parameters.particleTexturePath;
	p.m_blendAmount = m_parameters.particleBlendAmount;
	p.m_in2D = m_parameters.in2D;
	p.m_textureType = m_parameters.particleTextureType;
	p.m_alive = true;
	
	// Create the shape mesh for the particle
	p.CreateMesh();
	
	m_particles.push_back(p);
}
//==================================================================================================================================
void ParticleSystem::GenerateParticle(XMFLOAT3 position, uint32 lifetime)
{
	Particle p(m_D3DSystem);// = new Particle();
	
	p.m_position = position;
	p.m_velocity = m_parameters.systemVelocity;
	p.m_acceleration = m_parameters.systemAcceleration;
	p.m_size = m_parameters.systemSize;
	p.m_color = XMFLOAT4(m_parameters.systemColor.x, m_parameters.systemColor.y, m_parameters.systemColor.z,
		m_parameters.systemTransparency);
	p.m_transparency = m_parameters.systemTransparency;
	p.m_shape = m_parameters.systemShape;
	p.m_lifetime = lifetime;
	p.m_totalLifetime = lifetime;
	p.m_textureName = m_parameters.particleTextureName;
	p.m_texturePath = m_parameters.particleTexturePath;
	p.m_blendAmount = m_parameters.particleBlendAmount;
	p.m_in2D = m_parameters.in2D;
	p.m_textureType = m_parameters.particleTextureType;
	p.m_alive = true;
	
	// Create the shape mesh for the particle
	p.CreateMesh();
	
	m_particles.push_back(p);
}
//==================================================================================================================================
void ParticleSystem::Clear()
{
	m_particles.clear();
}
//==================================================================================================================================
void ParticleSystem::Update()
{
	vector<vector<Particle>::iterator> iters;
	vector<Particle>::iterator iter = m_particles.begin();
	
	for (; iter != m_particles.end(); iter++)
	{
		// Apply the master force of acceleration
		(*iter).AddForce(m_parameters.systemAcceleration);
		
		// Update the position and life of the particle
		(*iter).Update();
		
		// Flag particle to remove
		if ((*iter).Dead())
		{
			iters.push_back(iter);
		}
	}
	
	vector<vector<Particle>::iterator>::iterator p_iter = iters.begin();
	
	// Remove dead particles
	for (; p_iter != iters.end(); p_iter++)
	{
		// Regenerate the dead particle
		if (m_parameters.systemRegeneration)
		{
			switch (m_parameters.systemType)
			{
				// Generate a new particle from the old position and total lifetime
				// before degeneration
				case PST_Projectile:
					GenerateParticle((*p_iter)->m_position, (*p_iter)->m_totalLifetime);
					break;
				// Generate a new particle from the origin of the system
				default:
					GenerateParticle();
					break;
			}
		}
		
		m_particles.erase((*p_iter));
	}
}
//==================================================================================================================================
void ParticleSystem::Render(Camera* camera)
{
	vector<Particle>::iterator iter = m_particles.begin();
	
	for (; iter != m_particles.end(); iter++)
	{
		(*iter).Render(camera);
	}
}
//==================================================================================================================================