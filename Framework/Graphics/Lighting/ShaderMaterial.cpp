#include "ShaderMaterial.h"
#include "LightShadingBuffers.h"
using ZShadeSandboxLighting::ShaderMaterial;
//==============================================================================================================================
//==============================================================================================================================
ShaderMaterial::ShaderMaterial()
{
}
//==============================================================================================================================
ShaderMaterial::ShaderMaterial(D3D* d3d)
:	m_D3DSystem(d3d)
{
}
//==============================================================================================================================
ShaderMaterial::ShaderMaterial(D3D* d3d, string name)
:   m_D3DSystem(d3d)
,	sMaterialName(name)
{
	Init();
}
//==============================================================================================================================
ShaderMaterial::~ShaderMaterial()
{
	/*if (mMaterialTextures.size() > 0)
	{
		for (int i = 0; i < mMaterialTextures.size(); i++)
			mMaterialTextures[i]->texture->Shutdown();
		mMaterialTextures.clear();
	}*/
}
//==============================================================================================================================
void ShaderMaterial::BuildMaterialConstantBuffer(ID3D11Buffer*& buffer, XMFLOAT3 eye, XMFLOAT4 clipplane)
{
	ZShadeSandboxLighting::cbMaterialShadingBuffer cMSB;
	cMSB.g_EyePosW = eye;
	cMSB.g_Alpha = fAlpha;
	cMSB.g_ClipPlane = clipplane;
	cMSB.g_MaterialDiffuseColor = vDiffuseColor;
	cMSB.g_MaterialAmbientColor = vAmbientColor;
	cMSB.g_MaterialSpecularColor = vSpecularColor;
	cMSB.g_MaterialEmissiveColor = vEmissiveColor;
	cMSB.g_Emissivity = fEmissivity;
	cMSB.g_Reflectivity = fReflectivity;
	cMSB.g_Transmissivity = fTransmissivity;
	cMSB.g_RefractionIndex = fRefractionIndex;
	cMSB.g_AlphaToCoverageValue = vAlphaToCoverageValue;
	cMSB.g_DetailBrightness = fDetailBrightness;
	cMSB.g_TransmissionFilter  = vTransmissionFilter;
	cMSB.g_MaterialSpecularPower = fSpecularPower;
	cMSB.g_MaterialSpecularIntensity = fSpecularIntensity;
	cMSB.g_IlluminationModel = iIlluminationModel;
	cMSB.g_FlipTextureH = (bFlipTextureH) ? 1 : 0;
	cMSB.g_FlipTextureV = (bFlipTextureV) ? 1 : 0;
	cMSB.g_UsingDiffuseArrayTexture = (bHasDiffuseArrayTexture) ? 1 : 0;
	cMSB.g_UsingDiffuseTexture = (bHasDiffuseTexture) ? 1 : 0;
	cMSB.g_UsingAmbientTexture = (bHasAmbientTexture) ? 1 : 0;
	cMSB.g_UsingSpecularTexture = (bHasSpecularTexture) ? 1 : 0;
	cMSB.g_UsingEmissiveTexture = (bHasEmissiveTexture) ? 1 : 0;
	cMSB.g_UsingNormalMapTexture = (bHasNormalMapTexture) ? 1 : 0;
	cMSB.g_UsingBlendMapTexture = (bHasBlendMapTexture) ? 1 : 0;
	cMSB.g_UsingDetailMapTexture = (bHasDetailMapTexture) ? 1 : 0;
	cMSB.g_UsingAlphaMapTexture = (bHasAlphaMapTexture) ? 1 : 0;
	cMSB.g_UsingTransparency = (bHasTransparency) ? 1 : 0;
	cMSB.g_UsingShadowMap = (bHasShadowMap) ? 1 : 0;
	cMSB.g_UsingSSAOMap = (bHasSSAOMap) ? 1 : 0;
	cMSB.g_UsingDisplacementMap = (bHasDisplacementMap) ? 1 : 0;
	cMSB.g_FarPlane = fFarPlane;
	cMSB.g_SpecularToggle = (bSpecularToggle) ? 1 : 0;
	cMSB.g_EnableLighting = (bEnableLighting) ? 1 : 0;
	cMSB.matpadding = XMFLOAT3(0, 0, 0);
	cMSB.g_BlendAmount = fBlendAmount;
	// Map the Material shading constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxLighting::cbMaterialShadingBuffer*)mapped_res.pData = cMSB;
		}
		m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==============================================================================================================================
