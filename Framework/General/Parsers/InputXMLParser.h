//================================================================================================================
// InputXMLParser
//
//================================================================================================================
// History
//
// Created on 6/23/2015 by Dustin Watson
//================================================================================================================
#ifndef __INPUTXMLPARSER_H_
#define __INPUTXMLPARSER_H_
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "XMLParser.h"

struct SKeyBind
{
	string key;
	bool keyDown;
	bool hasLeftShift;
	bool hasLeftCtrl;
	string command;
};

//================================================================================================================
//================================================================================================================
class InputXMLParser : public XMLParser
{
	float fCamSpeedFast;
	float fCamSpeedSlow;
	vector<SKeyBind> mKeyBinds;
	
public:
	
	InputXMLParser();
	InputXMLParser(string filename);
	~InputXMLParser();
	
	void Read();
	
	float CamSpeedFast() { return fCamSpeedFast; }
	float CamSpeedSlow() { return fCamSpeedSlow; }
	
	int KeyBindSize() { return mKeyBinds.size(); }
	SKeyBind GetKeyBind(int i) { return mKeyBinds[i]; }
};
//================================================================================================================
//================================================================================================================
#endif//__INPUTXMLPARSER_H_