/*
 * test_cookie.c
 *
 *  Created on: 2015年6月10日
 *      Author: Administrator
 */

#include <chttp.h>

int main(void)
{
	chttp_cookie_t *cookie = NULL;
	const char *header_str = NULL;
	char *cookie_str = NULL;

	header_str =
			"HTTP/1.1 200 OK\r\n\
Date: Wed, 10 Jun 2015 03:32:04 GMT\r\n\
Server: Apache\r\n\
Set-Cookie: MANYOU_SESSIONID=366af6d035189ad10749c06dc6a6a587; expires=Wed, 10-Jun-2015 04:32:04 GMT; Path=/; domain=discuz.qq.com\r\n\
Set-Cookie: MANYOU_AUTH=b4241baf64e1e42ff674ed95d3f7bcbf; expires=Wed, 10-Jun-2015 04:32:04 GMT; Path=/; domain=discuz.qq.com\r\n\
Set-Cookie: MANYOU_DATA=fultUO73AtTbaa%2B4kghyZ2agYN%2B7a7HRMVtsbZfEdOKQ8foUD8usNVyiLMSTHo9mtCLOWI7Dsp%2BUmPed2kvE2Q%3D%3D; expires=Wed, 10-Jun-2015 04:32:04 GMT; Path=/; domain=discuz.qq.com\r\n\
Cache-Control: max-age=0\r\n\
Expires: Wed, 10 Jun 2015 03:32:04 GMT\r\n\
Content-Length: 197\r\n\
Connection: close\r\n\
Content-Type: text/javascript; charset=utf-8";

	chttp_cookie_parse(&cookie, header_str);

	cookie_str = chttp_cookie_tostring(cookie);

	printf("cookie:\n%s\n\nMANYOU_SESSIONID:%s\n\n", cookie_str, chttp_cookie_get_value(cookie, "MANYOU_SESSIONID"));

	free(cookie_str);

	chttp_cookie_free(cookie);
	return 0;
}
