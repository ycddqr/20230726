#ifndef READERS_H
#define READERS_H

#include "public.h"

typedef struct Reader
{
	char id[ID_MAX];								//				20
	char name[NAME_MAX];							//	姓名			20
	char pwd[PWD_MAX];								//	密码			20
	char phone[PHONE_MAX];							//	手机			12
	int borrow_book_num;							//	已借图书数	4
	int reservation_book_num;						//	预约图书数	4
	double balance;									//	余额			8
	int borrow_book[BORROW_BOOK_NUM_MAX];			//	已借图书信息	12
	int reservation_book[RESERVATION_BOOK_NUM_MAX];	//	预约图书信息	12
}Reader;

#endif
