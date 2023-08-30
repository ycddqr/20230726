#include "manageroper.h"
#include "readers.h"
#include "books.h"
#include "managers.h"
#include "fileoper.h"
#include "superoper.h"

Manager *manager;


bool manager_login(void)
{
	puts("请输入管理员id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("登录失败！");
		return false;
	}
	manager = query_list(manager_list,id,cmp_manager_id);
	if(NULL == manager)
	{
		puts("未找到管理员，登录失败！");
		return false;
	}
	puts("请输入密码：");
	char pwd[PWD_MAX] = {};
	if(!get_pwd(pwd,sizeof(pwd)))
	{
		puts("登录失败！");
		return false;
	}
	if(0 != strcmp(pwd,manager->pwd))
	{
		puts("密码错误！");
		return false;
	}
	return true;
}

void _show_reader(const void* ptr)		//	显示读者 依赖函数
{
	Reader *reader = (Reader *)ptr;
	printf("id：%s \t姓名：%s \t电话：%s \t已借图书数：%d \t已预约图书数：%d \t余额：%.2lf\n",reader->id,reader->name
		,reader->phone,reader->borrow_book_num,reader->reservation_book_num,reader->balance);
	if(0 < reader->borrow_book_num)
		printf("已借图书编号：");
	for(int i = 0 ; i < reader->borrow_book_num; i++)
	{
		printf("%d ",reader->borrow_book[i]);
	}
	if(0 < reader->reservation_book_num)
		printf("已借图书编号：");
	for(int i = 0 ; i < reader->reservation_book_num; i++)
	{
		printf("%d ",reader->reservation_book[i]);
	}
	printf("\n");
}

int cmp_reader_id(const void *p1,const void *p2)
{
	Reader *r1 = (Reader *)p1,*r2 = (Reader *)p2;
	if(0 == strcmp(r1->id,r2->id))
		return 0;
	return -1;
}

int cmp_book_id(const void *p1,const void *p2)
{
	Book *b1 = (Book *)p1,*b2 = (Book *)p2;
	if(0 == strcmp(b1->id,b2->id))
		return 0;
	return -1;
}

static int cmp_book_title(const void *p1,const void *p2)
{
	Book *b1 = (Book *)p1;
	char *b2 = (char *)p2;
	char *ptr = strstr(b1->title,b2);
	if(NULL != ptr)
		return 0;
	return -1;
}

static int cmp_book_author(const void *p1,const void *p2)
{
	Book *b1 = (Book *)p1;
	char *b2 = (char *)p2;
	char *ptr = strstr(b1->author,b2);
	if(NULL != ptr)
		return 0;
	return -1;
}

void _show_book(const void *ptr)
{
	Book *book = (Book *)ptr;
	printf("id：%s\t书名：%s\t作者：%s\t出版日期：%d\t出版社：%s\t售价：%.2lf\t借阅次数：%d\t图书状态：",book->id,book->title
		,book->author,book->time,book->press,book->price,book->borrow_num);
	if(IN_NOORDER == book->state)
		printf("在馆无预约\t");
	else if(IN_ORDER == book->state)
		printf("在馆被预约\t");
	else if(OUT_NOORDER == book->state)
		printf("离馆无预约\t");
	else
		printf("离馆被预约\t");
	if(-1 != book->score)
		printf("评分：%.2lf\n",book->score);
	else
		printf("评分：暂无\n");
}

void seek_book_id(void)			//	book	id查找
{
	puts("请输入要查找的图书id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("查找失败！");
		return;
	}
	Book *book = query_list(book_list,id,cmp_book_id);
	if(NULL == book)
	{
		puts("未找到图书！");
		return;
	}
	_show_book(book);
}

void seek_book_title(void)		//	book	书名模糊查找
{
	puts("请输入要查找的书名：");
	char title[TITLE_MAX] = {};
	if(!get_str_chinese(title,sizeof(title)))
	{
		return;
	}
	if(!query_list_vague(book_list,title,cmp_book_title,_show_book))
	{
		puts("未找到图书！");
	}
}

void seek_book_author(void)		//	book	作者名模糊查找
{
	puts("请输入要查找的作者：");
	char author[NAME_MAX] = {};
	if(!get_str_chinese(author,sizeof(author)))
	{
		return;
	}
	if(!query_list_vague(book_list,author,cmp_book_author,_show_book))
	{
		puts("未找到图书！");
	}
}

