#include "public.h"

bool input_str_judgment(char *str)		//	判断输入是否合理 是否是纯数字
{
	 while(*str)						//	循环，查看字符串每个字符的内容
	 {
	 	if(48 > *str || 57 < *str)		//	当其中一个字符不在数字ASCII码范围内
	 		return false;				//	此事字符串中不是纯数字，返回false
	 	str++;							//	继续查看下一个字符
	 }
	 return true;						//	退出循环时说明整个字符串都为数字，返回true
}

bool input_char_judgment(void)					//	判断数字输入是否含有字符
{
	if(10 != *(stdin->_IO_read_ptr) 			//	回车不算
		&& (48 >*(stdin->_IO_read_ptr) 			//	使用scanf输入数字读取到字符时会把字符留在输入缓冲区
		|| 57 < *(stdin->_IO_read_ptr)))			//	判断缓冲区的指针指向的东西的ASCII码取值
	{
		puts("输入中有字符，失败！");				//	ASCII码取值不在48到57内，则输入有其他字符
		return false;							//	输入不为纯数字，返回false
	}
	return true;
}

char get_cmd(char start,char end)
{
	puts("--------------------------");
	printf("请输入指令:");
	
	clear_stdin();
	for(;;)
	{
		char cmd = getch();
		if(start <= cmd && cmd <= end)
		{
			printf("%c\n",cmd);
			return cmd;
		}
	}
}

void anykey_continue(void)		//	按任意键继续 功能
{
	clear_stdin();				//	清除输入缓冲区
	puts("按任意键继续...");
	getch();					//	获取任意按键
}

void clear_stdin(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}

// 安全的字符串输入		数据过长返回NULL

char* get_str_chinese(char* str,size_t size)
{
	if(NULL == str || 1 >= size)
		return NULL;

	// 清理输入缓冲区
	clear_stdin();

	// 以安全的方式读取size-1个字符
	fgets(str,size,stdin);

	// 计算字符串的长度
	size_t len = strlen(str);

	if('\n' == str[0])
	{
		puts("输入为空！");
		return NULL;
	}
	// 判断最后一个字符是否是\n
	if('\n' == str[len-1])
		// 删除'\n'
		str[len-1] = '\0';
	else
	{
		puts("输入数据过长！");
		return NULL;
	}
	return str;	
}

char* get_str(char* str,size_t size)
{
	clear_stdin();							//	清除输入缓冲区
	size_t cnt = 0;
	while(cnt < size)						//	当输入个数小于总长度-1，进入循环
	{
		str[cnt] = getch();				//	获取按键
		
		if(10 == str[cnt])				//	遇到换行退出循环
		{
			break;
		}
		else if(27 == str[cnt])			//	遇到esc退出
		{
			return "\27";
		}
		else if(127 == str[cnt])			//	按下删除键，删除上一个字符
		{
			if(cnt > 0)
				cnt--;						//	删除时长度减1
			printf("\b \b");				//	光标向前移动
		}
		else
		{
			printf("%c",str[cnt]);
			cnt++;							//	输入字符正确，长度加1
		}
	}
	if(0 == cnt)
	{
		puts("输入数据为空！");
		return NULL;
	}
	if(size == cnt)
	{
		printf("\n");
		puts("输入数据过长！");
		return NULL;
	}
	str[cnt] = '\0';						//	在字符串最后加\0
	printf("\n");
	return str;
}
// 安全的密码输入
char* get_pwd(char* passwd,size_t size)
{
	clear_stdin();							//	清除输入缓冲区
	size_t cnt = 0;
	while(cnt < size)						//	当输入个数小于总长度-1，进入循环
	{
		passwd[cnt] = getch();				//	获取按键
		if(10 == passwd[cnt])				//	遇到换行退出循环
		{
			break;
		}
		else if(27 == passwd[cnt])
		{
			return "\27";
		}
		else if(127 == passwd[cnt])			//	按下删除键，删除上一个字符
		{
			if(cnt > 0)
				cnt--;						//	删除时长度减1
			printf("\b \b");				//	光标向前移动
		}
		else
		{
			cnt++;							//	输入字符正确，长度加1
			printf("*");					//	输入字符时显示*
		}
	}
	if(0 == cnt)
	{
		puts("输入数据为空！");
		return NULL;
	}
	if(size == cnt)
	{
		printf("\n");
		puts("输入数据过长！");
		return NULL;
	}
	passwd[cnt] = '\0';						//	在字符串最后加\0
	printf("\n");
	return passwd;
}
void put_str(const char* str,float sec)			//	以秒为单位显示字符串
{
	printf("%s\n",str);
	usleep(sec*1000000);						//	usleep把线程挂起一段时间，单位为微秒
}

static Node* create_node(const void* ptr)
{
	Node* node = malloc(sizeof(Node));
	node->ptr = (void*)ptr;
	node->prev = node;
	node->next = node;
	return node;
}

//	两个节点之间添加一个新节点
static void _add_node(Node* p,Node* n,const void* ptr)
{
	Node* node = create_node(ptr);
	p->next = node;
	n->prev = node;
	node->prev = p;
	node->next = n;
}
//	删除节点
static void _del_node(Node* node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node);
}
//	根据位置访问节点
static Node* _index_list(List* list,size_t index)
{
	if(index >= list->size) return NULL;
	if(index < list->size/2)
	{
		Node* n = list->head->next;
		while(index--) n = n->next;
		return n;
	}
	else
	{
		Node* n = list->head->prev;
		while(++index < list->size) n = n->prev;
		return n;
	}
}

//	创建链表
List* create_list(void)
{
	List* list = malloc(sizeof(List));
	list->head = create_node(NULL);	//	带头节点
	list->size = 0; 
	return list;
}


