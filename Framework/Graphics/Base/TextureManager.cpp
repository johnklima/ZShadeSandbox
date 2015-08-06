#include "afxHeader.h"
#include "TextureManager.h"
#include "CGlobal.h"
#include <Windows.h>
#include <wincodec.h>
//#include <ppl.h>
//#include <ppltasks.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "ScreenGrab.h"
#include "DirectXTex.h"
#include <Windows.h>
//#include <wrl/client.h>
#include "XMMath.h"
#include "DCRenderTexture.h"
#include "QuadMesh.h"
#include "Camera.h"
#include "ZShadeMessageCenter.h"
#include "InterfacePointers.h"
//using namespace Microsoft::WRL;
//===============================================================================================================================
//===============================================================================================================================
TextureManager* TextureManager::m_instance = 0;
//===============================================================================================================================
TextureManager::TextureManager(D3D* d3d)
:   mD3DSystem(d3d)
{
}
//===============================================================================================================================
TextureManager::~TextureManager()
{
}
//===============================================================================================================================
void TextureManager::NewInstance(D3D* d3d)
{
	m_instance = new TextureManager(d3d);
}
//===============================================================================================================================
TextureManager* TextureManager::Instance()
{
	return m_instance;
}
//===============================================================================================================================
ID3D11Texture2D* TextureManager::ConvertToTex2D(ID3D11Resource* res)
{
	ID3D11Texture2DPtr tex2D;

	tex2D.Attach(reinterpret_cast<ID3D11Texture2D*>(res));

	return tex2D.GetInterfacePtr();
}
//===============================================================================================================================
bool TextureManager::Exists(BetterString filename, D3D_VERSION d3d)
{
	if (d3d == DIRECTX10)
	{
		//map< string, ID3D10ShaderResourceView* >::iterator texture_it = m_pTextures.find(filename.toString());
		
		//if (texture_it != m_pTextureData.end())
		//	return true;
		//else
		//	return false;
	}
	else if (d3d == DIRECTX11)
	{
		map< string, ID3D11ShaderResourceView* >::iterator texture_it = m_pTextures.find(filename.toString());
		
		if (texture_it != m_pTextures.end())
			return true;
		else
			return false;
	}
	
	return false;
}
//===============================================================================================================================
HRESULT TextureManager::CreateTGATextureFromFile(LPCWSTR tex_filename, ID3D11ShaderResourceView** srv)
{
	HRESULT hr;

	TexMetadata imageMetadata;
	ScratchImage* image = new ScratchImage();

	hr = LoadFromTGAFile(tex_filename, &imageMetadata, *image);

	hr = CreateShaderResourceView(mD3DSystem->GetDevice11(), image->GetImages(), image->GetImageCount(), imageMetadata, srv);

	return hr;
}
//===============================================================================================================================
HRESULT TextureManager::LoadDDSTextureFromFile(LPCWSTR tex_filename, ID3D11ShaderResourceView** srv)
{
	HRESULT hr;
	
	TexMetadata imageMetadata;
	ScratchImage* image = new ScratchImage();
	
	hr = LoadFromDDSFile(tex_filename, DDS_FLAGS_NONE, &imageMetadata, *image);
	
	hr = CreateShaderResourceView(mD3DSystem->GetDevice11(), image->GetImages(), image->GetImageCount(), imageMetadata, srv);
	
	return hr;
}
//===============================================================================================================================
HRESULT TextureManager::LoadWICTextureFromFile(LPCWSTR tex_filename, ID3D11ShaderResourceView** srv)
{
	HRESULT hr;
	
	TexMetadata imageMetadata;
	ScratchImage* image = new ScratchImage();

	hr = LoadFromWICFile(tex_filename, WIC_FLAGS_NONE, &imageMetadata, *image);
	
	hr = CreateShaderResourceView(mD3DSystem->GetDevice11(), image->GetImages(), image->GetImageCount(), imageMetadata, srv);
	
	return hr;
}
//===============================================================================================================================
ID3D11ShaderResourceView* TextureManager::GetTexture(BetterString tex_filename)
{
	string fileExt = CGlobal::GetFileExt(tex_filename);
	
	if (fileExt == "dds")
	{
		return TextureManager::Instance()->GetTexture(tex_filename, DDS);
	}
	else if (fileExt == "tga")
	{
		return TextureManager::Instance()->GetTexture(tex_filename, TGA);
	}
	else if (fileExt == "png")
	{
		return TextureManager::Instance()->GetTexture(tex_filename, PNG);
	}
	
	return NULL;
}
//===============================================================================================================================
ID3D11ShaderResourceView* TextureManager::LoadSRV(LPCWSTR tex_filename, TextureType tt)
{
	ID3D11ShaderResourceView* mTexture = 0;
	
	switch (tt)
	{
		case DDS:
		{
			HRESULT result;
			result = CreateDDSTextureFromFile(mD3DSystem->GetDevice11(), tex_filename, nullptr, &mTexture);
			if (FAILED(result))
			{
				result = LoadDDSTextureFromFile(tex_filename, &mTexture);
				if (FAILED(result))
				{
					return 0;
				}
			}
		}
		break;
		case TGA:
		{
			HRESULT result = CreateTGATextureFromFile(tex_filename, &mTexture);
			if (FAILED(result))
			{
				return 0;
			}
		}
		break;
		case PNG:
		{
			HRESULT result;
			result = CreateWICTextureFromFile(mD3DSystem->GetDevice11(), tex_filename, nullptr, &mTexture);
			if (FAILED(result))
			{
				result = LoadWICTextureFromFile(tex_filename, &mTexture);
				if (FAILED(result))
				{
					return 0;
				}
			}
		}
		break;
	}
	
	return mTexture;
}
//===============================================================================================================================
ID3D11ShaderResourceView* TextureManager::GetTexture(BetterString tex_filename, TextureType tt)
{
	map< string, ID3D11ShaderResourceView* >::iterator texture_it = m_pTextures.find(tex_filename.toString());

	if (texture_it != m_pTextures.end())
	{
		return m_pTextures[tex_filename.toString()];
	}
	else
	{
		ID3D11ShaderResourceView* mTexture;

		BetterString str = tex_filename;
		
		unique_ptr<wchar_t> name = tex_filename.ToWideStr();
		
		mTexture = LoadSRV(name.get(), tt);

		m_pTextures.insert( make_pair(tex_filename.toString(), mTexture) );

		return mTexture;
	}
}
//===============================================================================================================================
bool TextureManager::loadTexture(BetterString filename, ID3D11Texture2D** texture, int& textureDataSize, byte*& srcTextureData, TextureType tt)
{
	ID3D11Device* device = mD3DSystem->GetDevice11();
	ID3D11DeviceContext* immediateContext = mD3DSystem->GetDeviceContext();
	ID3D11Texture2D* tempTexture;

	//string file = "Textures\\";
	//file += filename.toString();
	
	HRESULT hr = E_FAIL;
	
	unique_ptr<wchar_t> name = filename.ToWideStr();

	switch (tt)
	{
		case DDS:
		{
			hr = CreateDDSTextureFromFile(device, name.get(), (ID3D11Resource **)texture, NULL);
		}
		break;
		//case TGA:
		case PNG:
		{
			hr = CreateWICTextureFromFile(device, name.get(), (ID3D11Resource **)texture, NULL);
		}
		break;
	}
	
	//if(SUCCEEDED(D3DX11CreateTextureFromFile(device, file.c_str(), NULL, NULL, (ID3D11Resource **)texture, NULL)))
	if(SUCCEEDED(hr))
	{
		// Load the texture and store it
		GetTexture(filename, tt);
		
		D3D11_TEXTURE2D_DESC desc;
		(*texture)->GetDesc(&desc);

		// To keep it simple, we limit the textures we load to RGBA 8bits per channel
		if(desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			OutputDebugStringA( "We want to read a simple RGBA texture 8 bits per channel but the required texture has a different format." );
			return false;
		}

		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		if (device->CreateTexture2D(&desc, NULL, &tempTexture) != S_OK)
			return false;

		immediateContext->CopyResource(tempTexture, *texture);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (immediateContext->Map(tempTexture, 0, D3D11_MAP_READ, 0, &mappedResource) != S_OK)
			return false;

		textureDataSize =  mappedResource.RowPitch * desc.Height;
		if(srcTextureData)
			delete [] srcTextureData;
		srcTextureData = new byte[textureDataSize];
		memcpy(srcTextureData, mappedResource.pData, textureDataSize);

		immediateContext->Unmap(tempTexture, 0);

		return true;
	}
	else
		return false;
}
//===============================================================================================================================
ID3D11ShaderResourceView* TextureManager::CreateTexture2DArraySRV(std::vector<std::string>& filenames)
{
	//
	// Load the texture elements individually from file.  These textures
	// won't be used by the GPU (0 bind flags), they are just used to 
	// load the image data from file.  We use the STAGING usage so the
	// CPU can read the resource.
	//
	
	UINT size = filenames.size();
	HRESULT result;
	
	std::vector<ID3D11Texture2D*> srcTex(size);
	
	for(UINT i = 0; i < size; ++i)
	{
		unique_ptr<wchar_t> name = BetterString(filenames[i].c_str()).ToWideStr();
		
		// ReleaseAndGetAddressOf
		result = CreateDDSTextureFromFileEx(mD3DSystem->GetDevice11(), name.get(), 0u, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, 0,//D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
			0, false, reinterpret_cast<ID3D11Resource**>(&srcTex[i]), nullptr, nullptr);
		
		if (FAILED(result))
		{
			return 0;
		}
	}
	
	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//
	
	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);
	
	/*
		Usage: D3D11_USAGE_DEFAULT - Video RAM
			   D3D11_USAGE_STAGING - in the system RAM and cannot be used by the GPU at all
			   D3D11_USAGE_DYNAMIC - CPU- and GPU-addressable RAM
			   D3D11_USAGE_IMMUTABLE
	*/
	
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = size;
	texArrayDesc.Format             = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage				= D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags		= 0;// D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	texArrayDesc.MiscFlags          = 0;
	
	ID3D11Texture2D* texArray = 0;
	mD3DSystem->GetDevice11()->CreateTexture2D(&texArrayDesc, 0, &texArray);
	
	//
	// Copy individual texture elements into texture array.
	//
	
	// for each texture element...
	for(UINT texElement = 0; texElement < size; ++texElement)
	{
		// for each mipmap level...
		for(UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			mD3DSystem->GetDeviceContext()->CopySubresourceRegion(texArray, D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels), 0, 0, 0, srcTex[texElement], mipLevel, nullptr);
		}
	}
	
	// Release the temporary Video Memory used per texture 
	for (UINT i = 0; i < size; ++i)
		SAFE_RELEASE(srcTex[i]);

	//
	// Create a resource view to the texture array.
	//
	
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;
	
	ID3D11ShaderResourceView* texArraySRV = 0;
	mD3DSystem->GetDevice11()->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV);
	
	//
	// Cleanup - we only need the resource view.
	//
	
	SAFE_RELEASE(texArray);
	
	// Return the new texture array
	return texArraySRV;
}
//=========================================================================================================================
ID3D11ShaderResourceView* TextureManager::CreateRandomTexture1DSRV()
{
	// 
	// Create the random data.
	//
	XMFLOAT4 randomValues[1024];

	for(int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = ZShadeSandboxMath::ZMath::RandF(-1.0f, 1.0f);
		randomValues[i].y = ZShadeSandboxMath::ZMath::RandF(-1.0f, 1.0f);
		randomValues[i].z = ZShadeSandboxMath::ZMath::RandF(-1.0f, 1.0f);
		randomValues[i].w = ZShadeSandboxMath::ZMath::RandF(-1.0f, 1.0f);
	}

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(XMFLOAT4);
    initData.SysMemSlicePitch = 0;

	//
	// Create the texture.
	//
    D3D11_TEXTURE1D_DESC texDesc;
    texDesc.Width = 1024;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
    mD3DSystem->GetDevice11()->CreateTexture1D(&texDesc, &initData, &randomTex);

	//
	// Create the resource view.
	//
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
	ID3D11ShaderResourceView* randomTexSRV = 0;
    mD3DSystem->GetDevice11()->CreateShaderResourceView(randomTex, &viewDesc, &randomTexSRV);

	SAFE_RELEASE(randomTex);

	return randomTexSRV;
}
//===============================================================================================================================
bool TextureManager::Create1DTexture(ID3D11Texture1D*& texture, ID3D11ShaderResourceView*& srv, int size, void* data)
{
	D3D11_TEXTURE1D_DESC tex_desc;
	tex_desc.Width = size;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tex_desc.Usage = D3D11_USAGE_IMMUTABLE;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = data;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;
	
	mD3DSystem->GetDevice11()->CreateTexture1D(&tex_desc, &init_data, &texture);
	assert(texture);
	
	// Create shader resource
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srv_desc.Texture1D.MipLevels = 1;
	srv_desc.Texture1D.MostDetailedMip = 0;
	
	mD3DSystem->GetDevice11()->CreateShaderResourceView(texture, &srv_desc, &srv);
	assert(srv);
	
	return true;
}
//===============================================================================================================================
void TextureManager::WriteToFile(BetterString filename, BetterString textureName)
{
	// Locate the texture
	map<string, ID3D11ShaderResourceView*>::iterator iter = m_pTextures.find(textureName.toString());
	if (iter != m_pTextures.end())
	{
		WriteToFile(filename, (*iter).second);
	}
}
//===============================================================================================================================
void TextureManager::WriteToFile(BetterString filename, ID3D11ShaderResourceView* textureSRV)
{
	string fileExt = CGlobal::GetFileExt(filename);
	
	if (fileExt == "dds")
	{
		unique_ptr<wchar_t> name = filename.ToWideStr();
		WriteDDSToFile(name.get(), textureSRV);
	}
	else if (fileExt == "tga")
	{
		unique_ptr<wchar_t> name = filename.ToWideStr();
		WriteTGAToFile(name.get(), textureSRV);
	}
	else if (fileExt == "png")
	{
		unique_ptr<wchar_t> name = filename.ToWideStr();
		WritePNGToFile(name.get(), textureSRV);
	}
}
//===============================================================================================================================
void TextureManager::WriteDDSToFile(LPCWSTR filename, ID3D11ShaderResourceView* textureSRV)
{
	ID3D11Texture2D *textureInterface = 0;
	ID3D11Resource *textureResource;
	textureSRV->GetResource(&textureResource);
	textureResource->QueryInterface<ID3D11Texture2D>(&textureInterface);
	
	HRESULT result;
	result = SaveDDSTextureToFile(mD3DSystem->GetDeviceContext(), textureInterface, filename);
	
	if (FAILED(result))
	{
		ScratchImage image;
		result = CaptureTexture(mD3DSystem->GetDevice11(), mD3DSystem->GetDeviceContext(), textureInterface, image);
		
		if (SUCCEEDED(result))
		{
			result = SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DDS_FLAGS_NONE, filename);
			
			if (FAILED(result))
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Failed to save DDS texture !!");
			}
		}
	}
	
	SAFE_RELEASE(textureInterface);
	SAFE_RELEASE(textureResource);
}
//=========================================================================================================================
void TextureManager::WriteTGAToFile(LPCWSTR filename, ID3D11ShaderResourceView* textureSRV)
{
	// https://directxtex.codeplex.com/wikipage?title=CaptureTexture
	
    ID3D11Texture2D *textureInterface = 0;
	ID3D11Resource *textureResource;
	textureSRV->GetResource(&textureResource);
	textureResource->QueryInterface<ID3D11Texture2D>(&textureInterface);
	
	HRESULT result;
	//ScratchImage image;
	ScratchImage image;
	result = CaptureTexture(mD3DSystem->GetDevice11(), mD3DSystem->GetDeviceContext(), textureInterface, image);
	
	if (SUCCEEDED(result))
	{
		result = SaveToTGAFile(*image.GetImages(), filename);
		
		if (FAILED(result))
		{
			ZShadeMessageCenter::MsgBoxError(NULL, "Failed to save TGA texture !!");
		}
	}
	
	SAFE_RELEASE(textureInterface);
	SAFE_RELEASE(textureResource);
}
//===============================================================================================================================
void TextureManager::WritePNGToFile(LPCWSTR filename, ID3D11ShaderResourceView* textureSRV)
{
    ID3D11Texture2D *textureInterface = 0;
	ID3D11Resource *textureResource;
	textureSRV->GetResource(&textureResource);
	textureResource->QueryInterface<ID3D11Texture2D>(&textureInterface);
	
	HRESULT result;
	result = SaveWICTextureToFile(mD3DSystem->GetDeviceContext(), textureInterface, GUID_ContainerFormatPng, filename);
	
	if (FAILED(result))
	{
		ScratchImage image;
		result = CaptureTexture(mD3DSystem->GetDevice11(), mD3DSystem->GetDeviceContext(), textureInterface, image);
		
		if (SUCCEEDED(result))
		{
			result = SaveToWICFile(image.GetImages(), image.GetImageCount(), WIC_FLAGS_NONE, GUID_ContainerFormatPng, filename);
			
			if (FAILED(result))
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Failed to save PNG texture !!");
			}
		}
	}
	
	SAFE_RELEASE(textureInterface);
	SAFE_RELEASE(textureResource);
}
//===============================================================================================================================