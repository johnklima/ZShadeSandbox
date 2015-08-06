//==================================================================================================================================
// Particle.h
//
//==================================================================================================================================
// History
//
// -Created on 3/4/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PARTICLE_H
#define __PARTICLE_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "CircleMesh.h"
#include "QuadMesh.h"
#include "SphereMesh.h"
#include "Camera.h"
//==================================================================================================================================
enum ParticleShape
{
	PS_Circle,
	PS_Spherical,
	PS_Rectangular,
	PS_StreakedSpherical // Used to motion-blur particles
};
//==================================================================================================================================
class Particle
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// Particle mesh system
	ZShadeSandboxMesh::CircleMesh* m_CircleMesh;
	ZShadeSandboxMesh::SphereMesh* m_SphereMesh;
	ZShadeSandboxMesh::QuadMesh* m_RectangleMesh;
	
public:
	
	Particle(D3D* d3d);
	
	void AddForce(XMFLOAT3 force);
	
	void Update();
	
	void Render(Camera* camera);
	
	bool Dead();
	
	void CreateMesh();
	
	// Name of the texture file of the particle
	string m_textureName;

	// Actual path of the texture to locate
	string m_texturePath;

	// transparency blend amount for a transparent texture
	float m_blendAmount;

	// Is this particle in 2D or 3D
	bool m_in2D;

	// Dictates whether this particle uses texture or color vertices
	TextureType m_textureType;

	// initial position of the particle
	XMFLOAT3 m_position;
	
	// initial velocity of the particle
	XMFLOAT3 m_velocity;
	
	// acceleration of the particle
	XMFLOAT3 m_acceleration;
	
	// initial size of the particle (scale value)
	XMFLOAT3 m_size;
	
	// initial color of the particle
	XMFLOAT4 m_color;
	
	// initial transparency of the particle
	float m_transparency;
	
	// shape of the particle
	ParticleShape m_shape;
	
	// lifetime of the particle measured in frames
	uint32 m_lifetime;
	uint32 m_totalLifetime;
	
	float m_mass;
	
	bool m_alive;
	
	virtual Particle Clone();
	virtual void Clone(Particle &c);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PARTICLE_H