#include "InputController.h"
#include "LuaKeyMapper.h"
//================================================================================================================
//================================================================================================================
InputController::InputController()
{
}
//================================================================================================================
InputController::~InputController()
{
}
//================================================================================================================
void InputController::AddActionBinding(std::string keyName, std::string commandName, bool keyDown, bool hasLeftShift, bool hasLeftCtrl, void(*Command)(ActionBindKey key))
{
	ActionBindKey* abk = new ActionBindKey();
	abk->key = LuaKeyMapper::GetInstance()->get_key(keyName);
	abk->keyDown = keyDown;
	abk->hasLeftShift = hasLeftShift;
	abk->hasLeftCtrl = hasLeftCtrl;
	
	ActionBinder* ab = new ActionBinder();
	ab->commandName = commandName;
	ab->Command = Command;
	ab->myKeyBind = abk;

	mActionBinding.insert(make_pair(keyName, ab));
}
//================================================================================================================
void InputController::AddActionBinding(Keyboard::Key key, std::string commandName, bool keyDown, bool hasLeftShift, bool hasLeftCtrl, void (*Command)(ActionBindKey key))
{
	string keyName = LuaKeyMapper::GetInstance()->get_key(key);
	
	if (keyName == "Nothing") return;
	
	ActionBindKey* abk = new ActionBindKey();
	abk->key = key;
	abk->keyDown = keyDown;
	abk->hasLeftShift = hasLeftShift;
	abk->hasLeftCtrl = hasLeftCtrl;
	
	ActionBinder* ab = new ActionBinder();
	ab->commandName = commandName;
	ab->Command = Command;
	ab->myKeyBind = abk;
	
	mActionBinding.insert(make_pair(keyName, ab));
}
//================================================================================================================
std::string InputController::GetCommandName(std::string keyName)
{
	auto iter = (mActionBinding.find(keyName));
	return (*iter).second->commandName;
}
//================================================================================================================
std::string InputController::GetCommandName(Keyboard::Key key)
{
	string keyName = LuaKeyMapper::GetInstance()->get_key(key);

	if (keyName == "Nothing") return "Nothing";

	auto iter = (mActionBinding.find(keyName));
	return (*iter).second->commandName;
}
//================================================================================================================
InputController::ActionBinder* InputController::ReadActionBinder(std::string name, bool isKey)
{
	if (isKey)
	{
		auto iter = (mActionBinding.find(name));
		return (*iter).second;
	}
	else
	{
		auto iter = mActionBinding.begin();
		for (; iter != mActionBinding.end(); iter++)
		{
			if ((*iter).second->commandName == name)
			{
				return (*iter).second;
			}
		}
	}

	return 0;
}
//================================================================================================================
InputController::ActionBinder* InputController::ReadActionBinder(Keyboard::Key key)
{
	string keyName = LuaKeyMapper::GetInstance()->get_key(key);

	if (keyName == "Nothing") return 0;

	auto iter = (mActionBinding.find(keyName));
	return (*iter).second;
}
//================================================================================================================
void InputController::ActionBinder::RunCommand(Keyboard* keyboard)
{
	if (myKeyBind->keyDown)
	{
		if (myKeyBind->hasLeftShift)
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift))
			{
				if(keyboard->IsKeyDown(myKeyBind->key))
				{
					// Shift + Key Down
					Command(*myKeyBind);
				}
			}
		}
		else
		{
			if (myKeyBind->hasLeftCtrl)
			{
				if (keyboard->IsKeyDown(Keyboard::Key::LeftControl))
				{
					if (keyboard->IsKeyDown(myKeyBind->key))
					{
						// Ctrl + Key Down
						Command(*myKeyBind);
					}
				}
			}
			else
			{
				if (keyboard->IsKeyDown(myKeyBind->key))
				{
					// Key Down
					Command(*myKeyBind);
				}
			}
		}
	}
	else
	{
		if (myKeyBind->hasLeftShift)
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift))
			{
				if (keyboard->RisingEdge(myKeyBind->key))
				{
					// Shift + Key Press
					Command(*myKeyBind);
				}
			}
		}
		else
		{
			if (myKeyBind->hasLeftCtrl)
			{
				if (keyboard->IsKeyDown(Keyboard::Key::LeftControl))
				{
					if (keyboard->RisingEdge(myKeyBind->key))
					{
						// Ctrl + Key Press
						Command(*myKeyBind);
					}
				}
			}
			else
			{
				if (keyboard->RisingEdge(myKeyBind->key))
				{
					// Key Press
					Command(*myKeyBind);
				}
			}
		}
	}
}
//================================================================================================================
void InputController::RunCommand(std::string commandName, Keyboard* keyboard)
{
	auto iter = mActionBinding.begin();
	for (; iter != mActionBinding.end(); iter++)
	{
		if ((*iter).second->commandName == commandName)
		{
			(*iter).second->RunCommand(keyboard);
		}
	}
}
//================================================================================================================
void InputController::RunCommand(InputController::ActionBinder* binder, Keyboard* keyboard)
{
	binder->RunCommand(keyboard);
}
//================================================================================================================