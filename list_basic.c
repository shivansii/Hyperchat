#include <stdio.h>
#include <list.h>
#include <stdlib.h>

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


static int first_execution = 1;
extern NODE *nodePool;

LIST *listPool;
NODE *nodePool;
extern int currNodeCursor, currNodePoolSize;
extern int freedNodeHead, freedNodeTail;
extern int freedListHead, freedListTail; 
int currNodeCursor, currListCursor, currListPoolSize, currNodePoolSize; 
int freedNodeHead, freedNodeTail;
int freedListHead, freedListTail; 
LIST *ListCreate(){
	LIST *list_to_return;
	list_to_return = NULL;
	if (first_execution == 1){
		int i;
		
		listPool = malloc(sizeof(LIST) * MIN_LISTS);
		for(i = 0; i < MIN_LISTS; i++){
			listPool[i].head = -1;
			listPool[i].tail = -1;
			if(i == MIN_LISTS - 1){
				listPool[i].next_list = -1;
			}else{
				listPool[i].next_list = i + 1;
			}
			listPool[i].prev_list = i-1;
		}
		currListPoolSize = MIN_LISTS;
		
		nodePool = malloc(sizeof(NODE) * MIN_NODES);
		
		for(i = 0; i < MIN_NODES; i++){
			nodePool[i].next = -1;
			nodePool[i].prev = -1;
			nodePool[i].index = i;
		}
		currNodePoolSize = MIN_NODES;
		
		freedNodeHead = -1;
		freedNodeTail = -1;
		freedListHead = -1;
		freedListTail = -1;
		currNodeCursor = 0;
		currListCursor = 0;
		first_execution = 0;
    	}
	
	if(currListCursor == -1 || currListCursor >= currListPoolSize){
		if(freedListHead == -1){
			return NULL;
		}
		currListCursor = freedListHead;	
	}

    	listPool[currListCursor].cursor = -1;
    	listPool[currListCursor].size = 0;    
    	listPool[currListCursor].index = currListCursor;
    	listPool[currListCursor].head = -1;    
    	listPool[currListCursor].tail = -1;    
	list_to_return = &listPool[currListCursor];
	if(freedListHead == currListCursor){
		currListCursor = listPool[freedListHead].next_list;
		freedListHead = currListCursor;
		if (freedListHead == -1){
			freedListTail = -1;
		}
	}else{
		currListCursor++;
	}
	
    	return list_to_return;
}
    

int ListCount(LIST* list){
	if(list == NULL){
		printf("Error in ListCount: list is null\n");
		return -1;
	}
	return list->size;
}

