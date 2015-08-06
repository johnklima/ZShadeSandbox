//==================================================================================================================================
// ParticleSystem.h
//
// http://natureofcode.com/book/chapter-4-particle-systems/
//==================================================================================================================================
// History
//
// -Created on 3/4/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H
//==================================================================================================================================
//==================================================================================================================================
#include <vector>
#include "D3D.h"
#include "ZMath.h"
#include "Camera.h"
#include "Particle.h"
using namespace std;
//==================================================================================================================================
enum ParticleSystemType
{
	PST_Projectile,
	PST_Fountain,
	PST_Fireworks,
	PST_Explosion
};
//==================================================================================================================================
struct ParticleSystemParameters
{
	// Where does this particle system begin
	XMFLOAT3 systemOrigin;
	
	// Maximum amount of particles that this system can have at any time
	uint32 maxParticles;
	
	// Global velocity factor to apply to every particle
	XMFLOAT3 systemVelocity;
	
	// Global acceleration factor to apply to every particle
	XMFLOAT3 systemAcceleration;
	
	// Global size (scale) to apply to every particle
	XMFLOAT3 systemSize;
	
	// Global lifetime to apply to every particle
	uint32 systemLifetime;
	
	// Global color to apply to every particle
	// Only need to supply RGB since transparency is separate
	XMFLOAT3 systemColor;
	
	// Global transparency to apply to every particle
	float systemTransparency;
	
	// Global shape of particles
	ParticleShape systemShape;
	
	// What type of particle system are we creating
	ParticleSystemType systemType;
	
	// Name of the texture for each particle to load
	// If nothing is supplied for a texture name then the color will be used
	string particleTextureName;

	// Actual path of the texture to locate
	string particleTexturePath;

	// transparency blend amount for a transparent texture
	float particleBlendAmount;

	// Is this particle in 2D or 3D
	bool in2D;

	// Dictates whether this particle uses texture or color vertices
	TextureType particleTextureType;

	// Can this particle system keep regenerating itself
	bool systemRegeneration;
};
//==================================================================================================================================
// A particle can change in color, transparency and size
// Might need to create a separate thread to update all the particles
class ParticleSystem
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
public:
	
	ParticleSystem(D3D* d3d, ParticleSystemParameters psp);
	
	void Clear();
	
	// Generate new random particles used by the system
	void Generate();
	
	// Generates particles based on a known position
	void Generate(vector<XMFLOAT3> knownPositions);
	
	// Generates particles based on a known position and a given lifetime for each particle
	void Generate(vector<XMFLOAT3> knownPositions, vector<uint32> knownLifetime);
	
	// Generate a particle from the system
	void GenerateParticle();
	
	// Generate a particle from a known particle
	void GenerateParticle(Particle p);
	
	// Generate a particle from a known position
	void GenerateParticle(XMFLOAT3 position);
	
	// Generate a particle from a known position and a given lifetime
	void GenerateParticle(XMFLOAT3 position, uint32 lifetime);
	
	// Update all particles in the system
	void Update();
	
	// Render all particles in the system
	void Render(Camera* camera);
	
protected:
	
	ParticleSystemParameters m_parameters;
	
	vector<Particle> m_particles;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PARTICLESYSTEM_H