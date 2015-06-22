/*
 * cookie.c
 *
 *  Created on: 2015年6月9日
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cookie.h>

static chttp_cookie_t* _chttp_cookie_new(const char* key, const char* value);
static int _chttp_cookie_add(chttp_cookie_t** head, chttp_cookie_t* cookie);

/**
 * 用于解析服务端放来的头信息中的cookie
 * head: cookie列表的头指针
 * header: http响应头信息
 * return 0  success
 */
int chttp_cookie_parse(chttp_cookie_t** head, const char* header)
{
	int i, j, k;
	chttp_cookie_t *cookie = NULL;

	/*如果返回头为空，则不处理，header[0] == 0  这个判断是防止后面 strlen 处理 空字符串的时候导致错误 */
	if (header == NULL || header[0] == 0)
		return 1;

	/* 处理类似这种格式的cookie Set-Cookie: q3vF_2132_sid=HtwU4l; expires=Tue, 02-Jun-2015 12:56:53 GMT; path=/
	 查找 Set-Cookie: ， 找到每一个cookie*/
	for (i = 0; header[i] != '\0'; i++)
	{
		if (strncmp("Set-Cookie: ", header + i, 12) == 0)
		{
			char *key = NULL, *value = NULL;

			/*
			 * 查找 Set-Cookie:  后面的 = ， 这样就可以确定 cookie名称的长度,
			 */
			for (j = i + 12;
					header[j] != '=' && header[j] != '\r' && header[j] != 0;
					j++)
				;

			/**
			 * 查找 = 后面的 ; ， 从而确定 cookie值的长度
			 * 此处要注意处理 这样的cookie：Set-Cookie: noxue=www.zixue7.com  没有分号结尾 是以 \r 结尾
			 */
			for (k = j + 1;
					header[k] != ';' && header[k] != '\r' && header[k] != 0;
					k++)
				;

			/*如果key或者value的长度为0 表示cookie为空，那就进行下一次循环。*/
			if (0 == j || 0 == k)
				continue;

			key = (char*) calloc(1, j - i - 12 + 1);
			value = (char*) calloc(1, k - j - 1 + 1);

			memcpy(key, header + i + 12, j - i - 12);
			memcpy(value, header + j + 1, k - j - 1);

			cookie = _chttp_cookie_new(key, value);
			_chttp_cookie_add(head, cookie);

			if (key)
				free(key);

			if (value)
				free(value);

		}
	}

	return 0;
}

/*
 * 把两个cookie链表合并，后面的合并到前面。
 *cookies 不会被此函数释放掉，需要自己释放
 */
int chttp_cookie_add(chttp_cookie_t *head, chttp_cookie_t *cookies)
{
	while (NULL != cookies)
	{
		_chttp_cookie_add(&head, cookies);
	}
	return 0;
}

/**
 * 根据cookie名称返回对应的cookie值
 * head: cookie列表头指针
 * key: cookie的名称
 * if 成功就返回cookie内容，否则返回NULL
 * 返回的指针不能释放，因为返回的就是cookie节点中的内容
 */
const char* chttp_cookie_get_value(chttp_cookie_t* head, const char* key)
{
	chttp_cookie_t *temp = head;

	while (temp != NULL)
	{
		if (strcmp(temp->key, key) == 0)
		{
			return temp->value;
		}
		temp = temp->next;
	}

	return NULL;
}

/**
 * 返回这种格式的字符串cookie信息：aaa=bbb; ccc=ddd; eee=fff;
 * head: cookie列表的头指针
 * 返回cookie字符串的指针【需要释放】
 */
char* chttp_cookie_tostring(chttp_cookie_t* head)
{
	char *cookie = NULL;
	chttp_cookie_t *temp = head;
	size_t len = 0;

	/*计算cookie中内容的长度，为下面分配内存的长度*/
	while (temp != NULL)
	{
		/*添加 ; = 和空格 这3个字符的长度*/
		len += strlen(temp->key) + strlen(temp->value) + 3;
		temp = temp->next;
	}

	cookie = (char *) calloc(1, len + 5);

	temp = head;
	while (temp != NULL)
	{
		strcat(cookie, temp->key);
		strcat(cookie, "=");
		strcat(cookie, temp->value);
		strcat(cookie, "; ");
		temp = temp->next;
	}

	return cookie;
}

/**
 * 销毁cookie列表
 * head: cookie列表的头指针
 */
void chttp_cookie_free(chttp_cookie_t* head)
{
	chttp_cookie_t *temp = head, *t1 = NULL;
	while (temp != NULL)
	{
		t1 = temp->next;

		free(temp->key);
		free(temp->value);
		free(temp);

		temp = t1;
	}
}

/**
 * 创建一个cookie节点，返回该节点的指针
 * key: cookie的名称
 * value: cookie的值
 * return: 返回cookie节点的指针，需要用户释放（一般是直接加入到列表中，最后一起由cookie_free释放）
 */
static chttp_cookie_t* _chttp_cookie_new(const char* key, const char* value)
{
	chttp_cookie_t *cookie = NULL;
	if (key == NULL || value == NULL || key[0] == 0 || value[0] == 0)
		return NULL;

	cookie = (chttp_cookie_t*) malloc(sizeof(chttp_cookie_t));
	cookie->key = (char *) calloc(1, strlen(key) + 1);
	cookie->value = (char *) calloc(1, strlen(value) + 1);
	strcpy(cookie->key, key);
	strcpy(cookie->value, value);
	cookie->next = NULL;

	return cookie;
}

/**
 * 把cookie节点添加到 cookie列表中
 * head: cookie列表的头指针【必须初始化为NULL，否则会出错】
 * cookie: cookie节点指针
 * 成功返回0
 */
static int _chttp_cookie_add(chttp_cookie_t** head, chttp_cookie_t* cookie)
{
	chttp_cookie_t *temp = *head;
	chttp_cookie_t *prev = NULL;
	if (cookie == NULL)
		return 1;

	/*如果头指针为NULL 表示是头结点，这也是前面为何必须要初始化为NULL的原因*/
	if (temp == NULL)
	{
		*head = cookie;
	}
	else
	{

		for (; temp != NULL; temp = temp->next)
		{
			/*如果存在相同名称的cookie节点，就覆盖他*/
			if (strcmp(temp->key, cookie->key) == 0)
			{

				/*保存这个同名的cookie节点指针，方便后面释放*/
				chttp_cookie_t *t = temp;

				/*把同名的节点的next指针赋值给新的节点的next指针*/
				cookie->next = t->next;
				/*让同名节点的前面一个节点的next指针指向新的节点*/
				prev->next = cookie;

				/*释放以前的同名节点*/
				free(t->key);
				free(t->value);
				free(t);
				return 0;
			}

			/*保存当前循环的节点的前面一个节点，这样后面才能添加*/
			prev = temp;
		}

		/*如果没有重名的，就添加到cookie列表的最后面*/
		prev->next = cookie;
	}

	return 0;
}
