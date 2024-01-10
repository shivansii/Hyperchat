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
extern LIST *listPool;
extern int currListPoolSize;


void *ListSearch(LIST *list, Comparator comparator, void* comparisonArg){
	int i;
	void *item_to_return;
	item_to_return = NULL;
	if(list == NULL){
		printf("Error in ListSearch: list is NULL");
		return NULL;
	}

	if(comparator == NULL){
		printf("Error in ListSearch: comparator is NULL");
		return NULL;
	}

	if(comparisonArg == NULL){
		printf("Error in ListSearch: comparisonArg is NULL");
		return NULL;
	}

	if(*(LIST **)list != NULL && 
	   *(LIST**)list > listPool && 
	   *(LIST**)list < listPool + currListPoolSize){
		list = *(LIST **)list;
	}

	i = list->cursor;
	while(1){
		if(comparator(nodePool[i].item, comparisonArg) == 1){
			item_to_return =  nodePool[i].item;
			break;
		}
		if(i == list->tail){
			break;
		}
	}
	list->cursor = i;
	return item_to_return;
}

