#pragma warning(disable: 4996)

#include "my402list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* transaction for sort */
typedef struct tagTransaction
{
	char dp_wd[1 + 1];
	char date[10 + 1];
	char amount[12 + 1];
	char dscrp[24 + 1];

} Transaction;

void testLength(My402List* theList)
{
	printf("Length of the list: %d\n", My402ListLength(theList));
}

void testEmpty(My402List* theList)
{
	printf("Is the list empty?(1:true, 0:false): %d\n", My402ListEmpty(theList));
}

void testAppend(My402List* theList)
{
	FILE* fp = NULL;
	Transaction trn = { "", "", "", "" };
	char buff[1 + 10 + 12 + 24 + 1 + 1] = "";	//char + newline + \0

												/* Open the file */
	fp = fopen("test.tfile", "rb");
/*
	if (fp == NULL) //The file is missing
	{
		printf("ERROR: The file does not exist\n");
		//	fgets(buff, 48, stdin); //need to implement stdin
		//return (0);
	}
*/

	/* Read one line to the EOF one by one */
	while (fgets(buff, 49, fp) != NULL)
	{
		Transaction* trn = (Transaction*)malloc(sizeof(Transaction));
		memset(trn, 0, sizeof(Transaction));
		sscanf(buff, "%s\t%s\t%s\t%[a-zA-Z ]", trn->dp_wd, trn->date, trn->amount, trn->dscrp);

		/*DEBUG_START*/
		int temp2 = My402ListAppend(theList, trn);
		printf("Expected(1) : %d\n", temp2);

		My402ListElem* elemFirst = My402ListFirst(theList);
		Transaction* trnsFirst = (Transaction*)elemFirst->obj;
		printf("1st elem discription: %s\n", trnsFirst->dscrp);
		printf("1st elem address: %p\n", elemFirst->obj);

		My402ListElem* elemLast = My402ListLast(theList);
		Transaction* trnsLast = (Transaction*)elemLast->obj;
		printf("Last elem discription: %s\n", trnsLast->dscrp);
		printf("Last elem address: %p\n", elemLast->obj);

		/*DEBUG_END*/
	}
	fclose(fp);

}

void testPrepend(My402List* theList)
{
	FILE* fp = NULL;
	Transaction trn = { "", "", "", "" };
	char buff[1 + 10 + 24 + 12 + 1 + 1] = "";	//char + newline + \0

												/* Open the file */
	fp = fopen("test.tfile", "rb");
	/*if (fp == NULL) //The file is missing
	{
		printf("ERROR: The file does not exist\n");
		//	fgets(buff, 48, stdin); //need to implement stdin
		//return (0);
	}*/

	/* Read one line to the EOF one by one */
	while (fgets(buff, 49, fp) != NULL)
	{
		Transaction* trn = (Transaction*)malloc(sizeof(Transaction));
		memset(trn, 0, sizeof(Transaction));
		sscanf(buff, "%s\t%s\t%s\t%[a-zA-Z ]", trn->dp_wd, trn->date, trn->amount, trn->dscrp);

		/*DEBUG_START*/
		int temp2 = My402ListPrepend(theList, trn);
		printf("Expected(1) : %d\n", temp2);

		My402ListElem* elemFirst = My402ListFirst(theList);
		Transaction* trnsFirst = (Transaction*)elemFirst->obj;
		printf("1st elem discription: %s\n", trnsFirst->dscrp);
		printf("1st elem address: %p\n", elemFirst->obj);

		My402ListElem* elemLast = My402ListLast(theList);
		Transaction* trnsLast = (Transaction*)elemLast->obj;
		printf("Last elem discription: %s\n", trnsLast->dscrp);
		printf("Last elem address: %p\n", elemLast->obj);

		/*DEBUG_END*/
	}
	fclose(fp);
}
void testNext(My402List* theList)
{
	/*This is the options which element you want to refer */
	//My402ListElem* elemNext = My402ListNext(theList, &theList->anchor);
	My402ListElem* elemNext = My402ListNext(theList, theList->anchor.next);
	//My402ListElem* elemNext = My402ListNext(theList, theList->anchor.next->next);
	//My402ListElem* elemNext = My402ListNext(theList, theList->anchor.next->next->next);
	//My402ListElem* elemNext = My402ListNext(theList, theList->anchor.next->next->next->next);
	if(elemNext != NULL)
	{
		Transaction* tranNext = (Transaction*)elemNext->obj;
		printf("The next element: %s\n", tranNext->dscrp);
	}
	else
	{
		printf("The next element: %p\n", elemNext);
	}
}