void ShaderMaterial::BuildMaterialTessellationBuffer(ID3D11Buffer*& buffer, XMFLOAT3 eye)
{
	ZShadeSandboxLighting::cbMaterialTessellationBuffer cTB;
	cTB.g_EyePosW = eye;
	cTB.g_NoDistTessFactor = fNoDistTessFactor;
	cTB.g_MinTessDist = fMinTessDist;
	cTB.g_MaxTessDist = fMaxTessDist;
	cTB.g_MinTessFactor = fMinTessFactor;
	cTB.g_MaxTessFactor = fMaxTessFactor;
	cTB.tesspadding = XMFLOAT3(0, 0, 0);
	cTB.g_EnableDistTess = bEnableDistTess;
	// Map the Material tessellation constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxLighting::cbMaterialTessellationBuffer*)mapped_res.pData = cTB;
		}
		m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==============================================================================================================================
void ShaderMaterial::BuildMaterialDomainBuffer(ID3D11Buffer*& buffer, XMMATRIX world, Camera* camera, XMFLOAT4 clipplane, bool reflection)
{
	ZShadeSandboxLighting::cbMaterialDomainBuffer cDB;
	cDB.g_ClipPlane = clipplane;
	cDB.domainpadding = XMFLOAT3(0, 0, 0);
	cDB.g_FarPlane = fFarPlane;
	cDB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	cDB.g_matView = (reflection) ? camera->ReflectionView4x4() : camera->View4x4();
	if (m_D3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		cDB.g_matProj = camera->Ortho4x4();
	else if (m_D3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
		cDB.g_matProj = camera->Proj4x4();
	// Map the Material domain constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_D3DSystem->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxLighting::cbMaterialDomainBuffer*)mapped_res.pData = cDB;
		}
		m_D3DSystem->GetDeviceContext()->Unmap(buffer, 0);
	}
}
//==============================================================================================================================
void ShaderMaterial::GetTextures
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
)
{
	for (int i = 0; i < mMaterialTextures.size(); i++)
	{
		switch (GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuseArray:
				diffuseArrayTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
				diffuseTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAmbient:
				ambientTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eSpecular:
				specularTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eEmissive:
				emissiveTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eNormal:
				normalMapTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eBlend:
				blendMapTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDetail:
				detailMapTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAlpha:
				alphaMapTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eShadowMap:
				shadowMapTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eSSAOMap:
				ssaoMapTexture = GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDisplacementMap:
				displacementMapTexture = GetTexture(i)->getTexture11();
			break;
		}
	}
}
//==============================================================================================================================
void ShaderMaterial::Init()
{
	fAlpha = 1.0f;
	vDiffuseColor = XMFLOAT4(0, 0, 0, fAlpha);
	vAmbientColor = XMFLOAT4(0, 0, 0, fAlpha);
	vSpecularColor = XMFLOAT4(0, 0, 0, fAlpha);
	vEmissiveColor = XMFLOAT4(0, 0, 0, fAlpha);
	fFarPlane = 1000.0f;
	fEmissivity = 1.0f;
	fReflectivity = 1.0f;
	fTransmissivity = 1.0f;
	fBlendAmount = 0.0f;
	vReflection = XMFLOAT3(1, 1, 1);
	vAlphaToCoverageValue = XMFLOAT3(0, 0, 0);
	vTransmissionFilter = XMFLOAT3(1, 1, 1);
	fSpecularPower = 50.0f;
	fSpecularIntensity = 0.8f;
	fDetailBrightness = 1.8f;
	fRefractionIndex = 1.0f;
	iIlluminationModel = 2;
	bSpecularToggle = (iIlluminationModel == 2) ? true : false;
	bHasDiffuseArrayTexture = false;
	bHasDiffuseTexture = false;
	bHasAmbientTexture = false;
	bHasSpecularTexture = false;
	bHasEmissiveTexture = false;
	bHasNormalMapTexture = false;
	bHasBlendMapTexture = false;
	bHasDetailMapTexture = false;
	bHasTransparency = false;
	bHasAlphaMapTexture = false;
	bHasShadowMap = false;
	bHasSSAOMap = false;
	bHasDisplacementMap = false;
	bEnableLighting = true;
	bFlipTextureH = false;
	bFlipTextureV = false;
}
//==============================================================================================================================
int ShaderMaterial::GetMaterialTextureType(int i)
{
	if (MaterialExists(i))
		return mMaterialTextures[i]->materialTextureType;
	return EMaterialTextureType::eNone;
}
//==============================================================================================================================
Texture* ShaderMaterial::GetTexture(int i, int mtt)
{
	if (MaterialExists(i))
	{
		if (mMaterialTextures[i]->materialTextureType == mtt)
			return mMaterialTextures[i]->texture;
		else
			return NULL;
	}
	else
		return NULL;
}
//==============================================================================================================================
Texture* ShaderMaterial::GetTexture(int i)
{
	if (MaterialExists(i))
		return mMaterialTextures[i]->texture;
	else
		return NULL;
}
//==============================================================================================================================
bool ShaderMaterial::MaterialExists(int i)
{
	if (mMaterialTextures[i] != NULL)
		return true;
	return false;
}
//==============================================================================================================================
void ShaderMaterial::RemoveTexture(int i)
{
	if (MaterialExists(i))
	{
		mMaterialTextures[i]->texture->Shutdown();
		mMaterialTextures.erase(mMaterialTextures.begin() + i);
	}
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDiffuse;
	matTexture->texture = tex;

	bHasDiffuseTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialAmbientTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eAmbient;
	matTexture->texture = tex;

	bHasAmbientTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialSpecularTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eSpecular;
	matTexture->texture = tex;

	bHasSpecularTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialEmissiveTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eEmissive;
	matTexture->texture = tex;

	bHasEmissiveTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialDetailMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDetail;
	matTexture->texture = tex;

	bHasDetailMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialNormalMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eNormal;
	matTexture->texture = tex;

	bHasNormalMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialBlendMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eBlend;
	matTexture->texture = tex;

	bHasBlendMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialAlphaMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eAlpha;
	matTexture->texture = tex;

	bHasAlphaMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialShadowMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eShadowMap;
	matTexture->texture = tex;

	bHasShadowMap = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialSSAOMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eSSAOMap;
	matTexture->texture = tex;

	bHasSSAOMap = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialDisplacementMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDisplacementMap;
	matTexture->texture = tex;

	bHasDisplacementMap = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddDiffuseTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDiffuse;
	matTexture->texture = tex;
	
	bHasDiffuseTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddAmbientTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eAmbient;
	matTexture->texture = tex;

	bHasAmbientTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddSpecularTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eSpecular;
	matTexture->texture = tex;

	bHasSpecularTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddEmissiveTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eEmissive;
	matTexture->texture = tex;

	bHasEmissiveTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddDetailMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDetail;
	matTexture->texture = tex;
	
	bHasDetailMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddNormalMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eNormal;
	matTexture->texture = tex;
	
	bHasNormalMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddBlendMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eBlend;
	matTexture->texture = tex;
	
	bHasBlendMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddAlphaMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eAlpha;
	matTexture->texture = tex;
	
	bHasAlphaMapTexture = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddDisplacementMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDisplacementMap;
	matTexture->texture = tex;

	bHasDisplacementMap = true;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::CreateTexture2DArray(vector<string> textureFileNames)
{
	mDiffuseArrayTextureNames = textureFileNames;
	
	Texture* tex = new Texture(m_D3DSystem);
	tex->InitializeLayerMapArray(textureFileNames);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDiffuseArray;
	matTexture->texture = tex;
	
	bHasDiffuseArrayTexture = true;

	//mMaterialTextures.insert(make_pair(name, matTexture));
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetDiffuseArrayTextureNames(vector<string> names)
{
	mDiffuseArrayTextureNames = names;
}
//==============================================================================================================================
vector<string> ShaderMaterial::GetDiffuseArrayTextureNames()
{
	return mDiffuseArrayTextureNames;
}
//==============================================================================================================================
int ShaderMaterial::TextureCount()
{
	return mMaterialTextures.size();
}
//==============================================================================================================================