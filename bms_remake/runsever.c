#include "runsever.h"
#include "superoper.h"
#include "manageroper.h"
#include "readers.h"
#include "managers.h"
#include "readeroper.h"

extern Reader *reader;
extern Manager *manager;

void menu(void)				//	主菜单
{
	system("clear");
	puts("***************************");
	puts("1.登录超级管理员");
	puts("2.登录管理员");
	puts("3.登录读者用户");
	puts("4.退出系统");
}

void reader_menu(void)		//	读者子菜单
{
	system("clear");
	puts("***************************");
	puts("1.显示个人信息");
	puts("2.修改密码");
	puts("3.检索图书");
	puts("4.借阅图书");
	puts("5.预约图书");
	puts("6.归还图书");
	puts("7.返回上一级");
}

void super_menu(void)		//	超级管理员子菜单
{
	system("clear");
	puts("***************************");
	puts("1.修改超级管理员密码");
	puts("2.添加管理员");
	puts("3.删除管理员");
	puts("4.查找管理员");
	puts("5.列出管理员");
	puts("6.导入管理员信息");
	puts("7.导出管理员信息");
	puts("8.返回上一级");
}

void manager_reader_menu(void)		//	读者管理员子菜单
{
	system("clear");
	puts("***************************");
	puts("1.添加读者");
	puts("2.删除读者");
	puts("3.列出所有读者");
	puts("4.导入读者");
	puts("5.导出读者");
	puts("6.读者充值");
	puts("7.修改密码");
	puts("8.返回上一级");
}

void manager_book_menu(void)		//	图书管理员子菜单
{
	system("clear");
	puts("***************************");
	puts("1.添加图书");
	puts("2.删除图书");
	puts("3.修改图书信息");
	puts("4.查找图书");
	puts("5.列出所有图书信息");
	puts("6.导入图书");
	puts("7.导出图书");
	puts("8.修改密码");
	puts("9.返回上一级");
}

void reader_operate(void)
{
	if(reader_login())
	{
		for(;;)
		{
			reader_menu();
			switch(get_cmd('1','7'))
			{
				case '1':
					show_reader_information();
					anykey_continue();
				break;
				case '2':
					reader_mod_pwd();
					anykey_continue();
				break;
				case '3':
					seek_book();
					anykey_continue();
				break;
				case '4':
					reader_borrow_book();
					anykey_continue();
				break;
				case '5':
					reader_reservation_book();
					anykey_continue();
				break;
				case '6':
					reader_return_book();
					anykey_continue();
				break;
				case '7':
					return;
				break;
			}
		}
	}
	else
		anykey_continue();
}

void super_operate(void)
{
	if(super_login())
	{
		for(;;)
		{
			super_menu();
			switch(get_cmd('1','8'))
			{
				case '1':
					super_mod_pwd();
					anykey_continue();
				break;
				case '2':
					add_manager();
					anykey_continue();
				break;
				case '3':
					del_manager();
					anykey_continue();
				break;
				case '4':
					seek_manager();
					anykey_continue();
				break;
				case '5':
					show_manager();		//	分页显示

				break;
				case '6':
					import_manager();
					anykey_continue();
				break;
				case '7':
					export_manager();
					anykey_continue();
				break;
				case '8':
					return;
				break;
			}
		}
	}
	else
		anykey_continue();
}

void manager_operate(void)
{
	if(manager_login())
	{
		if(manager->post)
		{
			for(;;)
			{
				manager_reader_menu();
				switch(get_cmd('1','8'))
				{
					case '1':
						add_reader();
						anykey_continue();
					break;
					case '2':
						del_reader();
						anykey_continue();
					break;
					case '3':
						show_reader();		//	分页显示
				
					break;
					case '4':
						import_reader();
						anykey_continue();
					break;
					case '5':
						export_reader();
						anykey_continue();
					break;
					case '6':
						recharge_reader();
						anykey_continue();
					break;
					case '7':
						manager_mod_pwd();
						anykey_continue();
					break;
					case '8':
						return;
					break;
				}
			}
		}
		else
		{
			for(;;)
			{
				manager_book_menu();
				switch(get_cmd('1','9'))
				{
					case '1':
						add_book();
						anykey_continue();
					break;
					case '2':
						del_book();
						anykey_continue();
					break;
					case '3':
						mod_book();
						anykey_continue();
					break;
					case '4':
						seek_book();
						anykey_continue();
					break;
					case '5':
						show_book();		//	分页显示
					
					break;
					case '6':
						import_book();
						anykey_continue();
					break;
					case '7':
						export_book();
						anykey_continue();
					break;
					case '8':
						manager_mod_pwd();
						anykey_continue();
					break;
					case '9':
						return;
					break;
				}
			}
		}
	}
	else
		anykey_continue();
}

void run(void)
{
	for(;;)
	{
		menu();
		switch(get_cmd('1','4'))
		{
			case '1':
				super_operate();
			break;
			case '2':
				manager_operate();
			break;
			case '3':
				reader_operate();
			break;
			case '4':
				return;
			break;
		}
	}
}
