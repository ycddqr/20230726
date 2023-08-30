#include "fileoper.h"
#include "superoper.h"
#include "managers.h"
#include "readers.h"
#include "books.h"

List *super_list;
List *manager_list;
List *reader_list;
List *book_list;

int manager_id;
int reader_id;
int book_id;

char super_path[FILENAME] = "./data/Super.txt";
char manager_path[FILENAME] = "./data/Manager.txt";
char reader_path[FILENAME] = "./data/Reader.txt";
char book_path[FILENAME] = "./data/Book.txt";
char id_path[FILENAME] = "./data/id.txt";

void load_id(void)				//	加载当前id最大值
{
	FILE *fp = fopen(id_path,"r");
	if(NULL == fp)
	{
		manager_id = 10001;
		reader_id = 20001;
		book_id = 30001;
		return;
	}
	fscanf(fp,"%d %d %d ",&manager_id,&reader_id,&book_id);
	fclose(fp);
}
void load_super(void)			//	加载超级管理员数据
{
	FILE *fp = fopen(super_path,"r");
	if(NULL == fp)			//	无超级管理员，创建
	{
		puts("检测到第一次登录");
		puts("创建超级管理员中...");
		puts("请设置超级管理员账号：");
		char name[NAME_MAX] = {};
		Super *super = malloc(sizeof(Super));
		while(!get_str_chinese(name,sizeof(name)))		//	设置super账号，判断空和过长
		{
			puts("请重新输入！");
			puts("请设置超级管理员账号：");
		}
		strcpy(super->name,name);
		set_up_pwd(super->pwd);					//	设置超级管理员密码
		puts("超级管理员创建成功！");
		add_tail_list(super_list,super);
		return;
	}
	Super *super = malloc(sizeof(Super));
	int ret = fscanf(fp,"%s %s ",super->name,super->pwd);
	if(2 == ret)
	{
		add_tail_list(super_list,super);
	}
	else
	{
		puts("超级管理员加载失败！");
	}
	fclose(fp);
}

void load_manager(void)			//	加载管理员数据
{
	FILE *fp = fopen(manager_path,"r");
	if(NULL == fp)
	{
		puts("无管理员数据！");
		return;
	}
	for(;;)
	{
		Manager *manager = malloc(sizeof(Manager));
		int ret = fscanf(fp,"%s %s %s %hhd\n",manager->id,manager->name,manager->pwd,&manager->post);
		if(4 == ret)
		{
			add_tail_list(manager_list,manager);
		}
		else
		{
			free(manager);
			break;
		}
	}
	fclose(fp);
}

void load_reader(void)			//	加载读者数据
{
	FILE *fp = fopen(reader_path,"r");
	if(NULL == fp)
	{
		puts("无读者数据！");
		return;
	}
	for(;;)
	{
		Reader *reader = malloc(sizeof(Reader));
		int ret = fscanf(fp,"%s %s %s %s %d %d %lf ",reader->id,reader->name,reader->pwd,reader->phone
			,&reader->borrow_book_num,&reader->reservation_book_num,&reader->balance);
		for(int i = 0; i < reader->borrow_book_num; i++)
		{
			fscanf(fp,"%d ",&reader->borrow_book[i]);
		}
		for(int i = 0; i < reader->reservation_book_num; i++)
		{
			fscanf(fp,"%d ",&reader->reservation_book[i]);
		}
		fscanf(fp,"\n");
		if(7 == ret)
		{
			add_tail_list(reader_list,reader);
		}
		else
		{
			free(reader);
			break;
		}
	}
	fclose(fp);
}

void load_book(void)			//	加载图书数据
{
	FILE *fp = fopen(book_path,"r");
	if(NULL == fp)
	{
		puts("无图书数据！");
		return;
	}
	for(;;)
	{
		Book *book = malloc(sizeof(Book));
		int ret = fscanf(fp,"%s %s %s %d %s %lf %d %d %lu %lf\n",book->id,book->title,book->author,&book->time,book->press
			,&book->price,&book->borrow_num,&book->state,&book->borrow_time,&book->score);
		if(10 == ret)
		{
			add_tail_list(book_list,book);
		}
		else
		{
			free(book);
			break;
		}
	}
	fclose(fp);
}


void save_id(void)
{
	FILE *fp = fopen(id_path,"w");
	fprintf(fp,"%d %d %d ",manager_id,reader_id,book_id);
	fclose(fp);
}

void save_super(void)			//	保存超级管理员数据
{
	FILE *fp = fopen(super_path,"w");
	Super *super = (Super *)(super_list->head->next->ptr);
	fprintf(fp,"%s %s ",super->name,super->pwd);
	free(super);
	fclose(fp);
}

void save_manager(void)			//	保存管理员数据
{
	FILE *fp = fopen(manager_path,"w");
	for(Node *node = manager_list->head->next; node != manager_list->head; node = node->next)
	{
		Manager *manager = (Manager *)(node->ptr);
		fprintf(fp,"%s %s %s %hhd\n",manager->id,manager->name,manager->pwd,manager->post);
	}
	fclose(fp);
}

void save_reader(void)			//	保存读者数据
{
	FILE *fp = fopen(reader_path,"w");
	for(Node *node = reader_list->head->next; node != reader_list->head; node = node->next)
	{
		Reader *reader = (Reader *)(node->ptr);
		fprintf(fp,"%s %s %s %s %d %d %lf ",reader->id,reader->name,reader->pwd,reader->phone
			,reader->borrow_book_num,reader->reservation_book_num,reader->balance);
		for(int i = 0; i < reader->borrow_book_num; i++)
		{
			fprintf(fp,"%d ",reader->borrow_book[i]);
		}
		for(int i = 0; i < reader->reservation_book_num; i++)
		{
			fprintf(fp,"%d ",reader->reservation_book[i]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void save_book(void)			//	保存图书数据
{
	FILE *fp = fopen(book_path,"w");
	for(Node *node = book_list->head->next; node != book_list->head; node = node->next)
	{
		Book *book = (Book *)(node->ptr);
		fprintf(fp,"%s %s %s %d %s %lf %d %d %lu %lf\n",book->id,book->title,book->author,book->time,book->press
			,book->price,book->borrow_num,book->state,book->borrow_time,book->score);
	}
	fclose(fp);
}


void load(void)
{
	super_list = create_list();
	manager_list = create_list();
	reader_list = create_list();
	book_list = create_list();
	load_id();
	load_super();
	load_manager();
	load_reader();
	load_book();
}

void save(void)
{
	sprintf(super_path,"./data/Super.txt");
	sprintf(manager_path,"./data/Manager.txt");
	sprintf(reader_path,"./data/Reader.txt");
	sprintf(book_path,"./data/Book.txt");
	save_id();
	save_super();
	save_manager();
	save_reader();
	save_book();
	destroy_list(super_list);
	destroy_list(manager_list);
	destroy_list(reader_list);
	destroy_list(book_list);
}
