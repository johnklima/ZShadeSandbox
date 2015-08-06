//================================================================================================================
// DefaultInputController3D
//
// Builds default commands used in the engine and shows how to use the input controller
//================================================================================================================
// History
//
// Created on 6/23/2015 by Dustin Watson
//================================================================================================================
#ifndef __DEFAULTINPUTCONTROLLER3D_H
#define __DEFAULTINPUTCONTROLLER3D_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "InputController.h"
#include "Camera.h"

//================================================================================================================
//================================================================================================================
class DefaultInputController3D : public InputController
{
public:
	
	DefaultInputController3D();
	~DefaultInputController3D();
	
	void UpdateController(Keyboard* keyboard);
	
	void BuildCommands(
		void (*MoveForward)(ActionBindKey key),
		void (*MoveBackward)(ActionBindKey key),
		void (*StrafeLeft)(ActionBindKey key),
		void (*StrafeRight)(ActionBindKey key)
	);
	
	float CamSpeedFast() { return fCamSpeedFast; }
	float CamSpeedSlow() { return fCamSpeedSlow; }
	
	// Default commands to be ran
private:
	
	float fCamSpeedFast;
	float fCamSpeedSlow;
	
	// Execute the commands as an array
	vector<string> mCommandNames;
};
//================================================================================================================
//================================================================================================================
#endif//__DEFAULTINPUTCONTROLLER3D_H