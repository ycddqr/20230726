#include "readeroper.h"
#include "books.h"
#include "manageroper.h"
#include "readers.h"

Reader *reader;


bool reader_login(void)
{
	puts("请输入读者用户id：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("登录失败！");
		return false;
	}
	reader = query_list(reader_list,id,cmp_reader_id);
	if(NULL == reader)
	{
		puts("未找到读者用户，登录失败！");
		return false;
	}
	puts("请输入密码：");
	char pwd[PWD_MAX] = {};
	if(!get_pwd(pwd,sizeof(pwd)))
	{
		puts("登录失败！");
		return false;
	}
	if(0 != strcmp(pwd,reader->pwd))
	{
		puts("密码错误！");
		return false;
	}
	return true;
}

//	显示个人信息
void show_reader_information(void)
{
	_show_reader(reader);
}

//	修改密码
void reader_mod_pwd(void)
{
	puts("请输入原密码：");
	char pwd[PWD_MAX] = {};
	if(!get_pwd(pwd,sizeof(pwd)))
	{
		puts("修改失败！");
		return;
	}
	if(0 != strcmp(pwd,reader->pwd))
	{
		puts("输入密码错误！");
		return;
	}
	set_up_pwd(reader->pwd);
}
//	检索图书		调用图书管理员	seek_book();
//	借阅图书
void reader_borrow_book(void)
{
	if(3 == reader->borrow_book_num)
	{
		puts("已借3本书，无法继续借阅！");
		return;
	}
	puts("请输入要借阅书的编号：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("借阅失败！");
		return;
	}
	Book *book = query_list(book_list,id,cmp_book_id);
	if(NULL == book)
	{
		puts("未找到图书！");
		return;
	}
	if(IN_NOORDER == book->state)
	{
		if(book->price > reader->balance)
		{
			puts("余额不足，借阅失败！");
			return;
		}
		puts("借阅成功！");
		reader->borrow_book[reader->borrow_book_num] = atoi(book->id);
		reader->borrow_book_num++;
		reader->balance -= book->price;
		book->state = OUT_NOORDER;
		book->borrow_num++;
		book->borrow_time = time(NULL);
		return;
	}
	else if(IN_ORDER)		//	判断图书是否是这个读者预定
	{
		for(int i = 0; i < reader->reservation_book_num; i++)
		{
			if(reader->reservation_book[i] == atoi(book->id))		//	是这个读者预定的，可以借
			{
				if(book->price > reader->balance)
				{
					puts("余额不足，借阅失败！");
					return;
				}
				puts("借阅成功！");
				reader->reservation_book_num--;
				for(int j = i; j < reader->reservation_book_num; j++)
				{
					reader->reservation_book[j] = reader->reservation_book[j+1];
				}
				reader->balance -= book->price;
				book->state = OUT_NOORDER;
				book->borrow_num++;
				book->borrow_time = time(NULL);
				return;
			}
		}
		puts("图书已被他人预定，无法借阅！");				//	非读者预定，无法借阅
	}
	else
		puts("图书已被借出，无法借阅！");
}

//	预约图书
void reader_reservation_book(void)
{
	if(3 == reader->reservation_book_num)
	{
		puts("已预约3本书，无法继续预约！");
		return;
	}
	puts("请输入要预约书的编号：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("预约失败！");
		return;
	}
	Book *book = query_list(book_list,id,cmp_book_id);
	if(NULL == book)
	{
		puts("未找到图书！");
		return;
	}
	if(IN_NOORDER == book->state)
	{
		puts("书在馆未被预约，要直接借阅还是预约。");
		puts("1.借阅		2.预约");
		clear_stdin();
		int num = 0;
		scanf("%d",&num);
		if(!input_char_judgment())
		{
			return;
		}
		if(1 != num && 2 != num)
		{
			puts("输入不符合，失败！");
			return;
		}
		if(1 == num)
		{
			if(3 == reader->borrow_book_num)
			{
				puts("已借3本书，无法继续借阅！");
				return;
			}
			if(book->price > reader->balance)
			{
				puts("余额不足，借阅失败！");
				return;
			}
			puts("借阅成功！");
			reader->borrow_book[reader->borrow_book_num] = atoi(book->id);
			reader->borrow_book_num++;
			reader->balance -= book->price;
			book->state = OUT_NOORDER;
			book->borrow_num++;
			book->borrow_time = time(NULL);
			return;
		}
		if(2 == num)
		{
			puts("预约成功！");
			reader->reservation_book[reader->reservation_book_num] = atoi(book->id);
			reader->reservation_book_num++;
			book->state = IN_ORDER;
			return;
		}
	}
	else if(OUT_NOORDER == book->state)
	{
		puts("预约成功！");
		reader->reservation_book[reader->reservation_book_num] = atoi(book->id);
		reader->reservation_book_num++;
		book->state = OUT_ORDER;
		return;
	}
	else
		puts("图书已被他人预约，预约失败！");
}
//	归还图书
void reader_return_book(void)
{
	puts("请输入要归还书的编号：");
	char id[ID_MAX] = {};
	if(!get_str(id,sizeof(id)))
	{
		puts("归还失败！");
		return;
	}
	Book *book = query_list(book_list,id,cmp_book_id);
	if(NULL == book)
	{
		puts("未找到图书！");
		return;
	}
	for(int i = 0; i < reader->borrow_book_num; i++)
	{
		if(reader->borrow_book[i] == atoi(book->id))	//	已借书目录中找到该书
		{
			reader->borrow_book_num--;
			for(int j = i; j < reader->borrow_book_num; j++)
			{
				reader->borrow_book[j] = reader->borrow_book[j+1];
			}
			puts("还书成功！");
			if(OUT_NOORDER == book->state)
				book->state = IN_NOORDER;
			else
				book->state = IN_ORDER;
			unsigned long t = time(NULL) - book->borrow_time;
			printf("已借书时间：\t%lu 小时\t%lu 分\t%lu 秒\n",t/3600,t%3600/60,t%60);
			puts("请给此书评分(0~100)：");
			double score = -1;
			clear_stdin();
			scanf("%lf",&score);
			while(0 > score || 100 < score || !input_char_judgment())
			{
				puts("请重新评分：");
				clear_stdin();
				scanf("%lf",&score);
			}
			if(-1 == book->score)
				book->score = score;
			else
				book->score = (book->score + score)/2;
			return;
		}
	}
	puts("您未借此书，还书失败！");
}








