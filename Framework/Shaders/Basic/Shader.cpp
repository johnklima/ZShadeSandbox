#include "Shader.h"
#include "CGlobal.h"
#include <sstream>
using ZShadeSandboxShader::Shader;
//=================================================================================================================
//===============================================================================================================================
Shader::Shader(D3D* d3d)
:	m_pVertexShader(0)
,	m_pHullShader(0)
,	m_pDomainShader(0)
,	m_pPixelShader(0)
,	m_pGeometryShader(0)
,	m_pComputeShader(0)
,	m_Wireframe(false)
,	m_pD3DSystem(d3d)
,	m_UseInputLayout(true)
//,	m_UseOrtho(false)
//,	m_UseCustomWorld(false)
,	m_CurrentLayout11(0)
,	mCurrentLayoutName("")
,	mCurrentVSFuncName("")
,	mCurrentPSFuncName("")
,	mCurrentGSFuncName("")
,	mCurrentCSFuncName("")
{
}
//=================================================================================================================
Shader::Shader(const Shader& c)
{
}
//=================================================================================================================
Shader::~Shader()
{
	/*SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pHullShader);
	SAFE_RELEASE(m_pDomainShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pComputeShader);

	map<BetterString, ID3D11VertexShader*>::iterator   VertexShadersIter;
	map<BetterString, ID3D11PixelShader*>::iterator    PixelShadersIter;
	map<BetterString, ID3D11GeometryShader*>::iterator GeometryShadersIter;
	map<BetterString, ID3D11ComputeShader*>::iterator  ComputeShadersIter;

	for (VertexShadersIter = m_VertexShaders.begin();
		 VertexShadersIter != m_VertexShaders.end();
		 VertexShadersIter++)
	{
		SAFE_RELEASE(VertexShadersIter->second);
	}

	for (PixelShadersIter = m_PixelShaders.begin();
		 PixelShadersIter != m_PixelShaders.end();
		 PixelShadersIter++)
	{
		SAFE_RELEASE(PixelShadersIter->second);
	}

	for (GeometryShadersIter = m_GeometryShaders.begin();
		 GeometryShadersIter != m_GeometryShaders.end();
		 GeometryShadersIter++)
	{
		SAFE_RELEASE(GeometryShadersIter->second);
	}

	for (ComputeShadersIter = m_ComputeShaders.begin();
		 ComputeShadersIter != m_ComputeShaders.end();
		 ComputeShadersIter++)
	{
		SAFE_RELEASE(ComputeShadersIter->second);
	}*/
}
//=================================================================================================================
ID3D11VertexShader* Shader::GetVertexShader(string shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_VERTEX);
	return m_pVertexShader->mShader;
}
//=================================================================================================================
ID3D11PixelShader* Shader::GetPixelShader(string shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_PIXEL);
	return m_pPixelShader->mShader;
}
//=================================================================================================================
ID3D11GeometryShader* Shader::GetGeometryShader(string shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_GEOMETRY);
	return m_pGeometryShader->mShader;
}
//=================================================================================================================
ID3D11ComputeShader* Shader::GetComputeShader(string shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_COMPUTE);
	return m_pComputeShader->mShader;
}
//=================================================================================================================
void Shader::SetVertexShader()
{
	m_pD3DSystem->GetDeviceContext()->VSSetShader(m_pVertexShader->mShader, NULL, 0);

	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}
