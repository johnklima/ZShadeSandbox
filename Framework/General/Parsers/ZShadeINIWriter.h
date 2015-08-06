//=========================================================================
// ZShadeINIWriter.h class

#ifndef __ZSHADEINIWRITER_H_
#define __ZSHADEINIWRITER_H_

#pragma once

#include <windows.h>
#include <string>
using namespace std;

class ZShadeINIWriter
{
public:
	bool SetFile(std::string p_FilePath, bool editor);
	void WriteInt(std::string p_SectionName, std::string p_KeyName, int ivalue);
	void WriteFloat(std::string p_SectionName, std::string p_KeyName, float fvalue);
	void WriteBoolean(std::string p_SectionName, std::string p_KeyName, bool bvalue);
	void WriteString(std::string p_SectionName, std::string p_KeyName, std::string svalue);
	std::string GetFilePath() { return m_FilePath; }
	
    ZShadeINIWriter(std::string p_FilePath, bool editor);
    virtual ~ZShadeINIWriter();

private:
    std::string m_FilePath;
};

#endif /* __ZSHADEINIWRITER_H_ */
