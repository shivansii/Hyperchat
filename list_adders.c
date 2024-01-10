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
#include <stdlib.h>
#include <list.h>

extern NODE *nodePool;
extern int currNodePoolSize, currNodeCursor;
extern int freedNodeHead, freedNodeTail; 
extern LIST *listPool;
extern int currListPoolSize;


int ListAdd(LIST *list, void *item){
	NODE *n_node;
	int temp, next_free_node;
	if(freedNodeHead != -1){
		next_free_node = nodePool[freedNodeHead].next;
	}else{
		next_free_node = -1;
	}
	if(list == NULL){
		printf("Error in ListAdd: list is NULL");
		return -1;
	}
	if(currNodeCursor >= currNodePoolSize || currNodeCursor == -1){
		if(freedNodeHead == -1){
			int i;
			currNodeCursor = currNodePoolSize;
			currNodePoolSize *= 2;
			nodePool = realloc(nodePool, sizeof(NODE) * currNodePoolSize);
			for(i = currNodePoolSize/2; i < currNodePoolSize; i++){
				nodePool[i].next = -1;
				nodePool[i].prev = -1;
				nodePool[i].index = i;
			} 
		}else{
			currNodeCursor = freedNodeHead;	
			next_free_node = nodePool[currNodeCursor].next;
		}

	}
	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	if(list->size == 0){
		n_node = &nodePool[currNodeCursor];
		list->head = currNodeCursor;
		list->tail = currNodeCursor;
		list->cursor = currNodeCursor;
		nodePool[list->head].item = item;
		nodePool[list->head].next = -1;
		nodePool[list->head].prev = -1;
	}
	else if(list->cursor == list->tail){
		n_node = &nodePool[currNodeCursor];
		n_node->item = item;
		n_node->next = -1;
		n_node->prev = list->tail;
		nodePool[list->tail].next = currNodeCursor;
		list->tail = currNodeCursor;
		list->cursor = currNodeCursor;
	}
	else{
		n_node = &nodePool[currNodeCursor];
		/* change n_nodePool required fields */
		n_node->item = item;
		n_node->next = nodePool[list->cursor].next;
		n_node->prev = list->cursor;


		/* change list required fields*/
		temp = nodePool[list->cursor].next;
		nodePool[temp].prev = currNodeCursor;
		nodePool[list->cursor].next = currNodeCursor;
		list->cursor = currNodeCursor;
	}

	if(list->cursor == -1){
		return -1;
	}
	else{
		list->size++;
		if(freedNodeHead == currNodeCursor){
			currNodeCursor = next_free_node;
			freedNodeHead = next_free_node;
			if (freedNodeHead == -1){
				freedNodeTail = -1;
			}
		}else{
			currNodeCursor++;
		}
	
		return 0;
	}
}

int ListInsert(LIST *list, void *item){
	NODE *n_node;
	int  next_free_node;
	if(freedNodeHead != -1){
		next_free_node = nodePool[freedNodeHead].next;
	}else{
		next_free_node = -1;
	}
	
	if(list == NULL){
		printf("Error in ListInsert: list is NULL\n");
		return -1;
	}
	if(currNodeCursor >= currNodePoolSize || currNodeCursor == -1){
		if(freedNodeHead == -1){
			int i;
			currNodeCursor = currNodePoolSize;
			currNodePoolSize *= 2;
			nodePool = realloc(nodePool, sizeof(NODE) * currNodePoolSize);
			for(i = currNodePoolSize/2; i < currNodePoolSize; i++){
				nodePool[i].next = -1;
				nodePool[i].prev = -1;
				nodePool[i].index = i;
			} 
		}else{
			currNodeCursor = freedNodeHead;	
			next_free_node = nodePool[currNodeCursor].next;
		}
	}
	n_node = &nodePool[currNodeCursor];
	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	n_node->item = item;
	if(list->size == 0){
		list->head = currNodeCursor;
		list->tail = currNodeCursor;
		list->cursor = currNodeCursor;
		n_node->next = -1;
		n_node->prev = -1;
	}
	else if(list->cursor == list->head){
		n_node->next = list->head;
		n_node->prev = -1;
		nodePool[list->head].prev = currNodeCursor;
		list->head = currNodeCursor;
		list->cursor = currNodeCursor;
	}
	else{
		n_node->next = list->cursor;
		n_node->prev = nodePool[list->cursor].prev;
		nodePool[nodePool[list->cursor].prev].next = currNodeCursor;
		nodePool[list->cursor].prev = currNodeCursor;
		list->cursor = currNodeCursor;
	}
	if(list->cursor == -1){
		return -1;
	}
	else{
		list->size++;
		if(freedNodeHead == currNodeCursor){
			currNodeCursor = next_free_node;
			freedNodeHead = next_free_node;
			if (freedNodeHead == -1){
				freedNodeTail = -1;
			}
		}else{
			currNodeCursor++;
		}
		return 0;
	}
}

