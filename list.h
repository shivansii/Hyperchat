/*
 * GROUP 18
 *
 * Name: Kartik Kapoor
 * NSID: KAK110
 * Student Number: 11269913
 *
 * Name: Shivansi Prajapati
 * NSID: LZU375
 * Student Numbr: 11326745
 */

#define MIN_LISTS 10
#define MIN_NODES 20

typedef struct NODE{
	void *item;
	int index;
	int next;
	int prev;
} NODE;


typedef struct{
    	int cursor;
	int head;
	int tail;
	int index;
	int size;
	int next_list;
	int prev_list;
} LIST;

typedef void (*ItemFree)(void*);
typedef int (*Comparator)(void*, void*);

extern int currListCursor;
extern int currNodeCursor;


extern LIST *lists;
extern NODE *nodes;



/*
 * All List functions are defined below
 *
 */



/**
 * Basic functions
 */

LIST *ListCreate();
int ListCount(LIST* list);


/**
 * Adder functions
 */

int ListAdd(LIST *list, void *item);
int ListInsert(LIST *list, void *item);
int ListAppend(LIST *list, void *item);
int ListPrepend(LIST *list, void *item);

/**
 * Mover functions
 */

void *ListFirst(LIST *list);
void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);

/**
 * Remover functions
 * and concat
 */

void *ListRemove(LIST *list);
void ListConcat(LIST *list1, LIST *list2);
void ListFree(LIST *list, ItemFree itemFree);
void *ListTrim(LIST *list);


void *ListSearch(LIST *list, Comparator comparator, void* comparisonArg);
