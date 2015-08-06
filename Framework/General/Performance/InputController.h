//================================================================================================================
// InputController
//
//================================================================================================================
// History
//
// Created on 6/23/2015 by Dustin Watson
//================================================================================================================
#ifndef __INPUTCONTROLLER_H
#define __INPUTCONTROLLER_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include <map>
#include <string>
#include "Input.h"

struct ActionBindKey
{
	Keyboard::Key key;
	bool keyDown;// If true then the action will happen when the key is held down else action will happen only one key press
	bool hasLeftShift;
	bool hasLeftCtrl;
};

// Could also bind input command name to lua meaning specify a key with a command name and the command name is used in lua

//================================================================================================================
//================================================================================================================
class InputController
{
	struct ActionBinder
	{
		std::string commandName;
		ActionBindKey* myKeyBind;
		void (*Command)(ActionBindKey key);
		void RunCommand(Keyboard* keyboard);
	};

public:

	InputController();
	~InputController();

	void AddActionBinding(std::string keyName, std::string commandName, bool keyDown, bool hasLeftShift, bool hasLeftCtrl, void (*Command)(ActionBindKey key));
	void AddActionBinding(Keyboard::Key key, std::string commandName, bool keyDown, bool hasLeftShift, bool hasLeftCtrl, void (*Command)(ActionBindKey key));

	std::string GetCommandName(std::string keyName);
	std::string GetCommandName(Keyboard::Key key);

	// Gets the binded function to the command
	ActionBinder* ReadActionBinder(std::string name, bool isKey);
	ActionBinder* ReadActionBinder(Keyboard::Key key);

	void RunCommand(std::string commandName, Keyboard* keyboard);
	void RunCommand(ActionBinder* binder, Keyboard* keyboard);
	
protected:

	// 1st string is the name of the key
	// 2nd value is the name of the command executed by the key with the address of the command
	std::map<std::string, ActionBinder*> mActionBinding;
};
//================================================================================================================
//================================================================================================================
#endif//__INPUTCONTROLLER_H