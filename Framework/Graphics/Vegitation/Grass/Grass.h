//===============================================================================================================================
// Grass
//
//===============================================================================================================================
// History
//
// -Created on 7/11/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __GRASS_H
#define __GRASS_H
//===============================================================================================================================
//===============================================================================================================================
/* Notes
Build the initial vertex position on xz plane for each grass position.
The initial position is randomized according to the size of the grass patch.
The height of the grass is determined by the Terrain displacement map texture.

Geometry ZShadeSandboxShader::Shader
  The vertex buffer's vertices are first passed through the gemetry shader with the LOD integer passed in during the render call.
  It dynamically chooses how detailed the grass bladed will be and how much vertices make up the grass blade.
  Detail is determined by how many triangles the grass blade has. LOD for grass is how many times to segment the grass blade.
  The geometry shader uses the root position in the vertex buffer to determine where to generate the blade of grass.
  Since the geometry shader works best when it knows exactly how many vertices are used so each LOD will have a constant # of vertices.
  The geometry shader displaces the blade depending on the terrain and wind forces.

Grass Displacement (Done in Geometry ZShadeSandboxShader::Shader)
  1st Step: Make the grass blade's vertices contour to the terrain's form from the Terrain displacement map texture.

Randomness
  Use the grass blade's root position to generate a random number.
  Root position can be used to generate another random number by using:

      f(Vertex) = sin(kHalfPi * frac(Vertex.x) + kHalfPi * frac(Vertex.y))

  This formula returns a normalized random number given a root position.
  frac() return the fraction of a number (0.0 to 1.0).
  The fraction is taken so that the value returned from sin() is not periodic.
  Then multiply each of the fraction's number by half-PI for a result between 0 and PI.
  This random number can be used to randomize certain features of the grass blade.
  This number is used to orient the grass blades in a different direction.
  It is also used to randomize the heights and color of each blade to simulate the various stages of life for a grass blade.
  It can also be used to make the grass blade naturally lean in random directions.

Wind
  The amount of influence the wind has on the blade is directly proportional to the grass blade's current state.
  The oscillation of the blade can be simulated within the shader by taking the oscillation zone
  and oscillating the curve's vertices within the zone using the sinusoidal wave.
  Make sure that oscillations of every blade in the grass field is not uniform.
  Different blades have different weights and thus different momentums when interacting with wind.
  Skew the oscillation zone per blade as well as perform a phase shift on the oscillation to simulate this.
  The offset and phase shift amount can be determined by using the random number in the previous section.

Vertex Displacement
  The shader can displace the vertices of the grass blades according to the desired shape of the blade.
  The greater the wind force the more vertex will be translated horizontally.
  The amount that the vertex is displaced horizontally is directly proportional to the magnitude of the wind.
  The vertex must also be displaced vertically.
  The greater the wind force is on the blade, the more the vertex is displaced downwards in the y-direction.
  This function relates wind force and displacement:

      Vertex.xz = Vertex.xz + WindVec * WindCoeff
	  Vertex.y = Vertex.y - length(WindVec) * WindCoeff * 0.5
  
  The WindCoeff is determined by how far the current vertex is from the root of the blade.
  The higher up the vertex is on the blade the more the vertex is affected by wind.
  The vertex of the root of the blade will have a WindCoeff of zero meaning it will not move
  The vertex at the top of the blade will be most affected and will have WindCoeff of one.

  The WindCoeff ranges from [0..1] - Base to Tip of blade

Vertex ZShadeSandboxShader::Shader
  Only needs to pass the vertex data into the pixel shader, in which the vertex values are interpolated.

Pixel ZShadeSandboxShader::Shader
  Handles the coloration and texturing of the grass blades.
  -1st step is to render each blade's shape by using a technique called alpha-to-coverage.
   Alpha-to-coverage is a multisampling computer graphics technique used to render dense vegetation or grass.
   Alpha-to-coverage used one texture as the color map and another texture for the alpha map.
   Depending on the alpha values of the alpha map, certain pixels are culled so that only the shape of the blade remains.
    (Done using the clip(-1) function for the pixels that are culled)
  -2nd step handle the color tinting of each blade.
   Some grass blades my be healthy and green while others are old and withered.
   Simulate the variation and color by using the normalized random number (previously mentioned) and linearly
   interpolating between the diffuse color and an orange color for natural color variation.
  -Ambient Occlusion occurs when grass blades affect the attenuation of light due to the occlusion of nearby blades.
   This is what caused the grass blades to appear darker towards its root while appearing lighter towards its top.
   To simulate this the pixel shader must keep track of the pixel's relative height in the blade.
   A pixel that is located near the root of the blade will have a low value while pixels at the top have a high (bright) value.
  -Other lighting techniques can be used like per-pixel lighting and specular lighting can be applied to each blade.
   The normal of the current pixel can be used to calculate lighting and it is calculated in the geometry shader by taking
   the three vertices and calculating their cross product. This normal is fed into the vertex shader and linearly interpolated
   by the pixel shader. Taking the dot product of the normal by the light position achieves a per-pixel lighting effect.

Flicker Prevention
  To reduce flicker in the distance only need to decrease the number of root positions that is to be rendered during the render call.
  Decrease the blade count gradually (depending on distance from camera) allows for a smooth transition when the player
  traverses the scene.
  Decreasing the blade count also reduces the amount of vertices that get passed through the graphics pipeline and decreases the workload.
  Visual Artifact can occur in the distance where the blades are so thin that the grass blade flickers between the terrain and the blade's pixels.
  To solve this increase the thickness of the blade as the grass is father into the distance.
  The thickness is proportinal to the destance that the grass blade is to the camera.
  The width of the blade can be found by linearly interpolating between the minimum and maximum blade width based on distance
  of the blade to the camera:

      width = lerp(min width, max width, distance normalized)

  This allows distant grass blades to fill the screen which solves the problem of flickering in distant grass blades.

Vertext-Processing
  The entire scene is first rendered to a render target. Then several Vertext-processing effects can be applied in order to enhance
  the visual quality of the scene.

Fog
  The first Vertext-processing effect applied to the scene is similar to fog.
  Used to simulate the effect that occurs naturally when looking at far away objects.
  Far away objects tend to have a more desaturated color compared to objects near the camera.
  The distant objects' light rays must travel at a greater distance than the light rays of nearby objects.
  These light rays are intercepted by molecules in the air which absorb the light rays.
  The longer path the light ray must travel to reach the viewer, the more the light ray collides with
  air molecules and thus the more desaturated the colors are.
  To add this fog the render target stores the depth buffer of the scene, which provides non-linear depth info
  of each pixel in the render target. This value can be converted to the actual depth using a simple calculation.
  Using this depth info linearly interpolate between the raw render target's diffuse color and the environmental fog color.

Bloom
  Aims to reproduce the imaging artifacts of real-world cameras and the human eye lens.
  Based on the fact that in the real world, the lens can never focus perfectly and will convolve the incoming light rays to bounce
  and blur within the retina, causing the light to blur onto nearby pixels.
  To do bloom the Current raw render target of the scene must be provided.
  For each pixel in the render target, calculate its luminosity by taking the RGB vector and applying
  the dot product with the luminosity vector (0.30, 0.59, 0.11). This value describes how intensely bright the color value is.
  The luminosity of the pixel is passed through a threshold function such that given a luminosity value x and for a certain
  threshold value c:

      f(x) = 0 if x <= c or x if x > c

  Save this threshold data onto another render target. Apply a Gaussian blur to the threshold render target to give
  a nice bloom effect and save the data to another render target.
  Add the values from the blurred threshold render target onto the original render target, giving a nice bloom effect.

Depth-of-Field
  When the camera lens focuses is on a particular object, the object will appear sharp while out-of-focus objects will appear blurred.
  This effect is due to the lens' distance to its subject, its focal length as well as its circle-of-confusion.
  Render the scene several times around the focal point, then blur the render targets together to produce the depth-of-field effect.
  To achieve this, the current render target is first blurred and then outputted to another render target. Then, render
  the scene by sampling the depth buffer. Given the target depth (where you want the player to focus on), calculate, for each pixel,
  the difference of depth between the current depth and the target depth. Normalize the value and use the result as the linear
  interpolation factor between using the original render target and the blurred render target.
  This results in the final image, in which the closer the pixels are to the target depth, the more defined the pixels are;
  while pixels that are really far away from the target depth will have a more blurred appearance.

Shadows
  For shadows in the scene use standard shadow mapping.
  Render the scene from the point of view of the light to capture the depth of the scene. (LightViewProjection)
  Outdoor lighting is handled as an orthographic projection from the direction of the sun.
  NOTE: Since this step only acquires the depth of the scene. Any lighting and texturing of the blades should be
        removed from the pixel shader.
  However, to still preserve the shape of the blade, alpha-to-coverage must still be implemented in the pixel shader to clip out
  unneeded pixels.
  To render the shadows in the pixel shader, compare the depths of the current pixel to the depth from the shadow map.
  This is done by bringing the pixel to the view-space of the light.

  If (depth of shadow map's pixel < current pixel)
      there is a shadow
  else
      it is fully lit.

  This requires the blades of grass to be rendered twice: once to capture the shadow depth and once more during the final scene render.
  An efficient way to solve this problem would be to rende ronly a few select patches that require shadows.
  The Spots that need shadows are the ones that are closest to the camera.
  In order to render shadows according to the shadow mapping algorithm, the pixel shader needs two extra fields passed from
  the vertex shader: world position and projection texture coordinates.
  Since this reduces frame rate it is not currently a good candidate for a final render.

  The number of blades in a patch is dependent on the LOD
  The number of grass blades gradually decreases as the patch is farther away from the camera
  Use 4 LOD
    The coefficients of the number of grass blades in each LOD
    LOD 1: 0.3
	LOD 2: 0.85
	LOD 3: 0.95
	LOD 4: 1.0

  Inverse Squared: 1/pow(x,2)

*/
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "GrassFieldMesh.h"

//===============================================================================================================================
//===============================================================================================================================
class GrassField
{
	static const int MAX_LOD = 4;

	float m_LodCoeff[MAX_LOD];

public:
	GrassField(D3D* d3d);
	~GrassField();

	void Initialize(int fieldSize, int width, vector<float> heightmap);

	void Update(float dt);
	void Render(Camera* camera);

private:
	D3D* m_pD3DSystem;

	//Are we rendering points
	bool m_RenderPoints;

	//The mesh for the grass field
	GrassFieldMesh* m_pGFM;

	ID3D11ShaderResourceView* m_pAlphaTopSRV;
	ID3D11ShaderResourceView* m_pDiffuseTopSRV;
	ID3D11ShaderResourceView* m_pAlphaBodySRV;
	ID3D11ShaderResourceView* m_pDiffuseBodySRV;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__GRASS_H