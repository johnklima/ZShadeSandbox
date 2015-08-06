#include "FileParser.h"
#include "GlobalTokens.h"
#include <cctype>
//-----------------------------------------------------------------------------------------------------------------
FileParser::FileParser()
{
	tData.CurrentFilePos = 0;
	tData.CurrentFileToken = -1;
}
//-----------------------------------------------------------------------------------------------------------------
FileParser::FileParser(const FileParser& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
FileParser::~FileParser()
{
}
//-----------------------------------------------------------------------------------------------------------------
int FileParser::FindToken(char buffer[], bool alpdig)
{
	if (locateCertainToken(buffer[tData.CurrentFilePos]))
	{
		//We found a token
		tData.CurrentFilePos++;
		return tData.CurrentFileToken;
	}

	if (alpdig)
	{
		if (isalpha(buffer[tData.CurrentFilePos]))
		{
			tData.CurrentFilePos++;
			return ALPHA;
		}

		if (isdigit(buffer[tData.CurrentFilePos]))
		{
			tData.CurrentFilePos++;
			return DIGIT;
		}
	}

	//No Token Found
	return -1;
}
//-----------------------------------------------------------------------------------------------------------------
bool FileParser::locateCertainToken(char bufferSymbol)
{
	switch( bufferSymbol )
	{
		case MinusSign:
			tData.CurrentFileToken = DEC;
			return true;
			break;
		case PlusSign:
			tData.CurrentFileToken = INC;
			return true;
			break;
		case TimesSign:
			tData.CurrentFileToken = TIMES;
			return true;
			break;
		case EqualsSign:
			tData.CurrentFileToken = EQUAL;
			return true;
			break;
		case QuoteSign:
			tData.CurrentFileToken = QUOTE;
			return true;
			break;
		case SpaceSign:
			tData.CurrentFileToken = SPACE;
			return true;
			break;
		case NotEqualsTo:
			tData.CurrentFileToken = NOTEQUALS;
			return true;
			break;
		case LessThanSign:
			tData.CurrentFileToken = LESSTHAN;
			return true;
			break;
		case GreaterThanSign:
			tData.CurrentFileToken = GREATERTHAN;
			return true;
			break;
		case CommaSign:
			tData.CurrentFileToken = COMMA;
			return true;
			break;
		case OpenParSign:
			tData.CurrentFileToken = OPENP;
			return true;
			break;
		case ClosedParSign:
			tData.CurrentFileToken = CLOSEP;
			return true;
			break;
		case ColonSign:
			tData.CurrentFileToken = COLON;
			return true;
			break;
		case CommentSign:
			tData.CurrentFileToken = COMMENT;
			return true;
			break;
		case ClosedBraketSign:
			tData.CurrentFileToken = CLOSEB;
			return true;
			break;
		case OpenBraketSign:
			tData.CurrentFileToken = OPENB;
			return true;
			break;
		case EmptySign:
			tData.CurrentFileToken = EMPTY;
			return true;
			break;
		case '\r':
			tData.CurrentFileToken = EMPTY;
			return true;
			break;
		case SemicolonSign:
			tData.CurrentFileToken = SEMICOLON;
			return true;
			break;
		case OpenCurlyBrace:
			tData.CurrentFileToken = OPENCB;
			return true;
			break;
		case ClosedCurlyBrace:
			tData.CurrentFileToken = CLOSECB;
			return true;
			break;
		case PeriodSign:
			tData.CurrentFileToken = PERIOD;
			return true;
			break;
		case UnderscoreSign:
			tData.CurrentFileToken = UNDERSCORE;
			return true;
			break;
		default: break; // found nothing
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------------