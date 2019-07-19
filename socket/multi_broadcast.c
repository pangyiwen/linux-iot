/*
linux多播编程
linux多播编程步骤：

1>建立一个socket;
2>设置多播的参数，例如超时时间TTL，本地回环许可LOOP等
3>加入多播组
4>发送和接收数据
5>从多播组离开多播程序设计使用setsockopt()函数和getsockopt()函数来实现，组播的选项是IP层的。
        getsockopt()/setsockopt()的选项含义:
        IP_MULTICAST_TTL         设置多播组数据的TTL值
        IP_ADD_MEMBERSHIP        在指定接口上加入组播组
        IP_DROP_MEMBERSHIP       退出组播组
        IP_MULTICAST_IF          获取默认接口或设置接口
        IP_MULTICAST_LOOP        禁止组播数据回送

1．选项IP_MULTICASE_TTL
        选项IP_MULTICAST_TTL允许设置超时TTL，范围为0～255之间的任何值，例如：
        unsigned char ttl=255;
        setsockopt(s,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));

2．选项IP_MULTICAST_IF

        选项IP_MULTICAST_IF用于设置组播的默认默认网络接口，会从给定的网络接口发送，另一个网络接口会忽略此数据。例如：
        struct in_addr addr;
        setsockopt(s,IPPROTO_IP,IP_MULTICAST_IF,&addr,sizeof(addr));

        参数addr是希望多播输出接口的IP地址，使用INADDR_ANY地址回送到默认接口。
        默认情况下，当本机发送组播数据到某个网络接口时，在IP层，数据会回送到本地的回环接口，选项IP_MULTICAST_LOOP用于控制数据是否回送到本地的回环接口。
        例如：
        unsigned char loop;
        setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));参数loop设置为0禁止回送，设置为1允许回送。

3．选项IP_ADD_MEMBERSHIP和IP_DROP_MEMBERSHIP
   加入或者退出一个多播组，通过选项IP_ADD_MEMBERSHIP和IP_DROP_MEMBER- SHIP，对一个结构struct ip_mreq类型的变量进行控制

/*

*broadcast_server.c -多播服务程序

*/
#define MCAST_PORT 8888;
#define MCAST_ADDR "224.0.0.100"//*一个局部连接多播地址，路由器不进行转发*/
#define MCAST_DATA "BROADCAST TEST DATA"/*多播发送的数据*/
#define MCAST_INTERVAL 5/*发送间隔时间*/

int main(int argc, char*argv)
{
    int s;

    struct sockaddr_in mcast_addr;
    s = socket(AF_INET, SOCK_DGRAM, 0);/*建立套接字*/

    if (s == -1){
        perror("socket()");
        return -1;
    }

    memset(&mcast_addr, 0, sizeof(mcast_addr));          /*初始化IP多播地址为0*/
    mcast_addr.sin_family = AF_INET;                     /*设置协议族类行为AF*/
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);  /*设置多播IP地址*/
    mcast_addr.sin_port = htons(MCAST_PORT);             /*设置多播端口*/

    /*向多播地址发送数据*/
    while(1) {
        int n = sendto(s,                              /*套接字描述符*/
                       MCAST_DATA,                     /*数据*/
                       sizeof(MCAST_DATA),             /*长度*/
                       0,
                       (struct sockaddr*)&mcast_addr,
                       sizeof(mcast_addr));
        if( n < 0){
            perror("sendto()");
            return -2;
        }
        sleep(MCAST_INTERVAL);  /*等待一段时间*/

    }

    return 0；
}

/*
客户端
多播组的IP地址为224.0.0.88，端口为8888，当客户端接收到多播的数据后将打印出来。
客户端只有在加入多播组后才能接受多播组的数据，因此多播客户端在接收多播组的数据之前需要先加入多播组，当接收完毕后要退出多播组。

*broadcast_client.c -多播的客户端

*/
#define MCAST_PORT 8888;
#define MCAST_ADDR "224.0.0.100"/*一个局部连接多播地址，路由器不进行转发*/
#define MCAST_INTERVAL 5/*发送间隔时间*/
#define BUFF_SIZE 256/*接收缓冲区大小*/
int main(int argc, char*argv[])
{
    int s;  /*套接字文件描述符*/

    struct sockaddr_in local_addr;      /*本地地址*/
    int err = -1;
    s = socket(AF_INET, SOCK_DGRAM, 0); /*建立套接字*/

    if (s == -1){
        perror("socket()");
        return -1;
    }

    /*初始化地址*/
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(MCAST_PORT);

    /*绑定socket*/
    err = bind(s,(struct sockaddr*)&local_addr, sizeof(local_addr)) ;
    if(err < 0){
        perror("bind()");
        return -2;
    }
    /*设置回环许可*/
    int loop = 1;
    err = setsockopt(s,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));

    if(err < 0){
        perror("setsockopt():IP_MULTICAST_LOOP");
        return -3;
    }

    struct ip_mreq mreq;/*加入多播组*/
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); /*多播地址*/
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*网络接口为默认*/

    /*将本机加入多播组*/
    err = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
    if (err < 0){
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return -4;
    }

    int times = 0;
    int addr_len = 0;
    char buff[BUFF_SIZE];
    int n = 0;

    /*循环接收多播组的消息，5次后退出*/
    for(times = 0;times<5;times++){

        addr_len = sizeof(local_addr);
        memset(buff, 0, BUFF_SIZE);   /*清空接收缓冲区*/

        /*接收数据*/
        n = recvfrom(s, buff, BUFF_SIZE, 0,(struct sockaddr*)&local_addr,&addr_len);

        if( n== -1){
            perror("recvfrom()");
        }

        /*打印信息*/
        printf("Recv %dst message from server:%s\n", times, buff);

        sleep(MCAST_INTERVAL);

    }

    /*退出多播组*/
    err = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));

    close(s);

    return 0;

}