//=================================================================================================================
void Shader::SetPixelShader()
{
	m_pD3DSystem->GetDeviceContext()->PSSetShader(m_pPixelShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetHullShader()
{
	m_pD3DSystem->GetDeviceContext()->HSSetShader(m_pHullShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetDomainShader()
{
	m_pD3DSystem->GetDeviceContext()->DSSetShader(m_pDomainShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetGeometryShader()
{
	m_pD3DSystem->GetDeviceContext()->GSSetShader(m_pGeometryShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetComputeShader()
{
	m_pD3DSystem->GetDeviceContext()->CSSetShader(m_pComputeShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetDefaultInputLayout()
{
	m_pD3DSystem->GetDeviceContext()->IASetInputLayout(m_CurrentLayout11);
}
//=================================================================================================================
void Shader::SetInputLayout(string vertexShaderName)
{
	/*if (mCurrentLayoutName == vertexShaderName)
	{
		m_pD3DSystem->GetDeviceContext()->IASetInputLayout(m_CurrentLayout11);
	}
	else
	{
		m_CurrentLayout11 = m_VertexShaderLayout[vertexShaderName]->m_layout11;
		m_pD3DSystem->GetDeviceContext()->IASetInputLayout(m_CurrentLayout11);
		mCurrentLayoutName = vertexShaderName;
	}*/

	if (mCurrentLayoutName != vertexShaderName)
	{
		m_CurrentLayout11 = m_VertexShaderLayout[vertexShaderName]->m_layout11;
		m_pD3DSystem->GetDeviceContext()->IASetInputLayout(m_CurrentLayout11);
		mCurrentLayoutName = vertexShaderName;
	}
	else
	{
		SetDefaultInputLayout();
	}
}
//=================================================================================================================
void Shader::RenderDraw11(int vertexCount)
{
	m_pD3DSystem->GetDeviceContext()->Draw(vertexCount, 0);
}
//=================================================================================================================
void Shader::RenderIndex11(int indexCount)
{
	m_pD3DSystem->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}
//=================================================================================================================
void Shader::RenderIndex11(int startIndex, int indexCount)
{
	m_pD3DSystem->GetDeviceContext()->DrawIndexed(indexCount, startIndex, 0);
}
//=================================================================================================================
void Shader::RenderCS11(int threadX, int threadY, int threadZ)
{
	m_pD3DSystem->GetDeviceContext()->Dispatch( threadX, threadY, threadZ );
}
//=================================================================================================================
void Shader::RenderIndexInstanced11(int indexCount, int primCount)
{
	m_pD3DSystem->GetDeviceContext()->DrawIndexedInstanced(indexCount, primCount, 0, 0, 0);
}
//=================================================================================================================
void Shader::SwitchTo(string shaderFuncName, int type)
{
	switch (type)
	{
		case EShaderTypes::ST_VERTEX:
		{
			if (mCurrentVSFuncName == shaderFuncName) return;

			map<string, VertexShader*>::iterator   VertexShadersIter;
			for (VertexShadersIter = m_VertexShaders.begin();
				 VertexShadersIter != m_VertexShaders.end();
				 VertexShadersIter++)
			{
				if (VertexShadersIter->first == shaderFuncName)
				{
					m_pVertexShader = VertexShadersIter->second;
					mCurrentVSFuncName = shaderFuncName;
					break;
				}
			}
		}
		break;
		case EShaderTypes::ST_PIXEL:
		{
			if (mCurrentPSFuncName == shaderFuncName) return;

			map<string, PixelShader*>::iterator    PixelShadersIter;
			for (PixelShadersIter = m_PixelShaders.begin();
				 PixelShadersIter != m_PixelShaders.end();
				 PixelShadersIter++)
			{
				if (PixelShadersIter->first == shaderFuncName)
				{
					m_pPixelShader = PixelShadersIter->second;
					mCurrentPSFuncName = shaderFuncName;
					break;
				}
			}
		}
		break;
		case EShaderTypes::ST_GEOMETRY:
		{
			if (mCurrentGSFuncName == shaderFuncName) return;

			map<string, GeometryShader*>::iterator GeometryShadersIter;
			for (GeometryShadersIter = m_GeometryShaders.begin();
				 GeometryShadersIter != m_GeometryShaders.end();
				 GeometryShadersIter++)
			{
				if (GeometryShadersIter->first == shaderFuncName)
				{
					m_pGeometryShader = GeometryShadersIter->second;
					mCurrentGSFuncName = shaderFuncName;
					break;
				}
			}
		}
		break;
		case EShaderTypes::ST_COMPUTE:
		{
			if (mCurrentCSFuncName == shaderFuncName) return;

			map<string, ComputeShader*>::iterator  ComputeShadersIter;
			for (ComputeShadersIter = m_ComputeShaders.begin();
				 ComputeShadersIter != m_ComputeShaders.end();
				 ComputeShadersIter++)
			{
				if (ComputeShadersIter->first == shaderFuncName)
				{
					m_pComputeShader = ComputeShadersIter->second;
					mCurrentCSFuncName = shaderFuncName;
					break;
				}
			}
		}
		break;
	}
}
//=================================================================================================================
ID3D11InputLayout* Shader::GetInputLayout(string vertexShaderName)
{
	if (m_VertexShaderLayout[vertexShaderName] == 0) return 0;
	if (m_VertexShaderLayout[vertexShaderName]->m_layout11 == 0) return 0;
	return m_VertexShaderLayout[vertexShaderName]->m_layout11;
}
//=================================================================================================================
void Shader::SetInputLayoutDesc(string vertexShaderName, D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements)
{
	SLayout* sl = new SLayout();
	
	sl->m_pLayoutDesc = desc;
	sl->m_NumElements = NumElements;
	
	//m_VertexShaderLayout[vertexShaderName] = sl;

	m_VertexShaderLayout.insert(make_pair(vertexShaderName, sl));
}
//=================================================================================================================
void Shader::ClearInputLayout()
{
	if (m_VertexShaderLayout.size() != 0)
		m_VertexShaderLayout.clear();
}
//=================================================================================================================
void Shader::AssignVertexShaderLayout(string vertexShaderName)
{
	//Create the vertex input layout
	if (m_UseInputLayout)
	{
		ID3DBlob* blob = m_pVertexShader->mBlobData;
		
		auto iter = m_VertexShaderLayout.find(vertexShaderName);

		if (iter != m_VertexShaderLayout.end())
		{
			SLayout* sLayout = (*iter).second;

			ID3D11InputLayout* layout;
			D3D11_INPUT_ELEMENT_DESC* desc = sLayout->m_pLayoutDesc;
			UINT numElements = sLayout->m_NumElements;

			m_pD3DSystem->GetDevice11()->CreateInputLayout(
				desc,
				numElements,
				blob->GetBufferPointer(), blob->GetBufferSize(),
				&layout);

			m_VertexShaderLayout[vertexShaderName]->m_layout11 = layout;

			assert(m_VertexShaderLayout[vertexShaderName]->m_layout11);
		}
	}
}
//=================================================================================================================
void Shader::LoadVertexShader(string shaderFuncName)
{
	m_VertexShaders.insert(make_pair(shaderFuncName, (VertexShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_VERTEX);
}
//=================================================================================================================
void Shader::LoadPixelShader(string shaderFuncName)
{
	m_PixelShaders.insert(make_pair(shaderFuncName, (PixelShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_PIXEL);
}
//=================================================================================================================
void Shader::LoadHullShader(string shaderFuncName)
{
	m_pHullShader = (HullShader*)ShaderCompiler::GetShaderByName(shaderFuncName);
}
//=================================================================================================================
void Shader::LoadDomainShader(string shaderFuncName)
{
	m_pDomainShader = (DomainShader*)ShaderCompiler::GetShaderByName(shaderFuncName);
}
//=================================================================================================================
void Shader::LoadGeometryShader(string shaderFuncName)
{
	m_GeometryShaders.insert(make_pair(shaderFuncName, (GeometryShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_GEOMETRY);
}
//=================================================================================================================
void Shader::LoadComputeShader(string shaderFuncName)
{
	m_ComputeShaders.insert(make_pair(shaderFuncName, (ComputeShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_COMPUTE);
}
//=================================================================================================================