/*
 * chttp_string.h
 *
 *  Created on: 2015年6月12日
 *      Author: Administrator
 */

#include <string.h>
#include <chttp.h>

#ifndef INCLUDE_CHTTP_CHTTP_STRING_H_
#define INCLUDE_CHTTP_CHTTP_STRING_H_

typedef struct chttp_str_s chttp_str_t;

struct chttp_str_s
{
	char *data;
	size_t buf_size;
	size_t data_len;
};

chttp_str_t *chttp_str_append(chttp_str_t *str, const char *cstr);
void chttp_str_reset(chttp_str_t *str);
chttp_str_t *chttp_str_size_append(chttp_str_t *str, const char *cstr, size_t len);
void chttp_str_free(chttp_str_t *str);

#endif /* INCLUDE_CHTTP_CHTTP_STRING_H_ */
