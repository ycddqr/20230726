#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getch.h>
#include <unistd.h>
#include <time.h>

#define ID_MAX	21						//	ID字符串最大数量  +1
#define NAME_MAX 21						//	姓名字符串最大数量
#define PWD_MAX 21						//	密码字符串最大数量
#define PHONE_MAX 12					//	手机号11位，最后一位'/0'
#define BORROW_BOOK_NUM_MAX 3			//	借书最大数量
#define RESERVATION_BOOK_NUM_MAX 3		//	预约书最大数量
#define TITLE_MAX 41					//	书名字符串最大数量
#define PRESS_MAX 41					//	出版社名字符串最大数量
#define FILENAME 255					//	文件名最大字符数量

//	图书状态
#define IN_NOORDER 1
#define IN_ORDER 2
#define OUT_NOORDER 3
#define OUT_ORDER 4

typedef struct Node
{
	void* ptr;
	struct Node* prev;
	struct Node* next;
}Node;
 
typedef struct List
{
	Node* head;
	size_t size;
}List;

typedef int (*CMP)(const void*,const void*);
typedef void (*SHOW)(const void*);
typedef void (*CHANGE)(void*,const void*);


bool input_str_judgment(char *str);				//	判断输入是否合理 是否是纯数字
bool input_char_judgment(void);					//	判断数字输入是否含有字符
char get_cmd(char start,char end);				//	只输入start到end之间的字符
void anykey_continue(void);						//	按任意键继续 功能
void clear_stdin(void);							//	清理输入缓冲区
char* get_str(char* str,size_t size);			//	安全输入字符串
char* get_pwd(char* passwd,size_t size);		//	安全的密码输入
void put_str(const char* str,float sec);		//	以秒为单位显示字符串
void set_up_pwd(char *str);						//	设置密码输入两次
void show_num(List *list,int num,SHOW show);	//	分页显示
char* get_str_chinese(char* str,size_t size);   //	输入中文

//	创建链表
List* create_list(void);
void add_tail_list(List* list,const void* ptr);				//	尾添加
void *del_index_list(List *list,size_t index);				//	按位置删除节点
void* del_value_list(List* list,const void* ptr,CMP cmp);	//	按值删除
void show_list(List* list,SHOW show);						//	遍历 
bool mod_value_list(List* list,const void* old,CMP cmp,const void* data,CHANGE change);		//	按值修改
void* query_list(List* list,const void* data,CMP cmp);		//	查询 
bool query_list_vague(List* list,const void* data,CMP cmp,SHOW show);	//	模糊查询 
void clear_list(List* list);								//	清空
void destroy_list(List* list);								//	销毁


#endif