//	尾添加
void add_tail_list(List* list,const void* ptr)
{
	_add_node(list->head->prev,list->head,ptr);
	list->size++;
}

void *del_index_list(List *list,size_t index)
{
	Node *node = _index_list(list,index);
	if(NULL == node)
		return NULL;
	void *ptr = node->ptr;
	_del_node(node);
	list->size--;	
	return ptr;
}

//	按值删除
void* del_value_list(List* list,const void* ptr,CMP cmp)
{
	for(Node* n=list->head->next; n!=list->head; n=n->next)
	{
		if(0 == cmp(n->ptr,ptr))
		{
			void* ptr = n->ptr;
			_del_node(n);
			list->size--;
			return ptr;
		}
	}
	return NULL;
}

//	遍历 
void show_list(List* list,SHOW show)
{
	for(Node* n=list->head->next; n!=list->head; n=n->next)
	{
		show(n->ptr);
	}
} 


//	按值修改
bool mod_value_list(List* list,const void* old,CMP cmp,const void* data,CHANGE change)
{
	for(Node* n=list->head->next; n!=list->head; n=n->next)
	{
		if(0 == cmp(n->ptr,old))
		{
			change(n->ptr,data);
			return true;
		}
	}
	return false;
}

//	查询 
void* query_list(List* list,const void* data,CMP cmp) 
{
	for(Node* n=list->head->next; n!=list->head; n=n->next)
	{
		if(0 == cmp(n->ptr,data)) return n->ptr;
	}
	return NULL;
} 

//	模糊查询 
bool query_list_vague(List* list,const void* data,CMP cmp,SHOW show)
{
	bool flag = false;
	for(Node* n=list->head->next; n!=list->head; n=n->next)		// 遍历链表
	{
		if(0 == cmp(n->ptr,data))								// 与要找的值相等
		{
			flag = true;										// 找到返回true，一个也没找到返回flase
			show(n->ptr);										// 显示信息
		}
	}
	return flag;
} 

//	清空
void clear_list(List* list)
{
	while(list->size--)
	{
		void* ptr = del_index_list(list,0);	
		free(ptr);
	}
}

//	销毁
void destroy_list(List* list)
{
	clear_list(list);
	free(list->head);
	free(list);
}

//	设置密码输入两次
void set_up_pwd(char *str)
{
	char pwd[PWD_MAX] = {};
	char pwd2[PWD_MAX] = {};
	puts("请设置密码：");
	while(!get_pwd(pwd,sizeof(pwd)))
		puts("请再次输入：");
	puts("请再次输入密码：");
	while(!get_pwd(pwd2,sizeof(pwd2)))
		puts("请再次输入：");
	while(0 != strcmp(pwd,pwd2))
	{
		puts("两次密码不一致，请重新输入！");
		while(!get_pwd(pwd,sizeof(pwd)))
			puts("请再次输入：");
		puts("请再次输入密码：");
		while(!get_pwd(pwd2,sizeof(pwd2)))
			puts("请再次输入：");
	}
	puts("设置密码成功！");
	strcpy(str,pwd);
}

/*
void show_num(List *list,int num,SHOW show)			//	分页显示
{
	int cnt = 0,page = 1;
	bool flag = false;
	Node *n = list->head->next;
	system("clear");
	for(; num > cnt && n != list->head; cnt++,n = n->next)
	{
		show(n->ptr);
	}
	puts("上箭头上翻，下箭头下翻，esc退出");
	printf("---------第%d页----------\n",page);
	for(;;)
	{
		int key = getch();
		flag = false;
		if(27 == key)				//	按下esc
			return;
		if(184 == key)				//	按下向下
		{
			system("clear");
			for(cnt = 0; num > cnt && n != list->head; cnt++,n = n->next)
			{
				flag = true;
				show(n->ptr);
			}
			if(list->head == n)
				n = list->head->next;
			if(flag)
				page++;
			puts("上箭头上翻，下箭头下翻，esc退出");
			printf("---------第%d页----------\n",page);
		}
		if(183 == key)				//	按下向上
		{
			system("clear");
			for(int i = 0; i < num && n->prev != list->head; i++)
				n = n->prev;
			for(cnt = 0; num > cnt && n != list->head; cnt++,n = n->next)
			{	
				flag = true;
				show(n->ptr);
			}
			if(list->head == n)
				n = list->head->next;
			if(flag)
				page--;
			puts("上箭头上翻，下箭头下翻，esc退出");
			printf("---------第%d页----------\n",page);
		}
	}
}
*/

void show_num(List* list, int num, SHOW show)
{
	int cnt = 0;
	Node* n = list->head->next;
	system("clear");
	for(; n!=list->head && cnt < num; n=n->next, cnt++)
	{
		show(n->ptr);
	}
	printf("按上箭头上翻一页，下箭头下翻一页，按esc退出显示\n");
	int cmd = getch(), cnt_num = cnt;
	while(cmd != 27)	//	esc退出
	{
		cnt = 0;
		if(cmd == 183 && cnt_num > num)	//	往上翻一页
		{	
			
			system("clear");
			int differ = cnt_num%num ? cnt_num%num+num : num*2;
			cnt_num -= differ;
			while(differ--) n = n->prev;
			for(; n!=list->head && cnt < num; n=n->next, cnt++, cnt_num++)
			{
				show(n->ptr);
			}
			printf("按上箭头上翻一页，下箭头下翻一页，按esc退出显示\n");
		}
		if(cmd == 184 && cnt_num < list->size)	//	往下翻一页
		{
			system("clear");
			for(; n!=list->head && cnt < num; n=n->next, cnt++, cnt_num++)
			{
				show(n->ptr);
			}
			printf("按上箭头上翻一页，下箭头下翻一页，按esc退出显示\n");
		}		
		cmd = getch();
	}	
}



