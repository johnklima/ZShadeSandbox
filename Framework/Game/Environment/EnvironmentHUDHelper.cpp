#include "EnvironmentHUDHelper.h"
#include "Convert.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "Environment.h"
#include "HUD.h"
//================================================================================================================
//================================================================================================================
EnvironmentHUDHelper::EnvironmentHUDHelper()
{
}
//================================================================================================================
EnvironmentHUDHelper::~EnvironmentHUDHelper()
{
}
//================================================================================================================
bool EnvironmentHUDHelper::GetImageID(int& imageID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetHUDSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Image* p_clicked_image;
	bool clicked = env2D->GetHUDSystem()->ImageClicked(p_clicked_image, x, y);
	if (clicked)
	{
		imageID = p_clicked_image->ID();
	}

	return clicked;
}
//================================================================================================================
bool EnvironmentHUDHelper::GetTextID(int& textID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetHUDSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Text* p_clicked_text;
	bool clicked = env2D->GetHUDSystem()->TextClicked(p_clicked_text, x, y);
	if (clicked)
	{
		textID = p_clicked_text->ID();
	}

	return clicked;
}
//================================================================================================================
void EnvironmentHUDHelper::CreateHUD(string hud_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	HUD* hud = new HUD(env2D->GetD3D());
	env2D->GetHUDSystem() = hud;
}
//================================================================================================================
void EnvironmentHUDHelper::LoadHUD(string hud_filename, string& hud_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	ZShadeXMLLoader::LoadHUDXML(hud_filename, env2D->GetGD2D(), env2D->GetEngineOptions(), env2D->GetD3D(), env2D->GetHUDSystem());
}
//================================================================================================================
void EnvironmentHUDHelper::SaveHUD(string hud_filename)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	ZShadeXMLSaver::SaveHUDXML(hud_filename, env2D->GetHUDSystem());
}
//================================================================================================================
bool EnvironmentHUDHelper::ImageClicked(ZShadeSandboxGraphics::Image*& image, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetHUDSystem() == 0) return false;
	
	return env2D->GetHUDSystem()->ImageClicked(image, x, y);
}
//================================================================================================================
bool EnvironmentHUDHelper::MoveImageStart(ZShadeSandboxGraphics::Image*& image, int& imageID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetHUDSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Image* p_clicked_image;
	bool clicked = env2D->GetHUDSystem()->ImageClicked(p_clicked_image, x, y);
	if (clicked)
	{
		imageID = p_clicked_image->ID();
		
		// Get the actual image rather than clone it since we are moving it
		image = p_clicked_image;
	}
	
	return clicked;
}
//================================================================================================================
void EnvironmentHUDHelper::MoveImageEnd(ZShadeSandboxGraphics::Image*& image, int imageID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	// Create a new copy of the moved image
	ZShadeSandboxGraphics::Image* newCopy = image->Clone(env2D->GetGD2D()->m_hud_path, x, y);
	
	// Add the new image created to the hud
	EnvironmentHUDHelper::AddImage(newCopy);
	
	// Remove the old image from the hud
	EnvironmentHUDHelper::DeleteImage(imageID);
	
	// The old image has been removed so clear the memory
	image = 0;
}
//================================================================================================================
bool EnvironmentHUDHelper::CloneImageStart(ZShadeSandboxGraphics::Image*& image, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetHUDSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Image* p_clicked_image;
	bool clicked = env2D->GetHUDSystem()->ImageClicked(p_clicked_image, x, y);
	if (clicked)
	{
		image = p_clicked_image->Clone(env2D->GetGD2D()->m_hud_path, x, y);
	}
	
	return clicked;
}
//================================================================================================================
void EnvironmentHUDHelper::MoveImage(ZShadeSandboxGraphics::Image*& image, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	env2D->GetHUDSystem()->MoveImage(image, x, y);
}
//================================================================================================================
void EnvironmentHUDHelper::CloneImageEnd(ZShadeSandboxGraphics::Image*& image, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	// Create a new copy of the image
	ZShadeSandboxGraphics::Image* newCopy = image->Clone(env2D->GetGD2D()->m_hud_path, x, y);
	
	// Add the new image created to the hud
	EnvironmentHUDHelper::AddImage(newCopy);
	
	image = 0;
}
//================================================================================================================
void EnvironmentHUDHelper::AddImage(ZShadeSandboxGraphics::Image* image)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	env2D->GetHUDSystem()->AddImage(image);
}
//================================================================================================================
void EnvironmentHUDHelper::AddImage(string base_path, string textureName, int x, int y, int w, int h)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	env2D->GetHUDSystem()->AddImage(base_path, textureName, x, y, w, h);
}
//================================================================================================================
void EnvironmentHUDHelper::DeleteImage(ZShadeSandboxGraphics::Image* image)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	env2D->GetHUDSystem()->RemoveImage(image);
}
//================================================================================================================
void EnvironmentHUDHelper::DeleteImage(int id)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	env2D->GetHUDSystem()->RemoveImage(id);
}
//================================================================================================================
void EnvironmentHUDHelper::ResizeImageUpdate(
	ZShadeSandboxGraphics::Image*& image,
	bool updatePosition,
	bool updateSize,
	bool goFast,
	bool pressedLeftKey,
	bool pressedRightKey,
	bool pressedUpKey,
	bool pressedDownKey
)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	
}
//================================================================================================================
void EnvironmentHUDHelper::HighlightImage(ZShadeSandboxGraphics::Image*& imageWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetHUDSystem() == 0) return;
	
	ZShadeSandboxGraphics::Image* p_clicked_image;
	bool clicked = env2D->GetHUDSystem()->ImageClicked(p_clicked_image, x, y);
	
	if (clicked)
	{
		if (imageWithBox != 0)
		{
			if (imageWithBox != p_clicked_image)
			{
				highlightBox = 0;
				boxCreated = false;
			}
		}
		
		if (!boxCreated)
		{
			imageWithBox = p_clicked_image;
			
			string base_path = env2D->GetGD2D()->m_required_textures;
			base_path.append("\\");
			
			XMFLOAT3 position(imageWithBox->XPos(), imageWithBox->YPos(), 0);
			
			highlightBox = new Sprite(env2D->GetD3D());
			highlightBox->Initialize(
				base_path,
				"stamp_normal.png",
				position,
				imageWithBox->Width(),
				imageWithBox->Height(),
				STATIC
			);
			
			boxCreated = true;
		}
	}
	else
	{
		imageWithBox = 0;
		highlightBox = 0;
		boxCreated = false;
	}
}
//================================================================================================================
bool EnvironmentHUDHelper::TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	return env2D->GetMenuSystem()->TextClicked(text, x, y);
}
//================================================================================================================
bool EnvironmentHUDHelper::MoveTextStart(ZShadeSandboxGraphics::Text*& text, int& textID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Text* p_clicked_text;
	bool clicked = env2D->GetMenuSystem()->TextClicked(p_clicked_text, x, y);
	if (clicked)
	{
		textID = p_clicked_text->ID();
		
		// Get the actual text rather than clone it since we are moving the text
		text = p_clicked_text;
	}
	
	return clicked;
}
//================================================================================================================
void EnvironmentHUDHelper::MoveTextEnd(ZShadeSandboxGraphics::Text*& text, int textID, int x, int y)
{
	// Create a new copy of the moved text
	ZShadeSandboxGraphics::Text* newCopy = text->Clone(x, y);
	
	// Add the new text created to the menu
	EnvironmentHUDHelper::AddText(newCopy);
	
	// Remove the old text from the menu
	EnvironmentHUDHelper::DeleteText(textID);
	
	// The old text has been removed so clear the memory
	text = 0;
}
//================================================================================================================
bool EnvironmentHUDHelper::CloneTextStart(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Text* p_clicked_text;
	bool clicked = env2D->GetMenuSystem()->TextClicked(p_clicked_text, x, y);
	if (clicked)
	{
		text = p_clicked_text->Clone(x, y);
	}
	
	return clicked;
}
//================================================================================================================
void EnvironmentHUDHelper::MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->MoveText(text, x, y);
}
//================================================================================================================
void EnvironmentHUDHelper::CloneTextEnd(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	// Create a new copy of the moved text
	ZShadeSandboxGraphics::Text* newCopy = text->Clone(x, y);
	
	// Add the new text created to the menu
	EnvironmentHUDHelper::AddText(newCopy);
	
	text = 0;
}
//================================================================================================================
void EnvironmentHUDHelper::AddText(ZShadeSandboxGraphics::Text* text)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	env2D->GetMenuSystem()->AddText(env2D->ActiveMenuName(), text);
}
//================================================================================================================
void EnvironmentHUDHelper::AddText(string text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->AddText(text, x, y);
}
//================================================================================================================
void EnvironmentHUDHelper::DeleteText(ZShadeSandboxGraphics::Text* text)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->RemoveText(text);
}
//================================================================================================================
void EnvironmentHUDHelper::DeleteText(int id)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->RemoveText(id);
}
//================================================================================================================
void EnvironmentHUDHelper::HighlightText(ZShadeSandboxGraphics::Text*& textWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	ZShadeSandboxGraphics::Text* p_clicked_text;
	bool clicked = env2D->GetHUDSystem()->TextClicked(p_clicked_text, x, y);
	
	if (clicked)
	{
		if (textWithBox != 0)
		{
			if (textWithBox != p_clicked_text)
			{
				highlightBox = 0;
				boxCreated = false;
			}
		}
		
		if (!boxCreated)
		{
			textWithBox = p_clicked_text;
			
			string base_path = env2D->GetGD2D()->m_required_textures;
			base_path.append("\\");
			
			XMFLOAT3 position(textWithBox->GetXPos(), textWithBox->GetYPos(), 0);
			
			highlightBox = new Sprite(env2D->GetD3D());
			highlightBox->Initialize(
				base_path,
				"stamp_normal.png",
				position,
				textWithBox->Width(),
				textWithBox->Height(),
				STATIC
			);
			
			boxCreated = true;
		}
	}
	else
	{
		textWithBox = 0;
		highlightBox = 0;
		boxCreated = false;
	}
}
//================================================================================================================