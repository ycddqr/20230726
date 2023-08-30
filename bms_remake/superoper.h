#ifndef SUPEROPER_H
#define SUPEROPER_H

#include "public.h"

extern List *super_list;
extern List *manager_list;
extern int manager_id;
extern char manager_path[FILENAME];

typedef struct Super
{
	char name[NAME_MAX];
	char pwd[PWD_MAX];
}Super;

//	超级管理员登录
bool super_login(void);

//	修改超级管理员密码
void super_mod_pwd(void);
//	添加管理员
void add_manager(void);
//	删除管理员
void del_manager(void);
//	查找管理员
void seek_manager(void);
//	列出管理员
void show_manager(void);
//	导入管理员信息
void import_manager(void);
//	导出管理员信息
void export_manager(void);

int cmp_manager_id(const void *p1,const void *p2);


#endif
