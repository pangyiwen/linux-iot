#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "linked_list.h"
#include "list_interface_test.h"
/**
 * Sets a list structure to empty - all null values.
 * warning: Does not remove any items from the list.
 * @param newl a pointer to the list structure to be initialized
 */
void list_zero(LIST* newl)
{	
	pthread_mutex_lock(&newl->list_mutex);
	memset(newl, 0, sizeof(LIST));
	/*newl->first = NULL;
	newl->last = NULL;
	newl->current = NULL;
	newl->count = newl->size = 0;*/
	pthread_mutex_unlock(&newl->list_mutex);
}

/**
 * Allocates and initializes a new list structure.
 * @return a pointer to the new list structure
 */
LIST* list_init(void)
{
	LIST* newl = malloc(sizeof(LIST));
	pthread_mutex_init(&newl->list_mutex, NULL);
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
	alist->size += size;
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
	} else {
		newl->content = content;
		newl->next = index;
		newl->prev = index->prev;

		index->prev = newl;
		if ( newl->prev != NULL )
			newl->prev->next = newl;
		else
			alist->first = newl;
	} 
		++(alist->count);
		alist->size += size;
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
	alist->size -= sizeof(content);
	return 1;/*successfully removed item*/
}

/**
 * Removes but does not free the last item in a list.
 * @param alist the list from which the item is to be removed
 * @return the last item removed (or NULL if none was)
 */
void* list_pop_tail(LIST* alist)
{
	void* content = NULL;
	if (alist->count>0) {
		LIST_ELEMENT_S* last = alist->last;
		if (alist->current == last)
			alist->current = last->prev;
		if (alist->first == last)
			alist->first = NULL;
		content = last->content;
		alist->last = alist->last->prev;
		if (alist->last)
			alist->last->next = NULL;

		free(last);
		alist->size -= strlen(content);
		--(alist->count);
	}
	return content;
}

/**
 * Removes and frees all items in a list, leaving the list ready for new items.
 * @param alist the list to which the operation is to be applied
 */
void list_empty(LIST* alist)
{
	while (alist->first != NULL) {
		LIST_ELEMENT_S* first = alist->first;
		if (first->content != NULL) {
			free(first->content);
			first->content = NULL;
		}
		alist->first = first->next;
		free(first);
	}
	alist->count = 0;
	alist->size = 0;
	alist->current = alist->first = alist->last = NULL;
}

/**
 * Removes and frees all items in a list, and frees the list itself
 * @param alist the list to which the operation is to be applied
 */
void list_free(LIST* alist)
{
	list_empty(alist);
	free(alist);
}

void main(void)
{
	test();
}