//	读者管理员
//	添加读者
void add_reader(void)
{
	puts("请输入读者姓名：");
	char name[NAME_MAX] = {};
	if(!get_str_chinese(name,sizeof(name)))
	{
		puts("添加失败！");
		return;
	}
	puts("请输入读者电话(11位)：");
	char phone[PHONE_MAX] = {};
	if(!get_str(phone,sizeof(phone)))
	{
		puts("添加失败！");
		return;
	}
	if(11 != strlen(phone))
	{
		puts("电话号码长度不符！添加失败！");
		return;
	}
	if(!input_str_judgment(phone))
	{
		puts("输入中含有字符！添加失败！");
		return;
	}
	puts("读者添加成功！");
	Reader *reader = malloc(sizeof(Reader));
	sprintf(reader->id,"%d",reader_id++);
	sprintf(reader->pwd,"123");
	strcpy(reader->name,name);
	strcpy(reader->phone,phone);
	reader->borrow_book_num = 0;
	reader->reservation_book_num = 0;
	reader->balance = 0;
	add_tail_list(reader_list,reader);
}
//	删除读者
void del_reader(void)
{
	puts("请输入要删除的读者id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("删除失败！");
		return;
	}
	Reader *reader = query_list(reader_list,id,cmp_reader_id);	//	id查找找到要删除的读者
	if(NULL == reader)
	{
		puts("未找到读者！");
		return;
	}
	printf("已删除：");
	_show_reader(reader);
				//	恢复要删除的读者的书本状态
	for(int i = 0 ; i < reader->borrow_book_num; i++)		//	已借的图书out变in
	{
		Book *book = query_list(book_list,&reader->borrow_book[i],cmp_book_id);
		if(OUT_NOORDER == book->state)
		{
			book->state = IN_NOORDER;
		}
		else
		{
			book->state = IN_ORDER;
		}
	}
	for(int i = 0 ; i < reader->reservation_book_num; i++)	//	已预约的图书取消预约
	{
		Book *book = query_list(book_list,&reader->reservation_book[i],cmp_book_id);
		if(OUT_ORDER == book->state)
		{
			book->state = OUT_NOORDER;
		}
		else
		{
			book->state = IN_NOORDER;
		}
	}
	del_value_list(reader_list,id,cmp_reader_id);	//	删除读者
}
//	列出所有读者
void show_reader(void)
{
	show_num(reader_list,10,_show_reader);
}

//	导入读者
void import_reader(void)
{
	puts("请输入要导入读者信息的文件：");
	char str[FILENAME] = {};
	if(!get_str_chinese(str,sizeof(str)))
	{
		puts("导入失败！");
		return;
	}
	strcpy(reader_path,str);
	load_reader();
	puts("导入成功！");
}
//	导出读者
void export_reader(void)
{
	puts("请输入要导出读者信息的文件：");
	char str[FILENAME] = {};
	if(!get_str_chinese(str,sizeof(str)))
	{
		puts("导出失败！");
		return;
	}
	strcpy(reader_path,str);
	save_reader();
	puts("导出成功！");
}

//	读者充值
void recharge_reader(void)
{
	puts("请输入要充值的读者id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("充值失败！");
		return;
	}
	Reader *reader = query_list(reader_list,id,cmp_reader_id);
	if(NULL == reader)
	{
		puts("未找到读者！");
		return;
	}
	puts("请输入充值金额：");
	clear_stdin();
	double money = 0;
	scanf("%lf",&money);
	if(!input_char_judgment())
	{
		return;
	}
	if(0 > money || 10000000000 < money + reader->balance)
	{
		puts("输入金额大小不合适，充值失败！");
		return;
	}
	reader->balance += money;
	_show_reader(reader);
	puts("充值成功！");
}

//	修改密码
void manager_mod_pwd(void)
{
	puts("请输入原密码：");
	char pwd[PWD_MAX] = {};
	if(!get_pwd(pwd,sizeof(pwd)))
	{
		puts("修改失败！");
		return;
	}
	if(0 != strcmp(pwd,manager->pwd))
	{
		puts("输入密码错误！");
		return;
	}
	set_up_pwd(manager->pwd);
}


bool is_time_resonable(int time)		//	判断时间是不是合理
{
	if(time < 0 || time > 20230730)
		return false;
	int year = time/10000;				//把日期拆解成year年
	int month = time/100%100;			//month月
	int day = time%100;					//day日
	if(month > 12) return false;		//月份大于12日期不合理
	int true_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};//每个月的正确天数
	if ((0 == year % 4 && year % 100 != 0) || (0 == year % 400))
	{
		true_month[1]++;		//判断是不是闰年，闰年的时候二月是29天	
	}
	if(day > true_month[month-1]) return false; //天数比每月最大数还大日期不合理
	return true;	//其余情况都为合理情况
}

