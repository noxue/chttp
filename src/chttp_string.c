/*
 * chttp_string.c
 *
 *  Created on: 2015年6月12日
 *      Author: Administrator
 */

#include <chttp_string.h>

chttp_str_t *chttp_str_append(chttp_str_t *str, const char *cstr)
{
	if ((cstr != NULL && cstr[0] != 0))
		return chttp_str_size_append(str, cstr, strlen(cstr));
	else
		return str;
}

chttp_str_t *chttp_str_size_append(chttp_str_t *str, const char *cstr,
		size_t len)
{
	/*如果是第一次，则初始化并分配内存*/
	if (NULL == str)
	{
		str = (chttp_str_t*) malloc(sizeof(chttp_str_t));
		memset(str, 0, sizeof(chttp_str_t));
		str->data_len = len;
		str->buf_size = str->data_len * 2;

		str->data = malloc(str->buf_size);
		memset(str->data, 0, str->buf_size);

		memcpy(str->data, cstr, str->data_len);

		return str;
	}

	/*如果不是第一次，就检测当前空间是否够大，不够大就增加空间*/
	if (str->buf_size - str->data_len < len)
	{
		char *temp = NULL;

		do
		{
			temp = realloc(str->data, str->buf_size + len * 2);
		} while (!temp);

		str->data = temp;
		str->buf_size += len * 2;
	}

	/*把内容增加到缓冲区后面*/
	memcpy(str->data+str->data_len, cstr, len);
	str->data_len += len;

	return str;
}

/*重置字符串，缓冲区清零，内存空间大小不变*/
void chttp_str_reset(chttp_str_t *str)
{
	str->data_len = 0;
	memset(str->data, 0, str->buf_size);
}

/*释放字符串内存*/
void chttp_str_free(chttp_str_t *str)
{
	if (str)
	{
		if (str->data)
		{
			free(str->data);
		}
		free(str);
	}
}
