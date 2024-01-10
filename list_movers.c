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

extern NODE *nodePool;
extern LIST *listPool;
extern int currListPoolSize;

void *ListFirst(LIST *list){
	

	if(list == NULL){
		printf("Error in ListFirst: list is NULL\n");
		return NULL;
	}

	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	if(list->head == -1){
		printf("Error in ListFirst: list's first element is NULL\n");
		return NULL;
	}
	list->cursor = list->head;
	return nodePool[list->head].item;


}

void *ListLast(LIST *list){
	if(list == NULL){
		printf("Error in ListLast: list is NUL\n");
		return NULL;
	}

	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}


	if(list->tail == -1){
		printf("Error in ListLast: list's last  element is NULL\n");
		return NULL;
	}
	
	list->cursor = list->tail;
	return nodePool[list->tail].item;

}

void *ListNext(LIST *list){
	

	if(list == NULL){
		printf("Error in ListNext: list is NULL\n");
		return NULL;
	}

	
	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	if(list->cursor == -1){
		printf("Error in ListNext: list's current element is NULL\n");
		return NULL;
	}
	
	if (nodePool[list->cursor].next == -1){
		return NULL;
	}
	list->cursor = nodePool[list->cursor].next;
	return nodePool[list->cursor].item;
}

void *ListPrev(LIST *list){
	if(list == NULL){
		printf("Error in ListPrev: list is NULL\n");
		return NULL;
	}
	
	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	if(list->cursor == -1){
		printf("Error in ListPrev: list's current element is NULL\n");
		return NULL;
	}
	if (nodePool[list->cursor].prev == -1){
		return NULL;
	}
	list->cursor = nodePool[list->cursor].prev;
	return nodePool[list->cursor].item;
}

void *ListCurr(LIST *list){
	if(list == NULL){
		printf("Error in ListCurr: list is NULL\n");
		return NULL;
	}
	

	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	if(list->cursor == -1){
		printf("Error in ListCurr: list's current element is NULL\n");
		return NULL;
	}
	return nodePool[list->cursor].item;
}

