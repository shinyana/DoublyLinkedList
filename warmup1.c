//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs402.h"
#include "my402list.h"
#include <time.h>
#include <sys/stat.h>
/*
#include <unistd.h>
*/

#define NUM_DATE 11
#define NUM_DECIMAL 2
#define NUM_DESCRIPTION 25
#define COMMA_PATTERN 3
#define MILLION 10000000
#define MAXINT 2147483647

typedef struct tagTransaction
{
	char dp_wd;
	char date[NUM_DATE];
	int amount_dollar;
	int amount_cent;
	char description[NUM_DESCRIPTION];
} Transaction;

void printDigit(double* balance)
{
	char output_balance[] = "?,???,???.??";
	int flag = 0;

	if (*balance >= MILLION)
	{
	}
	else if (*balance <= -MILLION)
	{
		flag = 1;
	}
	else
	{
		if (*balance < 0)
		{
			flag = 1;
		}
		int len_balance = strlen(output_balance);
		memset(output_balance, '\0', len_balance);
		int count_balance = snprintf(output_balance, sizeof(output_balance), "%.2f", *balance);

		/* The pointer point to the end of output array */
		char* p_balance = output_balance + count_balance;

		int i = len_balance;
		/* fill decial digits */
		do
		{
			p_balance--;
			output_balance[--i] = *p_balance;
			count_balance--;
		}
		while (*p_balance != '.');

		if (*balance < 0)
		{
			count_balance--;
		}
		int count_int = 0;
		/* fill integer digits */
		while (count_int != count_balance)
		{
			count_int++;
			p_balance--;
			output_balance[--i] = *p_balance;
			if (count_int % COMMA_PATTERN == 0 && count_int != count_balance)
			{
				output_balance[--i] = ',';
			}
		}
		/* fill remaining space into space */
		while (i != 0)
		{
			output_balance[--i] = ' ';
		}
	}

	if (flag == 1)
	{
		fprintf(stdout, " (%s) |\n", output_balance);
	}
	else
	{
		fprintf(stdout, "  %s  |\n", output_balance);
	}
}

void printBalance(Transaction* trn, double* balance)
{
	if (trn->dp_wd == '+')
	{
		*balance += trn->amount_dollar + trn->amount_cent / 100.00;
	}
	else
	{
		*balance -= trn->amount_dollar + trn->amount_cent / 100.00;
	}
	printDigit(balance);
}

void printAmount(Transaction* trn)
{
	char output_dollar[] = "?,???,???";
	char output_cent[] = "??";

	if (trn->amount_dollar < MILLION)
	{
		int len_dollar_digit = strlen(output_dollar);
		int len_cent_digit = strlen(output_cent);
		memset(output_dollar, '\0', len_dollar_digit);
		memset(output_cent, '\0', len_cent_digit);

		int count_dollar = snprintf(output_dollar, sizeof(output_dollar), "%d", trn->amount_dollar);
		int count_cent = snprintf(output_cent, sizeof(output_cent), "%d", trn->amount_cent);

		char* p_dollar = output_dollar + count_dollar;

		int count = 0;
		int i = len_dollar_digit;
		/* fill integer digits */
		while (count != count_dollar)
		{
			count++;
			p_dollar--;
			output_dollar[--i] = *p_dollar;
			if (count % COMMA_PATTERN == 0 && count != count_dollar)
			{
				output_dollar[--i] = ',';
			}
		}
		/* fill remaining space into space */
		while (i != 0)
		{
			output_dollar[--i] = ' ';
		}

		if (count_cent != len_cent_digit)
		{
			output_cent[1] = output_cent[0];
			output_cent[0] = '0';
		}
	}
	if (trn->dp_wd == '-')
	{
		fprintf(stdout, " (%s.%s) |", output_dollar, output_cent);
	}
	else
	{
		fprintf(stdout, "  %s.%s  |", output_dollar, output_cent);
	}
}

void printDescription(Transaction* trn)
{
	int i = 0;
	for (i = 0; i < NUM_DESCRIPTION - 1; i++)
	{
		if (trn->description[i] == '\0')
		{
			trn->description[i] = ' ';
		}
	}
	fprintf(stdout, " %s |", trn->description);
}

