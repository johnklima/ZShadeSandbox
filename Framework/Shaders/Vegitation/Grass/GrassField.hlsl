//==============================================================================
// GrassField Shader
//
// Based on MIT Master Thesis: http://illogictree.com/upload/site/LeeRealtimeGrassThesis.pdf
//==============================================================================
// History
//
// -Created on 7/12/2014 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbConst : register(b1)
{
	float kHalfPi;
}

cbuffer cbMatrix : register(b1)
{
	float4x4 g_World;
	float4x4 g_View;
	float4x4 g_Proj;
	float3 g_EyePos;
}

//======================================================================================================

//
// Textures and Samplers
//

SamplerState PointSampler : register(s0);
SamplerState LinearSampler : register(s1);

Texture2D displacementMap : register(t0);

//Texture2D InputTexture0 : register(t0);
//Texture2D InputTexture1 : register(t1);
//Texture2D InputTexture2 : register(t2);
//Texture2D InputTexture3 : register(t3);

//Texture2D<int4> InputTextureInt0 : register(t0);
//Texture2D<int4> InputTextureInt1 : register(t1);
//Texture2D<int4> InputTextureInt2 : register(t2);
//Texture2D<int4> InputTextureInt3 : register(t3);

//Texture2D<uint4> InputTextureUint0 : register(t0);
//Texture2D<uint4> InputTextureUint1 : register(t1);
//Texture2D<uint4> InputTextureUint2 : register(t2);
//Texture2D<uint4> InputTextureUint3 : register(t3);

//======================================================================================================

//
// Structs
//

struct VertexInput
{
    float4 position : POSITION;
	//float2 tex_uv   : TEXCOORD0;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
	//float2 tex_uv   : TEXCOORD0;
};

struct GeometryOutput
{
    float4 position : SV_POSITION;
};

//======================================================================================================

//
// Global Functions
//

float PosRandom(float x, float y)
{
	return sin(kHalfPi * frac(x) + kHalfPi * frac(y));
}

//======================================================================================================

//
// Vertex Shaders
//

VertexOutput GrassFieldVS(VertexInput input)
{
	VertexOutput output;

	output.position = input.position;

	return output;
}

//======================================================================================================

//
// Geometry Shaders
//

//The Geometry Shader takes in a point which is the root node of each grass blade
//The maxvertexcount is dependent upon LOD
// Low  LOD 1: 4 verts
// Mid  LOD 2: 8 verts
// Hidh LOD 3: 12 verts
/*
Geometry Shader
  The vertex buffer's vertices are first passed through the gemetry shader with the LOD integer passed in during the render call.
  It dynamically chooses how detailed the grass bladed will be and how much vertices make up the grass blade.
  Detail is determined by how many triangles the grass blade has. LOD for grass is how many times to segment the grass blade.
  The geometry shader uses the root position in the vertex buffer to determine where to generate the blade of grass.
  Since the geometry shader works best when it knows exactly how many vertices are used so each LOD will have a constant # of vertices.
  The geometry shader displaces the blade depending on the terrain and wind forces.

Grass Displacement (Done in Geometry Shader)
  1st Step: Make the grass blade's vertices contour to the terrain's form from the Terrain displacement map texture.
*/
/*
Randomness
  Use the grass blade's root position to generate a random number.
  Root position can be used to generate another random number by using:

      f(Pos) = sin(kHalfPi * frac(Pos.x) + kHalfPi * frac(Pos.y))

  This formula returns a normalized random number given a root position.
  frac() return the fraction of a number (0.0 to 1.0).
  The fraction is taken so that the value returned from sin() is not periodic.
  Then multiply each of the fraction's number by half-PI for a result between 0 and PI.
  This random number can be used to randomize certain features of the grass blade.
  This number is used to orient the grass blades in a different direction.
  It is also used to randomize the heights and color of each blade to simulate the various stages of life for a grass blade.
  It can also be used to make the grass blade naturally lean in random directions.
*/
/*
Wind
  The amount of influence the wind has on the blade is directly proportional to the grass blade's current state.
  The oscillation of the blade can be simulated within the shader by taking the oscillation zone
  and oscillating the curve's vertices within the zone using the sinusoidal wave.
  Make sure that oscillations of every blade in the grass field is not uniform.
  Different blades have different weights and thus different momentums when interacting with wind.
  Skew the oscillation zone per blade as well as perform a phase shift on the oscillation to simulate this.
  The offset and phase shift amount can be determined by using the random number in the previous section.
*/
/*
Vertex Displacement
  The shader can displace the vertices of the grass blades according to the desired shape of the blade.
  The greater the wind force the more vertex will be translated horizontally.
  The amount that the vertex is displaced horizontally is directly proportional to the magnitude of the wind.
  The vertex must also be displaced vertically.
  The greater the wind force is on the blade, the more the vertex is displaced downwards in the y-direction.
  This function relates wind force and displacement:

      Pos.xz = Pos.xz + WindVec * WindCoeff
	  Pos.y = Pos.y - length(WindVec) * WindCoeff * 0.5
  
  The WindCoeff is determined by how far the current vertex is from the root of the blade.
  The higher up the vertex is on the blade the more the vertex is affected by wind.
  The vertex of the root of the blade will have a WindCoeff of zero meaning it will not move
  The vertex at the top of the blade will be most affected and will have WindCoeff of one.

  The WindCoeff ranges from [0..1] - Base to Tip of blade
*/

