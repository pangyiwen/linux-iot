#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linked_list.h"
/**
 * Sets a list structure to empty - all null values.
 * warning: Does not remove any items from the list.
 * @param newl a pointer to the list structure to be initialized
 */
void list_zero(LIST* newl)
{
	memset(newl, 0, sizeof(LIST));
	newl->first = NULL;
	newl->last = NULL;
	newl->current = NULL;
	newl->count = newl->size = 0;
}

/**
 * Allocates and initializes a new list structure.
 * @return a pointer to the new list structure
 */
LIST* list_init(void)
{
	LIST* newl = malloc(sizeof(LIST));
	list_zero(newl);
	return newl;
}

/**
 * Append an item to a list.
 * @param alist the list to which the item is to be added
 * @param content the list item content itself
 * @param size the size of the element
 */
void list_append(LIST* alist, void* content, size_t size)
{
	LIST_ELEMENT_S* newl = malloc(sizeof(LIST_ELEMENT_S));
	
	newl->content = content;
	newl->next = NULL;
	newl->prev = alist->last;
	if (alist->first == NULL)
		alist->first = newl;
	else
		alist->last->next = newl;
	alist->last = newl;
	++(alist->count);
	alist->size +=size;
}

void list_insert(LIST* alist, void* content, size_t size, LIST_ELEMENT_S* index)
{
	LIST_ELEMENT_S* newl = malloc(sizeof(LIST_ELEMENT_S));

	if ( index == NULL ) {
		newl->content = content;
		newl->next = NULL;
		newl->prev = alist->last;
		if (alist->first == NULL)
			alist->first = newl;
		else
			alist->last->next = newl;
		alist->last = newl;
		++(alist->count);
		alist->size +=size;
	} else {
		newl->content = content;
		newl->next = index;
		newl->prev = index->prev;

		index->prev = newl;
		if ( newl->prev != NULL )
			newl->prev->next = newl;
		else
			alist->first = newl;

		++(alist->count);
		alist->size += size;
	} 

}

LIST_ELEMENT_S* list_next_element(LIST* alist, LIST_ELEMENT_S** pos)
{
	return *pos = (*pos == NULL) ? alist->first : (*pos)->next;
}

LIST_ELEMENT_S* list_prev_element(LIST* alist, LIST_ELEMENT_S** pos)
{
	return *pos = (*pos == NULL) ? alist->last : (*pos)->prev;
}

/**
 * Finds an element in a list by comparing the content or pointer to the content.  A callback
 * function is used to define the method of comparison for each element.
 * @param alist the list in which the search is to be conducted
 * @param content pointer to the content to look for
 * @param callback pointer to a function which compares each element (NULL means compare by content pointer)
 * @return the list element found, or NULL
 */
LIST_ELEMENT_S* list_find_item(LIST* alist, void* content, int(*callback)(void*, void*))
{
	LIST_ELEMENT_S* rc = NULL;

	if (alist->current != NULL && ((callback == NULL && alist->current->content == content) ||
		(callback != NULL && callback(alist->current->content, content))))
		rc = alist->current;
	else {
		LIST_ELEMENT_S* current = NULL;

		/*find the content*/
		while (list_next_element(alist, &current) != NULL) {
			if (callback == NULL) {
				if (current->content == content) {
					rc = current;
					break;
				}
			} else {
				if (callback(current->content, content)) {
					rc = current;
					break;
				}
			} 
		}
		if (rc != NULL)
			alist->current = rc;	
	}
	return rc;
}

/**
 * Removes and optionally frees an element in a list by comparing the content.
 * A callback function is used to define the method of comparison for each element.
 * @param alist the list in which the search is to be conducted
 * @param content pointer to the content to look for
 * @param callback pointer to a function which compares each element
 * @param free_content boolean value to indicate whether the item found is to be freed
 * @return 1=item removed, 0=item not removed
 */
int list_unlink_item(LIST* alist, void* content, int(*callback)(void*, void*), int free_content)
{
	LIST_ELEMENT_S* next = NULL;
	LIST_ELEMENT_S* saved = alist->current;
	int saved_deleted = 0;

	if (!list_find_item(alist, content, callback))
		return 0;
	if (alist->current->prev == NULL)
		/*so this is the first element, and we have to update the "first" pointer*/
		alist->first = alist->current->next;
	else
		alist->current->prev->next = alist->current->next;
	if (alist->current->next == NULL)
		alist->last = alist->current->prev;
	else
		alist->current->next->prev = alist->current->prev;
	next = alist->current->next;
	if (free_content) {
		free(alist->current->content);
		alist->current->content = NULL;
	}
	if (saved == alist->current)
		saved_deleted = 1;
	free(alist->current);
	if (saved_deleted)
		alist->current = next;
	else
		alist->current = saved;
	--(alist->count);
	
	return 1;/*successfully removed item*/
}

//printf("%d/%s---------------\n", __LINE__, __FUNCTION__);
int main(void)
{
	/*
	void list_zero(LIST* newl);
	LIST* list_init(void);
	void list_append(LIST* alist, void* content, size_t size);
	void list_insert(LIST* alist, void* content, size_t size, LIST_ELEMENT_S* index);
	*/
	LIST* test_list = NULL;
	int content01 = 10;
	int content02 = 20;
	int content03 = 30;
	int content04 = 40;
	int content05 = 50;
	int content06 = 60;
	int content00 = 100;
	LIST_ELEMENT_S* inserted_list = NULL;

	test_list = list_init();
	list_zero(test_list);

	list_append(test_list, &content01, sizeof(int));
	list_append(test_list, &content02, sizeof(int));
	list_append(test_list, &content03, sizeof(int));
	list_append(test_list, &content04, sizeof(int));
	list_append(test_list, &content05, sizeof(int));
	list_append(test_list, &content06, sizeof(int));

	test_list->current = test_list->first;
	inserted_list = list_find_item(test_list, &content05, NULL);
	list_insert(test_list, &content00, sizeof(int),inserted_list);
	list_unlink_item(test_list, &content05, NULL, 0);

	test_list->current = test_list->first;

	printf("count:%d  size: %ld\n", test_list->count, test_list->size);
	while (test_list->current != NULL) {
		printf("content : %d \n", *(int *)test_list->current->content);
		test_list->current = test_list->current->next;
	}

	return 0;
}