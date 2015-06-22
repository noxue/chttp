/*
 * setopt.h
 *
 *  Created on: 2015年6月10日
 *      Author: Administrator
 */

#include <chttp.h>

#ifndef INCLUDE_CHTTP_SETOPT_H_
#define INCLUDE_CHTTP_SETOPT_H_

/*
 * 设置请求头的选项
 */
#define chttp_setopt(chttp,opt,param)   do{																														\
		switch(opt)																																									\
		{																																													\
		case CHTTP_OPT_POST: 							chttp->method_type 				= (chttp_method_type_t)param; 	break;		\
		case CHTTP_OPT_DEBUG:    					chttp->debug  						= (int)param; 								break;		\
		case CHTTP_OPT_URL: 							chttp->url								= (char*)param; 							break;		\
		case CHTTP_OPT_REFER:							chttp->refer								= (char*)param; 							break;		\
		case CHTTP_OPT_AGENT:							chttp->agent							= (char*)param; 							break;		\
		case CHTTP_OPT_POSTDATA:					chttp->postdata						= (char*)param; 							break;		\
		case CHTTP_OPT_COOKIE_STR:				chttp->cookie_str						= (char*)param; 							break;		\
		case CHTTP_OPT_CUSTOM:						chttp->custom_header_list		= (chttp_slist_t*)param; 				break;		\
		case CHTTP_OPT_COOKIE_LIST: 				chttp->cookies							= (chttp_cookie_t*)param; 			break;		\
		default:																																						break;		\
		}																																													\
}while(0)





#endif /* INCLUDE_CHTTP_SETOPT_H_ */
