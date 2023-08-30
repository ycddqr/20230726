#ifndef MANAGEROPER_H
#define MANAGEROPER_H

#include "public.h"

extern List *reader_list;
extern List *book_list;
extern int reader_id;
extern int book_id;
extern char reader_path[FILENAME];
extern char book_path[FILENAME];

//	管理员登录
bool manager_login(void);

//	读者管理员
//	添加读者
void add_reader(void);
//	删除读者
void del_reader(void);
//	列出所有读者
void show_reader(void);
//	导入读者
void import_reader(void);
//	导出读者
void export_reader(void);
//	读者充值
void recharge_reader(void);
//	修改密码
void manager_mod_pwd(void);

//	图书管理员
//	添加图书
void add_book(void);
//	删除图书
void del_book(void);
//	修改图书信息
void mod_book(void);
//	查找图书
void seek_book(void);
//	列出所有图书信息
void show_book(void);
//	导入图书
void import_book(void);
//	导出图书
void export_book(void);

void _show_reader(const void* ptr);		//	显示读者 依赖函数
int cmp_book_id(const void *p1,const void *p2);
int cmp_reader_id(const void *p1,const void *p2);

#endif
