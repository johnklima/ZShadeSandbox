//==========================================================
// BetterString
//==========================================================
#ifndef __BETTERSTRING_H__
#define __BETTERSTRING_H__
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <Windows.h>
#include "MemoryMgr.h"
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
class BetterString
{
public:
	BetterString();
	BetterString(string const& s);
	BetterString(const char* s);
	BetterString(std::wstring const& s);
	BetterString(const wchar_t* s);
	BetterString(const BetterString& s);
	~BetterString();

	//Size of the string
	int size() const;

	string toString() const;

	//Resize the string
	void resize(int p_newsize);

	//These will compare the strings
	bool compare(string const& s);
	bool compare(const BetterString& s);
	bool compare(const char * s);

	//Push new string data onto the string
	void push(string const &s);
	void push(const BetterString& s);
	void push(const char *s);

	//This inserts a char into the back of the string
	void push_char(char c);

	//pops off a string according to the supplied indices
	BetterString pop(vector<int> indices);

	//Make a new string
	void re_insert(string const &s);
	void re_insert(const BetterString &s);
	void re_insert(const char* s);

	//Return a substring of the string
	BetterString substring(int start = 0, int end = string::npos);

	BetterString Strip(int start, char end);
	BetterString Strip(char start, int end);
	BetterString Strip(BetterString contents);

	//Delete a single char from the string
	void delete_char(char c, bool all = false);

	//Insert a single char into the string
	void insert_char(string c, int pos);
	void insert_char(char c, int pos);

	//Get a specified index of the string
	int get_index(char c);

	//Replace a character in the string
	void replace_char(char c, string replacement, bool all = false);

	//Flip the string
	void flip();

	//Split the string into a vector
	vector<string> split(char separator = ' ');

	//Copies another string to this string
	void copy_one(string const &s);
	void copy_two(const char* s);
	void copy_three(wstring const &s);
	void copy_four(const wchar_t* s);
	void deep_copy(const BetterString &s);

	//Destroy the string
	void destroy();

	//Get the C string buffer
	const char* CStringBuffer();

	//ZShadeSandboxGlobal::Convert to wide char
	unique_ptr<wchar_t> ToWideStr() const;

	//Assignment operators
	BetterString& operator = (string const &s);
	BetterString& operator = (const char* s);
	BetterString& operator = (const BetterString &s);

	//String subscript
	char& operator [] (int i);
	char operator [] (int i) const;

	bool operator == (const BetterString &s) const;
	bool operator == (const char* s) const;

	bool operator != (const char* s) const;

	//Append a string to the current value
	BetterString& operator += (const char* s);
	BetterString& operator += (const BetterString &s);
	BetterString& operator += (string const &s);

	//Concatinate a string to this string
	BetterString& operator + (const char* s);
	BetterString& operator + (const BetterString &s);
	BetterString& operator + (string const &s);

	friend ostream& operator << (ostream& os, BetterString s)
	{
		os << (const char *)s;
		return os;
	}

	friend istream& operator >> (istream& is, BetterString s)
	{
		s.destroy();
		string str;
		cout << "Enter String: ";
		getline(is, str);
		s.deep_copy(str);
		return is;
	}

	//Cast operator to a c string buffer
	operator const char* (void) const
	{
		return m_string;
	}

	//Cast operator to a string
	operator string(void) const
	{
		return(string(m_string));
	}

private:
	char * m_string;

	int _length;

	static int npos;

	char m_current;
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
#endif//__BETTERSTRING_H__