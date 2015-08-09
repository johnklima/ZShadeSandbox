#include "XMLParser.h"
#include "Convert.h"
#include <sstream>
//================================================================================================================
//================================================================================================================
XMLParser::XMLParser()
:	bOpened(false)
{
}
//================================================================================================================
XMLParser::XMLParser(string filename)
:	bOpened(false)
{
	Open(filename);
}
//================================================================================================================
XMLParser::~XMLParser()
{
}
//================================================================================================================
void XMLParser::Open(string filename)
{
	doc.LoadFile( filename.c_str() );
	mRootNode = doc.FirstChild();
	mRootElement = doc.FirstChildElement();
	bOpened = true;
}
//================================================================================================================
const char* XMLParser::ReadElement(const char* childElementName)
{
	XMLElement* element = mRootNode->FirstChildElement(childElementName);
	return element->GetText();
}
//================================================================================================================
void XMLParser::ReadAttribute(const char* childElementName, const char* attributeName, bool& value)
{
	XMLElement* element = mRootNode->FirstChildElement(childElementName);
	element->QueryBoolAttribute(attributeName, &value);
}
//================================================================================================================
void XMLParser::ReadAttribute(const char* childElementName, const char* attributeName, int& value)
{
	XMLElement* element = mRootNode->FirstChildElement(childElementName);
	element->QueryIntAttribute(attributeName, &value);
}
//================================================================================================================
void XMLParser::ReadAttribute(const char* childElementName, const char* attributeName, float& value)
{
	XMLElement* element = mRootNode->FirstChildElement(childElementName);
	element->QueryFloatAttribute(attributeName, &value);
}
//================================================================================================================
void XMLParser::CreateRootNode(const char* childElementName)
{
	mDocument = new tinyxml2::XMLDocument();
	mRootNode = mDocument->InsertEndChild(mDocument->NewElement(childElementName));
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, int elementData)
{
	stringstream ss;
	ss << elementData;
	const char* element_str = ss.str().c_str();
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(element_str));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, string elementData)
{
	if (elementData == "") elementData = "NONE";
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(elementData.c_str()));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, bool elementData)
{
	const char* elementStr = (elementData == true) ? "true" : "false";
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(elementStr));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, const char* elementData)
{
	if (strcmp(elementData, "") == 0) elementData = "NONE";
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(elementData));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, float elementData)
{
	stringstream ss;
	ss << elementData;
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(ss.str().c_str()));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, XMFLOAT2 elementData)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(ZShadeSandboxGlobal::Convert::ConvertFloat2ToString(elementData).c_str()));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, XMFLOAT3 elementData)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(ZShadeSandboxGlobal::Convert::ConvertFloat3ToString(elementData).c_str()));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::InsertElement(const char* childElementName, XMFLOAT4 elementData)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->InsertFirstChild(mDocument->NewText(ZShadeSandboxGlobal::Convert::ConvertFloat4ToString(elementData).c_str()));
	mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::SetAttribute(const char* childElementName, const char* attributeName, bool value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::SetAttribute(const char* childElementName, const char* attributeName, int value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::SetAttribute(const char* childElementName, const char* attributeName, float value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::SetAttribute(const char* childElementName, const char* attributeName, string value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->SetAttribute(attributeName, value.c_str());
	if (endChild) mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::SetAttribute(const char* childElementName, const char* attributeName, const char* value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(childElementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mRootNode->InsertEndChild(element);
}
//================================================================================================================
void XMLParser::Save(string filename)
{
	mDocument->SaveFile(filename.c_str());
}
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
ArrayElementXMLParser::ArrayElementXMLParser(XMLElement* element)
:	mRootElement(element)
{
}
//================================================================================================================
ArrayElementXMLParser::~ArrayElementXMLParser()
{
}
//================================================================================================================
void ArrayElementXMLParser::ReadArrayElement(const char* siblingElementName)
{
	mArrayElementIter = mRootElement->FirstChildElement(siblingElementName);
	mArrayElement = mRootElement;
}
//================================================================================================================
void ArrayElementXMLParser::ReadArrayElement(const char* childElementName, const char* siblingElementName)
{
	mArrayElement = mRootElement->FirstChildElement(childElementName);
	mArrayElementIter = mArrayElement->FirstChildElement(siblingElementName);
}
//================================================================================================================
string ArrayElementXMLParser::ReadArrayElementValue()
{
	return mArrayElementIter->Value();
}
//================================================================================================================
string ArrayElementXMLParser::ReadArrayElementAttribute(const char* attributeName)
{
	return mArrayElementIter->Attribute(attributeName);
}
//================================================================================================================
const char* ArrayElementXMLParser::ReadArrayChildElement(const char* childElementName)
{
	XMLElement* element = mArrayElementIter->FirstChildElement(childElementName);
	return element->GetText();
}
//================================================================================================================
bool ArrayElementXMLParser::ArrayElementNotNull(const char* siblingElementName)
{
	if (mArrayElementIter != NULL)
	{
		mArrayElementIter = mArrayElementIter->NextSiblingElement(siblingElementName);
		if (mArrayElementIter == NULL) return false;
		return true;
	}
	
	return false;
}
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
ArrayElementXMLSaver::ArrayElementXMLSaver(tinyxml2::XMLDocument* document, XMLNode* rootNode)
:	mDocument(document)
,	mRootNode(rootNode)
,	bUseRootNode(true)
,	bHasArrayElement(false)
{
}
//================================================================================================================
ArrayElementXMLSaver::ArrayElementXMLSaver(tinyxml2::XMLDocument* document, XMLElement* rootElement)
:	mDocument(document)
,	mRootElement(rootElement)
,	bUseRootNode(false)
,	bHasArrayElement(false)
{
}
//================================================================================================================
ArrayElementXMLSaver::~ArrayElementXMLSaver()
{
}
//================================================================================================================
void ArrayElementXMLSaver::BeginNewArrayElement(const char* elementName)
{
	mArrayElement = mDocument->NewElement(elementName);
	bHasArrayElement = true;
}
//================================================================================================================
void ArrayElementXMLSaver::CreateElement(const char* elementName)
{
	mValueElement = mDocument->NewElement(elementName);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, int elementData)
{
	stringstream ss;
	ss << elementData;
	string element_str = ZShadeSandboxGlobal::Convert::ConvertIntToString(elementData);// ss.str().c_str();
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(element_str.c_str()));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, string elementData)
{
	if (elementData == "") elementData = "NONE";
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(elementData.c_str()));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, bool elementData)
{
	const char* elementStr = (elementData == true) ? "true" : "false";
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(elementStr));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, const char* elementData)
{
	if (strcmp(elementData, "") == 0) elementData = "NONE";
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(elementData));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, float elementData)
{
	stringstream ss;
	ss << elementData;
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(ss.str().c_str()));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, XMFLOAT2 elementData)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(ZShadeSandboxGlobal::Convert::ConvertFloat2ToString(elementData).c_str()));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, XMFLOAT3 elementData)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(ZShadeSandboxGlobal::Convert::ConvertFloat3ToString(elementData).c_str()));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::InsertElement(const char* elementName, XMFLOAT4 elementData)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->InsertFirstChild(mDocument->NewText(ZShadeSandboxGlobal::Convert::ConvertFloat4ToString(elementData).c_str()));
	mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* elementName, const char* attributeName, bool value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* elementName, const char* attributeName, int value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* elementName, const char* attributeName, float value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* elementName, const char* attributeName, string value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->SetAttribute(attributeName, value.c_str());
	if (endChild) mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* elementName, const char* attributeName, const char* value, bool endChild)
{
	XMLElement* element = mDocument->NewElement(elementName);
	element->SetAttribute(attributeName, value);
	if (endChild) mValueElement->InsertEndChild(element);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* attributeName, bool value)
{
	mValueElement->SetAttribute(attributeName, value);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* attributeName, int value)
{
	mValueElement->SetAttribute(attributeName, value);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* attributeName, float value)
{
	mValueElement->SetAttribute(attributeName, value);
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* attributeName, string value)
{
	mValueElement->SetAttribute(attributeName, value.c_str());
}
//================================================================================================================
void ArrayElementXMLSaver::SetAttribute(const char* attributeName, const char* value)
{
	mValueElement->SetAttribute(attributeName, value);
}
//================================================================================================================
void ArrayElementXMLSaver::EndCurrentElement()
{
	if (!bHasArrayElement)
		mRootNode->InsertEndChild(mValueElement);
	else
		mArrayElement->InsertEndChild(mValueElement);
}
//================================================================================================================
void ArrayElementXMLSaver::EndArrayElement()
{
	if (bUseRootNode)
		mRootNode->InsertEndChild(mArrayElement);
	else
		mRootElement->InsertEndChild(mArrayElement);
}
//================================================================================================================