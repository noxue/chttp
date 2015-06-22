/*
 * nx_http.c
 *
 *  Created on: 2015年6月8日
 *      Author: Administrator
 */

#include <chttp.h>
#include <chttp_string.h>

static int _parse_url(char *, int *, char *, int *, char *);
static int _http_pack(chttp_t *chttp);
static int _chttp_request(chttp_t *chttp);

/*
 * 初始化chttp
 * 返回初始化后的chttp_t 指针
 */
chttp_t* chttp_init()
{
	chttp_t *chttp = NULL;

	chttp = (chttp_t*) malloc(sizeof(chttp_t));
	memset(chttp, 0, sizeof(chttp_t));

	/*初始化 socket句柄*/
	chttp->sock = chttp_socket();

	return chttp;
}

/*执行http请求*/
char* chttp_exec(chttp_t *chttp)
{
	_http_pack(chttp);

	if (chttp->debug)
	{
		printf("\n\n%s\n", chttp->req_header->data);
	}

	/* 如果返回值不是0  就表示出错了。 */
	if (0 != _chttp_request(chttp))
	{
		return NULL;
	}

	if (chttp->debug)
	{
		printf("\n\n%s\n", chttp->res_header->data);
	}

	return chttp->res_content->data;
}

/*
 * 根据 chttp发送请求
 * 返回0表示成功，返回1 表示连接失败
 */
static int _chttp_request(chttp_t *chttp)
{
	chttp_str_t *html = NULL;
	char text[512], *temp = NULL;
	size_t len = 0;
	if (0 == chttp_socket_connect(chttp->sock, chttp->host->data, chttp->port))
	{
		return 1;
	}

	if (chttp->protocol_type == CHTTP)
	{

		chttp_socket_send(chttp->sock, chttp->req_header->data,
				chttp->req_header->data_len, 0);

		while ((len = chttp_socket_recv(chttp->sock, text, 512, 0)) > 0)
		{
			text[len] = 0;
			html = chttp_str_size_append(html, text, len);
		}
	}
	else
	{
		html = chttp_ssl_request(chttp->sock, chttp->req_header->data,
				chttp->req_header->data_len);
	}

	for (temp = html->data;
			*temp != 0
					&& !(*temp == '\r' && *(temp + 1) == '\n'
							&& *(temp + 2) == '\r' && *(temp + 3) == '\n');
			temp++)
		;

	/*保存头部*/
	chttp->res_header = chttp_str_size_append(chttp->res_header, html->data,
			(size_t) (temp - html->data));

	/*保存内容*/
	chttp->res_content = chttp_str_size_append(chttp->res_content, temp + 4,
			html->data_len - (temp - html->data) - 4);

	chttp_str_free(html);
	html = NULL;

	/*如果下面两个中的其中一个值不为空，表示要获取cookie，就解析cookie保存到 chttp->cookies中*/
	if (chttp->cookie_str != NULL || chttp->cookies != NULL)
	{
		chttp_cookie_parse(&chttp->cookies, chttp->res_header->data);
	}

	return 0;
}

/**
 * 释放chttp
 */
void chttp_free(chttp_t *chttp)
{
	if (!chttp)
		return;

	if (chttp->sock)
	{
		chttp_socket_close(chttp->sock);
		chttp->sock = 0;
	}

	/*如果存在cookie  释放他*/
	if (chttp->cookies)
	{
		chttp_cookie_free(chttp->cookies);
		chttp->cookies = NULL;
	}

	/*如果主机名需要释放*/
	if (chttp->host)
	{
		chttp_str_free(chttp->host);
		chttp->host = NULL;
	}

	if (chttp->req_header)
	{
		chttp_str_free(chttp->req_header);
		chttp->req_header = NULL;
	}

	if (chttp->res_content)
	{
		chttp_str_free(chttp->res_content);
		chttp->res_content = NULL;
	}

}

/*
 * 根据chttp组装请求包
 */
