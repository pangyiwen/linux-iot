/*
linux�ಥ���
linux�ಥ��̲��裺

1>����һ��socket;
2>���öಥ�Ĳ��������糬ʱʱ��TTL�����ػػ����LOOP��
3>����ಥ��
4>���ͺͽ�������
5>�Ӷಥ���뿪�ಥ�������ʹ��setsockopt()������getsockopt()������ʵ�֣��鲥��ѡ����IP��ġ�
        getsockopt()/setsockopt()��ѡ���:
        IP_MULTICAST_TTL         ���öಥ�����ݵ�TTLֵ
        IP_ADD_MEMBERSHIP        ��ָ���ӿ��ϼ����鲥��
        IP_DROP_MEMBERSHIP       �˳��鲥��
        IP_MULTICAST_IF          ��ȡĬ�Ͻӿڻ����ýӿ�
        IP_MULTICAST_LOOP        ��ֹ�鲥���ݻ���

1��ѡ��IP_MULTICASE_TTL
        ѡ��IP_MULTICAST_TTL�������ó�ʱTTL����ΧΪ0��255֮����κ�ֵ�����磺
        unsigned char ttl=255;
        setsockopt(s,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));

2��ѡ��IP_MULTICAST_IF

        ѡ��IP_MULTICAST_IF���������鲥��Ĭ��Ĭ������ӿڣ���Ӹ���������ӿڷ��ͣ���һ������ӿڻ���Դ����ݡ����磺
        struct in_addr addr;
        setsockopt(s,IPPROTO_IP,IP_MULTICAST_IF,&addr,sizeof(addr));

        ����addr��ϣ���ಥ����ӿڵ�IP��ַ��ʹ��INADDR_ANY��ַ���͵�Ĭ�Ͻӿڡ�
        Ĭ������£������������鲥���ݵ�ĳ������ӿ�ʱ����IP�㣬���ݻ���͵����صĻػ��ӿڣ�ѡ��IP_MULTICAST_LOOP���ڿ��������Ƿ���͵����صĻػ��ӿڡ�
        ���磺
        unsigned char loop;
        setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));����loop����Ϊ0��ֹ���ͣ�����Ϊ1������͡�

3��ѡ��IP_ADD_MEMBERSHIP��IP_DROP_MEMBERSHIP
   ��������˳�һ���ಥ�飬ͨ��ѡ��IP_ADD_MEMBERSHIP��IP_DROP_MEMBER- SHIP����һ���ṹstruct ip_mreq���͵ı������п���

/*

*broadcast_server.c -�ಥ�������

*/
#define MCAST_PORT 8888;
#define MCAST_ADDR "224.0.0.100"//*һ���ֲ����Ӷಥ��ַ��·����������ת��*/
#define MCAST_DATA "BROADCAST TEST DATA"/*�ಥ���͵�����*/
#define MCAST_INTERVAL 5/*���ͼ��ʱ��*/

int main(int argc, char*argv)
{
    int s;

    struct sockaddr_in mcast_addr;
    s = socket(AF_INET, SOCK_DGRAM, 0);/*�����׽���*/

    if (s == -1){
        perror("socket()");
        return -1;
    }

    memset(&mcast_addr, 0, sizeof(mcast_addr));          /*��ʼ��IP�ಥ��ַΪ0*/
    mcast_addr.sin_family = AF_INET;                     /*����Э��������ΪAF*/
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);  /*���öಥIP��ַ*/
    mcast_addr.sin_port = htons(MCAST_PORT);             /*���öಥ�˿�*/

    /*��ಥ��ַ��������*/
    while(1) {
        int n = sendto(s,                              /*�׽���������*/
                       MCAST_DATA,                     /*����*/
                       sizeof(MCAST_DATA),             /*����*/
                       0,
                       (struct sockaddr*)&mcast_addr,
                       sizeof(mcast_addr));
        if( n < 0){
            perror("sendto()");
            return -2;
        }
        sleep(MCAST_INTERVAL);  /*�ȴ�һ��ʱ��*/

    }

    return 0��
}

/*
�ͻ���
�ಥ���IP��ַΪ224.0.0.88���˿�Ϊ8888�����ͻ��˽��յ��ಥ�����ݺ󽫴�ӡ������
�ͻ���ֻ���ڼ���ಥ�����ܽ��ܶಥ������ݣ���˶ಥ�ͻ����ڽ��նಥ�������֮ǰ��Ҫ�ȼ���ಥ�飬��������Ϻ�Ҫ�˳��ಥ�顣

*broadcast_client.c -�ಥ�Ŀͻ���

*/
#define MCAST_PORT 8888;
#define MCAST_ADDR "224.0.0.100"/*һ���ֲ����Ӷಥ��ַ��·����������ת��*/
#define MCAST_INTERVAL 5/*���ͼ��ʱ��*/
#define BUFF_SIZE 256/*���ջ�������С*/
int main(int argc, char*argv[])
{
    int s;  /*�׽����ļ�������*/

    struct sockaddr_in local_addr;      /*���ص�ַ*/
    int err = -1;
    s = socket(AF_INET, SOCK_DGRAM, 0); /*�����׽���*/

    if (s == -1){
        perror("socket()");
        return -1;
    }

    /*��ʼ����ַ*/
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(MCAST_PORT);

    /*��socket*/
    err = bind(s,(struct sockaddr*)&local_addr, sizeof(local_addr)) ;
    if(err < 0){
        perror("bind()");
        return -2;
    }
    /*���ûػ����*/
    int loop = 1;
    err = setsockopt(s,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));

    if(err < 0){
        perror("setsockopt():IP_MULTICAST_LOOP");
        return -3;
    }

    struct ip_mreq mreq;/*����ಥ��*/
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); /*�ಥ��ַ*/
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*����ӿ�ΪĬ��*/

    /*����������ಥ��*/
    err = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
    if (err < 0){
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return -4;
    }

    int times = 0;
    int addr_len = 0;
    char buff[BUFF_SIZE];
    int n = 0;

    /*ѭ�����նಥ�����Ϣ��5�κ��˳�*/
    for(times = 0;times<5;times++){

        addr_len = sizeof(local_addr);
        memset(buff, 0, BUFF_SIZE);   /*��ս��ջ�����*/

        /*��������*/
        n = recvfrom(s, buff, BUFF_SIZE, 0,(struct sockaddr*)&local_addr,&addr_len);

        if( n== -1){
            perror("recvfrom()");
        }

        /*��ӡ��Ϣ*/
        printf("Recv %dst message from server:%s\n", times, buff);

        sleep(MCAST_INTERVAL);

    }

    /*�˳��ಥ��*/
    err = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));

    close(s);

    return 0;

}
