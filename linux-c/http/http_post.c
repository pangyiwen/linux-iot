#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>   
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>   
#include <sys/stat.h>  
#include "http_post.h"
 
 
unsigned char header[1024] = {0}; 
unsigned char send_request[1024] = {0};
unsigned char send_end[1024] = {0};
unsigned char http_boundary[64] = {0};

//获取文件大小
unsigned long get_file_size(const char *path)
{    
    unsigned long filesize = -1;        
    struct stat statbuff;    
    if(stat(path, &statbuff) < 0){    
        return filesize;    
    }else{    
        filesize = statbuff.st_size;    
    }    
    return filesize;    
}

//创建socket连接
int socket_connect(char *addr, int port)
{
    int sockfd = -1;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("create socket fail !\r\n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port);

    if(-1 == connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))){
        printf("connect fail !\r\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void create_http_header(char *url, char *filepath)
{
    struct timeval tv;
    long long int timestamp; 

    unsigned long totalsize = 0;
    unsigned long filesize = 0;
    unsigned long request_len = 0;
    unsigned long end_len = 0;

    //获取毫秒级的时间戳用于boundary的值
    gettimeofday(&tv, NULL);
    timestamp = (long long int)tv.tv_sec * 1000 + tv.tv_usec;
    snprintf(http_boundary, 64,"---------------------------%lld", timestamp);
    
    //填充请求信息|结束信息|头部信息
    filesize = get_file_size(filepath); //文件大小
    request_len = snprintf(send_request, 1024, UPLOAD_REQUEST, http_boundary, filepath); //请求信息
    end_len = snprintf(send_end, 1024, "\r\n--%s--\r\n", http_boundary); //结束信息
    totalsize = filesize + request_len + end_len;
    snprintf(header, 1024, HTTP_HEAD, SERVER_PATH, url, http_boundary, totalsize); //头信息

    return;
}

//接受http post回复的信息
void get_response_message(int *sockfd)
{
    int total, received, bytes;
    char response[MAXSIZE] = {0};
    memset(response, 0x00, sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    int i;

    do {
        bytes = read(*sockfd, response+received, total-received);
        if (bytes < 0)
            printf("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received += bytes;
    } while (received < total);

    if(received == total)
        printf("ERROR storing complete response from socket");
    
    //TODO:打印http服务器返回数据
    #if 0 
        for(i=0;i<total;i++)
        {
            printf("%c", response[i]);
        }
    #endif

    return;
}

//发送http post信息
int send_upload_massage(int *sockfd, char *filepath)
{
    FILE* fp = NULL;
    char request[MAXSIZE] = {0};
    int count = 0;

    //发送http头信息
    if(write(*sockfd, header, strlen(header)) < 0){
        printf("read file data fail!\r\n");
        return -1;
    }

    //发送文件请求信息
    if(write(*sockfd, send_request, strlen(send_request)) < 0){
        printf("read file data fail!\r\n");
        return -1;
    }

    fp = fopen(filepath, "rb+"); //打开要上传的文件
    if (fp == NULL){  
        printf("open file fail!\r\n");  
        return -1;  
    }

    //发送文件内容信息
    while((count = fread(request, 1, MAXSIZE, fp))>0)
    {
        if(write(*sockfd, request, count) < 0){
            printf("read file data fail!\r\n");
            goto ERROR_EXIT;
        }

        if(count < MAXSIZE){
            break;
        }
        bzero(request, 0);
    }

    fclose(fp);

    //发送http结束信息
    if(write(*sockfd, send_end, strlen(send_end)) < 0){
        printf("read file data fail!\r\n");
        return -1;
    }

    return 0;

ERROR_EXIT:
    fclose(fp);

    return -1;
}

//Post方式上传文件
int http_post_upload(unsigned char *ip, unsigned int port, char *url, char *filepath)
{
    int cfd = -1;  

    //创建socket连接
    cfd = socket_connect(ip, port);
    if(-1 ==  cfd){
        return -1;
    }

    //创建http头部信息
    create_http_header(url, filepath);

    //发送http post信息
    if(-1 == send_upload_massage(&cfd, filepath)) {
        printf("send upload massage fail!\r\n");
        goto ERROR_EXIT; 
    }

	//接受http post回复的信息
	get_response_message(&cfd);

    close(cfd);

	return 0;

ERROR_EXIT: 
    close(cfd);

    return -1;
}

/**for test**/
int main(int argc, char *argv[])  
{  
	int ret = 0;

	ret = http_post_upload(SERVER_ADDR, SERVER_PORT, SERVER_URL, argv[1]); //Post方式上传文件
	if(ret == -1)
	{
		printf("\n\n----------- Post file Fail!!\n");
	}

	return ret;  
}