//	图书管理员
//	添加图书
void add_book(void)
{
	puts("请输入要添加的书名：");
	char title[TITLE_MAX] = {};
	if(!get_str_chinese(title,sizeof(title)))
	{
		puts("添加失败！");
		return;
	}
	puts("请输入作者：");
	char author[NAME_MAX] = {};
	if(!get_str_chinese(author,sizeof(author)))
	{
		puts("添加失败！");
		return;
	}
	puts("请输入出版日期：");
	int time = 0;
	clear_stdin();
	scanf("%d",&time);
	if(!input_char_judgment())
	{
		return;
	}
	if(!is_time_resonable(time))
	{
		puts("输入日期不合理！添加失败！");
		return;
	}
	puts("请输入出版社：");
	char press[PRESS_MAX] = {};
	if(!get_str_chinese(press,sizeof(press)))
	{
		puts("添加失败！");
		return;
	}
	puts("请输入售价：");
	double price = 0;
	clear_stdin();
	scanf("%lf",&price);
	if(!input_char_judgment())
	{
		return;
	}
	if(0 > price)
	{
		puts("售价不合理，添加失败！");
		return;
	}
	Book *book = malloc(sizeof(Book));
	sprintf(book->id,"%d",book_id++);
	strcpy(book->title,title);
	strcpy(book->author,author);
	strcpy(book->press,press);
	book->time = time;
	book->price = price;
	book->borrow_num = 0;
	book->state = IN_NOORDER;
	book->borrow_time = 0;
	book->score = -1;
	add_tail_list(book_list,book);
}

//	删除图书
void del_book(void)
{
	puts("请输入要删除的图书id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		return;
	}
	Book *book = del_value_list(book_list,id,cmp_book_id);
	if(NULL != book)
	{
		_show_book(book);
	}
	else
	{
		puts("未找到图书！");
	}
}

//	修改图书信息
void mod_book(void)
{
	puts("请输入要修改的id");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("修改失败！");
		return;
	}
	Book *book = query_list(book_list,id,cmp_book_id);
	if(NULL == book)
	{
		puts("未找到图书！");
		return;
	}
	
	puts("请输入修改后的书名：");
	char title[TITLE_MAX] = {};
	if(!get_str_chinese(title,sizeof(title)))
	{
		puts("修改失败！");
		return;
	}
	puts("请输入修改后的作者：");
	char author[NAME_MAX] = {};
	if(!get_str_chinese(author,sizeof(author)))
	{
		puts("修改失败！");
		return;
	}
	puts("请输入修改后的出版日期：");
	int time = 0;
	clear_stdin();
	scanf("%d",&time);
	if(!input_char_judgment())
	{
		return;
	}
	if(!is_time_resonable(time))
	{
		puts("输入日期不合理！修改失败！");
		return;
	}
	puts("请输入修改后的出版社：");
	char press[PRESS_MAX] = {};
	if(!get_str_chinese(press,sizeof(press)))
	{
		puts("修改失败！");
		return;
	}
	puts("请输入修改后的售价：");
	double price = 0;
	clear_stdin();
	scanf("%lf",&price);
	if(!input_char_judgment())
	{
		return;
	}
	if(0 > price)
	{
		puts("售价不合理，修改失败！");
		return;
	}
	puts("请输入修改后的借阅次数：");
	int borrow_num = 0;
	clear_stdin();
	scanf("%d",&borrow_num);
	if(!input_char_judgment())
	{
		return;
	}
	if(0 > borrow_num)
	{
		puts("借阅次数不合理，修改失败！");
		return;
	}
	puts("请输入修改后的评分：");
	double score = 0;
	clear_stdin();
	scanf("%lf",&score);
	if(!input_char_judgment())
	{
		return;
	}
	if(0 > score || 100 < score)
	{
		puts("评分不合理，修改失败！");
		return;
	}
	strcpy(book->title,title);
	strcpy(book->author,author);
	book->time = time;
	strcpy(book->press,press);
	book->price = price;
	book->borrow_num = borrow_num;
	book->score = score;
	puts("修改成功！");
}


void _show_seek_book(void)
{
	system("clear");
	puts("***************************");
	puts("1.id查找");
	puts("2.书名查找");
	puts("3.作者查找");
	puts("4.返回上一级");
}
//	查找图书
void seek_book(void)
{
	for(;;)
	{
		_show_seek_book();
		switch(get_cmd('1','4'))
		{
			case '1':
			{
				seek_book_id();		//	id准确查找
				anykey_continue();
			}
			break;
			case '2':
			{
				seek_book_title();		//	书名模糊查找
				anykey_continue();
			}
			break;
			case '3':
			{
				seek_book_author();		//	作者名字模糊查找
				anykey_continue();
			}
			break;
			case '4':
			{
				return;
			}
			break;
		}
	}
}
//	列出所有图书信息
void show_book(void)
{
	show_num(book_list,10,_show_book);
}

//	导入图书
void import_book(void)
{
	puts("请输入要导入图书信息的文件：");
	char str[FILENAME] = {};
	if(!get_str_chinese(str,sizeof(str)))
	{
		puts("导入失败！");
		return;
	}
	strcpy(book_path,str);
	load_book();
	puts("导入成功！");
}
//	导出图书
void export_book(void)
{
	puts("请输入要导入图书信息的文件：");
	char str[FILENAME] = {};
	if(!get_str_chinese(str,sizeof(str)))
	{
		puts("导出失败！");
		return;
	}
	strcpy(book_path,str);
	save_book();
	puts("导入成功！");
}
//	修改密码	调用读者管理员修改密码


