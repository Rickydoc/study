//头文件，结构和函数的声明
#pragma once
#include <stdio.h>
#include <time.h>
//构造链表来存储学生信息
typedef struct Card
{
	//账号
	char id[40];
	//密码
	char password[40];
	//上机状态（0没有/1正在/2注销）
	int status;
	//使用结构存储开卡的年月日等时间信息
	struct Starttime
	{
		int year, month, day, hour, min, sec;
	}starttime, endtime, lasttime;
	//累计消费金额
	float totaluse;
	//上机次数
	int count;
	//卡里余额
	float balance;
	//卡是否已经注销（0/1）
	int del;
	//在定义里调用自己作为元素，在语法里是被允许的，创建一个指针指向下一个节点
	struct Card* next;
}card;
//用一个新的结构体来记录链表的头地址以及创建节点的数量
typedef struct Cardlist
{
	card* head;
	int count;
}cardlist;
void outputmenu();
card* addcard();
