#include "VertexList.h"
using ZShadeSandboxMath::VertexList;
//===============================================================================================================================
//===============================================================================================================================
VertexList::VertexList()
{
	clear();
}
//===============================================================================================================================
void VertexList::clear()
{
	head = tail = nullptr;
}
//===============================================================================================================================
void VertexList::add(ZShadeSandboxMath::HullVertex* item)
{
	if (head == nullptr)
	{
		head = item;
	}
	else
	{
		tail->next = item;
	}

	prev = tail;
	next = nullptr;
	tail = item;
}
//===============================================================================================================================
void VertexList::addAll(ZShadeSandboxMath::HullVertex* item)
{
	if (head == nullptr)
	{
		head = item;
	}
	else
	{
		tail->next = item;
	}

	item->prev = tail;

	while (item->next != nullptr)
	{
		item = item->next;
	}

	tail = item;
}
//===============================================================================================================================
void VertexList::erase(ZShadeSandboxMath::HullVertex* item)
{
	if (item->prev == nullptr)
	{
		head = item->next;
	}
	else
	{
		item->prev->next = item->next;
	}

	if (item->next == nullptr)
	{
		tail = item->prev;
	}
	else
	{
		item->next->prev = item->prev;
	}
}
//===============================================================================================================================
void VertexList::erase(ZShadeSandboxMath::HullVertex* item1, ZShadeSandboxMath::HullVertex* item2)
{
	if (item1->prev == nullptr)
	{
		head = item2->next;
	}
	else
	{
		item1->prev->next = item2->next;
	}

	if (item2->next == nullptr)
	{
		tail = item1->prev;
	}
	else
	{
		item2->next->prev = item1->prev;
	}
}
//===============================================================================================================================
void VertexList::insertBefore(ZShadeSandboxMath::HullVertex* item, ZShadeSandboxMath::HullVertex* next_item)
{
	item->prev = next_item->prev;

	if (next_item->prev == nullptr)
	{
		head = item;
	}
	else
	{
		next_item->prev->next = item;
	}

	item->next = next_item;
	next_item->prev = item;
}
//===============================================================================================================================
ZShadeSandboxMath::HullVertex* VertexList::first()
{
	return head;
}
//===============================================================================================================================
bool VertexList::isEmpty()
{
	return head == nullptr;
}
//===============================================================================================================================