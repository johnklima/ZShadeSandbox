//===============================================================================================================================
// DLList
//
//===============================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __DLLIST_H
#define __DLLIST_H
//===============================================================================================================================
//===============================================================================================================================
#include "LNode.h"
//===============================================================================================================================
// Double-linked list
template <typename T>
class DLList
{
	LNode<T>* head;
	LNode<T>* tail;

public:

	DLList();

	// Clears the list
	void clear();

	// Adds an item to the end of the list
	void add(T* item);

	// Adds a chain of items to the end of the list
	void addAll(T* item);

	// Deletes an item from the list
	void erase(T* item);

	// Deletes a chain of items from this list
	void erase(T* item1, T* item2);

	// Inserts a item into this list before another item
	void insertBefore(T* item, T* next_item);

	// Returns the first element in the list
	T* first();

	// Returns true if the list is empty
	bool isEmpty();
};
//===============================================================================================================================
template <typename T>
DLList<T>::DLList()
{
	clear();
}
//===============================================================================================================================
template <typename T>
void DLList<T>::clear()
{
	head = tail = nullptr;
}
//===============================================================================================================================
template <typename T>
void DLList<T>::add(T* item)
{
	if (head == nullptr)
	{
		head = item;
	}
	else
	{
		tail->next = item;
	}

	data->prev = tail;
	data->next = nullptr;
	tail = item;
}
//===============================================================================================================================
template <typename T>
void DLList<T>::addAll(T* item)
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
template <typename T>
void DLList<T>::erase(T* item)
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
template <typename T>
void DLList<T>::erase(T* item1, T* item2)
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
template <typename T>
void DLList<T>::insertBefore(T* item, T* next_item)
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
template <typename T>
T* DLList<T>::first()
{
	return head;
}
//===============================================================================================================================
template <typename T>
bool DLList<T>::isEmpty()
{
	return head == nullptr;
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__DLLIST_H