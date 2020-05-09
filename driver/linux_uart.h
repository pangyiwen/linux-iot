#ifndef __LINUX_UART_H_
#define __LINUX_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#define UART_SUCCESS    0
#define UART_FAILURE    -1

typedef struct uart_t{
	int uart_fd;
	const char *uart_path;      //串口绝对路径：/dev/tty*
	uint32_t baud_rate;	        //UART baudrate:9600, 115200 其他值初始化返回错误
	pthread_mutex_t uart_mutex; //串口锁
}UART_DRV_T;

int uart_open(UART_DRV_T *puart);

int uart_close(UART_DRV_T *puart);

//pdata: 接收数据buf       date_len: 接收数据长度          返回值：成功接收数据长度
int uart_recv(UART_DRV_T *puart, char *pdata, uint32_t data_len);

//pdata：发送数据buf        date_len: 发送数据长度          返回值：成功发送数据长度
int uart_send(UART_DRV_T *puart, char *pdata, uint32_t data_len);

//测试用
int uart_test(void);

#ifdef __cplusplus
}
#endif

#endif