void testPrev(My402List* theList)
{
	My402ListElem* elemPrev = My402ListPrev(theList, &(theList->anchor));
	if (elemPrev != NULL)
	{
		Transaction* tranPrev = (Transaction*)elemPrev->obj;
		printf("The previous element: %s\n", tranPrev->dscrp);
	}
	else
	{
		printf("The next element: %p\n", elemPrev);
	}
}

void testUnlink(My402List* theList)
{
	My402ListUnlink(theList, theList->anchor.next->next);
}

void testUnlinkAll(My402List* theList)
{
	My402ListUnlinkAll(theList);
}

void testInsert(My402List* theList)
{
	FILE* fp = NULL;
	Transaction trn = { "", "", "", "" };
	char buff[1 + 10 + 24 + 12 + 1 + 1] = "";	//char + newline + \0

	/* Open the file */
	fp = fopen("test1.tfile", "rb");
	/*if (fp == NULL) //The file is missing
	{
		printf("ERROR: The file does not exist\n");
		//	fgets(buff, 48, stdin); //need to implement stdin
		return (1);
	}*/

	/* Read one line to the EOF one by one */
	while (fgets(buff, 49, fp) != NULL)
	{
		Transaction* trn = (Transaction*)malloc(sizeof(Transaction));
		memset(trn, 0, sizeof(Transaction));
		sscanf(buff, "%s\t%s\t%s\t%[a-zA-Z ]", trn->dp_wd, trn->date, trn->amount, trn->dscrp);

		/*DEBUG_START*/
		
		int temp2 = My402ListInsertBefore(theList, trn, theList->anchor.next->next);
		printf("Expected(1) : %d\n", temp2);

		My402ListElem* elemFirst = My402ListFirst(theList);
		Transaction* trnsFirst = (Transaction*)elemFirst->obj;
		printf("1st elem discription: %s\n", trnsFirst->dscrp);
		printf("1st elem address: %p\n", elemFirst->obj);

		My402ListElem* elemLast = My402ListLast(theList);
		Transaction* trnsLast = (Transaction*)elemLast->obj;
		printf("Last elem discription: %s\n", trnsLast->dscrp);
		printf("Last elem address: %p\n", elemLast->obj);

		/*DEBUG_END*/
	}
	fclose(fp);
}

void testFirst(My402List* theList)
{
	My402ListElem* elem1st = My402ListFirst(theList);
	Transaction* tran1st = (Transaction*)elem1st->obj;
	printf("The previous element: %s\n", tran1st->dscrp);
}

void testFind(My402List* theList)
{
	/* In this case, find third element */
	My402ListElem* elem1st = My402ListFind(theList, theList->anchor.next->next->next->obj);
	Transaction* tran1st = (Transaction*)elem1st->obj;
	printf("The element: %s\n", tran1st->dscrp);
}

/* main */
int main(int argc, char* argv[])
{

	My402List theList;
	memset(&theList, 0, sizeof(My402List));

	int init = My402ListInit(&theList);
	printf("success of initialization? (expected:1): %d\n", init);
	putchar('\n');

	printf("initial status\n");
	testLength(&theList);
	testEmpty(&theList);
	putchar('\n');

	printf("Append 4 items\n");
	testAppend(&theList);
	testLength(&theList);
	testEmpty(&theList);
	putchar('\n');

	printf("insert 1 item\n");
	testInsert(&theList);
	testLength(&theList);
	testEmpty(&theList);
	testNext(&theList);
	putchar('\n');

	printf("find Beemer monthly payment\n");
	testFind(&theList);
	putchar('\n');


	//testPrepend(theList);

	printf("unlink one element\n");
	testUnlink(&theList);
	testLength(&theList);
	testEmpty(&theList);
	putchar('\n');

	printf("unlink all element\n");
	testUnlinkAll(&theList);
	testLength(&theList);
	testEmpty(&theList);

	getchar();

	return (0);
}