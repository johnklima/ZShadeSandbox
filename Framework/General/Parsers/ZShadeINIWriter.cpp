//=========================================================================
// ZShadeINIWriter.h class

#include "ZShadeINIWriter.h"


ZShadeINIWriter::ZShadeINIWriter(std::string p_FilePath, bool editor)
{
    SetFile(p_FilePath, editor);
}

ZShadeINIWriter::~ZShadeINIWriter()
{

}

bool ZShadeINIWriter::SetFile(std::string p_FilePath, bool editor)
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

void ZShadeINIWriter::WriteInt(std::string p_SectionName, std::string p_KeyName, int ivalue)
{
	char szValue[255];
	sprintf(szValue, "%d", ivalue);
	WritePrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), szValue, m_FilePath.c_str());
}

void ZShadeINIWriter::WriteFloat(std::string p_SectionName, std::string p_KeyName, float fvalue)
{
	char szValue[255];
	sprintf(szValue, "%f", fvalue);
	WritePrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), szValue, m_FilePath.c_str());
}

void ZShadeINIWriter::WriteBoolean(std::string p_SectionName, std::string p_KeyName, bool bvalue)
{
	char szValue[255];
	sprintf(szValue, "%s", bvalue ? "True" : "False");
	WritePrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), szValue, m_FilePath.c_str());
}

void ZShadeINIWriter::WriteString(std::string p_SectionName, std::string p_KeyName, std::string svalue)
{
	WritePrivateProfileString(p_SectionName.c_str(), p_KeyName.c_str(), svalue.c_str(), m_FilePath.c_str());
}
