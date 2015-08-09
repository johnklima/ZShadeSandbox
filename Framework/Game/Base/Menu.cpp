#include "Menu.h"
#include "Convert.h"
#include "EnvironmentMenuHelper.h"
//================================================================================================================
//================================================================================================================
ZShadeSandboxEnvironment::Menu::Menu(D3D* d3d)
:	mD3D(d3d)
,	m_background(NULL)
,	mButtonID(0)
,	mTextID(0)
{
}
//================================================================================================================
ZShadeSandboxEnvironment::Menu::~Menu()
{
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::RemoveButton(ZShadeSandboxGraphics::Button* b)
{
	ZShadeSandboxGraphics::Button* button = 0;
	auto current = m_menu_buttons.begin();
	while (current != m_menu_buttons.end())
	{
		button = (*current);
		if (button != 0 && button->ID() == b->ID())
		{
			current = m_menu_buttons.erase(current);
			mButtonID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		if (m_menu_buttons[i] == 0) continue;
		
		m_menu_buttons[i]->ID() = i;
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::RemoveText(ZShadeSandboxGraphics::Text* t)
{
	ZShadeSandboxGraphics::Text* text = 0;
	auto current = m_menu_text.begin();
	while (current != m_menu_text.end())
	{
		text = (*current);
		if (text != 0 && text->ID() == t->ID())
		{
			current = m_menu_text.erase(current);
			mTextID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		if (m_menu_text[i] == 0) continue;
		
		m_menu_text[i]->ID() = i;
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::RemoveButton(int id)
{
	ZShadeSandboxGraphics::Button* button = 0;
	auto current = m_menu_buttons.begin();
	while (current != m_menu_buttons.end())
	{
		button = (*current);
		if (button != 0 && button->ID() == id)
		{
			current = m_menu_buttons.erase(current);
			mButtonID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		if (m_menu_buttons[i] == 0) continue;
		
		m_menu_buttons[i]->ID() = i;
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::RemoveText(int id)
{
	ZShadeSandboxGraphics::Text* text = 0;
	auto current = m_menu_text.begin();
	while (current != m_menu_text.end())
	{
		text = (*current);
		if (text != 0 && text->ID() == id)
		{
			current = m_menu_text.erase(current);
			mTextID--;
		}
		else
		{
			++current;
		}
	}
	
	// Retractor the ids
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		if (m_menu_text[i] == 0) continue;
		
		m_menu_text[i]->ID() = i;
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::AddMenuNameToButton(string menu_name, int buttonID)
{
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* b = m_menu_buttons[i];
		
		if (b == 0) continue;
		
		if (b->ID() == buttonID)
		{
			b->Tag() = menu_name;
			
			break;
		}
	}
}
//================================================================================================================
bool ZShadeSandboxEnvironment::Menu::ButtonClicked(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* b = m_menu_buttons[i];

		if (b != NULL)
		{
			if (x >= b->XPos() && x <= b->XPos() + b->Width())
			{
				if (y >= b->YPos() && y <= b->YPos() + b->Height())
				{
					button = b;
					return true;
				}
			}
		}
	}

	return false;
}
//================================================================================================================
bool ZShadeSandboxEnvironment::Menu::TextClicked(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = m_menu_text[i];
		
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
void ZShadeSandboxEnvironment::Menu::MoveButton(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* b = m_menu_buttons[i];
		
		if (b == 0) continue;
		
		if (b == button)
		{
			b->MoveButtonX(x);
			b->MoveButtonY(y);
			return;
		}
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::MoveText(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = m_menu_text[i];
		
		if (t == 0) continue;
		
		if (t == text)
		{
			t->TextTransform() = XMMatrixTranslation(x, y, 0);
			return;
		}
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::AddBackground(ZShadeSandboxGraphics::Image* image)
{
	m_background = image;
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::AddButton(
	string base_path
,	string normalImageName
,	string pushedImageName
,	string highlightedImageName
,	string disabledImageName
,	int x
,	int y
)
{
	ZShadeSandboxGraphics::Button* newButton = new ZShadeSandboxGraphics::Button(
		mD3D,
		"",
		false,
		x,
		y,
		80,
		30,
		base_path,
		normalImageName,
		pushedImageName,
		highlightedImageName,
		disabledImageName,
		"blue.png",
		"red.png",
		"yellow.png",
		"black.png"
	);
	
	AddButton(newButton);
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::AddText(string text, int x, int y)
{
	ZShadeSandboxGraphics::Text* newText = new ZShadeSandboxGraphics::Text();
	newText->SetD3D(mD3D);
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
void ZShadeSandboxEnvironment::Menu::AddButton(ZShadeSandboxGraphics::Button* button)
{
	button->ID() = mButtonID;
	m_menu_buttons.push_back( button );
	mButtonID++;
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::AddText(ZShadeSandboxGraphics::Text* text)
{
	text->ID() = mTextID;
	m_menu_text.push_back( text );
	mTextID++;
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::UpdateButtonMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		for (int i = 0; i < m_menu_buttons.size(); i++)
		{
			ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

			if (button != NULL)
			{
				button->UpdateButtonMouseUp(btnState, x, y);

				if (button->WasLastStateUP())
				{
					button->ResetUPState();
					
					//// If this button has a menu tag then switch to that menu
					//if (button->Tag() != "NONE" && button->Tag() != "")
					//{
					//	EnvironmentMenuHelper::SetActiveRenderedMenu(button->Tag());
					//}
					//
					////Need to have the buttons do something
					////Should make this happen from a .lua script
					//switch (button->ScriptType())
					//{
					//	case ZShadeSandboxGraphics::EScriptType::eStart:
					//	case ZShadeSandboxGraphics::EScriptType::eContinue:
					//	case ZShadeSandboxGraphics::EScriptType::eOptions:
					//	case ZShadeSandboxGraphics::EScriptType::eExit:
					//	{
					//		if (button->GetScript() != NULL)
					//			button->GetScript()->Main();
					//	}
					//	break;
					//	default: break;
					//}
				}
			}
		}
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::UpdateButtonMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		for (int i = 0; i < m_menu_buttons.size(); i++)
		{
			ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

			if (button != NULL)
			{
				button->UpdateButtonMouseDown(btnState, x, y);

				if (ButtonClicked(button, x, y))
				{
					// If this button has a menu tag then switch to that menu
					if (button->Tag() != "NONE" && button->Tag() != "")
					{
						EnvironmentMenuHelper::SetActiveRenderedMenu(button->Tag());
					}

					//Need to have the buttons do something
					//Should make this happen from a .lua script
					switch (button->ScriptType())
					{
						case ZShadeSandboxGraphics::EScriptType::eStart:
						case ZShadeSandboxGraphics::EScriptType::eContinue:
						case ZShadeSandboxGraphics::EScriptType::eOptions:
						case ZShadeSandboxGraphics::EScriptType::eExit:
						{
							if (button->GetScript() != NULL)
							{
								button->GetScript()->Main();
							}
						}
						break;
						default: break;
					}
				}
			}
		}
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::UpdateButtonMouseOn(WPARAM btnState, int x, int y)
{
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

		if (button != NULL)
		{
			button->UpdateButtonMouseOn(btnState, x, y);
		}
	}
}
//================================================================================================================
void ZShadeSandboxEnvironment::Menu::UpdateButtonMouseMove(WPARAM btnState, int x, int y)
{
	// Move the selected item

}
//================================================================================================================
bool ZShadeSandboxEnvironment::Menu::Render(Camera* camera, int blendAmount)
{
	//Render the background sprite
	if (m_background != NULL)
	{
		mD3D->TurnOnAlphaBlending();
		{
			m_background->Draw(camera, blendAmount);
		}
		mD3D->TurnOffAlphaBlending();
	}
	
	//Render all the menu buttons
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

		if (button != NULL)
		{
			mD3D->TurnOnAlphaBlending();
			{
				button->Draw(camera, blendAmount);
			}
			mD3D->TurnOffAlphaBlending();
		}
	}
	
	//Render all the menu text
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* text = m_menu_text[i];
		
		if (text != NULL)
		{
			text->Render11();
		}
	}

	return true;
}
//================================================================================================================