//==============================================================================
// Deferred Directional Light with MSAA
//
// https://github.com/NVIDIAGameWorks/D3DSamples/tree/master/samples/DeferredShadingMSAA/src
// https://github.com/NVIDIAGameWorks/D3DSamples/blob/master/samples/DeferredShadingMSAA/src/DeferredRenderer.cpp
// https://github.com/NVIDIAGameWorks/D3DSamples/blob/master/samples/DeferredShadingMSAA/src/shaders/Lighting.hlsl
//==============================================================================
// History
//
// -Created on 5/22/2015 by Dustin Watson
//==============================================================================
#include "DeferredCommon.hlsl"

//
// Constants
//

cbuffer cbLightMatrixBuffer : register(b0)
{
	matrix		g_ProjToViewMatrix;
	matrix		g_ViewToWorldMatrix;
};

cbuffer cbMSAABuffer : register(b2)
{
	int		g_DoSecondPass;
	float3	padding3;
};

//======================================================================================================

//
// Samplers and textures
//

Texture2DMS<uint, MSAA_SAMPLE_COUNT> texCoverageMS : register(t3);

//======================================================================================================

//
// Structs
//

//======================================================================================================

//
// Global Functions
//

float3 LambertDiffuse(float3 lightNormal, float3 surfaceNormal, float3 lightColor, float3 pixelColor)
{
	const float INV_PI = 0.31830988618379069122f;
	
	
	// Compute the amount of contribution per light
	
	float diffuseAmount = saturate(dot(lightNormal, surfaceNormal));
	
	float3 diffuse = (diffuseAmount + 0.1f) * lightColor * pixelColor * INV_PI;
	
	return diffuse;
}

float SinTheta2(const float val)
{
	return max(0.0f, 1.0f - val * val);
}

float SinTheta(const float val)
{
	return sqrt(SinTheta2(val));
}

float3 OrenNayarDiffuse(float3 lightNormal, float3 viewNormal, float3 surfaceNormal, float3 lightColor, float3 pixelColor, float sigma)
{
	const float INV_PI = 0.31830988618379069122f;
	
	sigma = sigma / 180.0f * 3.1415192653f;
	float sigmaSqr = sigma * sigma;
	float A = 1.0f - (sigmaSqr / (2.0f * (sigmaSqr + 0.33f)));
	float B = 0.45f * sigmaSqr / (sigmaSqr + 0.09f);
	
	float fDotLt = saturate(dot(lightNormal, surfaceNormal));
	float fDotVw = saturate(dot(viewNormal, surfaceNormal));
	
	float fSinThetaI = SinTheta(fDotLt);
	float fSinThetaO = SinTheta(fDotVw);

	float fMaxCos = saturate(dot(normalize(lightNormal - surfaceNormal * fDotLt), normalize(viewNormal - surfaceNormal * fDotVw)));

	float fSinAlpha, fTanBeta;
	if(abs(fDotLt) > abs(fDotVw))
	{
		fSinAlpha = fSinThetaO;
		fTanBeta = fSinThetaI / clamp(abs(fDotLt), 0.01f, 1.0f);
	}
	else
	{
		fSinAlpha = fSinThetaI;
		fTanBeta = fSinThetaO / clamp(abs(fDotVw), 0.01f, 1.0f);
	}

    return pixelColor * lightColor * INV_PI * (A + B * fMaxCos * fSinAlpha * fTanBeta) * (fDotLt + 0.1f);
}

float3 BlinnPhoneSpecular
(	float3 toEye
,	float3 lightNormal
,	float3 surfaceNormal
, 	float3 materialSpecularColor
, 	float materialSpecularPower
, 	float lightSpecularIntensity
, 	float3 lightColor)
{
	// compute specular contribution
    float3 vHalf = normalize(lightNormal + toEye);
    float specularAmount = saturate(dot(surfaceNormal, vHalf));
    specularAmount = pow(specularAmount, max(materialSpecularPower, 0.0001f)) * lightSpecularIntensity;
    float3 specular = materialSpecularColor * lightColor * specularAmount;
	return specular;
}

float3 DepthToPosition(float iDepth, float4 iPosProj, matrix mProjInv, float fFarClip)
{
	float3 vPosView = mul(iPosProj, mProjInv).xyz;
	float3 vViewRay = float3(vPosView.xy * (fFarClip / vPosView.z), fFarClip);
	float3 vPosition = vViewRay * iDepth;
	return vPosition;
}

float2 ProjToScreen(float4 iCoord)
{
	float2 oCoord = iCoord.xy / iCoord.w;
	return 0.5f * (float2(oCoord.x, -oCoord.y) + 1);
}

float4 ScreenToProj(float2 iCoord)
{
	return float4(2.0f * float2(iCoord.x, 1.0f - iCoord.y) - 1, 0.0f, 1.0f);
}

void PerformFragmentAnalysis4(in uint4 inputCoverage[1], out uint sampleCount, out uint fragmentCount, out uint4 sampleWeights[1])
{
	uint4 coverage = inputCoverage[0];

	sampleWeights[0] = uint4( 1, 1, 1, 1 );

	// Kill the same primitive IDs in the pixel
	if( coverage.x == coverage.y ) { coverage.y = 0; sampleWeights[0].x += 1; sampleWeights[0].y = 0; }
	if( coverage.x == coverage.z ) { coverage.z = 0; sampleWeights[0].x += 1; sampleWeights[0].z = 0; }
	if( coverage.x == coverage.w ) { coverage.w = 0; sampleWeights[0].x += 1; sampleWeights[0].w = 0; }
	if( coverage.y == coverage.z ) { coverage.z = 0; sampleWeights[0].y += 1; sampleWeights[0].z = 0; }
	if( coverage.y == coverage.w ) { coverage.w = 0; sampleWeights[0].y += 1; sampleWeights[0].w = 0; }
	if( coverage.z == coverage.w ) { coverage.w = 0; sampleWeights[0].z += 1; sampleWeights[0].w = 0; }

	sampleWeights[0].x = coverage.x > 0 ? sampleWeights[0].x : 0;
	sampleWeights[0].y = coverage.y > 0 ? sampleWeights[0].y : 0;
	sampleWeights[0].z = coverage.z > 0 ? sampleWeights[0].z : 0;
	sampleWeights[0].w = coverage.w > 0 ? sampleWeights[0].w : 0;

	sampleCount = (sampleWeights[0].x > 0 ? 1 : 0) + (sampleWeights[0].y > 0 ? 1 : 0) + (sampleWeights[0].z > 0 ? 1 : 0) + (sampleWeights[0].w > 0 ? 1 : 0);
	fragmentCount = sampleWeights[0].x + sampleWeights[0].y + sampleWeights[0].z + sampleWeights[0].w;
}

//======================================================================================================

//
// Pixel Shaders
//

float4 PS(VQPixelInput input) : SV_Target
{
	
}

