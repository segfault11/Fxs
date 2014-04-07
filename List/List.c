#include "List.h"
#include <memory.h>
#include <stdlib.h>

typedef struct FxsListNode_
{
	struct FxsListNode_* prev;
    struct FxsListNode_* next;
	void* value;
}
FxsListNode;

typedef struct FxsList_
{
	FxsListNode* start;
	FxsListNode* end;	
	size_t size;
}
FxsList;

typedef struct FxsListIterator_
{
	int dir; 					/* direction of the iterator */
	FxsListPtr list;
	FxsListNode* currentNode;
}
FxsListIterator;
 
FxsListPtr FxsListCreate()
{
	FxsListPtr lp = (FxsListPtr)malloc(sizeof(FxsList));

	if (lp) 
	{
	    memset(lp, 0, sizeof(FxsList));
	}
	else
	{
		return NULL;
	}

	lp->start = (FxsListNode*)malloc(sizeof(FxsListNode));
	
	if (lp->start) 
	{
	    memset(lp->start, 0, sizeof(FxsListNode));
	}
	else
	{
		free(lp);
		return NULL;
	}

	lp->end = (FxsListNode*)malloc(sizeof(FxsListNode));
	
	if (lp->end) 
	{
	    memset(lp->end, 0, sizeof(FxsListNode));
	}
	else
	{
		free(lp->start);
		free(lp);
		return NULL;
	}

	lp->start->next = lp->end;
	lp->end->prev = lp->start;

	return lp;
}

void FxsListDestroy(FxsListPtr* l)
{
	if (!l || !*(l))
	{
		return;
	}

	free(*l);
	*l = NULL;
}

int FxsListPushBack(FxsListPtr l, void* val)
{
	FxsListNode* ln = (FxsListNode*)malloc(sizeof(FxsListNode));

	if (!ln)
	{
	    return 0;
	}

	ln->value = val;
	ln->next = l->end;
	ln->prev = l->end->prev;
	l->end->prev->next = ln;
	l->end->prev = ln;	
	l->size++;

	return 1;
}

int FxsListPushFront(FxsListPtr l, void* val)
{
	
	FxsListNode* ln = (FxsListNode*)malloc(sizeof(FxsListNode));

	if (!ln)
	{
	    return 0;
	}

	ln->value = val;
	ln->prev = l->start;
	ln->next = l->start->next;
	l->start->next->prev = ln;
	l->start->next = ln;	
	l->size++;

	return 1;
}

void* FxsListGetBack(FxsListPtr l)
{
	return l->end->prev->value;
}


void* FxsListGetFront(FxsListPtr l)
{
	return l->start->next->value;
}

size_t FxsListGetSize(FxsListPtr l)
{
    return l->size;
}

void FxsListPopFront(FxsListPtr l)
{
	if (!l->size)
	{
		return;
	}

	l->start->next = l->start->next->next;
	free(l->start->next->prev);
	l->start->next->prev = l->start;	

	l->size--;
}

void FxsListPopBack(FxsListPtr l)
{
	if (!l->size)
	{
		return;
	}

	l->end->prev = l->end->prev->prev;
	free(l->end->prev->next);
	l->end->prev->next = l->end;

	l->size--;
}

FxsListIteratorPtr FxsListIteratorCreate(FxsListPtr l, int pos, int dir)
{
	FxsListIteratorPtr li = NULL; 
	
	if (!l) 
	{
	    return NULL;
	}

	if (!l->size) 
	{
	    return NULL;
	}

	li = (FxsListIteratorPtr)malloc(sizeof(FxsListIterator));

	if (!li) 
	{
	    return NULL;
	}

	li->list = l;
	
	if (pos == FXS_LIST_FRONT) 
	{
		li->currentNode = l->start;
	}
	else
	{
		li->currentNode = l->end;
	}

	li->dir = dir;

	return li;
}

void FxsListIteratorDestroy(FxsListIteratorPtr* li)
{
	if (!li || !(*li))
	{
		return;
	}

	free(*li);
	*li = NULL;
}

int FxsListIteratorHasNext(FxsListIteratorPtr li)
{
	if (!li) 
	{
	    return 0;
	}

	if (li->dir == FXS_LIST_FRONT_TO_BACK) 
	{
		if (li->currentNode->next != li->list->end 
		&& li->currentNode != li->list->end)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (li->currentNode->prev != li->list->start 
		&& li->currentNode  != li->list->start)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void* FxsListIteratorNext(FxsListIteratorPtr li)
{
	if (!li) 
	{
	    return 0;
	}

	if (FxsListIteratorHasNext(li)) 
	{
		if (li->dir == FXS_LIST_FRONT_TO_BACK) 
		{
			li->currentNode = li->currentNode->next;
		}
		else
		{
			li->currentNode = li->currentNode->prev;
		}
	 	
		return li->currentNode->value;
	}
	else
	{
		return NULL;
	}
}

void FxsListIteratorRemove(FxsListIteratorPtr li)
{
	FxsListNode* ln = NULL;

	if (!li) 
	{
	  	return; 
	}
	
	if (li->currentNode == li->list->start || li->currentNode == li->list->end) 
	{
		return; 
	}

	li->currentNode->prev->next = li->currentNode->next;
	li->currentNode->next->prev = li->currentNode->prev;
	
	if (li->dir == FXS_LIST_FRONT_TO_BACK)
	{
		ln = li->currentNode->prev;
	}
	else
	{
		ln = li->currentNode->next;
	}
	
	free(li->currentNode);
	li->currentNode = ln;

	li->list->size--;
}

int FxsListIteratorSetPosition(FxsListIteratorPtr li, int pos)
{
    if (!li || !li->list->size)
    {
        return 0;
    }
    
    if (pos == FXS_LIST_FRONT)
    {
        li->currentNode = li->list->start;
    }
    
    if (pos == FXS_LIST_BACK)
    {
        li->currentNode = li->list->end;
    }
    
    return 1;
}

int FxsListIteratorSetDirection(FxsListIteratorPtr li, int dir)
{
	if (!li) 
	{
		return 0;
	}

	li->dir = dir;

	return 1;
}
