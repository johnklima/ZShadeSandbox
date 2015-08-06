#include "InputXMLParser.h"
#include "Convert.h"
//================================================================================================================
//================================================================================================================
InputXMLParser::InputXMLParser()
:	XMLParser()
{
}
//================================================================================================================
InputXMLParser::InputXMLParser(string filename)
:	XMLParser(filename)
{
	Read();
}
//================================================================================================================
InputXMLParser::~InputXMLParser()
{
}
//================================================================================================================
void InputXMLParser::Read()
{
	if (!bOpened) return;
	
	const char* camSpeedFast_str = ReadElement("CamSpeedFast");
	const char* camSpeedSlow_str = ReadElement("CamSpeedSlow");
	
	fCamSpeedFast = atof(camSpeedFast_str);
	fCamSpeedSlow = atof(camSpeedSlow_str);
	
	ArrayElementXMLParser keysParser(Element());
	keysParser.ReadArrayElement("Keys", "KeyBind");
	while (keysParser.ArrayElementNotNull("KeyBind"))
	{
		string eName = keysParser.ReadArrayElementValue();
		string eIDAtt = keysParser.ReadArrayElementAttribute("id");
		int id = ZShadeSandboxGlobal::Convert::ConvertStringToT<int>(eIDAtt);

		const char* key_str = keysParser.ReadArrayChildElement("Key");
		const char* key_down_str = keysParser.ReadArrayChildElement("KeyDown");
		const char* has_left_shift_str = keysParser.ReadArrayChildElement("HasLeftShift");
		const char* has_left_ctrl_str = keysParser.ReadArrayChildElement("HasLeftCtrl");
		const char* command_str = keysParser.ReadArrayChildElement("Command");
		
		SKeyBind kb;
		
		kb.key = string(key_str);
		kb.keyDown = (key_down_str == "true") ? true : false;
		kb.hasLeftShift = (has_left_shift_str == "true") ? true : false;
		kb.hasLeftCtrl = (has_left_ctrl_str == "true") ? true : false;
		kb.command = string(command_str);
		
		mKeyBinds.push_back(kb);
	}
}
//================================================================================================================