int ListAppend(LIST *list, void *item){
	NODE *n_node;
	int  next_free_node;
	if(freedNodeHead != -1){
		next_free_node = nodePool[freedNodeHead].next;
	}else{
		next_free_node = -1;
	}
	
	if(list == NULL){
		printf("Error in ListAppend: list is NULL\n");
		return -1;
	}
	
	if(currNodeCursor >= currNodePoolSize || currNodeCursor == -1){
		if(freedNodeHead == -1){
			int i;
			currNodeCursor = currNodePoolSize;
			currNodePoolSize *= 2;
			nodePool = realloc(nodePool, sizeof(NODE) * currNodePoolSize);
			for(i = currNodePoolSize/2; i < currNodePoolSize; i++){
				nodePool[i].next = -1;
				nodePool[i].prev = -1;
				nodePool[i].index = i;
			} 
		}else{
			currNodeCursor = freedNodeHead;	
			next_free_node = nodePool[currNodeCursor].next;
		}
  	}
	n_node = &nodePool[currNodeCursor];
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	n_node->item = item;	
	if(list->size == 0){
		list->head = currNodeCursor;
		list->tail = currNodeCursor;
		list->cursor = currNodeCursor;
		n_node->next = -1;
		n_node->prev = -1;
	}
	else{
		n_node->next = -1;
		n_node->prev = list->tail;
		nodePool[list->tail].next = currNodeCursor;
		list->tail = currNodeCursor;
		list->cursor = currNodeCursor;
	}
	if(list->cursor == -1){
		return -1;
	}
	else{
		list->size++;
		if(freedNodeHead == currNodeCursor){
			currNodeCursor = next_free_node;
			freedNodeHead = next_free_node;
			if (freedNodeHead == -1){
				freedNodeTail = -1;
			}
		}else{
			currNodeCursor++;
		}
		return 0;
	}
}

int ListPrepend(LIST *list, void *item){
	NODE *n_node;
	int  next_free_node;
	if(freedNodeHead != -1){
		next_free_node = nodePool[freedNodeHead].next;
	}else{
		next_free_node = -1;
	}
	
	if(list == NULL){
		printf("Error in ListPrepend: list is NULL\n");
		return -1;
	}
	if(currNodeCursor >= currNodePoolSize || currNodeCursor == -1){
		if(freedNodeHead == -1){
			int i;
			currNodeCursor = currNodePoolSize;
			currNodePoolSize *= 2;
			nodePool = realloc(nodePool, sizeof(NODE) * currNodePoolSize);
			for(i = currNodePoolSize/2; i < currNodePoolSize; i++){
				nodePool[i].next = -1;
				nodePool[i].prev = -1;
				nodePool[i].index = i;
			} 
		}else{
			currNodeCursor = freedNodeHead;	
			next_free_node = nodePool[currNodeCursor].next;
		}
	}
	n_node = &nodePool[currNodeCursor];
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	n_node->item = item;
	if(list->size == 0){
		list->head = currNodeCursor;
		list->tail = currNodeCursor;
		list->cursor = currNodeCursor;
		n_node->next = -1;
		n_node->prev = -1;
	}
	else{
		n_node->next = list->head;
		n_node->prev = -1;
		nodePool[list->head].prev = currNodeCursor;
		list->head = currNodeCursor;
		list->cursor = currNodeCursor;
	}
	if(list->cursor == -1){
		return -1;
	}
	else{
		list->size++;
		if(freedNodeHead == currNodeCursor){
			currNodeCursor = next_free_node;
			freedNodeHead = next_free_node;
			if (freedNodeHead == -1){
				freedNodeTail = -1;
			}
		}else{
			currNodeCursor++;
		}
		return 0;
	}
}

