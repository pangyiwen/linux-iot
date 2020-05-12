#ifndef __LINKED_LIST_H_
#define __LINKED_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_element
{
	struct list_element *prev,
	            		*next;
	void* content;      /*pointer to element content*/
}LIST_ELEMENT_S;

typedef struct
{
	LIST_ELEMENT_S 	*first,   	/*first element in the list */
	     			*last,		/*last element in the list */
	     			*current;	/*current element in the list, for iteration */
	int count;					/*count of items*/
	size_t size;         		/*heap storage used*/
} LIST; 

void list_zero(LIST* newl);
LIST* list_init(void);

void list_append(LIST* alist, void* content, size_t size);
void list_insert(LIST* alist, void* content, size_t size, LIST_ELEMENT_S* index);

LIST_ELEMENT_S* list_find_item(LIST* alist, void* content, int(*callback)(void*, void*));
int list_unlink_item(LIST* alist, void* content, int(*callback)(void*, void*), int free_content);

#ifdef __cplusplus
}
#endif

#endif