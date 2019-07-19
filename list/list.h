#ifndef __LIST_H_
#define __LIST_H_

#define MAX_SIZE       32
#define FAILURE       -1
#define SUCCESS        0

typedef struct list{
	char name[MAX_SIZE];
	int val;

	int num; //the number of node in the list
	struct list* head;
	struct list* pre;
	struct list* next;

}info_list;

typedef enum {
    LIST_GET_NODE_CMD_NUM = 1,
    LIST_GET_NODE_CMD_VAL,
    LIST_GET_NODE_CMD_NAME,

    LIST_GET_NODE_CMD_BUTT
}LIST_GET_NODE_MODE_E;


static int list_init_node(info_list * init_list);
static int list_add_tail_node(info_list* master_list, info_list* dest_node);
static int list_add_n_node(info_list* master_list, info_list* dest_node, int n);
static int list_get_node(info_list* source_list,info_list* result_list,LIST_GET_NODE_MODE_E mode);

#endif