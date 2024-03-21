#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// 这个函数专门用来输出一级菜单
void outputmenu()
{
	printf("----------菜单----------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("0.退出\n");
	printf("r.清空所有数据\n");
	printf("d.链表节点删除\n");
	printf("i.链表节点插入\n");
	printf("请选择菜单编号(0-8)：");
}

// 链表结构体定义
// TODO链表定义
typedef struct Card
{
	// 账号
	char id[40];
	// 密码
	char password[40];
	// 上机状态（0没有/1正在/2注销）
	int status;
	// 使用结构存储开卡的年月日等时间信息
	struct Starttime
	{
		int year, month, day, hour, min, sec;
	} starttime, endtime, lasttime;
	// 累计消费金额
	float totaluse;
	// 上机次数
	int count;
	// 卡里余额
	float balance;
	// 卡是否已经注销（0/1）
	int del;
	// 在定义里调用自己作为元素，在语法里是被允许的（递归），创建一个指针指向下一个节点
	struct Card *next;
} card;

// 创建一个结构体存储链表的头结点以及节点数量
typedef struct Cardlist
{
	card *head;
	int count;
} cardlist;

// 创建一个函数用来创建链表的节点
// 由于链表节点的创建是动态的，所以会重复多次，因此用函数来做比较合适
card *addcard()
{
	// 定义一个变量作为返回值，它需要指向一个结构体
	card *stu;
	// 使用malloc（memory allocation）函数分配一块内存，malloc是标准库函数，接受一个整数作为形参，也就是需要分配内存的大小
	// sizeof用于获取结构的大小，然后好让malloc函数为其分配相应的内存
	// 但是此时，它仅仅是一块空白内存，并不知道它需要存储什么类型的数据结构，需要用强制类型转换进行标识
	stu = (card *)malloc(sizeof(card));
	// 分配完地址后，让next指向NULL，不要让它成为野指针（程序的健壮性）
	stu->next = NULL;
	// 返回创建节点的地址
	return stu;
}

// 这个函数用来将成员信息写入文件
void spec2(cardlist member)
{
	int i;
	long int hash=7;
	char x;
	FILE *p;
	// 使用card指针来遍历链表节点
	card *f;
	// 指向链表头节点
	f = member.head;
	if ((p = fopen("member.txt", "w")) == NULL)
	{
		printf("Error：无法写入文件！");
		exit(0);
	}
	
	// TODO开始写入.txt文件
	// 这个文件是方便计算机读取的
	fprintf(p, "%d\n", member.count);
	for (i = 1; i < member.count; i++)
	{
		fputs(f->id, p);
		fprintf(p, "\n");
		fputs(f->password, p);
		fprintf(p, "\n");
		fprintf(p, "%d %d %d %d %d %d %d %f %d %f %d\n", f->status, f->starttime.year, f->starttime.month, f->starttime.day, f->starttime.hour, f->starttime.min, f->starttime.sec, f->totaluse, f->count, f->balance, f->del);
		fprintf(p, "%d %d %d %d %d %d\n", f->lasttime.year, f->lasttime.month, f->lasttime.day, f->lasttime.hour, f->lasttime.min, f->lasttime.sec);
		fprintf(p, "%d %d %d %d %d %d\n", f->endtime.year, f->endtime.month, f->endtime.day, f->endtime.hour, f->endtime.min, f->endtime.sec);
		f=f->next;
	}
	fclose(p);

	//TODO通过散列函数计算文件的哈希值，以防文件被篡改
	if ((p = fopen("member.txt", "r")) == NULL)
	{
		printf("Error：哈希加密失败，无法读取文件！");
		exit(0);
	}
	while((x=fgetc(p))!=EOF)
	{
		hash+=x+55;
        hash+=hash/2;
		hash++;
	}
	fclose(p);
	if ((p = fopen("hash.txt", "w")) == NULL)
	{
		printf("Error：哈希加密失败，无法存储哈希值！");
		exit(0);
	}
	else{
		fprintf(p,"%ld",hash);
	}
	fclose(p);
	
	//TODO写入Excel文件给人看
	f = member.head;
	remove("member.xls");
	if ((p = fopen("member.csv", "w")) == NULL)
	{
		printf("Error：无法写入文件！\n");
		exit(0);
	}
	//由于编码问题，只能用英文代替
	fprintf(p, "Num\tID\tPassword\tStatus\tStartTime\tTotaluse(RMB)\tCount\tBalance(RMB)\tDel status\n");
	for (i=1;i<member.count;i++){
		fprintf(p,"%d\t%s\t%s\t%d\t%04d/%02d/%02d %02d:%02d:%02d\t%.2f\t%d\t%.2f\t%d\n",i,f->id,f->password,f->status,f->starttime.year,f->starttime.month,f->starttime.day,f->starttime.hour,f->starttime.min,f->starttime.sec,f->totaluse,f->count,f->balance,f->del);
		f=f->next;
	}
	fclose(p);
	rename("member.csv","member.xls");
}

// 按键0所对应的功能
char func0()
{
	int i;
	char t[20];
	printf("是否退出计费管理系统？退出请只按一个0，取消按其它键...\n");
	for (i = 0; i < 20; i++)
		t[i] = '\0';
	gets(t);
	if (t[1] != '\0')
		return '1';
	if (t[0] == '0')
		return '0';
	return '1';
}

// 用来实现添加卡的功能，将其保存在链表中
// 函数的返回类型为cardlist，是因为函数的形参并不能反过来改变实参，因此需要将开卡后的新的链表汇总信息返回给main函数
cardlist func1(card *p, cardlist l)
{
	char id[40], password[40];
	int i = 0, j = 0, k = 0, charg = 0;
	float charge;
	p=l.head;
	for (i = 0; i < 40; i++)
	{
		id[i] = '\0';
		password[i] = '\0';
	}
	printf("----------添加卡----------\n");
	printf("请输入新卡的账号<长度为1-18>：");
	gets(id);

	// 检测账号长度是否合法
	if (id[18] != '\0')
	{
		printf("Error：账号长度最多为18位！请重新开始操作...\n");
		return l;
	}

	// 检测id是否重复，顺便跳过存储信息的节点进入新节点
	for (i = 0; i < l.count - 1; i++)
	{
		j = 0;
		for (k = 0; k < 40; k++)
		{
			if (p->id[k] != id[k])
				j++;
		}
		if (j == 0)
		{
			charg++;
		}
		p=p->next;
	}
	if (charg != 0)
	{
		printf("Error：注册失败，该账号已被注册！\n");
		return l;
	}
	
	// id不重复，开始注册并存储开卡信息
	printf("请设置新卡的密码<长度为10-18>：");
	gets(password);

	// 监测密码长度是否合法
	if (password[18] != '\0')
	{
		printf("Error：密码长度最多为18位！请重新开始操作...\n");
		return l;
	}
	else if (password[9] == '\0')
	{
		printf("Error：密码长度最少为10位！请重新开始操作...\n");
		return l;
	}

	// 输入金额
	printf("请输入开卡的金额<RMB>：");
	scanf_s(" %f", &charge);
	getchar();
	
	// 注册账号成功后开始初始化并存储开卡信息
	for (i = 0; i < 40; i++)
	{
		p->id[i] = id[i];
		p->password[i] = password[i];
	}
	p->status = 0;
	p->balance = charge;
	p->count = 0;
	p->totaluse = 0;
	p->del = 0;

	// 记录注册账号的时间
	// TODO注册时间
	time_t a;
	struct tm t;
	a = time(NULL);
	localtime_s(&t, &a);
	p->starttime.year = t.tm_year + 1900;
	p->starttime.month = t.tm_mon + 1;
	p->starttime.day = t.tm_mday;
	p->starttime.hour = t.tm_hour;
	p->starttime.min = t.tm_min;
	p->starttime.sec = t.tm_sec;
	p->lasttime = p->starttime;
	p->endtime = p->starttime;

	// 格式化输出开卡信息
	printf("开卡成功，恭喜您成为本店第%d位用户！\n", l.count);
	printf("------添加的卡的信息如下------\n");
	printf("账号：");
	puts(p->id);
	printf("密码：");
	puts(p->password);
	printf("余额：%.2f\n", p->balance);
	printf("开卡时间：%d/%02d/%02d %02d:%02d:%02d\n", p->starttime.year, p->starttime.month, p->starttime.day, p->starttime.hour, p->starttime.min, p->starttime.sec);
	printf("上机状态（0/1/2）：%d\n", p->status);

	// 开卡信息储存完成后，创建下一个空节点以待使用
	p->next = addcard();
	// 链表信息汇总，节点数量加1
	l.count++;
	// 将开卡后的信息写入文件
	spec2(l);
	// 将开卡后的新的链表汇总信息返回给main函数
	return l;
}

//用来实现查询卡的功能
void func2(cardlist l)
{
	char id[40];
	int i,j,k,charge=0;
	card *p;
	p=l.head;
	for (i=0;i<40;i++)
	{
		id[i]='\0';
	}
	printf("----------查询卡----------\n");
	printf("请输入你想要查询的卡号：");
	gets(id);
	for (i=1;i<l.count;i++)
	{
		k=0;
		for (j=0;j<40;j++)
		{
			if (p->id[j]!=id[j])
				k++;
		}
		if (k==0)
		{
			printf("------查询的卡的信息如下------\n");
			printf("账号：");
			puts(p->id);
			printf("密码：");
			puts(p->password);
			printf("开卡时间：");
			printf("%04d/%02d/%02d %02d:%02d:%02d\n",p->starttime.year,p->starttime.month,p->starttime.day,p->starttime.hour,p->starttime.min,p->starttime.sec);
			printf("上机状态（0/1/2）：%d\n",p->status);
			printf("上一次使用时间：");
			printf("%04d/%02d/%02d %02d:%02d:%02d\n",p->lasttime.year,p->lasttime.month,p->lasttime.day,p->lasttime.hour,p->lasttime.min,p->lasttime.sec);
			printf("上机次数：%d\n",p->count);
			printf("累计消费金额（RMB）：%.2f\n",p->totaluse);
			printf("卡里余额（RMB）：%.2f\n",p->balance);
			printf("是否注销（0/1）：%d\n",p->del);
			if (p->del==1)
			{
				printf("注销时间：");
				printf("%04d/%02d/%02d %02d:%02d:%02d\n",p->endtime.year,p->endtime.month,p->endtime.day,p->endtime.hour,p->endtime.min,p->endtime.sec);
			}
			return ;
		}
		p=p->next;
	}
	printf("卡号输入错误，未查询到该卡信息！\n");
}

//TODO充值卡
void func5(cardlist l)
{
	float charge;
	card *p;
	int i,j,k;
	char id[40];
	for (i=0;i<40;i++)
	{
		id[i]='\0';
	}
	printf("----------充值卡----------\n");
	printf("请输入需要充值的卡号：");
	gets(id);
	p=l.head;
	//开始遍历链表查询是否存在该卡
	for (i=1;i<l.count;i++)
	{
		k=0;
		for (j=0;j<40;j++)
		{
			if (p->id[j]!=id[j])
				k++;
		}
		if (k==0)
		{
			//如果卡号存在，开始进行充值操作
			printf("该卡累计消费金额为（RMB）：%.2f\n",p->totaluse);
			printf("该卡剩余余额为（RMB）：%.2f\n",p->balance);
			printf("请输入充值金额：");
			scanf("%f",&charge);
			getchar();
			p->balance+=charge;
			printf("充值成功！该卡当前余额为（RMB）：%.2f\n",p->balance);
			//充值完成则存入文件
			spec2(l);
			return ;
		}
		p=p->next;
	}
	printf("充值失败，卡号输入错误，未查询到该卡信息！\n");
}

//TODO删除节点（个人觉得没必要，为了上台演示写一下）
cardlist dele(cardlist l)
{
	card *p,*a;
	int i,j,k;
	char id[40];
	for (i=0;i<40;i++)
		id[i]='\0';
	printf("----------链表节点删除----------\n");
	printf("请输入需要删除的节点卡号：");
	gets(id);
	p=l.head;
	for (i=1;i<l.count;i++)
	{
		k=0;
		for (j=0;j<40;j++)
		{
			if (p->id[j]!=id[j])
				k++;
		}
		if (k==0)
		{
			if (i==1)
			{
				l.head=p->next;
				l.count--;
				spec2(l);
				free(p);
				printf("节点删除成功！\n");
				return l;
			}
			//如果节点存在，开始进行删除操作
			a->next=p->next;
			l.count--;
			spec2(l);
			free(p);
			printf("节点删除成功！\n");
			return l;
		}
		a=p;
		p=p->next;
	}
	printf("该节点不存在！\n");
	return l;
}

//TODO插入节点（更没用了）
cardlist insert(cardlist l)
{
	char id[40],idd[40], password[40];
	card *p,*q;
	q=addcard();
	int i = 0, j = 0, k = 0, charg = 0;
	float charge;
	p=l.head;
	for (i = 0; i < 40; i++)
	{
		id[i] = '\0';
		password[i] = '\0';
		idd[i]='\0';
	}
	printf("----------插入卡----------\n");
	printf("请输入新卡的账号<长度为1-18>：");
	gets(id);

	// 检测账号长度是否合法
	if (id[18] != '\0')
	{
		printf("Error：账号长度最多为18位！请重新开始操作...\n");
		return l;
	}

	// 检测id是否重复，顺便跳过存储信息的节点进入新节点
	for (i = 0; i < l.count - 1; i++)
	{
		j = 0;
		for (k = 0; k < 40; k++)
		{
			if (p->id[k] != id[k])
				j++;
		}
		if (j == 0)
		{
			charg++;
		}
		p=p->next;
	}
	if (charg != 0)
	{
		printf("Error：注册失败，该账号已被注册！\n");
		return l;
	}

	// id不重复，开始注册并存储开卡信息
	printf("请设置新卡的密码<长度为10-18>：");
	gets(password);

	// 监测密码长度是否合法
	if (password[18] != '\0')
	{
		printf("Error：密码长度最多为18位！请重新开始操作...\n");
		return l;
	}
	else if (password[9] == '\0')
	{
		printf("Error：密码长度最少为10位！请重新开始操作...\n");
		return l;
	}

	// 输入金额
	printf("请输入开卡的金额<RMB>：");
	scanf_s(" %f", &charge);
	getchar();
	//存储信息
	for (i = 0; i < 40; i++)
	{
		q->id[i] = id[i];
		q->password[i] = password[i];
	}
	q->status = 0;
	q->balance = charge;
	q->count = 0;
	q->totaluse = 0;
	q->del = 0;

	// 记录注册账号的时间
	// TODO注册时间
	time_t a;
	struct tm t;
	a = time(NULL);
	localtime_s(&t, &a);
	q->starttime.year = t.tm_year + 1900;
	q->starttime.month = t.tm_mon + 1;
	q->starttime.day = t.tm_mday;
	q->starttime.hour = t.tm_hour;
	q->starttime.min = t.tm_min;
	q->starttime.sec = t.tm_sec;
	q->lasttime = q->starttime;
	q->endtime = q->starttime;

	//查找卡号
	p=l.head;
	printf("您需要插在哪张卡后面：");
	gets(idd);
	for (i = 0; i < l.count - 1; i++)
	{
		j = 0;
		for (k = 0; k < 40; k++)
		{
			if (p->id[k] != idd[k])
				j++;
		}
		if (j == 0)
		{
			q->next=p->next;
			p->next=q;
			l.count++;
			printf("插入卡成功！\n");
			spec2(l);
			return l;
		}
		p=p->next;
	}
	printf("该节点不存在！\n");
	return l;
}
