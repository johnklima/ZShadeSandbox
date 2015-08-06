#include "EnvironmentMenuHelper.h"
#include "Convert.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "Environment2D.h"
#include "Environment.h"
//================================================================================================================
//================================================================================================================
EnvironmentMenuHelper::EnvironmentMenuHelper()
{
}
//================================================================================================================
EnvironmentMenuHelper::~EnvironmentMenuHelper()
{
}
//================================================================================================================
void EnvironmentMenuHelper::AddBackground(ZShadeSandboxGraphics::Image* image)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->AddBackground(env2D->ActiveMenuName(), image);
}
//================================================================================================================
void EnvironmentMenuHelper::CreateMenu(string menu_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	ZShadeSandboxEnvironment::Menu* m = new ZShadeSandboxEnvironment::Menu(env2D->GetD3D());
	m->MenuName() = menu_name;
	env2D->GetMenuSystem()->AddMenu(m);
}
//================================================================================================================
void EnvironmentMenuHelper::GrabMenuNames(vector<string>& names)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	env2D->GetMenuSystem()->GetMenuNames(names);
}
//================================================================================================================
void EnvironmentMenuHelper::LoadAMenu(string menu_filename, string& menuName)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	ZShadeXMLLoader::LoadMenuXML(menu_filename, env2D->GetGD2D(), env2D->GetEngineOptions(), env2D->GetD3D(), env2D->GetMenuSystem());
}
//================================================================================================================
void EnvironmentMenuHelper::SaveMenu(string menu_filename)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	ZShadeXMLSaver::SaveMenuXML(menu_filename, env2D->GetMenuSystem());
}
//================================================================================================================
void EnvironmentMenuHelper::SetActiveRenderedMenu(string menu_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	env2D->GetMenuSystem()->SetMenuToRender(menu_name);
}
//================================================================================================================
void EnvironmentMenuHelper::RemoveMenu(string menu_name)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	env2D->GetMenuSystem()->RemoveMenu(menu_name);
}
//================================================================================================================
bool EnvironmentMenuHelper::ButtonClicked(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	return env2D->GetMenuSystem()->ButtonClicked(button, x, y);
}
//================================================================================================================
bool EnvironmentMenuHelper::MoveButtonStart(ZShadeSandboxGraphics::Button*& button, int& buttonID, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Button* p_clicked_button;
	bool clicked = env2D->GetMenuSystem()->ButtonClicked(p_clicked_button, x, y);
	if (clicked)
	{
		buttonID = p_clicked_button->ID();
		
		// Get the actual button rather than clone it since we are moving it
		button = p_clicked_button;
	}
	
	return clicked;
}
//================================================================================================================
void EnvironmentMenuHelper::MoveButtonEnd(ZShadeSandboxGraphics::Button*& button, int buttonID, int x, int y)
{
	// Create a new copy of the moved button
	ZShadeSandboxGraphics::Button* newCopy = button->Clone(x, y);
	
	// Add the new button created to the menu
	EnvironmentMenuHelper::AddButton(newCopy);
	
	// Remove the old button from the menu
	EnvironmentMenuHelper::DeleteButton(buttonID);
	
	// The old button has been removed so clear the memory
	button = 0;
}
//================================================================================================================
bool EnvironmentMenuHelper::CloneButtonStart(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	ZShadeSandboxGraphics::Button* p_clicked_button;
	bool clicked = env2D->GetMenuSystem()->ButtonClicked(p_clicked_button, x, y);
	if (clicked)
	{
		button = p_clicked_button->Clone(x, y);
	}
	
	return clicked;
}
//================================================================================================================
void EnvironmentMenuHelper::MoveButton(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->MoveButton(button, x, y);
}
//================================================================================================================
void EnvironmentMenuHelper::CloneButtonEnd(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	// Create a new copy of the button
	ZShadeSandboxGraphics::Button* newCopy = button->Clone(x, y);
	
	// Add the new button created to the menu
	EnvironmentMenuHelper::AddButton(newCopy);
	
	button = 0;
}
//================================================================================================================
void EnvironmentMenuHelper::AddButton(ZShadeSandboxGraphics::Button* button)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	env2D->GetMenuSystem()->AddButton(env2D->ActiveMenuName(), button);
}
//================================================================================================================
void EnvironmentMenuHelper::AddButton(
	string base_path,
	string normalImageName,
	string pushedImageName,
	string highlightedImageName,
	string disabledImageName,
	int x,
	int y
)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->AddButton(base_path, normalImageName, pushedImageName, highlightedImageName, disabledImageName, x, y);
}
//================================================================================================================
void EnvironmentMenuHelper::DeleteButton(ZShadeSandboxGraphics::Button* button)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->RemoveButton(button);
}
//================================================================================================================
void EnvironmentMenuHelper::DeleteButton(int id)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->RemoveButton(id);
}
//================================================================================================================
void EnvironmentMenuHelper::ResizeButtonUpdate(
	ZShadeSandboxGraphics::Button*& button,
	bool updatePosition,
	bool updateSize,
	bool goFast,
	bool pressedLeftKey,
	bool pressedRightKey,
	bool pressedUpKey,
	bool pressedDownKey
)
{
	
}
//================================================================================================================
void EnvironmentMenuHelper::HighlightButton(ZShadeSandboxGraphics::Button*& buttonWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	ZShadeSandboxGraphics::Button* p_clicked_button;
	bool clicked = ButtonClicked(p_clicked_button, x, y);
	
	if (clicked)
	{
		if (buttonWithBox != 0)
		{
			if (buttonWithBox != p_clicked_button)
			{
				highlightBox = 0;
				boxCreated = false;
			}
		}
		
		if (!boxCreated)
		{
			buttonWithBox = p_clicked_button;
			
			string base_path = env2D->GetGD2D()->m_required_textures;
			base_path.append("\\");
			
			XMFLOAT3 position(buttonWithBox->XPos(), buttonWithBox->YPos(), 0);
			
			highlightBox = new Sprite(env2D->GetD3D());
			highlightBox->Initialize(
				base_path,
				"stamp_normal.png",
				position,
				buttonWithBox->Width(),
				buttonWithBox->Height(),
				STATIC
			);
			
			boxCreated = true;
		}
	}
	else
	{
		buttonWithBox = 0;
		highlightBox = 0;
		boxCreated = false;
	}
}
//================================================================================================================
bool EnvironmentMenuHelper::TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();
	
	// Did not find an environment
	if (env2D == 0) return false;
	
	if (env2D->GetMenuSystem() == 0) return false;
	
	return env2D->GetMenuSystem()->TextClicked(text, x, y);
}
//================================================================================================================
bool EnvironmentMenuHelper::MoveTextStart(ZShadeSandboxGraphics::Text*& text, int& textID, int x, int y)
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
void EnvironmentMenuHelper::MoveTextEnd(ZShadeSandboxGraphics::Text*& text, int textID, int x, int y)
{
	// Create a new copy of the moved text
	ZShadeSandboxGraphics::Text* newCopy = text->Clone(x, y);
	
	// Add the new text created to the menu
	EnvironmentMenuHelper::AddText(newCopy);
	
	// Remove the old text from the menu
	EnvironmentMenuHelper::DeleteText(textID);
	
	// The old text has been removed so clear the memory
	text = 0;
}
//================================================================================================================
bool EnvironmentMenuHelper::CloneTextStart(ZShadeSandboxGraphics::Text*& text, int x, int y)
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
void EnvironmentMenuHelper::MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->MoveText(text, x, y);
}
//================================================================================================================
void EnvironmentMenuHelper::CloneTextEnd(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	// Create a new copy of the moved text
	ZShadeSandboxGraphics::Text* newCopy = text->Clone(x, y);
	
	// Add the new text created to the menu
	EnvironmentMenuHelper::AddText(newCopy);
	
	text = 0;
}
//================================================================================================================
void EnvironmentMenuHelper::AddText(ZShadeSandboxGraphics::Text* text)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;

	env2D->GetMenuSystem()->AddText(env2D->ActiveMenuName(), text);
}
//================================================================================================================
void EnvironmentMenuHelper::AddText(string text, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->AddText(text, x, y);
}
//================================================================================================================
void EnvironmentMenuHelper::DeleteText(ZShadeSandboxGraphics::Text* text)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->RemoveText(text);
}
//================================================================================================================
void EnvironmentMenuHelper::DeleteText(int id)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	env2D->GetMenuSystem()->RemoveText(id);
}
//================================================================================================================
void EnvironmentMenuHelper::HighlightText(ZShadeSandboxGraphics::Text*& textWithBox, Sprite*& highlightBox, bool& boxCreated, int x, int y)
{
	Environment2D* env2D = ZShadeSandboxEnvironment::Environment::Env2D();

	// Did not find an environment
	if (env2D == 0) return;
	
	if (env2D->GetMenuSystem() == 0) return;
	
	ZShadeSandboxGraphics::Text* p_clicked_text;
	bool clicked = TextClicked(p_clicked_text, x, y);
	
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