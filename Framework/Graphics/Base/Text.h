//===============================================================================================================================
// Text
//
//===============================================================================================================================
// History
//
// -Created on 1/14/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __TEXT_H
#define __TEXT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <Windows.h>
#include "BetterString.h"
#include "FontShader.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxGraphics {
	class Text
	{
		D3D* m_D3DSystem;

		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;

		FontShader* m_FontShader;

	public:

		Text();
		Text(D3D* d3d);
		~Text();

		void SetD3D(D3D* d3d) { m_D3DSystem = d3d; }

		void Initialize();
		void CreateShadow();

		//"Microsoft Sans Serif"
		//fontSize = 8.5f
		void ChangeFont(LPCWSTR fontName = L"Times New Roman", float fontSize = 12.0f, UINT fontStyle = FontStyle::Regular, bool antiAliased = true);

		void RenderMeshBuffers();

		void Render11();
		void Render11(const BetterString name);
		
		int GetXPos();
		int GetYPos();
		int Width();
		int Height();

		XMMATRIX& TextTransform() { return transform; }
		XMMATRIX TextTransform() const { return transform; }

		XMFLOAT4& TextColor() { return color; }
		XMFLOAT4 TextColor() const { return color; }

		void SetText(BetterString name) { m_Name = name; }
		BetterString& TextName() { return m_Name; }
		BetterString TextName() const { return m_Name; }
		
		int& ID()			{ return mID; }
		int ID() const		{ return mID; }

		float& FontSize() { return m_fontSize; }
		float FontSize() const { return m_fontSize; }

		bool HasShadow() { return bHasShadow; }
		
		Text* Clone();
		Text* Clone(int x, int y);
		
	protected:

		int	mID;
		
		bool bHasShadow;
		float m_fontSize;

		BetterString m_Name;
		GDIFont font;

		XMMATRIX transform;
		XMFLOAT4 color;

		Text* textShadow;
	};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__TEXT_H