/*
 * chttp_error.h
 *
 *  Created on: 2015年6月13日
 *      Author: Administrator
 */

#include <chttp.h>

#ifndef INCLUDE_CHTTP_CHTTP_ERROR_H_
#define INCLUDE_CHTTP_CHTTP_ERROR_H_

typedef struct chttp_error_s chttp_error_t;

typedef enum
{
	CHTTP_ERR_NOCONNECT
}chttp_error_type_t;

struct chttp_error_s
{
	chttp_error_type_t type;
	chttp_str_t *err_str;
};

#endif /* INCLUDE_CHTTP_CHTTP_ERROR_H_ */
