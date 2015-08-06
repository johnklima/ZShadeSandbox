#include "Text.h"
using ZShadeSandboxGraphics::Text;
//===============================================================================================================================
//===============================================================================================================================
Text::Text()
{
	font = 0;
	textShadow = 0;
	m_D3DSystem = 0;
	bHasShadow = false;
}
//===============================================================================================================================
Text::Text(D3D* d3d)
:   m_D3DSystem(d3d)
{
	font = 0;
	textShadow = 0;
	bHasShadow = false;
}
//===============================================================================================================================
Text::~Text()
{
}
//===============================================================================================================================
void Text::Initialize()
{
	// Create the vertex buffer
    ZShadeSandboxMesh::VertexTexSprite verts[] =
    { { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT2(1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT2(0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) } };
    
	D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = sizeof(ZShadeSandboxMesh::VertexTexSprite) * 4;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = verts;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;
	
	m_D3DSystem->GetDevice11()->CreateBuffer(&desc, &initData, &vertexBuffer);
	
	// Create the index buffer
    USHORT indices[] = { 0, 1, 2, 3, 0, 2 };
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = sizeof(USHORT) * 6;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0;
    initData.pSysMem = indices;
    
	m_D3DSystem->GetDevice11()->CreateBuffer(&desc, &initData, &indexBuffer);
	
	m_FontShader = new FontShader(m_D3DSystem);

	font.SetD3D(m_D3DSystem);
	
	// Set to default font
	ChangeFont();
}
//===============================================================================================================================
void Text::CreateShadow()
{
	//textShadow.reset(new Text(m_D3DSystem));
	textShadow = new Text(m_D3DSystem);
	textShadow->Initialize();
	bHasShadow = true;
}
//===============================================================================================================================
void Text::ChangeFont(LPCWSTR fontName, float fontSize, UINT fontStyle, bool antiAliased)
{
	font.Initialize(fontName, fontSize, fontStyle, antiAliased);

	if (bHasShadow)
		textShadow->ChangeFont(fontName, fontSize, fontStyle, antiAliased);
}
//===============================================================================================================================
Text* Text::Clone()
{
	Text* newText = new Text();
	newText->SetD3D(m_D3DSystem);
	newText->Initialize();
	newText->ChangeFont(L"Times New Roman", m_fontSize, FontStyle::BoldItalic, true);
	if (bHasShadow)
		newText->CreateShadow();
	newText->TextName() = m_Name;
	newText->TextTransform() = transform;
	newText->TextColor() = color;
	return newText;
}
//===============================================================================================================================
Text* Text::Clone(int x, int y)
{
	Text* newText = new Text();
	newText->SetD3D(m_D3DSystem);
	newText->Initialize();
	newText->ChangeFont(L"Times New Roman", m_fontSize, FontStyle::BoldItalic, true);
	if (bHasShadow)
		newText->CreateShadow();
	newText->TextName() = m_Name;
	newText->TextTransform() = XMMatrixTranslation(x, y, 0);
	newText->TextColor() = color;
	return newText;
}
//===============================================================================================================================
int Text::GetXPos()
{
	XMVECTOR scal; //for scaling
	XMVECTOR quat; //for rotation
	XMVECTOR tran; //for translation
	XMMatrixDecompose(&scal, &quat, &tran, transform);

	XMFLOAT4 trans;
	XMStoreFloat4(&trans, tran);
	XMFLOAT4 rotz;
	XMStoreFloat4(&rotz, quat);
	XMFLOAT4 scale;
	XMStoreFloat4(&scale, scal);

	return trans.x;
}
//===============================================================================================================================
int Text::GetYPos()
{
	XMVECTOR scal; //for scaling
	XMVECTOR quat; //for rotation
	XMVECTOR tran; //for translation
	XMMatrixDecompose(&scal, &quat, &tran, transform);

	XMFLOAT4 trans;
	XMStoreFloat4(&trans, tran);
	XMFLOAT4 rotz;
	XMStoreFloat4(&rotz, quat);
	XMFLOAT4 scale;
	XMStoreFloat4(&scale, scal);

	return trans.y;
}
//===============================================================================================================================
int Text::Width()
{
	return font.TextureWidth();
}
//===============================================================================================================================
int Text::Height()
{
	return font.TextureHeight();
}
//===============================================================================================================================
void Text::RenderMeshBuffers()
{
	m_FontShader->SetVertexBuffer(vertexBuffer);
	
	// Set the index buffer
	m_D3DSystem->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set primitive topology
	m_D3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
//===============================================================================================================================
void Text::Render11()
{
	if (bHasShadow)
	{
		XMVECTOR scal; //for scaling
		XMVECTOR quat; //for rotation
		XMVECTOR tran; //for translation
		XMMatrixDecompose(&scal, &quat, &tran, transform);

		XMFLOAT4 trans;
		XMStoreFloat4(&trans, tran);
		XMFLOAT4 rotz;
		XMStoreFloat4(&rotz, quat);
		XMFLOAT4 scale;
		XMStoreFloat4(&scale, scal);

		XMMATRIX shadowTransform;
		shadowTransform = XMMatrixTranslation(trans.x + 2, trans.y + 2, trans.z);
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixScaling(scale.x, scale.y, scale.z));
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixRotationX(rotz.x));
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixRotationY(rotz.y));
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixRotationZ(rotz.z));

		textShadow->TextTransform() = shadowTransform;
		textShadow->TextColor() = XMFLOAT4(1, 1, 1, 1);
		textShadow->Render11(m_Name);
	}

	RenderMeshBuffers();
	m_FontShader->Render11(font, m_Name, transform, color);
}
//===============================================================================================================================
void Text::Render11(const BetterString name)
{
	SetText(name);
	
	if (bHasShadow)
	{
		XMVECTOR scal; //for scaling
		XMVECTOR quat; //for rotation
		XMVECTOR tran; //for translation
		XMMatrixDecompose(&scal, &quat, &tran, transform);

		XMFLOAT4 trans;
		XMStoreFloat4(&trans, tran);
		XMFLOAT4 rotz;
		XMStoreFloat4(&rotz, quat);
		XMFLOAT4 scale;
		XMStoreFloat4(&scale, scal);

		XMMATRIX shadowTransform;
		shadowTransform = XMMatrixTranslation(trans.x + 2, trans.y + 2, trans.z);
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixScaling(scale.x, scale.y, scale.z));
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixRotationX(rotz.x));
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixRotationY(rotz.y));
		shadowTransform = XMMatrixMultiply(shadowTransform, XMMatrixRotationZ(rotz.z));

		textShadow->TextTransform() = shadowTransform;
		textShadow->TextColor() = XMFLOAT4(1, 1, 1, 1);
		textShadow->Render11(name);
	}

	RenderMeshBuffers();
	m_FontShader->Render11(font, name, transform, color);
}
//===============================================================================================================================