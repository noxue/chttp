/*
 * chttp.h
 *
 *  Created on: 2015年6月8日
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INCLUDE_CHTTP_CHTTP_H_
#define INCLUDE_CHTTP_CHTTP_H_

/*
 *  定义win32平台的宏
 */
#if (defined(_WIN32) || defined(__WIN32__)) && \
     !defined(WIN32) && !defined(__SYMBIAN32__)
#define WIN32
#endif

#ifdef  __cplusplus
extern "C"
{
#endif



typedef struct chttp_s chttp_t;

/* 编译库的时候，导出函数，使用库的时候导入*/
#if defined(WIN32)
#	if defined(BUILD)
#		define CHTTP_API  __declspec(dllexport)
#	else
#		define CHTTP_API  __declspec(dllimport)
#	endif
#else
#	define CHTTP_API
#endif





/*
 * 定义协议类型，http或者https类型
 */
typedef enum
{
	CHTTP = 0, CHTTPS
} chttp_protocol_type_t;

/*
 *  定义提交方式，get或者post方式
 */
typedef enum
{
	CHTTP_METHOD_GET = 0, CHTTP_METHOD_POST
} chttp_method_type_t;




/*
 * 可以设置的一些请求头选项
 */
typedef enum
{


	/*网站url*/
	CHTTP_OPT_URL = 0,

	/*是否调试*/
	CHTTP_OPT_DEBUG,

	/*是否是post请求，0为get 1为post， 默认为get请求*/
	CHTTP_OPT_POST,

	/*设置请求来路地址*/
	CHTTP_OPT_REFER,

	/*设置客户端信息*/
	CHTTP_OPT_AGENT,

	/*post提交的内容，char*类型 */
	CHTTP_OPT_POSTDATA,

	/*以cookie列表头指针的方式传递cookie信息*/
	CHTTP_OPT_COOKIE_LIST,

	/*以cookie字符串的方式传递cookie信息， char* 类型*/
	CHTTP_OPT_COOKIE_STR,

	/*传递用户自定义的头信息，chttp_slist_t * 类型 */
	CHTTP_OPT_CUSTOM,

} chttp_opt_t;


#include <chttp_string.h>
#include <chttp_socket.h>
#include <cookie.h>
#include <chttp_error.h>


struct chttp_s
{
	/* 是否显示调试信息 */
	int debug;

	/*句柄*/
	chttp_socket_t sock;

	/* 协议类型 */
	chttp_protocol_type_t protocol_type;

	/* 请求方式*/
	chttp_method_type_t method_type;

	/*网站地址*/
	char *url;

	/*主机地址*/
	chttp_str_t *host;

	/*来路地址*/
	char *refer;

	/*端口*/
	int port;

	/*cookie，请求的时候设置要提交的cookie，返回请求的时候存储返回的cookie*/
	chttp_cookie_t *cookies;

	/*cookie字符串  请求的时候设置要提交的cookie，返回请求的时候存储返回的cookie*/
	char *cookie_str;

	/*post的时候要提交的数据*/
	char *postdata;

	/*客户端信息*/
	char *agent;

	/*用户自定义请求头信息*/
	chttp_slist_t *custom_header_list;

	/*请求头指针*/
	chttp_str_t *req_header;

	/*返回的头部*/
	chttp_str_t *res_header;

	/*返回的内容部分*/
	chttp_str_t *res_content;

	/*存储错误信息*/
	chttp_str_t *err_str;
};

CHTTP_API chttp_t* chttp_init();
CHTTP_API char* chttp_exec(chttp_t *chttp);
CHTTP_API char* chttp_lasterr(chttp_t *chttp);
CHTTP_API void chttp_free(chttp_t *chttp);


/*这个头文件中放了 _chttp_setopt(chttp,opt,param) 宏 用来处理不同的 param*/
#include <setopt.h>



#ifdef  __cplusplus
}
#endif

#endif /* INCLUDE_CHTTP_CHTTP_H_ */