void printDate(Transaction* trn)
{
	time_t timer = atoi(trn->date);
	char* p = ctime(&timer);
	fprintf(stdout, "| ");
	char* start_ptr = p;

	/* print a day */
	char* tab_ptr = strchr(start_ptr, ' ');
	if (tab_ptr != NULL)
	{
		*tab_ptr++ = '\0';
	}

	fprintf(stdout, "%s ", start_ptr);
	start_ptr = tab_ptr;

	/* print a month */
	tab_ptr = strchr(start_ptr, ' ');
	if (tab_ptr != NULL)
	{
		*tab_ptr++ = '\0';
	}
	fprintf(stdout, "%s ", start_ptr);
	start_ptr = tab_ptr;

	/* print a day */
	tab_ptr = strchr(start_ptr, ' ');

	/* if the day has one digit */
	char* opt_ptr = tab_ptr + 2;
	if (*opt_ptr == ' ')
	{
		tab_ptr = opt_ptr;
	}

	if (tab_ptr != NULL)
	{
		*tab_ptr++ = '\0';
	}
	fprintf(stdout, "%s ", start_ptr);
	start_ptr = tab_ptr;

	/* skip time section */
	tab_ptr = strchr(start_ptr, ' ');
	if (tab_ptr != NULL)
	{
		*tab_ptr++ = '\0';
	}

	/* print a year */
	start_ptr = tab_ptr;
	tab_ptr = strchr(start_ptr, '\n');
	if (tab_ptr != NULL)
	{
		*tab_ptr++ = '\0';
	}

	fprintf(stdout, "%s |", start_ptr);
}

void outputResult(My402List* the_list)
{
	fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
	fprintf(stdout, "|       Date      | Description              |         Amount |        Balance |\n");
	fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");

	My402ListElem* p_elem = NULL;
	double balance = 0.0;

	for (p_elem = My402ListFirst(the_list); p_elem != NULL; p_elem = My402ListNext(the_list, p_elem))
	{
		Transaction* trn = (Transaction*)p_elem->obj;
		printDate(trn);
		printDescription(trn);
		printAmount(trn);
		printBalance(trn, &balance);
	}
	fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
}

void BubbleForward(My402List* pList, My402ListElem** pp_elem1, My402ListElem** pp_elem2)
{
	My402ListElem *elem1 = (*pp_elem1), *elem2 = (*pp_elem2);
	void *obj1 = elem1->obj, *obj2 = elem2->obj;
	My402ListElem* elem1prev = My402ListPrev(pList, elem1);
	My402ListElem* elem2next = My402ListNext(pList, elem2);

	My402ListUnlink(pList, elem1);
	My402ListUnlink(pList, elem2);
	if (elem1prev == NULL)
	{
		(void)My402ListPrepend(pList, obj2);
		*pp_elem1 = My402ListFirst(pList);
	}
	else
	{
		(void)My402ListInsertAfter(pList, obj2, elem1prev);
		*pp_elem1 = My402ListNext(pList, elem1prev);
	}
	if (elem2next == NULL)
	{
		(void)My402ListAppend(pList, obj1);
		*pp_elem2 = My402ListLast(pList);
	}
	else
	{
		(void)My402ListInsertBefore(pList, obj1, elem2next);
		*pp_elem2 = My402ListPrev(pList, elem2next);
	}
}

