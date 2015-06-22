/*
 * clist.c
 *
 *  Created on: 2015年6月9日
 *      Author: Administrator
 */

#include <slist.h>

/*
 * 向字符串链表中追加字符串
 * list 链表头指针，必须初始化为NULL
 * 返回字符串链表头指针
 */
chttp_slist_t * chttp_slist_append(chttp_slist_t *list, const char *data)
{
	chttp_slist_t *node = NULL, *temp = list;

	node = (chttp_slist_t*) calloc(1, sizeof(chttp_slist_t));
	node->data = (char *) calloc(1, strlen(data) + 1);
	node->next = NULL;
	strcpy(node->data, data);

	if (NULL == temp)
	{
		list = node;
	}
	else
	{
		/*找到最后一个节点,把新节点追加到最后*/
		for (temp = list; temp->next != NULL; temp = temp->next)
			;
		temp->next = node;
	}

	return list;
}

/*
 * 释放字符串链表
 */
void chttp_slist_free_all(chttp_slist_t *list)
{
	chttp_slist_t *temp = NULL;
	while (list)
	{
		/*保存要释放的节点*/
		temp = list;
		list = list->next;

		/*先释放节点中的变量data*/
		free(temp->data);
		/*再释放节点*/
		free(temp);
	}
}
