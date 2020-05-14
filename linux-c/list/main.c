#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

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

/**************test**************/
    tmp_list = g_list;

    while(tmp_list != NULL){
        printf("name: %s   val: %d    num: %d \n",tmp_list->name,tmp_list->val,tmp_list->num);
        tmp_list = tmp_list->next;
    }

/*****************************************list_get_node**************************************************/
    /**tmp_list此时未null，所以需要重新分配空间**/
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

/*****************************************list_get_node**************************************************/

    return ret;
}


