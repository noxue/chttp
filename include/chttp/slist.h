/*
 * clist.h
 *
 *  Created on: 2015年6月9日
 *      Author: Administrator
 */
#include <chttp.h>

#ifndef INCLUDE_CHTTP_SLIST_H_
#define INCLUDE_CHTTP_SLIST_H_

#ifdef  __cplusplus
extern "C" {
#endif


typedef struct chttp_slist_s chttp_slist_t;

struct chttp_slist_s
{
	char *data;
	chttp_slist_t *next;
};

CHTTP_API chttp_slist_t* chttp_slist_append(chttp_slist_t *list, const char *data);
CHTTP_API void chttp_slist_free_all(chttp_slist_t *list);

#ifdef  __cplusplus
}
#endif

#endif /* INCLUDE_CHTTP_SLIST_H_ */
