#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "linked_list.h"
/*****************************************************************
 ************************  test  case  ***************************
 ****************************************************************/
 
/*****************************************************************
 ************************ just for test***************************
 ****************************************************************/
//printf("%d/%s---------------\n", __LINE__, __FUNCTION__);
#define STRING01 "01:a"
#define STRING02 "02:b"
#define STRING03 "03:c"
#define STRING04 "04:d"
#define STRING05 "05:e"
#define STRING06 "06:f"
#define STRING07 "07:g"

char* test_buf[] = {
	STRING01,
	STRING02,
	STRING03,
	STRING04,
	STRING05,
	STRING06,
	STRING07
};
LIST* test_list = NULL;
struct  test_info
{
	int count;
	int success_count;
	int failed_count;
}test_info;

void print_list_items(void)
{
	LIST_ELEMENT_S* current = NULL;
	while (list_next_element(test_list, &current) != NULL)
		printf("content : %s \n", (char *)current->content);

	printf("count:%d  size: %ld\n", test_list->count, test_list->size);
}
/*****************************test case***********************/
void test_00_list_init(void)
{
	test_info.count++;
	printf("=======================%s===================\n", __FUNCTION__);
	test_list = list_init();
	list_free(test_list);
	print_list_items();
	printf("\n\n");
}

void test_01_list_init(void)
{
	test_info.count++;
	printf("=======================%s===================\n", __FUNCTION__);
	test_list = list_init();
	print_list_items();
	printf("\n\n");
}

void test_02_list_zero(void)
{
	test_info.count++;
	printf("=======================%s===================\n", __FUNCTION__);
	list_zero(test_list);
	print_list_items();
	printf("\n\n");
}

void test_03_list_append(void)
{
	test_info.count++;
	printf("=======================%s===================\n", __FUNCTION__);
	list_append(test_list, STRING01, strlen(STRING01));
	list_append(test_list, STRING02, strlen(STRING02));
	list_append(test_list, STRING03, strlen(STRING03));
	list_append(test_list, STRING04, strlen(STRING04));
	list_append(test_list, STRING05, strlen(STRING05));
	list_append(test_list, STRING06, strlen(STRING06));
	list_append(test_list, STRING07, strlen(STRING07));
	print_list_items();
	printf("\n\n");
}

void test_04_list_find_item(void)
{
	LIST_ELEMENT_S* found_list = NULL;

	test_info.count++;
	printf("=======================%s===================\n", __FUNCTION__);
	printf("dest list:%s\n", STRING03);
	found_list = list_find_item(test_list, STRING03, NULL);
	printf("found list:%s\n", (char *)found_list->content);
	print_list_items();
	printf("\n\n");
}

void test_05_list_insert(void)
{
	LIST_ELEMENT_S* inserted_list = NULL;
	char insert_comment[] = "insert";
	test_info.count++;
	printf("=======================%s===================\n", __FUNCTION__);
	inserted_list = list_find_item(test_list, STRING03, NULL);
	print_list_items();
	//printf("before insert dest list:%s\n", (char *)inserted_list->prev->content);
	list_insert(test_list, insert_comment, strlen(insert_comment), inserted_list);
	//printf("after insert dest list:%s\n", (char *)inserted_list->prev->content);
	printf("\n-------after insert  [  %s  ]  -------\n", insert_comment);
	print_list_items();
	printf("\n<end><%s>\n", __FUNCTION__);
	printf("\n\n");
}

void test_06_list_unlink_item(void)
{
	LIST_ELEMENT_S* unlink_list = NULL;
	test_info.count++;
	printf("==========================================\n");
	printf("<start><%s>\n\n", __FUNCTION__);
	print_list_items();
	list_unlink_item(test_list, STRING03, NULL, 0);
	printf("\n-------after  unlink  [  %s  ]  -------\n", STRING03);
	print_list_items();
	printf("\n<end><%s>\n", __FUNCTION__);
	printf("\n\n\n");
}

void test_07_list_pop_tail(void)
{
	int i = 0;
	int item_number = 0;
	char* pop_content = NULL;
	test_info.count++;
	printf("==========================================\n");
	printf("<start><%s>\n\n", __FUNCTION__);
	/*test case*/
	print_list_items();
	printf("--------------ready to pop----------------\n");
	item_number = test_list->count;
	for (i = 0; i < item_number; i++) {
		pop_content = (char *)list_pop_tail(test_list);
		printf("pop_list[%d]:%s\n", i, (char *)pop_content);
	}
	printf("count:%d  size: %ld\n", test_list->count, test_list->size);
	printf("\n<end><%s>\n", __FUNCTION__);
	printf("\n\n\n");
}

void test_XX_example(void)
{
	test_info.count++;
	printf("==========================================\n");

	printf("\n<end><%s>\n", __FUNCTION__);
	printf("\n\n\n");
}

//printf("%d/%s ---- debug------\n", __LINE__, __FUNCTION__);
/*************************************************************/
int test(void)
{
	#if 0
	test_00_list_init();
	test_01_list_init();
	test_02_list_zero();
	test_03_list_append();
	test_04_list_find_item();
	test_05_list_insert();
	test_06_list_unlink_item();
	test_07_list_pop_tail();
	#else
	char insert_comment[] = "insert";
	LIST_ELEMENT_S* inserted_list = NULL;
	int item_number = 0;
	int i = 0;
	char *pop_content = NULL;

	test_list = list_init();
	list_append(test_list, STRING01, strlen(STRING01));
	list_append(test_list, STRING02, strlen(STRING02));
	list_append(test_list, STRING03, strlen(STRING03));
	list_append(test_list, STRING04, strlen(STRING04));
	list_append(test_list, STRING05, strlen(STRING05));
	list_append(test_list, STRING06, strlen(STRING06));
	list_append(test_list, STRING07, strlen(STRING07));
	printf("\n------after list append-------\n");
	print_list_items();

	inserted_list = list_find_item(test_list, STRING03, NULL);
	list_insert(test_list, insert_comment, strlen(insert_comment), inserted_list);

	item_number = test_list->count;
	printf("\n------ready pop list-------\n");
	for (i = 0; i < item_number; i++) {
		pop_content = (char *)list_pop_tail(test_list);
		printf("pop_list[%d]:%s\n", i, (char *)pop_content);
	}
	printf("\n------after pos list-------\n");
	print_list_items();

	#endif
	return 0;
}

