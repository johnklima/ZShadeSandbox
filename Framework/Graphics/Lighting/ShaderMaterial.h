//==============================================================================================================================
// ShaderMaterial.h
//
//==============================================================================================================================
// History
//
// -Created on 4/17/2014 by Dustin Watson
//==============================================================================================================================
#ifndef __SHADERMATERIAL_H
#define __SHADERMATERIAL_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Texture.h"
#include "Camera.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
namespace EMaterialTextureType
{
	enum
	{
		eNone,
		eDiffuseArray,
		eDiffuse,
		eAmbient,
		eSpecular,
		eEmissive,
		eDetail,
		eNormal,
		eBlend,
		eAlpha,
		eShadowMap,
		eSSAOMap,
		eDisplacementMap
	};
}

namespace EMaterialTessellationType
{
	enum Type
	{
		eQuad,
		eTri
	};
}

//==============================================================================================================================
//==============================================================================================================================
class ShaderMaterial
{
	struct MaterialTextureType
	{
		int materialTextureType;
		Texture* texture;
	};

public:
	
	ShaderMaterial();
	ShaderMaterial(D3D* d3d);
	ShaderMaterial(D3D* d3d, string name);
	
	void SetD3D(D3D* d3d) { m_D3DSystem = d3d; }

	// Release all textures
	~ShaderMaterial();
	
	// Initializes all values in the material
	void Init();

	// Creates a Texture2DArray SRV
	void CreateTexture2DArray(vector<string> textureFileNames);

	void AddDiffuseTexture(BetterString texturePath, BetterString textureName);
	void AddAmbientTexture(BetterString texturePath, BetterString textureName);
	void AddSpecularTexture(BetterString texturePath, BetterString textureName);
	void AddEmissiveTexture(BetterString texturePath, BetterString textureName);
	void AddDetailMapTexture(BetterString texturePath, BetterString textureName);
	void AddNormalMapTexture(BetterString texturePath, BetterString textureName);
	void AddBlendMapTexture(BetterString texturePath, BetterString textureName);
	void AddAlphaMapTexture(BetterString texturePath, BetterString textureName);
	void AddDisplacementMapTexture(BetterString texturePath, BetterString textureName);
	
	void SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialAmbientTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialSpecularTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialEmissiveTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialDetailMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialNormalMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialBlendMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialAlphaMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialShadowMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialSSAOMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialDisplacementMapTexture(ID3D11ShaderResourceView* texSRV);

	void RemoveTexture(int i);

	void SetDiffuseArrayTextureNames(vector<string> names);
	vector<string> GetDiffuseArrayTextureNames();
	vector<MaterialTextureType*> Textures() const;
	Texture* GetTexture(int i, int mtt);
	Texture* GetTexture(int i);
	int GetMaterialTextureType(int i);
	bool MaterialExists(int i);
	int TextureCount();
	
	void GetTextures
	(	ID3D11ShaderResourceView*& diffuseArrayTexture
	,	ID3D11ShaderResourceView*& diffuseTexture
	,	ID3D11ShaderResourceView*& ambientTexture
	,	ID3D11ShaderResourceView*& specularTexture
	,	ID3D11ShaderResourceView*& emissiveTexture
	,	ID3D11ShaderResourceView*& normalMapTexture
	,	ID3D11ShaderResourceView*& blendMapTexture
	,	ID3D11ShaderResourceView*& detailMapTexture
	,	ID3D11ShaderResourceView*& alphaMapTexture
	,	ID3D11ShaderResourceView*& shadowMapTexture
	,	ID3D11ShaderResourceView*& ssaoMapTexture
	,	ID3D11ShaderResourceView*& displacementMapTexture
	);
	
	void BuildMaterialConstantBuffer(ID3D11Buffer*& buffer, XMFLOAT3 eye, XMFLOAT4 clipplane);
	void BuildMaterialTessellationBuffer(ID3D11Buffer*& buffer, XMFLOAT3 eye);
	void BuildMaterialDomainBuffer(ID3D11Buffer*& buffer, XMMATRIX world, Camera* camera, XMFLOAT4 clipplane, bool reflection);
	
public:
	
