#include "ParticleTrajectoryTest.h"
//==================================================================================================================================
//==================================================================================================================================
ParticleTrajectoryTest::ParticleTrajectoryTest(D3D* d3d)
{
	// Create a particle system that shoots particles from the origin
	
	ParticleSystemParameters psp;
	
	// Where does this particle system begin
	psp.systemOrigin = XMFLOAT3(0, 0, 0);
	// Maximum amount of particles that this system can have at any time
	psp.maxParticles = 1;
	// Global velocity factor to apply to every particle
	psp.systemVelocity = XMFLOAT3(0, 0, 0);
	// Accelerate the particles along the y-axis
	psp.systemAcceleration = XMFLOAT3(0, 0.980665f, 0);
	// Global size (scale) to apply to every particle
	psp.systemSize = XMFLOAT3(50, 50, 50);
	// Global lifetime to apply to every particle
	psp.systemLifetime = 255;
	// Global color to apply to every particle
	psp.systemColor = XMFLOAT3(0, 0, 0);
	// Global transparency to apply to every particle
	psp.systemTransparency = 255;
	// Global shape of particles
	psp.systemShape = PS_Circle;
	// What type of particle system are we creating
	psp.systemType = PST_Projectile;
	// Can this particle system keep regenerating itself
	psp.systemRegeneration = false;
	// Texture name for the particle to load
	psp.particleTextureName = "blue.png";
	// Full path to the texture
	psp.particleTexturePath = "";
	// Transparency blend amount for the particles
	psp.particleBlendAmount = 0;
	// Is this particle in 2D or 3D
	psp.in2D = false;
	// This particle is a PNG or DDS texture
	psp.particleTextureType = PNG;

	m_ParticleSystem = new ParticleSystem(d3d, psp);
	
	// Create some particles around the origin
	m_ParticleSystem->Generate();
}
//==================================================================================================================================
void ParticleTrajectoryTest::Update()
{
	m_ParticleSystem->Update();
}
//==================================================================================================================================
void ParticleTrajectoryTest::Render(Camera* camera)
{
	m_ParticleSystem->Render(camera);
}
//==================================================================================================================================