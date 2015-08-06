//******************************************************************************************************
// FileParser.h
//******************************************************************************************************
#ifndef __FILEPARSER_H
#define __FILEPARSER_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
class FileParser
{
public:
	FileParser();
	FileParser(const FileParser& other);
	~FileParser();

	int FindToken(char buffer[], bool alpdig);

	int GetCurrentToken() { return tData.CurrentFileToken; }
	void SetCurrentPos(int size) { tData.CurrentFilePos = size; }
	int GetCurrentPos() { return tData.CurrentFilePos; }
	void ResetCurrentPos() { tData.CurrentFilePos = 0; }

private:
	typedef struct tagTOKENDATA
	{
		//Searching token
		int CurrentFileToken;
		//Position of search
		int CurrentFilePos;
	} token_data;

	token_data tData;

	bool locateCertainToken(char token);
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__FILEPARSER_H