#include "my402list.h"
#include <stdlib.h>
#include <string.h>

/*
 *PRE : the_list != NULL
 *POST: no changes in the list
 */
int My402ListLength(My402List* the_list)
{
	return the_list->num_members;
}

/*
 *PRE : the_list != NULL
 *POST: no changes in the list
 */
int My402ListEmpty(My402List* the_list)
{
	return the_list->anchor.next == &the_list->anchor && the_list->anchor.prev == &the_list->anchor ? TRUE : FALSE;
}

/*
 *PRE : the_list != NULL
 *POST: the_elem is allocated
 *the_list->num_members++
 */
My402ListElem* createElement(My402List* the_list, void* the_obj)
{
	My402ListElem* the_elem;
	if(!(the_elem = (My402ListElem*)malloc(sizeof(My402ListElem)))){
		return NULL;
	}
	memset(the_elem, 0, sizeof(My402ListElem));
	
	the_elem->obj = the_obj;
	the_elem->next = NULL;
	the_elem->prev = NULL;

	the_list->num_members++;

	return the_elem;
}
/*
 *PRE : the_list != NULL
 *      the_elem != NULL
 *POST: anchor.next != NULL
 *      anchor.prev != NULL
 */
int justAddElement(My402List* the_list, My402ListElem* the_elem)
{
	the_elem->next = &(the_list->anchor);
	the_elem->prev = &(the_list->anchor);
	the_list->anchor.next = the_elem;
	the_list->anchor.prev = the_elem;

	return TRUE;
}

/*
 *PRE : the_list != NULL
 *POST: the_list->num_members++
 */
int My402ListAppend(My402List* the_list, void* the_obj)
{
	/* create new element */
	My402ListElem* newElem;
	if(!(newElem = createElement(the_list, the_obj)))
	{
		return FALSE;
	}

	/* there is no elements in the list */
	if (the_list->anchor.prev == &the_list->anchor)
	{
		return justAddElement(the_list, newElem);
	}
	/* there are elements in the list */
	newElem->prev = the_list->anchor.prev;
	newElem->next = the_list->anchor.prev->next;
	the_list->anchor.prev->next = newElem;
	the_list->anchor.prev = newElem;

	return TRUE;
}

/*
 *PRE : the_list != NULL
 *POST: the_list->num_members++
 */
int My402ListPrepend(My402List* the_list, void* the_obj)
{
	/* create new element */
	My402ListElem* newElem;
	if (!(newElem = createElement(the_list, the_obj)))
	{
		return FALSE;
	}

	/* there is no elements in the list */
	if (the_list->anchor.next == &the_list->anchor)
	{
		return justAddElement(the_list, newElem);
	}
	/* there are elements in the list */
	newElem->prev = the_list->anchor.next->prev;
	newElem->next = the_list->anchor.next;
	the_list->anchor.next->prev = newElem;
	the_list->anchor.next = newElem;

	return TRUE;
}

/*
 *PRE : the_list != NULL
 *      the_elem points to the address in the list
 *POST: the_list->num_members--
 */
void My402ListUnlink(My402List* the_list, My402ListElem* the_elem)
{
		My402ListElem* np = the_elem->next;
		My402ListElem* pp = the_elem->prev;
		np->prev = pp;
		pp->next = np;
		free(the_elem);
		the_list->num_members--;
}

/*
 *PRE : the_list != NULL
 *POST: the_list->num_members = 0
 *      the_list->anchor = {NULL,NULL,NULL)
 */
void My402ListUnlinkAll(My402List* the_list)
{
	/* point to first element and second element in the list */
	My402ListElem* p = the_list->anchor.next;
	My402ListElem* np = the_list->anchor.next;

	/* delete all elements in the list */
	while (np != &(the_list->anchor))
	{
		np = p->next;
		free(p);
		p = np;

		the_list->num_members--;
	}
	/* make the list empty */
	the_list->anchor.next = &the_list->anchor;
	the_list->anchor.prev = &the_list->anchor;
}

/*
 *PRE : the_list != NULL
 *      the_elem points to the address in the list or null
 *POST: num_members++
 */
int My402ListInsertBefore(My402List* the_list, void* the_obj, My402ListElem* the_elem)
{
	if (the_elem == NULL)
	{
		return My402ListPrepend(the_list, the_obj);
	}
	
	My402ListElem* newElem;
	if (!(newElem = createElement(the_list, the_obj)))
	{
		return FALSE;
	}
		
	My402ListElem* p = the_elem;
	My402ListElem* pp = the_elem->prev;
	p->prev = newElem;
	pp->next = newElem;
	newElem->next = p;
	newElem->prev = pp;

	return TRUE;
}

/*
 *PRE : the_list != NULL
 *      the_elem points to the address in the list or null
 *POST: num_members++
 */
int My402ListInsertAfter(My402List* the_list, void* the_obj, My402ListElem* the_elem)
{
	if (the_elem == NULL)
	{
		return My402ListAppend(the_list, the_obj);
	}

	My402ListElem* newElem;
	if (!(newElem = createElement(the_list, the_obj)))
	{
		return FALSE;
	}	

	My402ListElem* p = the_elem;
	My402ListElem* np = the_elem->next;
	p->next = newElem;
	np->prev = newElem;
	newElem->next = np;
	newElem->prev = p;

	return TRUE;
}

/*
 *PRE : the_list != NULL
 *POST: no changes in the list
 */
My402ListElem* My402ListFirst(My402List* the_list)
{
	if(the_list->anchor.next == &(the_list->anchor))
	{
		return FALSE;
	}
	return the_list->anchor.next;
}

/*
 *PRE : the_list != NULL
 *POST: no changes in the list
 */
My402ListElem* My402ListLast(My402List* the_list)
{
	if (the_list->anchor.next == &(the_list->anchor))
	{
		return FALSE;
	}
	return the_list->anchor.prev;
}

/*
 *PRE : the_list != NULL, the_elem != NULL
 *POST: no changes in the list
 */
My402ListElem* My402ListNext(My402List* the_list, My402ListElem* the_elem)
{
	if (the_elem->next == &(the_list->anchor))
	{
		return NULL;
	}
	return the_elem->next;
}

/*
 *PRE : the_list != NULL, the_elem != NULL
 *POST: no changes in the list
 */
My402ListElem* My402ListPrev(My402List* the_list, My402ListElem* the_elem)
{
	if (the_elem->prev == &(the_list->anchor))
	{
		return NULL;
	}
	return the_elem->prev;
}

/*
 *PRE : the_list != NULL
 *POST: no changes in the list
 */
My402ListElem* My402ListFind(My402List* the_list, void* the_obj)
{
	My402ListElem* p = NULL;

	for (p = My402ListFirst(the_list); p != NULL; p = My402ListNext(the_list, p))
	{
		if (p->obj == the_obj)
		{
			return p;
		}
	}
	return NULL;
}

/*
 *PRE : nothing
 *POST: the_list->num_members = 0
 *the_list->anchor = {NULL,NULL,NULL}
 */
int My402ListInit(My402List* the_list)
{
	/* num_members and anchor need to be set */
	if(the_list == NULL)
	{
		return FALSE;
	}

	the_list->num_members = 0;
	the_list->anchor.obj = NULL;
	the_list->anchor.next = &the_list->anchor;
	the_list->anchor.prev = &the_list->anchor;

	return TRUE;
}
