//===============================================================================================================================
// LuaKeyMapper
//
//===============================================================================================================================
// History
//
// -Created on 7/10/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __LUAKEYMAPPER_H
#define __LUAKEYMAPPER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <string>
#include <map>
#include "Input.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
#pragma region "LuaKeys"
static string luaKey[160] =
{
	"KEY_A",
	"KEY_ADD",
	"KEY_APPS",
	"KEY_ATTN",
	"KEY_B",
	"KEY_BACK",
	"KEY_BROWSERBACK",
	"KEY_BROWSERFAVORITES",
	"KEY_BROWSERFORWARD",
	"KEY_BROWSERHOME",
	"KEY_BROWSERREFRESH",
	"KEY_BROWSERSEARCH",
	"KEY_BROWSERSTOP",
	"KEY_C",
	"KEY_CAPSLOCK",
	"KEY_CHATPADGREEN",
	"KEY_CHATPADORANGE",
	"KEY_CRSEL",
	"KEY_D",
	"KEY_D0",
	"KEY_D1",
	"KEY_D2",
	"KEY_D3",
	"KEY_D4",
	"KEY_D5",
	"KEY_D6",
	"KEY_D7",
	"KEY_D8",
	"KEY_D9",
	"KEY_DECIMAL",
	"KEY_DELETE",
	"KEY_DIVIDE",
	"KEY_DOWN",
	"KEY_E",
	"KEY_END",
	"KEY_ENTER",
	"KEY_ERASEEOF",
	"KEY_ESCAPE",
	"KEY_EXECUTE",
	"KEY_EXSEL",
	"KEY_F",
	"KEY_F1",
	"KEY_F10",
	"KEY_F11",
	"KEY_F12",
	"KEY_F13",
	"KEY_F14",
	"KEY_F15",
	"KEY_F16",
	"KEY_F17",
	"KEY_F18",
	"KEY_F19",
	"KEY_F2",
	"KEY_F20",
	"KEY_F21",
	"KEY_F22",
	"KEY_F23",
	"KEY_F24",
	"KEY_F3",
	"KEY_F4",
	"KEY_F5",
	"KEY_F6",
	"KEY_F7",
	"KEY_F8",
	"KEY_F9",
	"KEY_G",
	"KEY_H",
	"KEY_HELP",
	"KEY_HOME",
	"KEY_I",
	"KEY_IMECONVERT",
	"KEY_IMENOCONVERT",
	"KEY_INSERT",
	"KEY_J",
	"KEY_K",
	"KEY_KANA",
	"KEY_KANJI",
	"KEY_L",
	"KEY_LAUNCHAPPLICATION1",
	"KEY_LAUNCHAPPLICATION2",
	"KEY_LAUCHMAIL",
	"KEY_LEFT",
	"KEY_LEFTALT",
	"KEY_LEFTCONTROL",
	"KEY_LEFTSHIFT",
	"KEY_LEFTWINDOWS",
	"KEY_M",
	"KEY_MEDIANEXTTRACK",
	"KEY_MEDIAPLAYPAUSE",
	"KEY_MEDIAPREVIOUSTRACK",
	"KEY_MEDIASTOP",
	"KEY_MULTIPLY",
	"KEY_N",
	"KEY_NONE",
	"KEY_NUMLOCK",
	"KEY_NUMPAD0",
	"KEY_NUMPAD1",
	"KEY_NUMPAD2",
	"KEY_NUMPAD3",
	"KEY_NUMPAD4",
	"KEY_NUMPAD5",
	"KEY_NUMPAD6",
	"KEY_NUMPAD7",
	"KEY_NUMPAD8",
	"KEY_NUMPAD9",
	"KEY_O",
	"KEY_OEM8",
	"KEY_OEMAUTO",
	"KEY_OEMBACKSLASH",
	"KEY_OEMCLEAR",
	"KEY_OEMCLOSEBRACKETS",
	"KEY_OEMCOMMA",
	"KEY_OEMCOPY",
	"KEY_OEMENLW",
	"KEY_OEMMINUS",
	"KEY_OEMOPENBRACKETS",
	"KEY_OEMPERIOD",
	"KEY_OEMPLUS",
	"KEY_OEMPIPE",
	"KEY_OEMQUESTION",
	"KEY_OEMQUOTES",
	"KEY_OEMSEMICOLON",
	"KEY_OEMTILDE",
	"KEY_P",
	"KEY_PA1",
	"KEY_PAGEDOWN",
	"KEY_PAGEUP",
	"KEY_PAUSE",
	"KEY_PLAY",
	"KEY_PRINT",
	"KEY_PRINTSCREEN",
	"KEY_PROCESSKEY",
	"KEY_Q",
	"KEY_R",
	"KEY_RIGHT",
	"KEY_RIGHTALT",
	"KEY_RIGHTCONTROL",
	"KEY_RIGHTSHIFT",
	"KEY_RIGHTWINDOWS",
	"KEY_S",
	"KEY_SCROLL",
	"KEY_SELECT",
	"KEY_SELECTMEDIA",
	"KEY_SEPARATOR",
	"KEY_SLEEP",
	"KEY_SPACE",
	"KEY_SUBTRACT",
	"KEY_T",
	"KEY_TAB",
	"KEY_U",
	"KEY_UP",
	"KEY_V",
	"KEY_VOLUMEDOWN",
	"KEY_VOLUMEMUTE",
	"KEY_VOLUMEUP",
	"KEY_W",
	"KEY_X",
	"KEY_Y",
	"KEY_Z",
	"KEY_ZOOM"
};
//===============================================================================================================================
#pragma endregion
//===============================================================================================================================
class LuaKeyMapper
{
public:
	LuaKeyMapper();
	LuaKeyMapper(const LuaKeyMapper& o);
	~LuaKeyMapper();

	Keyboard::Key get_key(string key);
	std::string get_key(Keyboard::Key key);

	static void NewInstance() { instance = new LuaKeyMapper(); }
	static LuaKeyMapper* GetInstance()
	{
		if (instance == NULL)
			instance = new LuaKeyMapper();
		return instance;
	}

private:
	static LuaKeyMapper* instance;

	map<string, Keyboard::Key> m_keys;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__LUAKEYMAPPER_H