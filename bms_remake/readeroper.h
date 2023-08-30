#ifndef READEROPER_H
#define READEROPER_H

#include "public.h"

extern List *reader_list;
extern List *book_list;

bool reader_login(void);

//	显示个人信息
void show_reader_information(void);
//	修改密码
void reader_mod_pwd(void);
//	检索图书		调用图书管理员	seek_book();
//	借阅图书
void reader_borrow_book(void);
//	预约图书
void reader_reservation_book(void);
//	归还图书
void reader_return_book(void);

#endif
