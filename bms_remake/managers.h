#ifndef MANAGERS_H
#define MANAGERS_H

#include "public.h"

typedef struct Manager
{
        char id[ID_MAX];
        char name[NAME_MAX];		//	姓名
        char pwd[PWD_MAX];			//	密码
        char post;					//	职位	0.图书管理员	1.读者管理员
}Manager;

#endif//MANAGERS_H
