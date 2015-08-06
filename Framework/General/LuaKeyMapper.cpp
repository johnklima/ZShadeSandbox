#include "LuaKeyMapper.h"
//===============================================================================================================================
//===============================================================================================================================
LuaKeyMapper* LuaKeyMapper::instance = 0;
//===============================================================================================================================
LuaKeyMapper::LuaKeyMapper()
{
	m_keys.insert(make_pair(luaKey[0], Keyboard::Key::A));
	m_keys.insert(make_pair(luaKey[1], Keyboard::Key::Add));
	m_keys.insert(make_pair(luaKey[2], Keyboard::Key::Apps));
	m_keys.insert(make_pair(luaKey[3], Keyboard::Key::Attn));
	m_keys.insert(make_pair(luaKey[4], Keyboard::Key::B));
	m_keys.insert(make_pair(luaKey[5], Keyboard::Key::Back));
	m_keys.insert(make_pair(luaKey[6], Keyboard::Key::BrowserBack));
	m_keys.insert(make_pair(luaKey[7], Keyboard::Key::BrowserFavorites));
	m_keys.insert(make_pair(luaKey[8], Keyboard::Key::BrowserForward));
	m_keys.insert(make_pair(luaKey[9], Keyboard::Key::BrowserHome));
	m_keys.insert(make_pair(luaKey[10], Keyboard::Key::BrowserRefresh));
	m_keys.insert(make_pair(luaKey[11], Keyboard::Key::BrowserSearch));
	m_keys.insert(make_pair(luaKey[12], Keyboard::Key::BrowserStop));
	m_keys.insert(make_pair(luaKey[13], Keyboard::Key::C));
	m_keys.insert(make_pair(luaKey[14], Keyboard::Key::CapsLock));
	m_keys.insert(make_pair(luaKey[15], Keyboard::Key::ChatPadGreen));
	m_keys.insert(make_pair(luaKey[16], Keyboard::Key::ChatPadOrange));
	m_keys.insert(make_pair(luaKey[17], Keyboard::Key::Crsel));
	m_keys.insert(make_pair(luaKey[18], Keyboard::Key::D));
	m_keys.insert(make_pair(luaKey[19], Keyboard::Key::D0));
	m_keys.insert(make_pair(luaKey[20], Keyboard::Key::D1));
	m_keys.insert(make_pair(luaKey[21], Keyboard::Key::D2));
	m_keys.insert(make_pair(luaKey[22], Keyboard::Key::D3));
	m_keys.insert(make_pair(luaKey[23], Keyboard::Key::D4));
	m_keys.insert(make_pair(luaKey[24], Keyboard::Key::D5));
	m_keys.insert(make_pair(luaKey[25], Keyboard::Key::D6));
	m_keys.insert(make_pair(luaKey[26], Keyboard::Key::D7));
	m_keys.insert(make_pair(luaKey[27], Keyboard::Key::D8));
	m_keys.insert(make_pair(luaKey[28], Keyboard::Key::D9));
	m_keys.insert(make_pair(luaKey[29], Keyboard::Key::Decimal));
	m_keys.insert(make_pair(luaKey[30], Keyboard::Key::Delete));
	m_keys.insert(make_pair(luaKey[31], Keyboard::Key::Divide));
	m_keys.insert(make_pair(luaKey[32], Keyboard::Key::Down));
	m_keys.insert(make_pair(luaKey[33], Keyboard::Key::E));
	m_keys.insert(make_pair(luaKey[34], Keyboard::Key::End));
	m_keys.insert(make_pair(luaKey[35], Keyboard::Key::Enter));
	m_keys.insert(make_pair(luaKey[36], Keyboard::Key::EraseEof));
	m_keys.insert(make_pair(luaKey[37], Keyboard::Key::Escape));
	m_keys.insert(make_pair(luaKey[38], Keyboard::Key::Execute));
	m_keys.insert(make_pair(luaKey[39], Keyboard::Key::Exsel));
	m_keys.insert(make_pair(luaKey[40], Keyboard::Key::F));
	m_keys.insert(make_pair(luaKey[41], Keyboard::Key::F1));
	m_keys.insert(make_pair(luaKey[42], Keyboard::Key::F10));
	m_keys.insert(make_pair(luaKey[43], Keyboard::Key::F11));
	m_keys.insert(make_pair(luaKey[44], Keyboard::Key::F12));
	m_keys.insert(make_pair(luaKey[45], Keyboard::Key::F13));
	m_keys.insert(make_pair(luaKey[46], Keyboard::Key::F14));
	m_keys.insert(make_pair(luaKey[47], Keyboard::Key::F15));
	m_keys.insert(make_pair(luaKey[48], Keyboard::Key::F16));
	m_keys.insert(make_pair(luaKey[49], Keyboard::Key::F17));
	m_keys.insert(make_pair(luaKey[50], Keyboard::Key::F18));
	m_keys.insert(make_pair(luaKey[51], Keyboard::Key::F19));
	m_keys.insert(make_pair(luaKey[52], Keyboard::Key::F2));
	m_keys.insert(make_pair(luaKey[53], Keyboard::Key::F20));
	m_keys.insert(make_pair(luaKey[54], Keyboard::Key::F21));
	m_keys.insert(make_pair(luaKey[55], Keyboard::Key::F22));
	m_keys.insert(make_pair(luaKey[56], Keyboard::Key::F23));
	m_keys.insert(make_pair(luaKey[57], Keyboard::Key::F24));
	m_keys.insert(make_pair(luaKey[58], Keyboard::Key::F3));
	m_keys.insert(make_pair(luaKey[59], Keyboard::Key::F4));
	m_keys.insert(make_pair(luaKey[60], Keyboard::Key::F5));
	m_keys.insert(make_pair(luaKey[61], Keyboard::Key::F6));
	m_keys.insert(make_pair(luaKey[62], Keyboard::Key::F7));
	m_keys.insert(make_pair(luaKey[63], Keyboard::Key::F8));
	m_keys.insert(make_pair(luaKey[64], Keyboard::Key::F9));
	m_keys.insert(make_pair(luaKey[65], Keyboard::Key::G));
	m_keys.insert(make_pair(luaKey[66], Keyboard::Key::H));
	m_keys.insert(make_pair(luaKey[67], Keyboard::Key::Help));
	m_keys.insert(make_pair(luaKey[68], Keyboard::Key::Home));
	m_keys.insert(make_pair(luaKey[69], Keyboard::Key::I));
	m_keys.insert(make_pair(luaKey[70], Keyboard::Key::ImeConvert));
	m_keys.insert(make_pair(luaKey[71], Keyboard::Key::ImeNoConvert));
	m_keys.insert(make_pair(luaKey[72], Keyboard::Key::Insert));
	m_keys.insert(make_pair(luaKey[73], Keyboard::Key::J));
	m_keys.insert(make_pair(luaKey[74], Keyboard::Key::K));
	m_keys.insert(make_pair(luaKey[75], Keyboard::Key::Kana));
	m_keys.insert(make_pair(luaKey[76], Keyboard::Key::Kanji));
	m_keys.insert(make_pair(luaKey[77], Keyboard::Key::L));
	m_keys.insert(make_pair(luaKey[78], Keyboard::Key::LaunchApplication1));
	m_keys.insert(make_pair(luaKey[79], Keyboard::Key::LaunchApplication2));
	m_keys.insert(make_pair(luaKey[80], Keyboard::Key::LaunchMail));
	m_keys.insert(make_pair(luaKey[81], Keyboard::Key::Left));
	m_keys.insert(make_pair(luaKey[82], Keyboard::Key::LeftAlt));
	m_keys.insert(make_pair(luaKey[83], Keyboard::Key::LeftControl));
	m_keys.insert(make_pair(luaKey[84], Keyboard::Key::LeftShift));
	m_keys.insert(make_pair(luaKey[85], Keyboard::Key::LeftWindows));
	m_keys.insert(make_pair(luaKey[86], Keyboard::Key::M));
	m_keys.insert(make_pair(luaKey[87], Keyboard::Key::MediaNextTrack));
	m_keys.insert(make_pair(luaKey[88], Keyboard::Key::MediaPlayPause));
	m_keys.insert(make_pair(luaKey[89], Keyboard::Key::MediaPreviousTrack));
	m_keys.insert(make_pair(luaKey[90], Keyboard::Key::MediaStop));
	m_keys.insert(make_pair(luaKey[91], Keyboard::Key::Multiply));
	m_keys.insert(make_pair(luaKey[92], Keyboard::Key::N));
	m_keys.insert(make_pair(luaKey[93], Keyboard::Key::None));
	m_keys.insert(make_pair(luaKey[94], Keyboard::Key::NumLock));
	m_keys.insert(make_pair(luaKey[95], Keyboard::Key::NumPad0));
	m_keys.insert(make_pair(luaKey[96], Keyboard::Key::NumPad1));
	m_keys.insert(make_pair(luaKey[97], Keyboard::Key::NumPad2));
	m_keys.insert(make_pair(luaKey[98], Keyboard::Key::NumPad3));
	m_keys.insert(make_pair(luaKey[99], Keyboard::Key::NumPad4));
	m_keys.insert(make_pair(luaKey[100], Keyboard::Key::NumPad5));
	m_keys.insert(make_pair(luaKey[101], Keyboard::Key::NumPad6));
	m_keys.insert(make_pair(luaKey[102], Keyboard::Key::NumPad7));
	m_keys.insert(make_pair(luaKey[103], Keyboard::Key::NumPad8));
	m_keys.insert(make_pair(luaKey[104], Keyboard::Key::NumPad9));
	m_keys.insert(make_pair(luaKey[105], Keyboard::Key::O));
	m_keys.insert(make_pair(luaKey[106], Keyboard::Key::Oem8));
	m_keys.insert(make_pair(luaKey[107], Keyboard::Key::OemAuto));
	m_keys.insert(make_pair(luaKey[108], Keyboard::Key::OemBackslash));
	m_keys.insert(make_pair(luaKey[109], Keyboard::Key::OemClear));
	m_keys.insert(make_pair(luaKey[110], Keyboard::Key::OemCloseBrackets));
	m_keys.insert(make_pair(luaKey[111], Keyboard::Key::OemComma));
	m_keys.insert(make_pair(luaKey[112], Keyboard::Key::OemCopy));
	m_keys.insert(make_pair(luaKey[113], Keyboard::Key::OemEnlW));
	m_keys.insert(make_pair(luaKey[114], Keyboard::Key::OemMinus));
	m_keys.insert(make_pair(luaKey[115], Keyboard::Key::OemOpenBrackets));
	m_keys.insert(make_pair(luaKey[116], Keyboard::Key::OemPeriod));
	m_keys.insert(make_pair(luaKey[117], Keyboard::Key::OemPipe));
	m_keys.insert(make_pair(luaKey[118], Keyboard::Key::OemPlus));
	m_keys.insert(make_pair(luaKey[119], Keyboard::Key::OemQuestion));
	m_keys.insert(make_pair(luaKey[120], Keyboard::Key::OemQuotes));
	m_keys.insert(make_pair(luaKey[121], Keyboard::Key::OemSemicolon));
	m_keys.insert(make_pair(luaKey[122], Keyboard::Key::OemTilde));
	m_keys.insert(make_pair(luaKey[123], Keyboard::Key::P));
	m_keys.insert(make_pair(luaKey[124], Keyboard::Key::Pa1));
	m_keys.insert(make_pair(luaKey[125], Keyboard::Key::PageDown));
	m_keys.insert(make_pair(luaKey[126], Keyboard::Key::PageUp));
	m_keys.insert(make_pair(luaKey[127], Keyboard::Key::Pause));
	m_keys.insert(make_pair(luaKey[128], Keyboard::Key::Play));
	m_keys.insert(make_pair(luaKey[129], Keyboard::Key::Print));
	m_keys.insert(make_pair(luaKey[130], Keyboard::Key::PrintScreen));
	m_keys.insert(make_pair(luaKey[131], Keyboard::Key::ProcessKey));
	m_keys.insert(make_pair(luaKey[132], Keyboard::Key::Q));
	m_keys.insert(make_pair(luaKey[133], Keyboard::Key::R));
	m_keys.insert(make_pair(luaKey[134], Keyboard::Key::Right));
	m_keys.insert(make_pair(luaKey[135], Keyboard::Key::RightAlt));
	m_keys.insert(make_pair(luaKey[136], Keyboard::Key::RightControl));
	m_keys.insert(make_pair(luaKey[137], Keyboard::Key::RightShift));
	m_keys.insert(make_pair(luaKey[138], Keyboard::Key::RightWindows));
	m_keys.insert(make_pair(luaKey[139], Keyboard::Key::S));
	m_keys.insert(make_pair(luaKey[140], Keyboard::Key::Scroll));
	m_keys.insert(make_pair(luaKey[141], Keyboard::Key::Select));
	m_keys.insert(make_pair(luaKey[142], Keyboard::Key::SelectMedia));
	m_keys.insert(make_pair(luaKey[143], Keyboard::Key::Separator));
	m_keys.insert(make_pair(luaKey[144], Keyboard::Key::Sleep));
	m_keys.insert(make_pair(luaKey[145], Keyboard::Key::Space));
	m_keys.insert(make_pair(luaKey[146], Keyboard::Key::Subtract));
	m_keys.insert(make_pair(luaKey[147], Keyboard::Key::T));
	m_keys.insert(make_pair(luaKey[148], Keyboard::Key::Tab));
	m_keys.insert(make_pair(luaKey[149], Keyboard::Key::U));
	m_keys.insert(make_pair(luaKey[150], Keyboard::Key::Up));
	m_keys.insert(make_pair(luaKey[151], Keyboard::Key::V));
	m_keys.insert(make_pair(luaKey[152], Keyboard::Key::VolumeDown));
	m_keys.insert(make_pair(luaKey[153], Keyboard::Key::VolumeMute));
	m_keys.insert(make_pair(luaKey[154], Keyboard::Key::VolumeUp));
	m_keys.insert(make_pair(luaKey[155], Keyboard::Key::W));
	m_keys.insert(make_pair(luaKey[156], Keyboard::Key::X));
	m_keys.insert(make_pair(luaKey[157], Keyboard::Key::Y));
	m_keys.insert(make_pair(luaKey[158], Keyboard::Key::Z));
	m_keys.insert(make_pair(luaKey[159], Keyboard::Key::Zoom));
}
//===============================================================================================================================
LuaKeyMapper::LuaKeyMapper(const LuaKeyMapper& o)
{
}
//===============================================================================================================================
LuaKeyMapper::~LuaKeyMapper()
{
}
//===============================================================================================================================
Keyboard::Key LuaKeyMapper::get_key(string key)
{
	return m_keys[ key ];
}
//===============================================================================================================================
std::string LuaKeyMapper::get_key(Keyboard::Key key)
{
	auto iter = m_keys.begin();
	for (; iter != m_keys.end(); iter++)
	{
		Keyboard::Key currKey = (*iter).second;
		
		if (currKey == key)
		{
			return (*iter).first;
		}
	}
	
	return "Nothing";
}
//===============================================================================================================================