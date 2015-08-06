//=========================================================================
// ZShadeINIParser.h class

#include "ZShadeINIParser.h"


ZShadeINIParser::ZShadeINIParser(std::string p_FilePath, bool editor)
{
    SetFile(p_FilePath, editor);
}

ZShadeINIParser::~ZShadeINIParser()
{

}

bool ZShadeINIParser::SetFile(std::string p_FilePath, bool editor)
{
	if (editor)
	{
#define MAX_PATH_LENGTH 1024

		char CurrentPath[MAX_PATH_LENGTH];
		//for( int i=0; i<MAX_PATH_LENGTH; i++ ) CurrentPath[i] = ' ';
		GetCurrentDirectory(MAX_PATH_LENGTH, CurrentPath);

		m_FilePath = CurrentPath;
		m_FilePath.append("/");
		m_FilePath.append(p_FilePath);
	}
	else
	{
		m_FilePath = p_FilePath;
	}
    return true;
}

int ZShadeINIParser::GetInt(std::string p_SectionName, std::string p_KeyName)
{
    return GetPrivateProfileIntA(p_SectionName.c_str(), p_KeyName.c_str(), 0, m_FilePath.c_str());
}

std::string ZShadeINIParser::GetString(std::string p_SectionName, std::string p_KeyName)
{
    char Value[INIBUFFERSIZE];
    GetPrivateProfileStringA(p_SectionName.c_str(), p_KeyName.c_str(), "", Value, INIBUFFERSIZE, m_FilePath.c_str());
    std::string ReturnedValue(Value);
    return ReturnedValue;
}

float ZShadeINIParser::GetFloat(std::string p_SectionName, std::string p_KeyName)
{
	char szResult[INIBUFFERSIZE];
	//char szDefault[255];
	float fltResult;
	//sprintf(szDefault, "%f", 0.0f);
	GetPrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), "", szResult, INIBUFFERSIZE, m_FilePath.c_str());
	fltResult =  atof(szResult);
	return fltResult;
}

bool ZShadeINIParser::GetBoolean(std::string p_SectionName, std::string p_KeyName)
{
	char szResult[INIBUFFERSIZE];
	//char szDefault[255];
	bool bolResult;
	//sprintf(szDefault, "%s", "false");
	GetPrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), "", szResult, INIBUFFERSIZE, m_FilePath.c_str());
	bolResult =  (strcmp(szResult, "True") == 0 || strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}

void ZShadeINIParser::WriteString(std::string p_SectionName, std::string p_KeyName, std::string Value)
{
	WritePrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), Value.c_str(), m_FilePath.c_str());
}