static int _http_pack(chttp_t *chttp)
{
	char host[512], get[512];
	int proto = 0, port = 0;

	_parse_url(chttp->url, &proto, host, &port, get);

	chttp->port = port;
	chttp->host = chttp_str_append(chttp->host, host);

	/*协议类型 HTTP / HTTPS*/
	chttp->protocol_type = proto;

	if (CHTTP_METHOD_GET == chttp->method_type)
	{
		/* GET http://www.zixue7.com/forum.php HTTP/1.1 */
		chttp->req_header = chttp_str_append(chttp->req_header, "GET ");
		chttp->req_header = chttp_str_append(chttp->req_header, get);
		chttp->req_header = chttp_str_append(chttp->req_header, " HTTP/1.1");
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

		/* Host: www.zixue7.com */
		chttp->req_header = chttp_str_append(chttp->req_header, "Host: ");
		chttp->req_header = chttp_str_append(chttp->req_header, host);
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

		/* User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0 */
		if (NULL == chttp->agent)
		{
			chttp->req_header =
					chttp_str_append(chttp->req_header,
							"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0");
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"User-Agent: ");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->agent);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}

		/* Referer: http://www.zixue7.com/thread-190-1-1.html
		 * 如果没有设置 refer 就默认当前url为来路域名
		 * */
		if (NULL != chttp->refer)
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"Referer: ");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->refer);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"Referer: ");
			chttp->req_header = chttp_str_append(chttp->req_header, chttp->url);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}

		/* Cookie:  */
		if (NULL != chttp->cookie_str)
		{
			chttp->req_header = chttp_str_append(chttp->req_header, "Cookie: ");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->cookie_str);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else if (NULL != chttp->cookies)
		{
			char *pcookie = chttp_cookie_tostring(chttp->cookies);
			chttp->req_header = chttp_str_append(chttp->req_header, "Cookie: ");
			chttp->req_header = chttp_str_append(chttp->req_header, pcookie);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
			free(pcookie);
		}

		/* Connection: Close */
		chttp->req_header = chttp_str_append(chttp->req_header,
				"Connection: Close");
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

	}else /*处理普通的post请求*/
	{
		/*
		 * POST http://www.baidu.com/login.php HTTP/1.1
		 Host: www.baidu.com
		 User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0
		 Accept: text/html,application/xhtml+xml,application/xml;q=0.9,* / *;q=0.8
		 Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3
		 Accept-Encoding: gzip, deflate
		 Cookie: BAIDUID=997EB76C2725522C8EBE30B4127E263B:FG=1; BD_UPN=13314352; BIDUPSID=4007768C25D14E4883B1A37FF77956F7; PSTM=1433399829; BDUSS=WtqUnh0ZUxWTGtlTzRmN3R1UE9lTFg3MldkdDNwYm9sNWdDcWUyb2R2Q2c3NWhWQVFBQUFBJCQAAAAAAAAAAAEAAAA-p74KMTczMTI2MDE5AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAKBicVWgYnFVQ; ispeed_lsm=2; H_PS_645EC=6486zzMln6d2lC8Qm7giGBWd%2F%2BmuOLT%2BiRFKT3r7Bt4dKyss0zo03gO3lgd4o4UMgAb1; H_PS_PSSID=14591_1425_14860_12658_14511_14444_14734_12825_14429_12867_14622_14669_12723_14549_14625_14484_14903_11807_13937; BDSFRCVID=q-usJeC62mE0-a3lqpqSKd_zjCh-TrTTH6aIS1uXeqpgMuCZms2OEG0PJOlQpYD-LEWhogKK3gOTH4jP; H_BDCLCKID_SF=JJ4q_ItaJDvbfP0k247qe5OQqxby26nDH6TeaJ5nJDoVsDtz-jbSKJ8L0PRbybTB0K-L-PomQpP-HqTbDJoibJ_QbPct3hTL-br9Kl0MLpccDMjxWf8VXMLiMMnMBMPe52OnaIb_LIcjqR8ZD6LKj55P; BD_CK_SAM=1; BDSVRTM=0
		 Connection: keep-alive
		 Content-Type: application/x-www-form-urlencoded
		 Content-Length: 46

		 username=admin&submit=%CC%E1%BD%BB%B2%E9%D1%AF
		 */
		chttp->req_header = chttp_str_append(chttp->req_header, "POST ");
		chttp->req_header = chttp_str_append(chttp->req_header, get);
		chttp->req_header = chttp_str_append(chttp->req_header, " HTTP/1.1");
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

		/* Host: www.zixue7.com */
		chttp->req_header = chttp_str_append(chttp->req_header, "Host: ");
		chttp->req_header = chttp_str_append(chttp->req_header, host);
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

		/* User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0 */
		if (NULL == chttp->agent)
		{
			chttp->req_header =
					chttp_str_append(chttp->req_header,
							"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0");
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"User-Agent: ");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->agent);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}

		/* Referer: http://www.zixue7.com/thread-190-1-1.html
		 * 如果没有设置 refer 就默认当前url为来路域名
		 * */
		if (NULL != chttp->refer)
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"Referer: ");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->refer);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"Referer: ");
			chttp->req_header = chttp_str_append(chttp->req_header, chttp->url);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}

		/* Cookie:  */
		if (NULL != chttp->cookie_str)
		{
			chttp->req_header = chttp_str_append(chttp->req_header, "Cookie: ");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->cookie_str);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else if (NULL != chttp->cookies)
		{
			char *pcookie = chttp_cookie_tostring(chttp->cookies);
			chttp->req_header = chttp_str_append(chttp->req_header, "Cookie: ");
			chttp->req_header = chttp_str_append(chttp->req_header, pcookie);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
			free(pcookie);
		}

		/* Connection: Close */
		chttp->req_header = chttp_str_append(chttp->req_header,
				"Connection: Close");
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

		/*Content-Type: application/x-www-form-urlencoded*/
		chttp->req_header = chttp_str_append(chttp->req_header,
				"Content-Type: application/x-www-form-urlencoded");
		chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");

		/*Content-Length: 46*/
		if (NULL != chttp->postdata && 0 != chttp->postdata[0])
		{
			char buf[100];
			sprintf(buf, "Content-Length: %d\0", strlen(chttp->postdata));
			chttp->req_header = chttp_str_append(chttp->req_header, buf);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
			chttp->req_header = chttp_str_append(chttp->req_header,
					chttp->postdata);
			chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
		}
		else
		{
			chttp->req_header = chttp_str_append(chttp->req_header,
					"Content-Length: 0\r\n");
		}

	}

	chttp->req_header = chttp_str_append(chttp->req_header, "\r\n");
	return 0;
}

