#include <stdio.h>
#include <stdlib.h>
#include "DLinkedList.h"

DLinkedList_t* dllist_alloc (void* data) {
	DLinkedList_t* dllist = (DLinkedList_t*) malloc (sizeof (DLinkedList_t));

	if (! dllist)
		return NULL;

	// init
	dllist->prev = NULL;
	dllist->next = NULL;
	dllist->data = data;

	return dllist;
}

DLinkedList_t* dllist_init (void) {
	DLinkedList_t* pList = dllist_alloc (NULL);

	if (! pList)
		return NULL;

	pList->prev = pList;
	return pList;
}

void dllist_free (DLinkedList_t* pList) {
	if (! pList)
		return;

	free (pList);
}

void dllist_append (DLinkedList_t* pList, void* data) {
/* add the data in a new node at the end of the dllist
   the prev field of pList point now on that new node */
	if (! pList)
		return;

	DLinkedList_t* pListNew = dllist_alloc (data);
	pListNew->prev = pList->prev; // pList->prev is the former last node
	pList->prev = pListNew; // the last one is pListNew
	pListNew->prev->next = pListNew;
}

void dllist_replace_two_nodes_by_one_with_data (DLinkedList_t* pList, DLinkedList_t* pListHead, void* data) {
/* remove the data of the next node of pList
   if the next node and prev node of pList are equals, the prev field of pList become pList (because this mean
   there are only two node left in the dlllist (in that case, pList will contain the final result of the matricial product)
   and that's why the prev field of pList point on itself */
	if (! pList || ! pList->next) // null or one node
		return;

	DLinkedList_t* pListOldNext = pList->next;
	pList->next = pListOldNext->next;

	if (pListOldNext->next)
		pListOldNext->next->prev = pList;
	else // we want to remove the last one.
		pListHead->prev = pList; // = pListHead

	dllist_free (pListOldNext);

	pList->data = data;
}
