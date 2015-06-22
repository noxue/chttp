/*
 * chttp_socket.c
 *
 *  Created on: 2015年6月10日
 *      Author: Administrator
 */

#include <chttp_socket.h>
#include <assert.h>

/*
 * 初始化socket  返回socket句柄
 */
chttp_socket_t chttp_socket()
{
	chttp_socket_t sock = 0;

#if defined(WIN32)
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 0;
	}
#endif
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (-1 == sock)
		return 0;

	return sock;
}

/*
 * 通过域名连接服务器
 *失败 返回0
 */
chttp_socket_t chttp_socket_connect(chttp_socket_t sock, char *host, int port)
{
	struct hostent *phost;
	struct sockaddr_in addr;
	int rc = 0;

	phost = gethostbyname(host);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *((unsigned long*) phost->h_addr_list[0]);

	rc = connect(sock, (struct sockaddr*) &addr, sizeof(addr));

	if (0 != rc)
		return 0;

	return sock;
}

/*
 * 发送数据
 * socket   socket句柄
 * buffer    要发送的数据的缓冲区
 * flags      send函数的标志
 * ssl	        是否sll加密传输  0为不加密，1为加密
 */
int chttp_socket_send(chttp_socket_t socket, void *buffer, size_t size,
		int flags)
{
	return send(socket, buffer, size, flags);
}

/*
 * 接收数据
 * socket   socket句柄
 * buffer    接收数据的缓冲区
 * flags      recv函数的标志
 * ssl	        是否sll加密传输  0为不加密，1为加密
 */
int chttp_socket_recv(chttp_socket_t socket, void *buffer, size_t size,
		int flags)
{
	return recv(socket, buffer, size, flags);
}

/*
 * 执行ssl请求，返回chttp_str_t *  需要释放内存
 */
chttp_str_t* chttp_ssl_request(chttp_socket_t socket, void *buffer, size_t size)
{
#if defined(_HTTPS)
	SSL_CTX *ctx = NULL;
	SSL *ssl = NULL;
	int ret = 0;
	char text[1024];
	chttp_str_t *html = NULL;

	SSL_library_init();
	SSL_load_error_strings();

	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL)
		return NULL;

	ssl = SSL_new(ctx);
	if (ssl == NULL)
		return NULL;

	ret = SSL_set_fd(ssl, socket);
	if (0 == ret)
		return NULL;

	RAND_poll();
	while (RAND_status() == 0)
	{
		unsigned short rand_ret = rand() % 65536;
		RAND_seed(&rand_ret, sizeof(rand_ret));
	}

	ret = SSL_connect(ssl);
	if (ret != 1)
		return NULL;

	if (!(SSL_write(ssl, buffer, size)))
	{
		return NULL;
	}

	while ((ret = SSL_read(ssl, text, 1024)) > 0)
	{
		html = chttp_str_size_append(html, text, ret);
	}

	//free resource
	if (ssl)
	{
		SSL_free(ssl);
		SSL_CTX_free(ctx);
		ERR_free_strings();
	}

	return html;
#else
	printf("不支持https方式请求，请在编译的时候加入开启 _HTTPS \n");
	exit(1);
#endif
}

/*
 *关闭socket连接
 */
void chttp_socket_close(chttp_socket_t sock)
{
#if defined(WIN32)
	closesocket(sock);
	WSACleanup();
#else
	close(sock);
#endif
}
