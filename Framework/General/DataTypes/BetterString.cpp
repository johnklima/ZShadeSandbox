//===============================================================================================
//--------------------------------- String implementation --------------------------------------
//===============================================================================================
#include "BetterString.h"
#include "Convert.h"
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
int BetterString::npos = 0;
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
BetterString::BetterString(void) : _length(0)
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
}
//--------------------------------------------------------------------------------------
BetterString::BetterString( string const & s ) : _length(0)
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	copy_one( s );
}
//--------------------------------------------------------------------------------------
BetterString::BetterString( const char * s ) : _length(0)
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	copy_two( s );
}
//--------------------------------------------------------------------------------------
BetterString::BetterString(std::wstring const& s)
{
	m_string = NULL;

	//Initialize the string buffer to null
	destroy();

	copy_three(s);
}
//--------------------------------------------------------------------------------------
BetterString::BetterString(const wchar_t* s)
{
	m_string = NULL;

	//Initialize the string buffer to null
	destroy();

	copy_four(s);
}
//--------------------------------------------------------------------------------------
BetterString::BetterString( const BetterString & s ) : _length(0)
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	deep_copy( s );
}
//--------------------------------------------------------------------------------------
BetterString::~BetterString(void)
{
	destroy();
}
//--------------------------------------------------------------------------------------
string BetterString::toString() const
{
	return ZShadeSandboxGlobal::Convert::ConvertCharPointerToString(m_string);
}
//--------------------------------------------------------------------------------------
int BetterString::size(void) const
{
	return(_length);
}
//--------------------------------------------------------------------------------------
void BetterString::copy_one( string const & s )
{
	_length = (int)s.length() + 1;
	
	npos = _length;
	
	m_string = new char[_length];
	
	if( m_string )
	{
		strncpy_s(m_string, _length, s.c_str(), _TRUNCATE);
	}
}
//--------------------------------------------------------------------------------------
void BetterString::copy_two( const char * s )
{
	if( s )
	{
		_length = strlen( s ) + 1;
		
		npos = _length;
		
		m_string = new char[_length];
		
		if( m_string )
		{
			strncpy_s(m_string, _length, s, _TRUNCATE);
		}
	}
}
//--------------------------------------------------------------------------------------
void BetterString::copy_three(wstring const &s)
{
	string str = ZShadeSandboxGlobal::Convert::ConvertWStringToString(s);

	copy_one(str);
}
//--------------------------------------------------------------------------------------
void BetterString::copy_four(const wchar_t* s)
{
	string str = ZShadeSandboxGlobal::Convert::ConvertWStringToString(s);

	copy_one(str);
}
//--------------------------------------------------------------------------------------
void BetterString::deep_copy( const BetterString & s )
{
	if( (const char *)s )
	{
		_length = strlen( (const char *)s ) + 1;
		
		npos = _length;
		
		m_string = new char[_length];
		
		if( m_string )
		{
			strncpy_s(m_string, _length, (const char *)s, _TRUNCATE);
		}
	}
}
//--------------------------------------------------------------------------------------
void BetterString::push( string const & p_pCharacter )
{
	if( p_pCharacter != "" )
	{
		if( !m_string )
			//Just create a regular string
			copy_one( p_pCharacter );
		else
		{
			//Append the string to the current string
			unsigned int IncreasedBufferSize = strlen( p_pCharacter.c_str() ) + 1;
			
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, m_string );
			
			destroy();
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), p_pCharacter.c_str() );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), p_pCharacter.c_str(), _TRUNCATE);
		}
	}
}
//--------------------------------------------------------------------------------------
void BetterString::push( const BetterString & p_pCharacter )
{
	if( p_pCharacter != "" )
	{
		if( !m_string )
			//Just create a regular string
			deep_copy( p_pCharacter );
		else
		{
			//Append the string to the current string
			unsigned int IncreasedBufferSize = strlen( (const char *)p_pCharacter ) + 1;
			
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, m_string );
			
			destroy();
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), (const char *)p_pCharacter );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), (const char *)p_pCharacter, _TRUNCATE);
		}
	}
}
//--------------------------------------------------------------------------------------
void BetterString::push( const char * p_pCharacter )
{
	if( p_pCharacter )
	{
		if( !m_string )
			//Just create a regular string
			copy_two( p_pCharacter );
		else
		{
			//Concatenate the string to the current string
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, (const char *)m_string );
			
			destroy();
			
			unsigned int IncreasedBufferSize = strlen( p_pCharacter ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), p_pCharacter );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), p_pCharacter, _TRUNCATE);
		}
	}
}
//--------------------------------------------------------------------------------------
void BetterString::push_char( char p_pCharacter )
{
	if( !m_string )
	{
		//Need to make new
		string pString = "";
		
		pString.push_back( p_pCharacter );
		
		//Make the new string
		const BetterString & p_pString = (BetterString)pString;
		destroy();
		deep_copy( p_pString );
	}
	else
	{
		string pString = toString();
		
		pString.push_back( p_pCharacter );
		
		//Remake the string
		const BetterString & p_pString = (BetterString)pString;
		destroy();
		deep_copy( p_pString );
	}
}
//--------------------------------------------------------------------------------------
BetterString BetterString::pop( vector<int> p_pIndicies )
{
	// pop the values @ the specified indicies back to the user
	BetterString p_pBuildString;
	
	BetterString p_pString = (*this);
	
	vector<int>::iterator v_iter = p_pIndicies.begin();
	
	for( ; v_iter != p_pIndicies.end(); v_iter++ )
		p_pBuildString.push_char( p_pString[(*v_iter)] );
	
	return(p_pBuildString);
}
//--------------------------------------------------------------------------------------
void BetterString::re_insert( string const & p_pString )
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	copy_one( p_pString );
}
//--------------------------------------------------------------------------------------
void BetterString::re_insert( const BetterString & p_pString )
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	deep_copy( p_pString );
}
//--------------------------------------------------------------------------------------
void BetterString::re_insert( const char * p_pStringBuffer )
{
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	copy_two( p_pStringBuffer );
}
//--------------------------------------------------------------------------------------
void BetterString::destroy()
{
	if( m_string )
	{
		delete [] m_string;
		m_string = NULL;
	}
}
//--------------------------------------------------------------------------------------
void BetterString::resize(int p_pNewSize)
{
	string pNewString = toString();
	
	pNewString.resize(p_pNewSize);
	
	m_string = NULL;
	
	//Initialize the string buffer to null
	destroy();
	
	copy_one( pNewString );
}
//--------------------------------------------------------------------------------------
const char * BetterString::CStringBuffer(void)
{
	return(m_string);
}
//--------------------------------------------------------------------------------------
bool BetterString::compare( string const & p_pString )
{
	return( (strcmp( p_pString.c_str(), m_string ) == 0) ? true : false );
}
//--------------------------------------------------------------------------------------
bool BetterString::compare( const BetterString & p_pString )
{
	return( (strcmp( (const char *)p_pString, m_string ) == 0) ? true : false );
}
//--------------------------------------------------------------------------------------
bool BetterString::compare( const char * p_pStringBuffer )
{
	return( (strcmp( p_pStringBuffer, m_string ) == 0) ? true : false );
}
//--------------------------------------------------------------------------------------
BetterString BetterString::substring( int p_pStart, int p_pEnd )
{
	if( m_string )
	{
		string tempString = "";
		
		tempString = m_string;
		
		tempString = tempString.substr(p_pStart, p_pEnd-p_pStart);
		
		BetterString pNewString(tempString);
		
		return(pNewString);
	}
	else return("");
}
//--------------------------------------------------------------------------------------
BetterString BetterString::Strip(int start, char end)
{
	string pString = toString();
	BetterString temp = "";
	int c = 0, i = start;

	for (; i < pString.size(); i++)
	{
		if (pString[i] != end)
		{
			temp.insert_char(pString[i], c++);
		}
	}

	return temp;
}
//--------------------------------------------------------------------------------------
BetterString BetterString::Strip(char start, int end)
{
	string pString = toString();
	BetterString temp = "";
	int c = 0, t = 0, i = 0;

	for (; i < end; i++)
	{
		if (pString[i] == start)
		{
			t = i + 1;
		}
	}

	for (i = t; i < end; i++)
	{
		temp.insert_char(pString[i], c++);
	}

	return temp;
}
//--------------------------------------------------------------------------------------
BetterString BetterString::Strip(BetterString contents)
{
	BetterString str(toString());

	str = str.substring( 0, (str.size() - contents.size()) - 1 );

	if (str == "")
	{
		str = str.substring( contents.size() );
	}

	return str;
}
//--------------------------------------------------------------------------------------
void BetterString::delete_char( char p_pCharacter, bool p_pAllOccurances )
{
	if( m_string )
	{
		string pString = toString();
		
		if( p_pAllOccurances )
		{
			while( true )
			{
				int pIndex = get_index(p_pCharacter);
				
				if( pIndex == -1 ) break;
				
				pString.erase(pIndex, 1);
				
				//Remake the string
				const BetterString & p_pString = (BetterString)pString;
				destroy();
				deep_copy( p_pString );
			}
		}
		else
		{
			int pIndex = get_index(p_pCharacter);
			
			if( pIndex == -1 ) return;
			
			pString.erase(pIndex, 1);
			
			//Remake the string
			const BetterString & p_pString = (BetterString)pString;
			destroy();
			deep_copy( p_pString );
		}
	}
}
//--------------------------------------------------------------------------------------
unique_ptr<wchar_t> BetterString::ToWideStr() const
{
	string fn = toString();

	std::wstring widestr = std::wstring(fn.begin(), fn.end());
	
	unique_ptr<wchar_t> outString;// = new wchar_t[fn.size() + 1]; //allocate heap memory
	outString.reset(new wchar_t[fn.size() + 1]);
	wcscpy_s(outString.get(), fn.size() + 1, widestr.c_str()); //copy the string to heap memory

	return outString;
}
//--------------------------------------------------------------------------------------
void BetterString::insert_char( string p_pCharacter, int p_pPosition )
{
	if( m_string )
	{
		string pString = toString();
		
		pString.insert(p_pPosition, p_pCharacter);
		
		//Remake the string
		const BetterString & p_pString = (BetterString)pString;
		destroy();
		deep_copy( p_pString );
	}
}
//--------------------------------------------------------------------------------------
void BetterString::insert_char( char p_pCharacter, int p_pPosition )
{
	if( m_string )
	{
		string pString = toString();
		
		string str(1, p_pCharacter);
		
		pString.insert(p_pPosition, str);
		
		//Remake the string
		const BetterString & p_pString = (BetterString)pString;
		destroy();
		deep_copy( p_pString );
	}
}
//--------------------------------------------------------------------------------------
int BetterString::get_index( char p_pCharacter )
{
	const char * pString = m_string;
	
	int pIndex = 0;
	
	if( pString )
	{
		while( *pString != p_pCharacter )
		{
			if( *pString == (char)NULL )
			{
				pIndex = -1;
				break;
			}
			*pString++;
			pIndex++;
		}
	}
	
	return(pIndex);
}
//--------------------------------------------------------------------------------------
void BetterString::replace_char( char p_pChrToReplace, string p_pReplacement, bool p_pAllOccurances )
{
	if( m_string )
	{
		string pString = toString();
		
		if( p_pAllOccurances )
		{
			while( true )
			{
				int pIndex = get_index(p_pChrToReplace);
				
				if( pIndex == -1 ) break;
				
				pString.replace(pIndex, 1, p_pReplacement);
				
				//Remake the string
				const BetterString & p_pString = (BetterString)pString;
				destroy();
				deep_copy( p_pString );
			}
		}
		else
		{
			int pIndex = get_index(p_pChrToReplace);
			
			if( pIndex == -1 ) return;
			
			pString.replace(pIndex, 1, p_pReplacement);
			
			//Remake the string
			const BetterString & p_pString = (BetterString)pString;
			destroy();
			deep_copy( p_pString );
		}
	}
}
//--------------------------------------------------------------------------------------
void BetterString::flip( void )
{
	string strValue = toString();
	
	int begin = 0;
	int end = size() - 1;
	
	while( begin < end )
	{
		end--;
		char temp = strValue[begin];
		strValue[begin] = strValue[end];
		strValue[end] = temp;
		begin++;
	}
	
	//Remake the string
	const BetterString & p_pString = (BetterString)strValue;
	destroy();
	deep_copy( p_pString );
}
//--------------------------------------------------------------------------------------
vector<string> BetterString::split( char separator )
{
	string strValue = toString();
	
	vector<string> vecstrResult;
	
	int startpos = 0;
	int endpos = 0;
	
	endpos = strValue.find_first_of(separator, startpos);
	
	while( endpos != -1 )
	{
		vecstrResult.push_back(strValue.substr(startpos, endpos - startpos)); // add to vector
		
		startpos = endpos + 1; //jump past separator
		
		endpos = strValue.find_first_of(separator, startpos); // find next
		
		if( endpos == -1 )
		{
			//lastone, so no 2nd param required to go to end of string
			vecstrResult.push_back(strValue.substr(startpos));
		}
	}
	
	return(vecstrResult);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator = ( string const & p_pString )
{
	if( m_string != p_pString.c_str() )
	{
		destroy();
	}
	
	copy_one( p_pString );
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator = ( const char * p_pStringBuffer )
{
	if( m_string != p_pStringBuffer )
	{
		destroy();
	}
	
	copy_two( p_pStringBuffer );
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator = ( const BetterString & p_pString )
{
	if( m_string != p_pString.m_string )
	{
		destroy();
	}
	
	deep_copy( p_pString );
	
	return(*this);
}
//--------------------------------------------------------------------------------------
char & BetterString::operator [] (int p_pIndex)
{
	try
	{
		return(m_string[p_pIndex]);
	}
	catch( char * pMsg )
	{
		//Log error message
		
		m_current = '\0';
		
		if( m_string ) m_current = m_string[0];
		
		return(m_current);
	}
}
//--------------------------------------------------------------------------------------
char BetterString::operator [] (int p_pIndex) const
{
	return(m_string[p_pIndex]);
}
//--------------------------------------------------------------------------------------
bool BetterString::operator == ( const BetterString & p_pString ) const
{
	return( (strcmp(m_string, (const char *)p_pString) == 0) ? true : false );
}
//--------------------------------------------------------------------------------------
bool BetterString::operator == ( const char * p_pStringBuffer ) const
{
	return( (strcmp(m_string, p_pStringBuffer) == 0) ? true : false );
}
//--------------------------------------------------------------------------------------
bool BetterString::operator != ( const char * p_pStringBuffer ) const
{
	return( (strcmp(m_string, p_pStringBuffer) == 0) ? false : true );
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator += ( string const & p_pString )
{
	if( p_pString != "" )
	{
		if( !m_string )
			//Just create a regular string
			copy_one( p_pString );
		else
		{
			//Append the string to the current string
			unsigned int IncreasedBufferSize = strlen( p_pString.c_str() ) + 1;
			
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, m_string );
			
			destroy();
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), p_pString.c_str() );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), p_pString.c_str(), _TRUNCATE);
		}
	}
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator += ( const char * p_pValue )
{
	if( p_pValue )
	{
		if( !m_string )
			//Just create a regular string
			copy_two( p_pValue );
		else
		{
			//Append the string to the current string
			unsigned int IncreasedBufferSize = strlen( p_pValue ) + 1;
			
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, (const char *)m_string );
			
			destroy();
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), p_pValue );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), p_pValue, _TRUNCATE);
		}
	}
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator += ( const BetterString & p_pString )
{
	if( p_pString != "" )
	{
		if( !m_string )
			//Just create a regular string
			deep_copy( p_pString );
		else
		{
			//Append the string to the current string
			unsigned int IncreasedBufferSize = strlen( (const char *)p_pString ) + 1;
			
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, m_string );
			
			destroy();
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), (const char *)p_pString );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), (const char *)p_pString, _TRUNCATE);
		}
	}
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator + ( string const & p_pString )
{
	if( p_pString != "" )
	{
		if( !m_string )
			//Just create a regular string
			copy_one( p_pString );
		else
		{
			//Append the string to the current string
			unsigned int IncreasedBufferSize = strlen( p_pString.c_str() ) + 1;
			
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, m_string );
			
			destroy();
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), p_pString.c_str() );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), p_pString.c_str(), _TRUNCATE);
		}
	}
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator + ( const char * p_pValue )
{
	if( p_pValue )
	{
		if( !m_string )
			//Just create a regular string
			copy_two( p_pValue );
		else
		{
			//Concatenate the string to the current string
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, (const char *)m_string );
			
			destroy();
			
			unsigned int IncreasedBufferSize = strlen( p_pValue ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), p_pValue );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), p_pValue, _TRUNCATE);
		}
	}
	
	return(*this);
}
//--------------------------------------------------------------------------------------
BetterString & BetterString::operator + ( const BetterString & p_pString )
{
	if( p_pString != "" )
	{
		if( !m_string )
			//Just create a regular string
			deep_copy( p_pString );
		else
		{
			//Concatenate the string to the current string
			unsigned int p_lengthOld = strlen( m_string ) + 1;
			
			char * pTempBuffer = new char[p_lengthOld];
			
			strcpy_s( pTempBuffer, p_lengthOld, m_string );
			
			destroy();
			
			unsigned int IncreasedBufferSize = strlen( (const char *)p_pString ) + 1;
			
			_length = p_lengthOld + IncreasedBufferSize;
			
			npos = _length;
			
			m_string = new char[p_lengthOld + IncreasedBufferSize];
			
			//strcpy_s( m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer );
			//strcat_s( m_string, (p_lengthOld + IncreasedBufferSize), (const char *)p_pString );

			strncpy_s(m_string, (p_lengthOld + IncreasedBufferSize), pTempBuffer, _TRUNCATE);
			strncat_s(m_string, (p_lengthOld + IncreasedBufferSize), (const char *)p_pString, _TRUNCATE);
		}
	}
	
	return(*this);
}
//--------------------------------------------------------------------------------------
