/*
 * test_chttp.c
 *
 *  Created on: 2015年6月11日
 *      Author: Administrator
 */

#include <chttp.h>

/*如果定义了 DEBUG  就会显示出  请求头和返回头*/
#define DEBUG 1

int main(int argc, char **argv)
{

	chttp_t *chttp = NULL;
	char *temp = NULL;

	chttp = chttp_init();

	chttp_setopt(chttp, CHTTP_OPT_URL, "http://www.zixue7.com/forum.php");
	chttp_setopt(chttp, CHTTP_OPT_POST, 1);
	chttp_setopt(chttp, CHTTP_OPT_DEBUG, 0);
	chttp_setopt(chttp, CHTTP_OPT_COOKIE_STR, "");
	chttp_setopt(chttp, CHTTP_OPT_POSTDATA, "content=adminadmin");
	if(NULL==chttp_exec(chttp))
	{
		printf("请求失败了\n");
		chttp_free(chttp);
	}

	temp = chttp_cookie_tostring(chttp->cookies);
	printf("response header:\n%s\n\ncookie:\n%s\n\nhtml:\n%s\n",
			chttp->res_header->data, temp, chttp->res_content->data);

	free(temp);
	chttp_free(chttp);

	return 0;
}
