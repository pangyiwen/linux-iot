#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>


static void usage( const char* proc )
{
    printf("Please use %s [ip] [port]\n",proc);
}

void thread_run( void * arg )
{
    printf("create a new thread\n");

    int fd=(int)arg;
    char buf[1024];

    while(1)
    {
        memset(buf,'\0',sizeof(buf));
        ssize_t _s=read(fd,buf,sizeof(buf)-1); //读取客户端的数据
        if(_s>0)
        {
            buf[_s]='\0';
            printf("client say : %s\n",buf);
        }
        memset(buf,'\0',sizeof(buf));
        printf("please write :");
        fflush(stdout);
        ssize_t _s2=read(0,buf,sizeof(buf)-1);//输入数据
        if(_s2>0){
            write(fd,buf,strlen(buf));//放出去
        }
    }
}

int main( int argc,char *argv[] )
{
    int ret = 0;

    if(argc!=3){
        usage(argv[0]);
        exit(0);
    }

    //1.create socket
    int sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock<0){
        printf("Create socket error\n");
        return 1;
    }
    printf("Create socket success\n");

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));
    local.sin_addr.s_addr = inet_addr(argv[1]);

    //2.bind
    ret = bind(sock,(struct sockaddr*)&local,sizeof(local));
    if(ret < 0){
        printf("Bind error: ret %d \n",ret);
        close(sock);
        return 2;
    }
    printf("Bind success\n");

    //3.listen
    if(listen(sock,10)<0){
        printf("Listen error\n");
        close(sock);
        return 3;
    }
    printf("Listen success\n");

    //4.accept
    struct sockaddr_in peer;
    socklen_t len=sizeof(peer);
    while(1){   //不断接受客户端的接入
        int fd=accept(sock,(struct sockaddr*)&peer,&len);
        if(fd<0){
            perror("accept error\n");
            close(sock);
            return 4;
        }

        printf("get connect,client ip is %s port is %d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));

        pthread_t id;
        pthread_create(&id,NULL,thread_run,(void*)fd); //有客户端接入，就创建新的线程
        pthread_detach(id);

    }
    close(sock);
    return 0;
}




































