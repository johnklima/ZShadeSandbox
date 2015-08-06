#include "FontShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
FontShader::FontShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
FontShader::FontShader(const FontShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
FontShader::~FontShader()
{

}
//==============================================================================================================================
bool FontShader::Initialize()
{
	bool result;

	if (m_pD3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX10)
		;//result = InitializeShader10(d3d, "FX\\terrain2.fx");
	else if (m_pD3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX11)
		result = InitializeShader11();
	
	if(!result)
	{
		return false;
	}

	return true;
}
//==============================================================================================================================
void FontShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
}
//==============================================================================================================================
void FontShader::RenderBatch11(ID3D11ShaderResourceView* texture, const SpriteDrawData* drawData, UINT64 numSprites)
{
	// Set per-batch constants
    VSPerBatchCB perBatch;
	
	// Get the viewport dimensions
    UINT numViewports = 1;
    D3D11_VIEWPORT vp;
    m_pD3DSystem->GetDeviceContext()->RSGetViewports(&numViewports, &vp);
    perBatch.ViewportSize = XMFLOAT2(static_cast<float>(vp.Width), static_cast<float>(vp.Height));
	
    // Get the size of the texture
    ID3D11Resource* resource;
    ID3D11Texture2DPtr texResource;
    D3D11_TEXTURE2D_DESC desc;
    texture->GetResource(&resource);
    texResource.Attach(reinterpret_cast<ID3D11Texture2D*>(resource));
    texResource->GetDesc(&desc);
    perBatch.TextureSize = XMFLOAT2(static_cast<float>(desc.Width), static_cast<float>(desc.Height));
	
    // Copy it into the buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    m_pD3DSystem->GetDeviceContext()->Map(m_pPerBatchCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	CopyMemory(mapped.pData, &perBatch, sizeof(VSPerBatchCB));
    m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerBatchCB, 0);
	
	// Set shaders
	SetInputLayout("SpriteInstancedVS");
	SwitchTo("SpriteInstancedVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	SetVertexShader();
	SetPixelShader();
	
	// Make sure the draw rects are all valid
    for (UINT64 i = 0; i < numSprites; ++i)
    {
        XMFLOAT4 drawRect = drawData[i].DrawRect;
        //_ASSERT(drawRect.x >= 0 && drawRect.x < desc.Width);
        //_ASSERT(drawRect.y >= 0 && drawRect.y < desc.Height);
        //_ASSERT(drawRect.z > 0 && drawRect.x + drawRect.z <= desc.Width);
        //_ASSERT(drawRect.w > 0 && drawRect.y + drawRect.w <= desc.Height);
    }

    UINT64 numSpritesToDraw = min(numSprites, MaxBatchSize);
	
	// Copy in the instance data
    //D3D11_MAPPED_SUBRESOURCE mapped;
    m_pD3DSystem->GetDeviceContext()->Map(m_pInstanceData, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    CopyMemory(mapped.pData, drawData, static_cast<size_t>(sizeof(SpriteDrawData) * numSpritesToDraw));
    m_pD3DSystem->GetDeviceContext()->Unmap(m_pInstanceData, 0);
	
	// Set the constant buffer
    ID3D11Buffer* constantBuffers[1] = { m_pPerBatchCB };
    m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffers);
	
	// Set the vertex buffers
    UINT strides [2] = { sizeof(ZShadeSandboxMesh::VertexTexSprite), sizeof(SpriteDrawData) };
    UINT offsets [2] = { 0, 0 };
    ID3D11Buffer* vertexBuffers[2] = { vertexBuffer, m_pInstanceData };
    m_pD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
	
	// Set the texture
	ID3D11ShaderResourceView* ps_srvs[1] = { texture };
    m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	
	// Perform draw call
	RenderIndexInstanced11(6, static_cast<UINT>(numSpritesToDraw));
	
	ps_srvs[0] = NULL;
	m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	
	// If there's any left to be rendered, do it recursively
    if(numSprites > numSpritesToDraw)
        RenderBatch11(texture, drawData + numSpritesToDraw, numSprites - numSpritesToDraw);
}
//==============================================================================================================================
bool FontShader::Render11(const GDIFont& font, const BetterString text, const XMMATRIX& transform, const XMFLOAT4& color)
{
	// Turn off the Z-Buffer if we are in 3D
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == ZSHADE_3D)
		m_pD3DSystem->TurnOffZBuffer();
	
	// Set the states
    float blendFactor[4] = {1, 1, 1, 1};
    m_pD3DSystem->GetDeviceContext()->RSSetState(m_pD3DSystem->DisableDepthClip());
    m_pD3DSystem->GetDeviceContext()->OMSetBlendState(m_pD3DSystem->AlphaBlend(), blendFactor, 0xFFFFFFFF);
    m_pD3DSystem->GetDeviceContext()->OMSetDepthStencilState(m_pD3DSystem->DepthWriteEnabled(), 0);
	
	int length = text.size();
	
    XMMATRIX textTransform = XMMatrixIdentity();
	
	unique_ptr<wchar_t> wtext = text.ToWideStr();
	
    UINT64 numCharsToDraw = min(length, MaxBatchSize);
    UINT64 currentDraw = 0;
	
    for (UINT64 i = 0; i < numCharsToDraw; ++i)
    {
        WCHAR character = wtext.get()[i];
		
        if(character == ' ')
            textTransform._41 += font.SpaceWidth();
        else if(character == '\n' || character == '\0')
        {
            textTransform._42 += font.CharHeight();
            textTransform._41 = 0;
        }
        else
        {
			GDIFont::CharDesc desc = font.GetCharDescriptor(character);
			
            textDrawData[currentDraw].Transform = XMMatrixMultiply(textTransform, transform);
            textDrawData[currentDraw].Color = color;
            textDrawData[currentDraw].DrawRect.x = desc.X;
            textDrawData[currentDraw].DrawRect.y = desc.Y;
            textDrawData[currentDraw].DrawRect.z = desc.Width;
            textDrawData[currentDraw].DrawRect.w = desc.Height;
            currentDraw++;
			
            textTransform._41 += desc.Width + 1;
        }
    }
	
    // Submit a batch
    RenderBatch11(font.SRView(), textDrawData, currentDraw);
	
	// Perform clean up
	m_pD3DSystem->GetDeviceContext()->RSSetState(0);
    m_pD3DSystem->GetDeviceContext()->OMSetBlendState(m_pD3DSystem->BlendDisabled(), blendFactor, 0xFFFFFFFF);
    m_pD3DSystem->GetDeviceContext()->OMSetDepthStencilState(0, 0);
	
	// Turn on the Z-Buffer if we are in 3D
	if (m_pD3DSystem->GetEngineOptions()->m_DimType == ZSHADE_3D)
		m_pD3DSystem->TurnOnZBuffer();
	
	if(length > numCharsToDraw)
		Render11(font, text + numCharsToDraw, textTransform, color);
	
	return true;
}
//==============================================================================================================================
bool FontShader::InitializeShader11()
{
	ConstantBuffer<VSPerBatchCB> perBatchCB(m_pD3DSystem);
	perBatchCB.Initialize(PAD16(sizeof(VSPerBatchCB)));
	m_pPerBatchCB = perBatchCB.Buffer();

	// Create the instance data buffer
	D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = sizeof(SpriteDrawData) * MaxBatchSize;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	m_pD3DSystem->GetDevice11()->CreateBuffer(&desc, NULL, &m_pInstanceData);
	
	ClearInputLayout();
	SetInputLayoutDesc("SpriteVS", ZShadeSandboxMesh::VertexLayout::mesh_layout_sprite_pos_tex, 2);
	SetInputLayoutDesc("SpriteInstancedVS", ZShadeSandboxMesh::VertexLayout::mesh_layout_sprite_instanced, 8);
	
	// Compile the sprite gui shader
	//Compile( "Basic\\Sprite.hlsl", "SpriteVS", ST_VERTEX );
	//Compile( "Basic\\Sprite.hlsl", "SpriteInstancedVS", ST_VERTEX );
	//Compile( "Basic\\Sprite.hlsl", "SpritePS", ST_PIXEL );
	
	LoadVertexShader("SpriteVS");
	LoadVertexShader("SpriteInstancedVS");
	LoadPixelShader("SpritePS");

	SwitchTo("SpriteVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	AssignVertexShaderLayout("SpriteVS");

	SwitchTo("SpriteInstancedVS", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);
	AssignVertexShaderLayout("SpriteInstancedVS");

	return true;
}
//==============================================================================================================================
void FontShader::ShutdownShader()
{
	
}
//==============================================================================================================================