/*
 * cookie.h
 *
 *  Created on: 2015年6月9日
 *      Author: Administrator
 */
#include <slist.h>
#include <chttp.h>

#ifndef INCLUDE_CHTTP_COOKIE_H_
#define INCLUDE_CHTTP_COOKIE_H_


#ifdef  __cplusplus
extern "C" {
#endif


typedef struct chttp_cookie_s chttp_cookie_t;

struct chttp_cookie_s
{
	char *key;
	char *value;
	chttp_cookie_t *next;
};

CHTTP_API int chttp_cookie_parse(chttp_cookie_t** head, const char* header);
CHTTP_API int chttp_cookie_add(chttp_cookie_t *head, chttp_cookie_t *cookies);
CHTTP_API const char* chttp_cookie_get_value(chttp_cookie_t* head, const char* key);
CHTTP_API char* chttp_cookie_tostring(chttp_cookie_t* head);
CHTTP_API void chttp_cookie_free(chttp_cookie_t* head);


#ifdef  __cplusplus
}
#endif

#endif /* INCLUDE_CHTTP_COOKIE_H_ */
