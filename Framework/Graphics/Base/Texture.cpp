#include "Texture.h"
#include "TextureManager.h"
//-----------------------------------------------------------------------------------------------------------------
Texture::Texture(D3D* d3d)
:	m_D3DSystem(d3d)
{
	m_engineOptions = d3d->GetEngineOptions();
}
//-----------------------------------------------------------------------------------------------------------------
Texture::Texture(const Texture& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
Texture::~Texture()
{
}
//-----------------------------------------------------------------------------------------------------------------
bool Texture::Initialize(string base_image_path, string filename)
{
	m_textureName = filename;
	//m_windowName = windowName;
	//m_textureWidth = textureWidth;
	//m_textureHeight = textureHeight;
	
	BetterString fname = "";
	
	fname = base_image_path;
	fname += "\\";
	fname += filename;
	
	if (m_engineOptions->m_d3dVersion == DIRECTX10)
	{
		// Nothing
	}
	else if (m_engineOptions->m_d3dVersion == DIRECTX11)
	{
		m_texture11 = TextureManager::Instance()->GetTexture(fname);
	}
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool Texture::InitializeLayerMapArray(vector<string> textureFileNames)
{
	m_texture11 = TextureManager::Instance()->CreateTexture2DArraySRV(textureFileNames);
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void Texture::GetImageSize(int& width, int& height)
{
	if (m_engineOptions->m_d3dVersion == DIRECTX10)
	{
		ID3D10Resource* res;

		m_texture10->GetResource(&res);

		D3D10_TEXTURE2D_DESC desc;

		m_renderTargetTexture10 = reinterpret_cast<ID3D10Texture2D*>( res );

		m_renderTargetTexture10->GetDesc( &desc );

		width = desc.Width;
		height = desc.Height;
	}
	else if (m_engineOptions->m_d3dVersion == DIRECTX11)
	{
		ID3D11Resource* res;

		m_texture11->GetResource(&res);
		
		D3D11_TEXTURE2D_DESC desc;

		m_renderTargetTexture11 = reinterpret_cast<ID3D11Texture2D*>( res );

		m_renderTargetTexture11->GetDesc( &desc );

		width = desc.Width;
		height = desc.Height;
	}
}
//-----------------------------------------------------------------------------------------------------------------
string Texture::GetTextureName()
{
	return m_textureName;
}
//-----------------------------------------------------------------------------------------------------------------
void Texture::Shutdown()
{
	if (m_engineOptions->m_d3dVersion == DIRECTX10)
	{
		if (m_texture10)
		{
			m_texture10->Release();
			m_texture10 = 0;
		}
	}
	else if (m_engineOptions->m_d3dVersion == DIRECTX11)
	{
		if (m_texture11)
		{
			m_texture11->Release();
			m_texture11 = 0;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------