/*
 * test.c
 *
 *  Created on: 2015年6月10日
 *      Author: Administrator
 */


#include <chttp.h>

int main(void)
{
	chttp_slist_t *strs=NULL, *pstrs=NULL;

	strs = chttp_slist_append(strs, "admin");
	strs = chttp_slist_append(strs, "root");
	strs = chttp_slist_append(strs, "admin");

	for(pstrs=strs; pstrs!=NULL; pstrs=pstrs->next)
	{
		printf("%s\n", pstrs->data);
	}

	chttp_slist_free_all(strs);
	return 0;
}
