//=========================================================================
// ZShadeINIParser.h class
// http://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File

#ifndef __ZSHADEINIPARSER_H_
#define __ZSHADEINIPARSER_H_

#pragma once

#include <windows.h>
#include <string>
using namespace std;

#define INIBUFFERSIZE 1024

class ZShadeINIParser
{
public:
	bool SetFile(std::string p_FilePath, bool editor);
    int GetInt(std::string p_SectionName, std::string p_KeyName);
    float GetFloat(std::string p_SectionName, std::string p_KeyName);
	bool GetBoolean(std::string p_SectionName, std::string p_KeyName);
	string GetString(std::string p_SectionName, std::string p_KeyName);
	string GetFilePath() { return m_FilePath; }
	
	void WriteString(std::string p_SectionName, std::string p_KeyName, std::string Value);

    ZShadeINIParser(std::string p_FilePath, bool editor);
    virtual ~ZShadeINIParser();

private:
    std::string m_FilePath;
};

#endif /* __ZSHADEINIPARSER_H_ */
