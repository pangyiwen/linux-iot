#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

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

static info_list* g_list;

typedef enum {
    LIST_GET_NODE_CMD_NUM = 1,
    LIST_GET_NODE_CMD_VAL,
    LIST_GET_NODE_CMD_NAME,

    LIST_GET_NODE_CMD_BUTT
}LIST_GET_NODE_MODE_E;


static info_list* list_init_node(void){
	info_list* create_node;
	char init_name[MAX_SIZE] = "pangyiwen";
/*
	if(init_list != NULL){
		printf("[ERR] The pointer is not expected NULL\n");
		return FAILURE;
 	}
*/
    create_node = (info_list*)malloc(sizeof(info_list));
    memset(create_node,0,sizeof(info_list));

	strcpy(create_node->name,"pangyiwen");
	create_node->val  = 0;

	create_node->num  = 1;
	create_node->head  = create_node;
	create_node->pre   = NULL;
	create_node->next  = NULL;

	return create_node;
}

static int list_add_tail_node(info_list* master_list, info_list* dest_node){
	info_list* tmp_node;

	tmp_node = master_list;

	while(tmp_node->next != NULL){
		tmp_node = tmp_node->next;
	}

	tmp_node->next = dest_node;
	dest_node->num = tmp_node->num + 1;
	dest_node->head = tmp_node->head;
	dest_node->pre = tmp_node;

	return SUCCESS;

}

static int list_add_n_node(info_list* master_list, info_list* dest_node, int n){
	info_list* tmp_node;
	info_list* next_node;

	tmp_node = master_list;

	while(tmp_node->next != NULL){
		if(tmp_node->num == n){
			next_node = tmp_node->next;

			tmp_node->next = dest_node;

			dest_node->num = tmp_node->num + 1;
			dest_node->head = tmp_node->head;
			dest_node->pre = tmp_node;
			dest_node->next = next_node;

			next_node->pre = dest_node;

			while(next_node != NULL){
				next_node->num += 1;
				next_node = next_node->next;
			}
			return SUCCESS;
		}
		tmp_node = tmp_node->next;
	}

	printf("[error] %d is out of the list\n",n);

	return FAILURE;   // error code expected
}

static int list_get_node(info_list* source_list,info_list* result_list,LIST_GET_NODE_MODE_E mode){
    info_list* tmp_list;

    if(source_list == NULL){
        printf("[ERROR] the source_list is null : 0x%p\n",source_list);
        return FAILURE;
    }

    tmp_list = source_list;

    switch(mode){
        case LIST_GET_NODE_CMD_NUM:
            while(tmp_list->num != result_list->num){
                    tmp_list = source_list->next;
                    if(tmp_list == NULL){
                        printf("[SUCCESS] can't find the expected num : %d \n",result_list->num);
                    }
                }
            result_list = tmp_list;
            return SUCCESS;

        case LIST_GET_NODE_CMD_VAL:
            while(tmp_list->val != result_list->val){
                    tmp_list = source_list->next;
                    if(tmp_list == NULL){
                        printf("[SUCCESS] can't find the expected val :%d \n",result_list->val);
                    }
                }
            result_list = tmp_list;
            return SUCCESS;

        case LIST_GET_NODE_CMD_NAME:
            while(strcmp(tmp_list->name,result_list->name) == 0){
                    tmp_list = source_list->next;
                    if(tmp_list == NULL){
                        printf("[SUCCESS] can't find the expected val :%s \n",result_list->name);
                    }
                }
            result_list = tmp_list;
            return SUCCESS;

        default:
            printf("[ERROR] the mode is error,mode: %d \n",mode);
            return FAILURE;
    }

    return FAILURE;
}



/*
//*********sample for the interface functions*********

int main(void){
    int ret;
	info_list* tmp_list;

	g_list = list_init_node();
    strcpy(g_list->name,"ABC");
	g_list->val  = 101;

    tmp_list = list_init_node();
    strcpy(tmp_list->name,"BCD");
	tmp_list->val  = 102;
    list_add_tail_node(g_list,tmp_list);

    tmp_list = list_init_node();
    strcpy(tmp_list->name,"CDE");
	tmp_list->val  = 103;
    list_add_tail_node(g_list,tmp_list);

    tmp_list = list_init_node();
    strcpy(tmp_list->name,"DEF");
    tmp_list->val  = 104;
    ret = list_add_n_node(g_list,tmp_list,2);
    if(ret != SUCCESS){
        printf("[ERROR] add node failed \n");
        return FAILURE;
    }

//**************test**************
    tmp_list = g_list;

    while(tmp_list != NULL){
        printf("name: %s   val: %d    num: %d \n",tmp_list->name,tmp_list->val,tmp_list->num);
        tmp_list = tmp_list->next;
    }

//*****************************************list_get_node**************************************************
    //**tmp_list此时未null，所以需要重新分配空间**
    tmp_list = (info_list*)malloc(sizeof(info_list));
    memset(tmp_list,0,sizeof(info_list));

    tmp_list->num = 2;
    ret = list_get_node(g_list,tmp_list,LIST_GET_NODE_CMD_NUM);
    if(ret != SUCCESS){
        printf("[ERROR] add node failed \n");
        return FAILURE;
    }
    printf("[num = 2] name: %s   val: %d    num: %d \n",tmp_list->name,tmp_list->val,tmp_list->num);

    tmp_list->val = 101;
    ret = list_get_node(g_list,tmp_list,LIST_GET_NODE_CMD_VAL);
    if(ret != SUCCESS){
        printf("[ERROR] add node failed \n");
        return FAILURE;
    }
    printf("[val = 101] name: %s   val: %d    num: %d \n",tmp_list->name,tmp_list->val,tmp_list->num);

    strcpy(tmp_list->name,"CDE");
    ret = list_get_node(g_list,tmp_list,LIST_GET_NODE_CMD_VAL);
    if(ret != SUCCESS){
        printf("[ERROR] add node failed \n");
        return FAILURE;
    }
    printf("[name = CDE] name: %s   val: %d    num: %d \n",tmp_list->name,tmp_list->val,tmp_list->num);

//*****************************************list_get_node**************************************************

    return ret;
}
*/

