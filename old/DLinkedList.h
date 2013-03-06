#ifndef __DLINKEDLIST_H__
#define __DLINKEDLIST_H__

typedef struct DLinkedList {
	struct DLinkedList* prev;
	struct DLinkedList* next;
	void* data;
} DLinkedList_t;

DLinkedList_t* dllist_alloc (void* data);
//alloc memory for a new DLinkedList


DLinkedList_t* dllist_init (void);
// init the first element of the list.


void dllist_free (DLinkedList_t* pList);
//free the pList passed in param


void dllist_append (DLinkedList_t* pList, void* data);
//add data, at the end of the list


void dllist_replace_two_nodes_by_one_with_data (DLinkedList_t* pList, DLinkedList_t* head, void* data);
/*remove pList->next and free it. This function is used in the matricial product, calculating the product of two matrix
and stocking the result in pList->data*/

#endif /* __DLINKEDLIST_H__ */

