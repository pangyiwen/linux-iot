#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    int len = 0;
    int i = 0;
    int j = 0;
    char *my_name = "hello";
    char *my_name_tmp;
    char buff[20] = {0};
    char arry[20];

    //memset(arry,0,sizeof(char)*20);

    len = strlen((char*)my_name);
    j = len;
    printf("len : %d \n",len);
    my_name_tmp = my_name;
    while(len != 0){
        strncpy(&buff[i],my_name_tmp,1);

        len--;
        i++;
        my_name_tmp++;
    }

    snprintf(arry,sizeof(arry),"%s",my_name);

/************solution  1 ************/
    for(i = 0;i < j;i++){
        printf("%c\n",buff[i]);
    }

/************solution  2 ************/
    for(i = 0;i < j;i++){
        printf("%c\n",arry[i]);
    }


    return 0;
}

