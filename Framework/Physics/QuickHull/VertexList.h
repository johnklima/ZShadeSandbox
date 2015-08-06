//===============================================================================================================================
// VertexList
//
//===============================================================================================================================
// History
//
// -Created on 4/6/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __VERTEXLIST_H
#define __VERTEXLIST_H
//===============================================================================================================================
//===============================================================================================================================
#include "HullVertex.h"
#include "DLList.h"
//===============================================================================================================================
// Double-linked list of hull faces
namespace ZShadeSandboxMath {
class VertexList
{
	ZShadeSandboxMath::HullVertex* head;
	ZShadeSandboxMath::HullVertex* tail;

	ZShadeSandboxMath::HullVertex* next;
	ZShadeSandboxMath::HullVertex* prev;

public:
	
	VertexList();

	// Clears the list
	void clear();

	// Adds an item to the end of the list
	void add(ZShadeSandboxMath::HullVertex* item);

	// Adds a chain of items to the end of the list
	void addAll(ZShadeSandboxMath::HullVertex* item);

	// Deletes an item from the list
	void erase(ZShadeSandboxMath::HullVertex* item);

	// Deletes a chain of items from this list
	void erase(ZShadeSandboxMath::HullVertex* item1, ZShadeSandboxMath::HullVertex* item2);

	// Inserts a item into this list before another item
	void insertBefore(ZShadeSandboxMath::HullVertex* item, ZShadeSandboxMath::HullVertex* next_item);

	// Returns the first element in the list
	ZShadeSandboxMath::HullVertex* first();

	// Returns true if the list is empty
	bool isEmpty();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__VERTEXLIST_H