/**
 * 解析url
 * 传入参数  例如 http://www.zixue7.com:80/index.php
 * host 返回url的主机地址 例如   www.zixue7.com
 * proto 返回请求类型，CHTTP 或者 CHTTPS
 * port  返回url的端口
 * get 返回url相对路径，比如 /index.php
 * 返回值为0  表示成功
 */
static int _parse_url(char *url, int *proto, char *host, int *port, char *get)
{

	char site_url[512];
	int i;

	//url is null
	if (url == NULL || url[0] == 0)
		return 1;

	memset(site_url, 0, 512);
	if (strncmp("http://", url, 7) == 0 || strncmp("HTTP://", url, 7) == 0)
	{ //is http?
	  //is http
		*proto = CHTTP;

		//copy url to site_url without http://
		strcpy(site_url, url + 7);

	}
	else if (strncmp("https://", url, 7) == 0
			|| strncmp("HTTPS://", url, 7) == 0)
	{		//is https?
		//is https
		*proto = CHTTPS;

		//copy url to site_url without https://
		strcpy(site_url, url + 8);
	}
	else
	{
		//is http
		*proto = CHTTP;
		//copy url to site_url
		strcpy(site_url, url);
	}

	//find first /
	for (i = 0; site_url[i] != '/' && site_url[i] != 0; i++)
		;

	//don't find '/'
	if (i == strlen(site_url))
	{
		get[0] = '/';
		get[1] = 0;
	}
	else
	{
		strcpy(get, site_url + i);
		site_url[i] = 0;
	}

	//find the first ':' from site_url
	for (i = 0; site_url[i] != ':' && site_url[i] != 0; i++)
		;

	//don't find ':'
	if (i == strlen(site_url))
	{

		if (*proto == CHTTP)
		{
			//default http port 80
			*port = 80;
		}
		else if (*proto == CHTTPS)
		{
			//default https port 443
			*port = 443;
		}
	}
	else
	{	//find ':'
		char p[6];
		strcpy(p, site_url + i + 1);
		site_url[i] = 0;
		*port = strtol(p, NULL, 10);
	}

	//return host
	strcpy(host, site_url);

	return 0;
}

