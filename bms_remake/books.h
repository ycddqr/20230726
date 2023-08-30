#ifndef BOOK_H
#define BOOK_H

#include "public.h"

typedef struct Book
{
	char id[ID_MAX];				//			20
	char title[TITLE_MAX];			//	书名		40
	char author[NAME_MAX];			//	作者		20
	int time;						//	出版日期	4
	char press[PRESS_MAX];			//	出版社	40
	double price;					//	售价		8
	int borrow_num;					//	借阅次数	4
	int state;						//	图书状态	4
	unsigned long borrow_time;		//	借书时间	4
	double score;					//	评分		8
}Book;

#endif
