#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/select.h>

#include "linux_uart.h"


/*
struct uart_t {
	int uart_fd;
	const char *uart_path;
	unsigned int baud_rate;	 //UART baudrate: 1200, 2400 4800, 9600, 19200, 38400, 57600, 115200
} UART_DRV_T;
*/

int uart_open(UART_DRV_T *puart)
{
    int err_code = UART_SUCCESS;
    int fd = -1;
    char tty_path[20];
    struct termios options;

    if (puart == NULL) {
        printf("--%s/%d-----puart is invalid-----\n", __FUNCTION__, __LINE__);
        err_code = UART_FAILURE;
    }

    if (err_code == UART_SUCCESS) {
        strcpy(tty_path, puart->uart_path);
        fd = open(tty_path , O_RDWR | O_NOCTTY | O_NDELAY);

        if (fd < 0) {
            printf("--%s/%d---open uart [%s] failed--\n", __FUNCTION__, __LINE__, tty_path);
            err_code = UART_FAILURE;
        }
    }

    if (err_code == UART_SUCCESS) {
        // clear all flags
		memset(&options, 0, sizeof(options));
        if (tcgetattr(fd, &options) == UART_FAILURE) {
            printf("--%s/%d-----tcgetattr failed-----\n", __FUNCTION__, __LINE__);
            goto ERROR_END;
        }
        
        options.c_cflag |= CS8;         //8位数据位
        options.c_cflag &= ~PARENB;     //不进行奇偶校验
        options.c_iflag &= ~INPCK;      //关闭输入奇偶校验
        options.c_cflag &= ~CSTOPB;     //2位停止位，否则为1位
        options.c_cc[VTIME] = 50;       //0.05秒
	    options.c_cc[VMIN] = 0;
        options.c_cflag |= CLOCAL | CREAD;
    	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    	options.c_oflag &= ~OPOST;
    	options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

        switch(puart->baud_rate) {
            case 115200:
                // 115200bps
        		cfsetispeed(&options, B115200);
        		cfsetospeed(&options, B115200);
                break;
            case 9600:
                // 9600bps
        		cfsetispeed(&options, B9600);
        		cfsetospeed(&options, B9600);
                break;
            default:
                printf("--%s/%d---baud_rate is invalid-----\n", __FUNCTION__, __LINE__);
                goto ERROR_END;
        }
        
        //溢出数据可以接收，但不读
        if (tcflush(fd, TCIFLUSH) == UART_FAILURE) {
            printf("--%s/%d-----tcflush failed-----\n", __FUNCTION__, __LINE__);
            goto ERROR_END;
        }
    }

    if (err_code == UART_SUCCESS) {
        //设置新属性，TCSANOW：所有改变立即生效
        if (tcsetattr(fd, TCSANOW, &options)  == UART_FAILURE) {
            printf("--%s/%d-----tcflush failed-----\n", __FUNCTION__, __LINE__);
            goto ERROR_END;
        }
    }
    
    pthread_mutex_lock(&puart->uart_mutex); //初始化锁 mutex==1
    puart->uart_fd = fd;

    printf("%s/%d---open com success: %s  %d\n", __FUNCTION__, __LINE__, puart->uart_path, puart->baud_rate);
    return err_code;
    
ERROR_END:
    uart_close(puart);

    return UART_FAILURE;
}

int uart_close(UART_DRV_T *puart)
{
    int err_code = UART_SUCCESS;
    int fd = puart->uart_fd;

    if (puart == NULL) {
        printf("--%s/%d-----puart is invalid-----\n", __FUNCTION__, __LINE__);
        return UART_FAILURE;
    }

    if (fd >= 0) {
        if(close(fd)) {
            printf("%s/%d -- close uart failed : %s\n", __FUNCTION__, __LINE__, puart->uart_path);
            err_code = UART_FAILURE;
        }
        
        pthread_mutex_destroy(&puart->uart_mutex);
        
    } else {
        err_code = UART_FAILURE;
    }
    
    return err_code;
}

int uart_recv(UART_DRV_T *puart, char *pdata, uint32_t data_len)
{
    int ret, len;
    fd_set  fs_read;
    struct timeval tv_timeout;
    
    if (puart == NULL) {
        printf("--%s/%d-----puart is invalid-----\n", __FUNCTION__, __LINE__);
        return UART_FAILURE;
    }

    FD_ZERO(&fs_read);
	FD_SET(puart->uart_fd , &fs_read);
    tv_timeout.tv_sec = 2;
	tv_timeout.tv_usec = 0;
    
    ret = select(puart->uart_fd + 1, &fs_read, NULL, NULL, &tv_timeout);
    if (ret <= 0) {
        data_len = 1;
        printf("%s/%d--select----error--[%d]-", __FUNCTION__, __LINE__, ret);
        return UART_FAILURE;
    }

    if (FD_ISSET(puart->uart_fd, &fs_read)) {
        len = read(puart->uart_fd, pdata, data_len);
        //printf("%s/%d--reveive----success--[%d]-\n", __FUNCTION__, __LINE__, len);
        return len;
    } else {
        printf("%s/%d--FD_ISSET----error---", __FUNCTION__, __LINE__);
        return UART_FAILURE;
    }
}

//return send data number
int uart_send(UART_DRV_T *puart, char *pdata, uint32_t data_len)
{
    int len;
    if (puart == NULL) {
        printf("--%s/%d-----puart is invalid-----\n", __FUNCTION__, __LINE__);
        return UART_FAILURE;
    }

    pthread_mutex_lock(&puart->uart_mutex);
    
    tcflush(puart->uart_fd, TCIFLUSH);
    
    len = write(puart->uart_fd, pdata, data_len);
    
    pthread_mutex_unlock(&puart->uart_mutex);
    
    return len;
}

UART_DRV_T g_uart_t;
#define TEST_USART_NAME	"/dev/ttyHSL3"
/*
thread_type uart_receive_thread = 0;
thread_return_type uart_receive_thread_loop(void* n)
{
    char rev_buf[128] = {0};
    char byte_buf[4] = {0};
    uint32_t byte_len = 1;
    uint32_t  msg_len = 0;
    uint32_t i = 0;
    int ret;
    
    for (;;) {
        do {
            ret = uart_recv(&g_uart_t, byte_buf, byte_len);
            //printf("----->%d/%s\n",byte_len, byte_buf);
            if (ret == 1) {
                if (i == 0) {
                    if (byte_buf[0] != '{') {
                        continue;
                    }
                }
                
                rev_buf[i] = byte_buf[0];
                i++;
                
                if (byte_buf[0] == '}') {
                    msg_len = i;
                    printf("MSG ARRIVED %d:%s\n", msg_len, rev_buf);
                    memset(rev_buf, 0, sizeof(rev_buf));
                    i = 0;
                }
            }
        }while(1);
    } 
}

int uart_test(void)
{
    g_uart_t.uart_path = TEST_USART_NAME;
    g_uart_t.baud_rate = 9600;
    uart_open(&g_uart_t);
    printf("open dev:%s %d\n", TEST_USART_NAME, g_uart_t.baud_rate);
    uart_receive_thread = Thread_start(uart_receive_thread_loop, NULL);

    while(1) {
        printf("============Pang========\n");
        sleep(10);
    }
    
    return UART_SUCCESS;
}
*/