float3 triBase[3] =
{
	float3( 1,  1, 1),
	float3( 1, -1, 1),
	float3(-1, -1, 1)
};

float3 quadBase[4] =
{
	float3(-1, -1, 1),
	float3(-1,  1, 1),
	float3( 1,  1, 1),
	float3( 1, -1, 1)
};
//------------------------------------------------------------------
[maxvertexcount(6)]
void GrassFieldLowLOD_GS(point VertexOutput input[1], inout TriangleStream<GeometryOutput> OutputStream)
{
	//Create a triangle from a point
	float3 pos = input[0].position.xyz;

	float3 tris[6];
	tris[0] = pos + quadBase[0];
	tris[1] = pos + quadBase[1];
	tris[2] = pos + quadBase[2];

	tris[3] = pos + quadBase[0];
	tris[4] = pos + quadBase[2];
	tris[5] = pos + quadBase[3];

	GeometryOutput output;

	for (int i = 0; i < 6; i++)
	{
		output.position = float4(tris[i], 1);
		output.position = mul(output.position, g_World);
		output.position = mul(output.position, g_View);
		output.position = mul(output.position, g_Proj);
		//output.color = input[0].color;

		OutputStream.Append(output);
	}

	OutputStream.RestartStrip();
}
/*
[maxvertexcount(4)]
void GrassFieldLowLOD_GS(point VertexOutput input[1], inout TriangleStream<GeometryOutput> OutputStream)
{
	float displacement = displacementMap.SampleLevel( LinearSampler, input[0].position.xz, 0 ).r;

	float3 verts1;
	verts1.x = input[0].position + 500;
	verts1.y = input[0].position;
	verts1.z = input[0].position;

	float3 verts2;
	verts2.x = input[0].position + 500;
	verts2.y = input[0].position + 0;
	verts2.z = input[0].position;

	float3 verts3;
	verts3.x = input[0].position;
	verts3.y = (input[0].position + 500) + 0;
	verts3.z = input[0].position;

	float3 verts4;
	verts4.x = input[0].position + 500;
	verts4.y = (input[0].position + 500) + 0;
	verts4.z = input[0].position;

	//
	//Create two triangles
	//

	GeometryOutput output;
	
	output.position = float4(verts1, 1);
	output.position = mul(output.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	OutputStream.Append(output);

	output.position = float4(verts2, 1);
	output.position = mul(output.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	OutputStream.Append(output);

	output.position = float4(verts3, 1);
	output.position = mul(output.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	OutputStream.Append(output);

	//output.position = float4(verts4, 1);
	//output.position = mul(output.position, g_World);
	//output.position = mul(output.position, g_View);
	//output.position = mul(output.position, g_Proj);
	//OutputStream.Append(output);


	OutputStream.RestartStrip();
	*/

	//
	// Compute the local coordinate system of the sprite relative to the world
	// space such that the billboard is aligned with the y-axis and faces the eye.
	//

	/*float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = g_EyePos - input[0].position;
	look.y = 0.0f; // y-axis aligned, so project to xz-plane
	look = normalize(look);
	float3 right = cross(up, look);

	//
	// Compute triangle strip vertices (quad) in world space.
	//
	float halfWidth  = 0.5f*24.0f;//gin[0].SizeW.x;
	float halfHeight = 0.5f*24.0f;//gin[0].SizeW.y;
	
	float4 v[4];
	v[0] = float4(input[0].position + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(input[0].position + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(input[0].position - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(input[0].position - halfWidth*right + halfHeight*up, 1.0f);

	float4x4 gViewProj = mul(g_View, g_Proj);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GeometryOutput output;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		output.position = mul(v[i], gViewProj);
		OutputStream.Append(output);
	}
}
*/

//======================================================================================================

//
// Pixel Shaders
//

float4 GrassFieldPS(GeometryOutput input) : SV_Target
{
	//Return a green color
	return float4(0,1,0,1);
}
