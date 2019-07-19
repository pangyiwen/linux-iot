#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void usage( const char *proc )
{
    printf("Please use %s [ip] [port]\n",proc);
}

int main( int argc, char *argv[] )
{
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

    struct sockaddr_in remote;
    remote.sin_family=AF_INET;
    remote.sin_port=htons(atoi(argv[2]));
    remote.sin_addr.s_addr=inet_addr(argv[1]);

    //2.connect
    if(connect(sock,(struct sockaddr*)&remote,sizeof(remote))<0){
        printf("Connect failed,reasons:%s\n",strerror(errno));
        close(sock);
        return 2;
    }

    printf("Connect success\n");

    char buf[1024];
    while(1){
        memset(buf,'\0',sizeof(buf));
        printf("Please write:");
        fflush(stdout);
        ssize_t _s=read(0,buf,sizeof(buf)-1); //输入要发送的数据
        if(_s>0){
            buf[_s-1]='\0';
            write(sock,buf,strlen(buf));//发送数据
            _s=read(sock,buf,sizeof(buf)-1);//接受数据
            if(_s>0){
                if(strncasecmp(buf,"quit",4)==0){
                    printf("quit\n");
                    break;
                }
                buf[_s-1]='\0';
                printf("read is:%s\n",buf);
            }
        }
    }
    close(sock);
}

