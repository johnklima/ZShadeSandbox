#include "HUD.h"
#include "Convert.h"
//================================================================================================================
//================================================================================================================
HUD::HUD(D3D* d3d)
:   m_D3DSystem(d3d)
,	mImageID(0)
,	mTextID(0)
{
}
//================================================================================================================
HUD::~HUD()
{
}
//================================================================================================================
void HUD::AddImage(string base_path, string textureName, int x, int y, int w, int h)
{
	ZShadeSandboxGraphics::Image* im = new ZShadeSandboxGraphics::Image(
		m_D3DSystem,
		textureName,
		base_path,
		x,
		y,
		w,
		h
	);
	
	AddImage(im);
}
//================================================================================================================
void HUD::AddText(string text, int x, int y)
{
	ZShadeSandboxGraphics::Text* newText = new ZShadeSandboxGraphics::Text();
	newText->SetD3D(m_D3DSystem);
	newText->Initialize();
	newText->ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	newText->TextName() = text;
	if (newText->TextName() == "NONE")
		newText->TextName() = "";
	newText->TextTransform() = XMMatrixTranslation(x, y, 0);
	newText->TextColor() = XMFLOAT4(1, 1, 1, 1);
	
	AddText(newText);
}
//================================================================================================================
void HUD::AddImage(ZShadeSandboxGraphics::Image* im)
{
	im->ID() = mImageID;
	m_images.push_back( im );
	mImageID++;
}
//================================================================================================================
void HUD::AddText(ZShadeSandboxGraphics::Text* t)
{
	t->ID() = mTextID;
	m_texts.push_back( t );
	mTextID++;
}
//================================================================================================================
void HUD::RemoveImage(ZShadeSandboxGraphics::Image* im)
{
	ZShadeSandboxGraphics::Image* image = 0;
	auto current = m_images.begin();
	while (current != m_images.end())
	{
		image = (*current);
		if (image != 0 && image->ID() == im->ID())
		{
			current = m_images.erase(current);
			mImageID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_images.size(); i++)
	{
		if (m_images[i] == 0) continue;
		
		m_images[i]->ID() = i;
	}
}
//================================================================================================================
void HUD::RemoveText(ZShadeSandboxGraphics::Text* t)
{
	ZShadeSandboxGraphics::Text* text = 0;
	auto current = m_texts.begin();
	while (current != m_texts.end())
	{
		text = (*current);
		if (text != 0 && text->ID() == t->ID())
		{
			current = m_texts.erase(current);
			mTextID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_texts.size(); i++)
	{
		if (m_texts[i] == 0) continue;
		
		m_texts[i]->ID() = i;
	}
}
//================================================================================================================
void HUD::RemoveImage(int id)
{
	ZShadeSandboxGraphics::Image* image = 0;
	auto current = m_images.begin();
	while (current != m_images.end())
	{
		image = (*current);
		if (image != 0 && image->ID() == id)
		{
			current = m_images.erase(current);
			mImageID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_images.size(); i++)
	{
		if (m_images[i] == 0) continue;
		
		m_images[i]->ID() = i;
	}
}
//================================================================================================================
void HUD::RemoveText(int id)
{
	ZShadeSandboxGraphics::Text* text = 0;
	auto current = m_texts.begin();
	while (current != m_texts.end())
	{
		text = (*current);
		if (text != 0 && text->ID() == id)
		{
			current = m_texts.erase(current);
			mTextID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_texts.size(); i++)
	{
		if (m_texts[i] == 0) continue;
		
		m_texts[i]->ID() = i;
	}
}
//================================================================================================================
bool HUD::ImageClicked(ZShadeSandboxGraphics::Image*& im, int x, int y)
{
	for (int i = 0; i < m_images.size(); i++)
	{
		ZShadeSandboxGraphics::Image* image = m_images[i];

		if (image != NULL)
		{
			if (x >= image->XPos() && x <= image->XPos() + image->Width())
			{
				if (y >= image->YPos() && y <= image->YPos() + image->Height())
				{
					im = image;
					return true;
				}
			}
		}
	}

	return false;
}
//================================================================================================================
bool HUD::TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	for (int i = 0; i < m_texts.size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = m_texts[i];
		
		if (t == 0) continue;
		
		if (x >= t->GetXPos() && x <= t->GetXPos() + t->Width())
		{
			if (y >= t->GetYPos() && y <= t->GetYPos() + t->Height())
			{
				text = t;
				return true;
			}
		}
	}

	return false;
}
//================================================================================================================
void HUD::MoveImage(ZShadeSandboxGraphics::Image*& im, int x, int y)
{
	for (int i = 0; i < m_images.size(); i++)
	{
		ZShadeSandboxGraphics::Image* image = m_images[i];
		
		if (image == 0) continue;
		
		if (image == im)
		{
			image->XPos() = x;
			image->YPos() = y;
			return;
		}
	}
}
//================================================================================================================
void HUD::MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	for (int i = 0; i < m_texts.size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = m_texts[i];
		
		if (t == 0) continue;
		
		if (t == text)
		{
			t->TextTransform() = XMMatrixTranslation(x, y, 0);
			return;
		}
	}
}
//================================================================================================================
//void HUD::RemoveImage(ZShadeSandboxGraphics::Image* im)
//{
//	int value = 0;
//	bool kill = false;
//	for (int i = 0; i < m_images.size(); i++)
//	{
//		ZShadeSandboxGraphics::Image* p_im = m_images[i];
//
//		if (p_im != NULL)
//		{
//			if (p_im == im)
//			{
//				value = i;
//				kill = true;
//				break;
//			}
//		}
//	}
//
//	if (kill)
//	{
//		m_images.erase(m_images.begin() + value);
//	}
//}
////================================================================================================================
//void HUD::RemoveText(ZShadeSandboxGraphics::Text* t)
//{
//	int value = 0;
//	bool kill = false;
//	for (int i = 0; i < m_texts.size(); i++)
//	{
//		ZShadeSandboxGraphics::Text* pt = m_texts[i];
//
//		if (pt != NULL)
//		{
//			if (pt == t)
//			{
//				value = i;
//				kill = true;
//				break;
//			}
//		}
//	}
//
//	if (kill)
//	{
//		m_texts.erase(m_texts.begin() + value);
//	}
//}
//================================================================================================================
//bool HUD::HUDImageSelected(ZShadeSandboxGraphics::Image*& im, int x, int y)
//{
//	for (int i = 0; i < m_images.size(); i++)
//	{
//		ZShadeSandboxGraphics::Image* p_im = m_images[i];
//
//		if (p_im != NULL)
//		{
//			if (x >= p_im->XPos() && x <= p_im->XPos() + p_im->Width())
//			{
//				if (y >= p_im->YPos() && y <= p_im->YPos() + p_im->Height())
//				{
//					im = p_im;
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}
////================================================================================================================
//bool HUD::HUDTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y)
//{
//	for (int i = 0; i < m_texts.size(); i++)
//	{
//		ZShadeSandboxGraphics::Text* t = m_texts[i];
//
//		if (t != NULL)
//		{
//			if (x >= t->GetXPos() && x <= t->GetXPos() + t->GetWidth())
//			{
//				if (y >= t->GetYPos() && y <= t->GetYPos() + t->GetHeight())
//				{
//					text = t;
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}
//================================================================================================================
void HUD::Render(Camera* camera, int blendAmount)
{
	for (int i = 0; i < m_images.size(); i++)
	{
		m_images[i]->Draw(camera, blendAmount);
	}

	for (int i = 0; i < m_texts.size(); i++)
	{
		m_texts[i]->Render11();
	}
}
//================================================================================================================