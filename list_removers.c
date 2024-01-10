/*
 * GROUP 18
 * 
 * NAME: Kartik Kapoor
 * NSID: KAK110
 * Student Number: 11269913
 * 
 * NAME: Shivansi Prajapati
 * NSID: LZU375
 * Student Number: 11326745
 *
 */

#include <stdio.h>
#include <list.h>
#include <stdlib.h>

extern NODE *nodePool;
extern LIST *listPool;
extern int currListPoolSize, freedNodeHead, freedNodeTail;
extern int freedListHead, freedListTail;

void *ListRemove(LIST *list){
	void *item_to_return;
	int new_cursor;

	if(list == NULL){
		printf("Error in ListRemove: list is NULL\n");
		return NULL;
	}

	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}


	if(list->cursor == -1){
		printf("Error in ListRemove: list cursor is NULL\n");
		return NULL;
	}
	
	item_to_return = nodePool[list->cursor].item;
	new_cursor =  nodePool[list->cursor].next;

	if(list->cursor == list->head){
		list->head = new_cursor;
	}

	if(list->cursor == list->tail){
		new_cursor = nodePool[list->cursor].prev;
		list->tail = new_cursor;
	}
	
	nodePool[nodePool[list->cursor].next].prev = nodePool[list->cursor].prev;
	nodePool[nodePool[list->cursor].prev].next = nodePool[list->cursor].next;
	nodePool[list->cursor].next = -1;
	nodePool[list->cursor].prev = -1;
	if(freedNodeHead == -1){
		freedNodeHead = nodePool[list->cursor].index;
		freedNodeTail = nodePool[list->cursor].index;
	}else{
		nodePool[freedNodeTail].next = nodePool[list->cursor].index;
		nodePool[nodePool[list->cursor].index].prev = freedNodeTail;
		freedNodeTail = nodePool[list->cursor].index;
	}

	
	list->cursor = new_cursor;
	list->size--;
	
	return item_to_return;		
}


void ListConcat(LIST *list1, LIST* list2){
	printf("Got to ListConcat\n");

	if(list1 == NULL || list2 == NULL){
		printf("Error in ListConcat: one of the lists passed is NULL\n");
		return;
	}

	if(*(LIST **)list1 != NULL && 
	   *(LIST**)list1 > listPool && 
	   *(LIST**)list1 < listPool + currListPoolSize){
		list1 = *(LIST **)list1;
	}

	if(*(LIST **)list2 != NULL && 
	   *(LIST**)list2 > listPool && 
	   *(LIST**)list2 < listPool + currListPoolSize){
		list2 = *(LIST **)list2;
	}
	
	if(list2->size == 0){
		return;
	}
	
	if(list1->size == 0){
		list1->head = list2->head;
		list1->tail = list2->tail;
		list1->cursor = list2->head;
	}
	list1->size += list2->size;	
	nodePool[list1->tail].next = list2->head;
	nodePool[list2->head].prev = list1->tail;
	list1->tail = list2->tail;

	/** free list2 **/
	list2->head = -1;
	list2->tail = -1;
	list2->cursor = -1;	
	list2->size = 0;
	if( freedListHead == -1){
		freedListHead = list2->index;
		freedListTail = list2->index;
	}else{
		listPool[freedListTail].next_list = list2->index;
		listPool[list2->index].next_list = -1;
		freedListTail = list2->index;
	}	
	return;
}




void ListFree(LIST *list, ItemFree itemFree){
	int i;
	NODE *node_to_free;
	if(list == NULL){
		printf("Error in ListFree: list is NULL\n");
		return;
	}

	if(itemFree == NULL){
		printf("Error in ListFree: list is NULL\n");
		return;
	}
	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}
	if(list->size != 0){
		i = list->head;
		for(;;){
			node_to_free = &nodePool[i];
			itemFree(node_to_free->item);
			i = nodePool[i].next;
			node_to_free->item = NULL;
			node_to_free->next = -1;
			node_to_free->prev = -1;
			if(freedNodeHead == -1){
				freedNodeHead = node_to_free->index;
				freedNodeTail = node_to_free->index;
			}else{
				nodePool[freedNodeTail].next = 
						node_to_free->index;
				nodePool[node_to_free->index].next = -1;	
				nodePool[node_to_free->index].prev =
						 freedNodeTail;
				
				freedNodeTail = node_to_free->index;
			}
			if(i == -1){
				break;
			}
		}
	}
	/** free list **/
	list->head = -1;
	list->tail = -1;
	list->cursor = -1;	
	list->size = 0;
	if( freedListHead == -1){
		freedListHead = list->index;
		freedListTail = list->index;
	}else{
		listPool[freedListTail].next_list = list->index;
		listPool[list->index].next_list = -1;
		freedListTail = list->index;
	}		
	return;	
}

void *ListTrim(LIST *list){
	void *item_to_return;
	int new_cursor;

	if(list == NULL){
		printf("Error in ListTrim: list is NULL\n");
		return NULL;
	}
	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}


	if(list->tail == -1){
		printf("Error in ListTrim: list  is invalid\n");
		return NULL;
	}
	
	item_to_return = nodePool[list->tail].item;
	if(list->size == 1){
		list->tail = -1;
		list->head = -1;
		list->size = 0;
		list->cursor = -1;
		if(freedNodeHead == -1){
			freedNodeHead = nodePool[list->tail].index;
			freedNodeTail = nodePool[list->tail].index;
		}else{
			nodePool[freedNodeTail].next = nodePool[list->tail].index;
			nodePool[nodePool[list->tail].index].prev = freedNodeTail;
			freedNodeTail = nodePool[list->tail].index;
		}
		return item_to_return; 
	}

	
	new_cursor =  list->cursor;
	
	if(new_cursor == list->tail){
		new_cursor = nodePool[list->tail].prev;
	}	
	
	nodePool[nodePool[list->tail].next].prev = nodePool[list->tail].prev;
	nodePool[nodePool[list->tail].prev].next = nodePool[list->tail].next;
	nodePool[list->tail].next = -1;
	nodePool[list->tail].prev = -1;
	
	if(freedNodeHead == -1){
		freedNodeHead = nodePool[list->tail].index;
		freedNodeTail = nodePool[list->tail].index;
	}else{
		nodePool[freedNodeTail].next = nodePool[list->tail].index;
		nodePool[nodePool[list->tail].index].prev = freedNodeTail;
		freedNodeTail = nodePool[list->tail].index;
	}

	list->cursor = new_cursor;
	list->size--;
	
	return item_to_return;		
}