void sortAscending(My402List* the_list)
{
	int num_elem = My402ListLength(the_list);

	My402ListElem* p_elem = NULL;
	int i = 0;

	for (i = 0; i < num_elem; i++)
	{
		int j = 0;
		int something_swapped = FALSE;
		My402ListElem* p_next_elem = NULL;

		for (p_elem = My402ListFirst(the_list), j = 0; j < num_elem - i - 1; p_elem = p_next_elem, j++)
		{
			Transaction* cur_trn = (Transaction*)(p_elem->obj);
			time_t cur_timer = atoi(cur_trn->date);

			p_next_elem = My402ListNext(the_list, p_elem);
			Transaction* next_trn = (Transaction*)(p_next_elem->obj);
			time_t next_timer = atoi(next_trn->date);

			if (cur_timer > next_timer)
			{
				BubbleForward(the_list, &p_elem, &p_next_elem);
				something_swapped = TRUE;
			}
		}
		if (!something_swapped) break;
	}
}
void showFieldError(My402List* the_list)
{
	fprintf(stderr, "ERROR: The number of fields is not appropriate.(Expected: 4 fields with 3 tabs)\n");
	fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
	exit(EXIT_FAILURE);
}
void scanOneLine(My402List* the_list, char* buf)
{
	Transaction* trn;
	if (!(trn = (Transaction*)malloc(sizeof(Transaction))))
	{
		fprintf(stderr, "ERROR: Fail to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	memset(trn, 0, sizeof(Transaction));

	/* first field */
	char* start_ptr = buf;
	char* tab_ptr = strchr(start_ptr, '\t');

	if (tab_ptr == NULL)
	{
		showFieldError(the_list);
	}

	*tab_ptr++ = '\0';
	trn->dp_wd = *start_ptr;

	if (!(trn->dp_wd == '+' || trn->dp_wd == '-'))
	{
		fprintf(stderr, "ERROR: The first character in the first field must be '+' or '-'.\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}

	if (strlen(start_ptr) != 1)
	{
		fprintf(stderr, "ERROR: The expected length of the string is one in the first field.\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}
	start_ptr = tab_ptr;

	/* Second field */
	tab_ptr = strchr(start_ptr, '\t');
	if (tab_ptr == NULL)
	{
		showFieldError(the_list);
	}

	*tab_ptr++ = '\0';
	strncpy(trn->date, start_ptr, NUM_DATE);
	int i = 0;
	for (i = 0; i < NUM_DATE - 1; i++)
	{
		if (!(trn->date[i] >= '0' && trn->date[i] <= '9'))
		{
			fprintf(stderr, "ERROR: The expected string consists of ten numbers in the second field.\n");
			fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
			exit(EXIT_FAILURE);
		}
	}

	if (trn->date[NUM_DATE - 1] != '\0')
	{
		fprintf(stderr,	"ERROR: The input length is longer than the expected length of the string in the second field. (Expected:10)\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}
	time_t timer_now = time(NULL);
	time_t timer_aqu = atoi(trn->date);
	if (difftime(timer_now, timer_aqu) < 0)
	{
		fprintf(stderr, "ERROR: The expected date in the second field must be past from now.\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}
	My402ListElem* p_elem = NULL;
	for (p_elem = My402ListFirst(the_list); p_elem != NULL; p_elem = My402ListNext(the_list, p_elem))
	{
		Transaction* temp = (Transaction*)p_elem->obj;
		if (atoi(temp->date) == atoi(trn->date))
		{
			fprintf(stderr, "ERROR: The date in the second field already exists in the list.\n");
			fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
			exit(EXIT_FAILURE);
		}
	}
	start_ptr = tab_ptr;
	

	/* Third field */
	tab_ptr = strchr(start_ptr, '\t');
	if (tab_ptr == NULL)
	{
		showFieldError(the_list);
	}

	*tab_ptr++ = '\0';
	char* opt_ptr = strchr(start_ptr, '.');
	if (opt_ptr == NULL)
	{
		fprintf(stderr, "ERROR: A period is missing in the third field([dollor].[cent]).\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}
	*opt_ptr++ = '\0';
	if (strlen(opt_ptr) != NUM_DECIMAL)
	{
		fprintf(stderr, "ERROR: The number of decimal must be two.\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}
	char* p = NULL;

	/* check whether string consists of numbers for integer */
	for (p = start_ptr; *p != '\0'; p++)
	{
		if (!(*p >= '0' && *p <= '9'))
		{
			fprintf(stderr, "ERROR: The expected string consists of numbers in the third field.\n");
			fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
			exit(EXIT_FAILURE);
		}
	}
	/* check whether string consists of numbers for decimal */
	for (p = opt_ptr; *p != '\0'; p++)
	{
		if (!(*p >= '0' && *p <= '9'))
		{
			fprintf(stderr, "ERROR: The expected string consists of numbers in the third field.\n");
			fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
			exit(EXIT_FAILURE);
		}
	}

	trn->amount_dollar = atoi(start_ptr);
	trn->amount_cent = atoi(opt_ptr);
	if (trn->amount_dollar >= MAXINT)
	{
		fprintf(stderr, "ERROR: Input integer in the third field must be smaller than 2147483647.\n");
		fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
		exit(EXIT_FAILURE);
	}

	start_ptr = tab_ptr;

	/* Fourth field */
	tab_ptr = strchr(start_ptr, '\t');
	if (tab_ptr != NULL)
	{
		showFieldError(the_list);
	}

	tab_ptr = strchr(start_ptr, '\n');
	if (tab_ptr != NULL)
	{
		*tab_ptr++ = '\0';
		strncpy(trn->description, start_ptr, NUM_DESCRIPTION);
		if (trn->description[NUM_DESCRIPTION - 1] != '\0')
		{
			trn->description[NUM_DESCRIPTION - 1] = '\0';
		}
	}

	/* Add transaction to the list */
	if (!(My402ListAppend(the_list, trn)))
	{
		fprintf(stderr, "ERROR: Fail to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
}

void readfile(FILE* fp, My402List* the_list)
{
	char buf[1026]; /*newline and null*/
	memset(buf, '\n', sizeof(buf));
	
	/* Read one line */
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//if (strlen(buf) > 1024)
		if (buf[1024] != '\n')
		{
			fprintf(stderr, "ERROR: The length of transaction is too long.\n");
			fprintf(stderr, "See input file: line %d\n", My402ListLength(the_list) + 1);
			exit(EXIT_FAILURE);
		}
		scanOneLine(the_list, buf);
	}
}

void openFile(My402List* the_list, char* argv[])
{
	FILE* fp = NULL;
	struct stat st;
	
	stat(argv[2], &st);

	/* Check whether 2nd argument is directory or not */
	if ((st.st_mode & S_IFMT) == S_IFDIR) 
	{
		fprintf(stderr, "ERROR: Input file %s is directory.\n", argv[2]);
		exit(EXIT_FAILURE);
	}

//	if ((st.st_mode & ~S_IFMT) == S_IRUSR + S_IWUSR)
//	{
//		fprintf(stderr, "ERROR: Input file %s cannot be opened - access denies.\n", argv[2]);
//		exit(EXIT_FAILURE);
//	}

	/* Use std input */
	if (argv[2] == NULL)
	{
		fp = stdin;
	}
	/* Open a file */
	else
	{
		fp = fopen(argv[2], "r");
		if (fp == NULL)
		{
			fprintf(stderr, "ERROR: Input file %s does not exist in the directory.\n", argv[2]);
			exit(EXIT_FAILURE);
		}
	}
	/* Read a file */
	readfile(fp, the_list);
	fclose(fp);

	if (the_list->anchor.next == &the_list->anchor && the_list->anchor.prev == &the_list->anchor)
	{
		fprintf(stderr, "ERROR: There is no transactions from the input.\n");
		exit(EXIT_FAILURE);
	}
}

void showUsage()
{
	fprintf(stderr, "       Usage: warmup1 sort [tfile]\n");
	exit(EXIT_FAILURE);
}

void errorCheckArg(int argc, char* argv[])
{
	/* Check the number of argument */
	if (argc > 3)
	{
		fprintf(stderr, "ERROR: The number of argument from command line is smaller or equal to three.\n");
		showUsage();
	}
	/* 1st argument is missing */
	if (argv[1] == NULL)
	{
		fprintf(stderr, "ERROR: Put the [sort] command in the first argument.\n");
		showUsage();
	}
	/* 1st argument error check */
	int len = strlen(argv[1]);
	if (strncmp(argv[1], "sort", len) != 0 || len < (int)strlen("sort"))
	{
		fprintf(stderr, "ERROR: Invalid command name in the first argument.\n");
		showUsage();
	}
}

int main(int argc, char* argv[])
{
	My402List the_list;
	memset(&the_list, 0, sizeof(My402List));

	/* Initialization */
	if (!(My402ListInit(&the_list)))
	{
		fprintf(stderr, "ERROR: Fail to initialize the list.\n");
		exit(EXIT_FAILURE);
	}

	/* Check argument */
	errorCheckArg(argc, argv);

	/* Read a file or stdin */
	openFile(&the_list, argv);

	/* Sort transactions in ascending order with date */
	sortAscending(&the_list);

	/* Output the result */
	outputResult(&the_list);

	return EXIT_SUCCESS;
}
