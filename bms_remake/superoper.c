#include "superoper.h"
#include "managers.h"
#include "fileoper.h"



//	超级管理员登录
bool super_login(void)
{
	char name[NAME_MAX] = {};
	char pwd[PWD_MAX] = {};
	Super *super = (Super *)(super_list->head->next->ptr);
	puts("请输入超级管理员账号：");
	while(!get_str_chinese(name,sizeof(name)))		//	输入账号过长，或为空，重新输入
	{
		puts("请重新输入！");
	}
	if(0 != strcmp(super->name,name))
	{
		puts("超级管理员账号错误！");
		return false;
	}
	puts("请输入超级管理员密码：");
	while(!get_pwd(pwd,sizeof(pwd)))		//	输入密码过长，或为空，重新输入
	{
		puts("请重新输入！");
	}
	if(0 != strcmp(super->pwd,pwd))
	{
		puts("超级管理员密码错误！");
		return false;
	}
	return true;
}

//	修改超级管理员密码
void super_mod_pwd(void)
{
	Super *super = (Super *)(super_list->head->next->ptr);
	puts("请输入原密码：");
	char pwd[PWD_MAX] = {};
	if(!get_pwd(pwd,sizeof(pwd)))
	{
		puts("修改失败！");
		return;
	}
	if(0 != strcmp(pwd,super->pwd))
	{
		puts("输入密码错误！");
		return;
	}
	set_up_pwd(super->pwd);
}

//	添加管理员
void add_manager(void)
{
	puts("请输入管理员姓名：");
	char name[NAME_MAX] = {};
	if(!get_str_chinese(name,sizeof(name)))
	{
		puts("添加失败！");
		return;
	}
	puts("请输入管理员职位(0.图书管理员 1.读者管理员)：");
	char post[2] = {-1};
	if(!get_str(post,sizeof(post)))
	{
		puts("添加失败！");
		return;
	}
	if('0' != *post && '1' != *post)
	{
		puts("输入数据有误，添加失败！");
		return;
	}
	Manager *manager = malloc(sizeof(Manager));
	sprintf(manager->id,"%d",manager_id++);
	strcpy(manager->name,name);
	sprintf(manager->pwd,"123");
	manager->post = *post-48;
	add_tail_list(manager_list,manager);
}


int cmp_manager_id(const void *p1,const void *p2)		//	cmp_id
{
	Manager *s1 = (Manager *)p1,*s2 = (Manager *)p2;
	return strcmp(s1->id,s2->id);
}

static int cmp_manager_name(const void *p1,const void *p2)		//	cmp_name	模糊
{
	Manager *s1 = (Manager *)p1;
	char *s2 = (char *)p2;
	char *ptr = strstr(s1->name,s2);
	if(NULL != ptr)
		return 0;
	return -1;
}

static void _show_manager(const void *ptr)						//	show_manager
{
	Manager *manager = (Manager *)ptr;
	printf("id：%s \t姓名：%s \t职位：",manager->id,manager->name);
	0 == manager->post ? printf("图书管理员\n") : printf("读者管理员\n");
}

//	删除管理员
void del_manager(void)
{
	puts("请输入要删除的id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		return;
	}
	Manager *manager = del_value_list(manager_list,id,cmp_manager_id);
	if(NULL != manager)
	{
		_show_manager(manager);
	}
	else
	{
		puts("未找到管理员！");
	}
}

void _show_seek_manager(void)				//	查找	菜单
{
	system("clear");
	puts("***************************");
	puts("1.id查找");
	puts("2.姓名查找");
	puts("3.返回上一级");
}

void seek_manager_id(void)					//	以id准确查找
{
	puts("请输入要查找的id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		return;
	}
	Manager *manager = query_list(manager_list,id,cmp_manager_id);
	if(NULL == manager)
	{
		puts("未找到管理员！");
		return;
	}
	_show_manager(manager);
}
void seek_manager_name(void)
{
	puts("请输入要查找的姓名：");
	char name[NAME_MAX] = {};
	if(!get_str_chinese(name,sizeof(name)))
	{
		return;
	}
	if(!query_list_vague(manager_list,name,cmp_manager_name,_show_manager))
	{
		puts("未找到管理员！");
	}
}

//	查找管理员
void seek_manager(void)
{
	for(;;)
	{
		_show_seek_manager();
		switch(get_cmd('1','3'))
		{
			case '1':
			{
				seek_manager_id();		//	id准确查找
				anykey_continue();
			}
			break;
			case '2':
			{
				seek_manager_name();	//	姓名模糊查找
				anykey_continue();
			}
			break;
			case '3':
			{
				return;
			}
			break;
		}
	}
}

//	列出管理员
void show_manager(void)
{
	show_num(manager_list,10,_show_manager);
}

//	导入管理员信息
void import_manager(void)
{
	puts("请输入要导入管理员信息的文件：");
	char str[FILENAME] = {};
	if(!get_str_chinese(str,sizeof(str)))
	{
		puts("导入失败！");
		return;
	}
	strcpy(manager_path,str);
	load_manager();
	puts("导入成功！");
}

//	导出管理员信息
void export_manager(void)
{
	puts("请输入要导出管理员信息的文件：");
	char str[FILENAME] = {};
	if(!get_str_chinese(str,sizeof(str)))
	{
		puts("导出失败！");
		return;
	}
	strcpy(manager_path,str);
	save_manager();
	puts("导出成功！");
}

