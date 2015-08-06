#include "DefaultInputController3D.h"
#include "InputXMLParser.h"
//================================================================================================================
//================================================================================================================
DefaultInputController3D::DefaultInputController3D()
:	InputController()
{
}
//================================================================================================================
DefaultInputController3D::~DefaultInputController3D()
{
}
//================================================================================================================
void DefaultInputController3D::BuildCommands(
	void (*MoveForward)(ActionBindKey key),
	void (*MoveBackward)(ActionBindKey key),
	void (*StrafeLeft)(ActionBindKey key),
	void (*StrafeRight)(ActionBindKey key)
)
{
	// Read the input configuration
	InputXMLParser parser;
	parser.Open("Input.xml");
	parser.Read();
	
	// Now build the input controller
	fCamSpeedFast = parser.CamSpeedFast();
	fCamSpeedSlow = parser.CamSpeedSlow();
	
	for (int i = 0; i < parser.KeyBindSize(); i++)
	{
		mCommandNames.push_back(parser.GetKeyBind(i).command);
	}
	
	AddActionBinding(
		parser.GetKeyBind(0).key,
		parser.GetKeyBind(0).command,
		parser.GetKeyBind(0).keyDown,
		parser.GetKeyBind(0).hasLeftShift,
		parser.GetKeyBind(0).hasLeftCtrl,
		MoveForward
	);
	
	AddActionBinding(
		parser.GetKeyBind(1).key,
		parser.GetKeyBind(1).command,
		parser.GetKeyBind(1).keyDown,
		parser.GetKeyBind(1).hasLeftShift,
		parser.GetKeyBind(1).hasLeftCtrl,
		MoveBackward
	);
	
	AddActionBinding(
		parser.GetKeyBind(2).key,
		parser.GetKeyBind(2).command,
		parser.GetKeyBind(2).keyDown,
		parser.GetKeyBind(2).hasLeftShift,
		parser.GetKeyBind(2).hasLeftCtrl,
		StrafeLeft
	);
	
	AddActionBinding(
		parser.GetKeyBind(3).key,
		parser.GetKeyBind(3).command,
		parser.GetKeyBind(3).keyDown,
		parser.GetKeyBind(3).hasLeftShift,
		parser.GetKeyBind(3).hasLeftCtrl,
		StrafeRight
	);
}
//================================================================================================================
void DefaultInputController3D::UpdateController(Keyboard* keyboard)
{
	for (int i = 0; i < mCommandNames.size(); i++)
	{
		RunCommand(mCommandNames[i], keyboard);
	}
}
//================================================================================================================