	string						sMaterialName;
	string						sDetailMapTextureName;
	string						sDiffuseTextureName;
	string						sAmbientTextureName;
	string						sSpecularTextureName;
	string						sEmissiveTextureName;
	string						sAlphaTextureName;
	string						sNormalMapTextureName;
	
	// diffuse reflectivity
	XMFLOAT4					vDiffuseColor;
	// ambient reflectivity
	XMFLOAT4					vAmbientColor;
	// specular reflectivity
	XMFLOAT4					vSpecularColor;
	// emissive reflectivity
	XMFLOAT4					vEmissiveColor;
	
	// Reflection vector
	XMFLOAT3					vReflection;
	
	// Effectiveness in emitting light
	float						fEmissivity;
	
	// The ratio of the energy of a wave reflected from the material to the energy possessed by the wave striking the material
	float						fReflectivity;
	
	// Measure of the ability for this material to transmit light
	float						fTransmissivity;
	
	// Value that clips pixel during alpha blending
	XMFLOAT3					vAlphaToCoverageValue;
	float						fBlendAmount;

	// Any light passing through the object is filtered by the transmission filter,
	// which only allows the specific colors to pass through.
	// For example, Tf 0 1 0 allows all the green to pass through and filters out all the red and blue. 
	XMFLOAT3					vTransmissionFilter;

	// amount this material dissolves into the background. A factor of 1.0 is fully opaque.
	// This is the default when a new material is created. A factor of 0.0 is fully dissolved (completely transparent).
	float						fAlpha;

	// specular exponent for the current material. This defines the focus of the specular highlight. 
	float 						fSpecularPower;
	float						fSpecularIntensity;

	// Added brightness value for the detail map
	float						fDetailBrightness;

	// Specifies the optical density for the surface. This is also known as index of refraction. 
	// "optical_density" is the value for the optical density. The values can range from 0.001 to 10.
	// A value of 1.0 means that light does not bend as it passes through an object.
	// Increasing the optical_density increases the amount of bending. Glass has an index of refraction of about 1.5.
	// Values of less than 1.0 produce bizarre results and are not recommended. 
	float						fRefractionIndex;
	
	// Used with logarithmic depth buffer calculation
	float						fFarPlane;
	
	// Tessellation parameters to get LOD from camera to mesh
	float						fNoDistTessFactor;
	float						fMinTessDist;
	float						fMaxTessDist;
	float						fMinTessFactor;
	float						fMaxTessFactor;
	bool						bEnableDistTess;
	
	/*
	Illumination models are mathematical equations that represent various material lighting and shading effects.

	0	Color on and Ambient off
	1	Color on and Ambient on
	2	Highlight on
	3	Reflection on and Ray trace on
	4	Transparency: Glass on
	Reflection: Ray trace on
	5	Reflection: Fresnel on and Ray trace on
	6	Transparency: Refraction on
	Reflection: Fresnel off and Ray trace on
	7	Transparency: Refraction on
	Reflection: Fresnel on and Ray trace on
	8	Reflection on and Ray trace off
	9	Transparency: Glass on
	Reflection: Ray trace off
	10	Casts shadows onto invisible surfaces
	*/
	int							iIlluminationModel;

	bool 						bSpecularToggle;
	bool						bHasDiffuseArrayTexture;
	bool						bHasDiffuseTexture;
	bool						bHasAmbientTexture;
	bool						bHasSpecularTexture;
	bool						bHasEmissiveTexture;
	bool						bHasNormalMapTexture;
	bool						bHasBlendMapTexture;
	bool						bHasDetailMapTexture;
	bool						bHasTransparency;
	bool						bHasAlphaMapTexture;
	bool						bHasShadowMap;
	bool						bHasSSAOMap;
	bool						bHasDisplacementMap;
	bool						bEnableLighting;
	bool						bFlipTextureH;
	bool						bFlipTextureV;
	
private:
	
	D3D* m_D3DSystem;
	vector<MaterialTextureType*> mMaterialTextures;
	vector<string>				 mDiffuseArrayTextureNames;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SHADERMATERIAL_H