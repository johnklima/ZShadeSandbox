//================================================================================================================
// XMLParser
//
//================================================================================================================
// History
//
// Created on 6/23/2015 by Dustin Watson
//================================================================================================================
#ifndef __XMLPARSER_H_
#define __XMLPARSER_H_
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "tinyxml2.h"
#include <string>
using namespace std;
using namespace tinyxml2;

//================================================================================================================
//================================================================================================================
class XMLParser
{
public:
	
	XMLParser();
	XMLParser(string filename);
	~XMLParser();
	
	XMLElement* Element() { return mRootElement; }
	tinyxml2::XMLDocument* Document() { return mDocument; }

	//
	// Loading
	//
	
	void Open(string filename);
	
	const char* ReadElement(const char* childElementName);
	
	void ReadAttribute(const char* childElementName, const char* attributeName, bool& value);
	void ReadAttribute(const char* childElementName, const char* attributeName, int& value);
	void ReadAttribute(const char* childElementName, const char* attributeName, float& value);

	//
	// Saving
	//
	
	void CreateRootNode(const char* childElementName);
	void InsertElement(const char* childElementName, int elementData);
	void InsertElement(const char* childElementName, string elementData);
	void InsertElement(const char* childElementName, bool elementData);
	void InsertElement(const char* childElementName, const char* elementData);
	void InsertElement(const char* childElementName, float elementData);
	void InsertElement(const char* childElementName, XMFLOAT2 elementData);
	void InsertElement(const char* childElementName, XMFLOAT3 elementData);
	void InsertElement(const char* childElementName, XMFLOAT4 elementData);
	void SetAttribute(const char* childElementName, const char* attributeName, bool value, bool endChild = false);
	void SetAttribute(const char* childElementName, const char* attributeName, int value, bool endChild = false);
	void SetAttribute(const char* childElementName, const char* attributeName, float value, bool endChild = false);
	void SetAttribute(const char* childElementName, const char* attributeName, string value, bool endChild = false);
	void SetAttribute(const char* childElementName, const char* attributeName, const char* value, bool endChild = false);
	void Save(string filename);

protected:
	
	bool bOpened;
	tinyxml2::XMLDocument doc;
	XMLNode* mRootNode;
	XMLElement* mRootElement;
	tinyxml2::XMLDocument* mDocument;
};
//================================================================================================================
// Helper class to read xml array elements
class ArrayElementXMLParser
{
public:
	
	ArrayElementXMLParser(XMLElement* element);
	~ArrayElementXMLParser();
	
	void ReadArrayElement(const char* siblingElementName);
	void ReadArrayElement(const char* childElementName, const char* siblingElementName);
	string ReadArrayElementValue();
	string ReadArrayElementAttribute(const char* attributeName);
	const char* ReadArrayChildElement(const char* childElementName);
	bool ArrayElementNotNull(const char* siblingElementName);
	
	XMLElement* ArrayElement() { return mArrayElement; }
	XMLElement* ArrayElementIter() { return mArrayElementIter; }
	
private:
	
	XMLElement* mRootElement;
	XMLElement* mArrayElement;
	XMLElement* mArrayElementIter;
};
//================================================================================================================
// Helper class to save data to xml file
class ArrayElementXMLSaver
{
public:

	ArrayElementXMLSaver(tinyxml2::XMLDocument* document, XMLElement* rootElement);
	~ArrayElementXMLSaver();

	void BeginNewArrayElement(const char* elementName);
	void CreateElement(const char* elementName);
	void InsertElement(const char* elementName, int elementData);
	void InsertElement(const char* elementName, string elementData);
	void InsertElement(const char* elementName, bool elementData);
	void InsertElement(const char* elementName, float elementData);
	void InsertElement(const char* elementName, const char* elementData);
	void InsertElement(const char* elementName, XMFLOAT2 elementData);
	void InsertElement(const char* elementName, XMFLOAT3 elementData);
	void InsertElement(const char* elementName, XMFLOAT4 elementData);
	void SetAttribute(const char* elementName, const char* attributeName, bool value, bool endChild = false);
	void SetAttribute(const char* elementName, const char* attributeName, int value, bool endChild = false);
	void SetAttribute(const char* elementName, const char* attributeName, float value, bool endChild = false);
	void SetAttribute(const char* elementName, const char* attributeName, string value, bool endChild = false);
	void SetAttribute(const char* elementName, const char* attributeName, const char* value, bool endChild = false);
	void SetAttribute(const char* attributeName, bool value);
	void SetAttribute(const char* attributeName, int value);
	void SetAttribute(const char* attributeName, float value);
	void SetAttribute(const char* attributeName, string value);
	void SetAttribute(const char* attributeName, const char* value);
	void EndCurrentElement();
	void EndArrayElement();

	XMLElement* ArrayElement() { return mArrayElement; }
	XMLElement* ValueElement() { return mValueElement; }

private:

	tinyxml2::XMLDocument* mDocument;
	XMLElement* mRootElement;
	XMLElement* mArrayElement;
	XMLElement* mValueElement;
};
//================================================================================================================
//================================================================================================================
#endif//__XMLPARSER_H_