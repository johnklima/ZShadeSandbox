//==============================================================================
// Tree Shader
//
//==============================================================================
// History
//
// -Created on 8/14/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

#define NUM_LEAVES_PER_TREE 1000

cbuffer cbTreeBuffer : register(b0)
{
	float4 g_EyePos;
	float g_TreeBillWidth;
	float g_TreeBillHeight;
	float padding1;
	float padding2;
	float4 g_DiffuseColor;
	int g_HasTexture;
	int g_HasNormalMap;
	int g_IsInstance;
	int g_IsLeaf;
};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix g_World;
	matrix g_View;
	matrix g_Projection;
};

cbuffer cbInternalBuffer : register(b2)
{
	matrix g_LeafOnTree[NUM_LEAVES_PER_TREE];
};

//======================================================================================================

//
// Samplers and textures
//

Texture2D g_TreeTexture		: register(t0);

SamplerState linearSampler	: register(s0);

//======================================================================================================

//
// Structs
//

struct TreeVertexInput
{
	float4 position		: POSITION;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 instancePos	: INSTANCEPOS;
	uint instanceID		: SV_InstanceID;
};

struct TreeShaderInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
	float4 worldPos		: TEXCOORD1;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

//======================================================================================================

//
// Vertex Shader
//

TreeShaderInput TreeVS(TreeVertexInput input)
{
	TreeShaderInput output;
	
	if (g_IsInstance == 1)
	{
		// Get leaves position on tree and add trees position
		if (g_IsLeaf == 1)
		{
			// We have 1000 leaves per tree, so we can find the current leaf (in the tree) we are on (so we can get it's
			//	matrix from the matrix array stored in cbPerScene) by first getting the current tree (instanceID / NUM_LEAVES_PER_TREE).
			//	We can then find the current leaf in the tree we are on by multiplying the current tree id with the number of leaves per tree,
			//	then subtracting that total from the current instance id.
			
			uint currTree = (input.instanceID / NUM_LEAVES_PER_TREE);
			
			uint currLeafInTree = input.instanceID - (currTree * NUM_LEAVES_PER_TREE);
			
			input.position = mul(input.position, g_LeafOnTree[currLeafInTree]);
		}
		
		input.position += float4(input.instancePos, 0.0f);
	}
	
	output.position = mul(input.position, g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Projection);
	
	output.worldPos = mul(input.position, g_World);
	
	output.normal = mul(input.normal, g_World);
	
	output.tangent = mul(input.tangent, g_World);
	
    output.uv = input.uv;
	
	return output;
}

//======================================================================================================

//
// Geometry Shader for billboarding
//

[maxvertexcount(4)]
void TreeBillboardGS(point TreeShaderInput input[1], inout TriangleStream<TreeShaderInput> OutputStream)
{
	// The point passed in is in the horizontal center of the billboard, and at the bottom vertically. Because of this,
	// we will take the trees width and divide it by two when finding the x axis for the quads vertices.
	float halfWidth = g_TreeBillWidth / 2.0f;
	
	// The billboard will only be rotated on the y axis, so it's up vector will always be 0,1,0. Because of this, we can
	// find the billboards vertices using the cameras position and the billboards position. We start by getting the billboards
	// plane normal:
	float3 planeNormal = input[0].worldPos - g_EyePos;
	planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);
	
	float3 upVector = float3(0.0f, 1.0f, 0.0f);
	
	// Now we need to find the billboards right vector, so we can easily find the billboards vertices from the input point
	float3 rightVector = normalize(cross(planeNormal, upVector)); // Cross planes normal with the up vector (+y) to get billboards right vector
	
	rightVector = rightVector * halfWidth; // change the length of the right vector to be half the width of the billboard
	
	// Get the billboards "height" vector which will be used to find the top two vertices in the billboard quad
	upVector = float3(0, g_TreeBillHeight, 0);
	
	// Create the billboards quad
	float3 vert[4];
	
	// We get the points by using the billboards right vector and the billboards height
	vert[0] = input[0].worldPos - rightVector; // Get bottom left vertex
	vert[1] = input[0].worldPos + rightVector; // Get bottom right vertex
	vert[2] = input[0].worldPos - rightVector + upVector; // Get top left vertex
	vert[3] = input[0].worldPos + rightVector + upVector; // Get top right vertex
	
	// Get billboards texture coordinates
	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);
	
	// Now we "append" or add the vertices to the outgoing stream list
	TreeShaderInput outputVert;
	
	for(int i = 0; i < 4; i++)
	{
		outputVert.position = mul(float4(vert[i], 1.0f), g_World);
		outputVert.position = mul(outputVert.position, g_View);
		outputVert.position = mul(outputVert.position, g_Projection);
		
		outputVert.worldPos = float4(vert[i], 0.0f);
		outputVert.uv = texCoord[i];
		
		// These will not be used for billboards
		outputVert.normal = float3(0, 0, 0);
		outputVert.tangent = float3(0, 0, 0);
		
		OutputStream.Append(outputVert);
	}
}

//======================================================================================================

//
// Pixel Shader
//

float4 TreePS(TreeShaderInput input) : SV_Target
{
	float4 diffuse = g_TreeTexture.Sample( linearSampler, input.uv );
	
	clip(diffuse.a - 0.25f);

	return diffuse;
}

float4 TreeWireframePS(TreeShaderInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}