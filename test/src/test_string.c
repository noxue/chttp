/*
 * test_string.c
 *
 *  Created on: 2015年6月12日
 *      Author: Administrator
 */


#include <chttp.h>

int main(void)
{
	chttp_str_t *str =NULL;

	str = chttp_str_append(str, "www.zixue7.com");
	str = chttp_str_append(str, "www.zixue7.com");

	printf("%s", str->data);

	chttp_str_free(str);

	return 0;
}
