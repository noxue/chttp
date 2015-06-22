/*
 * chttp_socket.h
 *
 *  Created on: 2015年6月10日
 *      Author: Administrator
 */

#include <chttp.h>


/*
 *  包含不同平台的头文件
 */
#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#endif

#if defined(_HTTPS)
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#endif

#ifndef INCLUDE_CHTTP_CHTTP_SOCKET_H_
#define INCLUDE_CHTTP_CHTTP_SOCKET_H_

#if defined(WIN32)
typedef SOCKET chttp_socket_t;
#else
typedef int chttp_socket_t;
#endif

/*
 * 初始化socket  返回socket句柄
 */
chttp_socket_t chttp_socket();
chttp_socket_t chttp_socket_connect(chttp_socket_t sock, char *host, int port);
int chttp_socket_send(chttp_socket_t socket, void *buffer, size_t size, int flags);
int chttp_socket_recv(chttp_socket_t socket, void *buffer, size_t size, int flags);

chttp_str_t* chttp_ssl_request(chttp_socket_t socket, void *buffer, size_t size);

void chttp_socket_close(chttp_socket_t sock);

#endif /* INCLUDE_CHTTP_CHTTP_SOCKET_H_ */
