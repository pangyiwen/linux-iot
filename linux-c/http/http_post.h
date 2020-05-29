#ifndef __HTTP_POST__
#define __HTTP_POST__
 
#define SERVER_ADDR	"192.168.1.44"
#define SERVER_PORT	8080
#define SERVER_URL	"www.test.com"
#define SERVER_PATH	"/upload"
#define MAXSIZE 1500

#define HTTP_HEAD 	"POST %s HTTP/1.1\r\n"\
					"Host: %s\r\n"\
					"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:59.0) Gecko/20100101 Firefox/59.0\r\n"\
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"\
					"Accept-Language: en-US,en;q=0.5\r\n"\
					"Accept-Encoding: gzip, deflate\r\n"\
					"Content-Type: multipart/form-data; boundary=%s\r\n"\
					"Content-Length: %ld\r\n"\
					"Connection: close\r\n"\
					"Upgrade-Insecure-Requests: 1\r\n"\
					"DNT: 1\r\n\r\n"\
					
					
#define UPLOAD_REQUEST	"--%s\r\n"\
						"Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n"\
						"Content-Type: application/octet-stream\r\n\r\n"


unsigned long get_file_size(const char *path);
int socket_connect(char *addr, int port);
void create_http_header(char *url, char *filepath);
void get_response_message(int *sockfd);
int send_upload_massage(int *sockfd, char *filepath);
int http_post_upload(unsigned char *ip, unsigned int port, char *url, char *filepath); //Post方式
 
#endif