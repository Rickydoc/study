#include "data.h"
int main()
{
    int j, count = 0, k;
    int x,charge=0;
    long int hash=7,key;
    char n, i = '1', t[20], m;
    FILE *file;

    // 创建一个card型指针，用来在链表上移动，精确操作链表每个节点
    card *member;

    // 创建一个cardlist型结构，用来汇总链表信息，存储链表的头节点以及节点数量，方便找到每个节点
    cardlist memberlist;

    // 创建链表并初始化链表信息，此时创建链表第一个节点，并记录头节点，初始化节点数量
    memberlist.head = addcard();
    memberlist.count = 1;

    // 让指针指向链表第一个节点，链表创建完成
    member = memberlist.head;
    if ((file = fopen("LOG.txt", "r")) == NULL)
    {
		fclose(file);
        if ((file = fopen("LOG.txt", "ab+")) == NULL)
		{
			printf("Error：无法写入日志文件！");
			exit(0);
		}
		fprintf(file,"--------------------程序日志--------------------\n");
        fclose(file);
    }

    // TODO读取文件
    // 初始化链表信息，将上一次程序运行的数据读取到当前程序
    FILE *p;
    if ((p = fopen("member.txt", "r")) == NULL)
    {
        // 检测是否存在能够读取成员信息的文件
        printf("无法查询到以前的成员信息，该文件不存在或者已经被损坏！\n");
        printf("请重新录入以往成员信息！\n");
        if ((file = fopen("LOG.txt", "ab+")) == NULL)
	    {
		    printf("Error：无法写入日志文件！");
		    exit(0);
	    }
	    fprintf(file,"\n程序启动时，检测到以往成员信息不存在！（");
        fclose(file);
        log();
        file = fopen("LOG.txt", "ab+");
        fprintf(file,"）\n");
        fclose(file);
        fclose(p);
    }
    else
    {
        //TODO检测保存数据的文件，判断文件是否被篡改
        while((x=fgetc(p))!=EOF)
	    {
		    hash+=x+55;
            hash+=hash/2;
		    hash++;
	    }
        fclose(p);
        if ((p = fopen("hash.txt", "r")) == NULL)
	    {
		    printf("Error：哈希文件丢失，无法判断成员信息是否被篡改！\n");
            printf("请重新录入以往成员信息！\n");
		    fclose(p);
            if ((file = fopen("LOG.txt", "ab+")) == NULL)
	        {
		        printf("Error：无法写入日志文件！");
		        exit(0);
	        }
	        fprintf(file,"\n程序启动时，检测到哈希文件丢失，并清除了其它成员信息！（");
            fclose(file);
            log();
            file = fopen("LOG.txt", "ab+");
            fprintf(file,"）\n");
            fclose(file);
            charge=0;
	    }
        else
        {
            fscanf(p,"%ld",&key);
            fclose(p);
            if (hash==key)
                charge=1;
            else
                charge=-1;
        }

        //哈希值不匹配，则不再读取数据并清除所有数据文件
        if (charge!=1)
        {
            remove("member.txt");
            if (charge==-1)
            {
                printf("Error：哈希值不匹配！数据文件被篡改！无法读取成员信息！\n");
                printf("请重新录入以往成员信息！\n");
            }
            if ((file = fopen("LOG.txt", "ab+")) == NULL)
	        {
		        printf("Error：无法写入日志文件！");
		        exit(0);
	        }
	        fprintf(file,"\n程序启动时，检测到数据文件被篡改，并清除了其它成员信息！（");
            fclose(file);
            log();
            file = fopen("LOG.txt", "ab+");
            fprintf(file,"）\n");
            fclose(file);
        }
        else
        {
            //数据一切正常，开始格式化读取信息
            // 读取第一行的节点数量
            p = fopen("member.txt", "r");
            fscanf(p, " %d", &memberlist.count);
            fgetc(p);
            for (k = 1; k < memberlist.count; k++)
            {
                for (j = 0; j < 40; j++)
                {
                    member->id[j] = '\0';
                    member->password[j] = '\0';
                }
                for (j = 0; 1; j++)
                {
                    m = fgetc(p);
                    if (m != '\n')
                        member->id[j] = m;
                    else
                    {
                        member->id[j] = '\0';
                        break;
                    }
                }
                for (j = 0; 1; j++)
                {
                    m = fgetc(p);
                    if (m != '\n')
                        member->password[j] = m;
                    else
                    {
                        member->password[j] = '\0';
                        break;
                    }
                }
                fscanf(p," %d",&member->status);
                fscanf(p," %d",&member->starttime.year);
                fscanf(p," %d",&member->starttime.month);
                fscanf(p," %d",&member->starttime.day);
                fscanf(p," %d",&member->starttime.hour);
                fscanf(p," %d",&member->starttime.min);
                fscanf(p," %d",&member->starttime.sec);
                fscanf(p," %f",&member->totaluse);
                fscanf(p," %d",&member->count);
                fscanf(p," %f",&member->balance);
                fscanf(p," %d",&member->del);
                fscanf(p," %d",&member->lasttime.year);
                fscanf(p," %d",&member->lasttime.month);
                fscanf(p," %d",&member->lasttime.day);
                fscanf(p," %d",&member->lasttime.hour);
                fscanf(p," %d",&member->lasttime.min);
                fscanf(p," %d",&member->lasttime.sec);
                fscanf(p," %d",&member->endtime.year);
                fscanf(p," %d",&member->endtime.month);
                fscanf(p," %d",&member->endtime.day);
                fscanf(p," %d",&member->endtime.hour);
                fscanf(p," %d",&member->endtime.min);
                fscanf(p," %d",&member->endtime.sec);
                m = fgetc(p);
                //TODO循环结束记得增加新的节点存储信息
                member->next=addcard();
                member=member->next;
            }
            fclose(p);
            if ((file = fopen("LOG.txt", "ab+")) == NULL)
	        {
		        printf("Error：无法写入日志文件！");
		        exit(0);
	        }
	        fprintf(file,"\n--------------------程序正常启动（");
            fclose(file);
            log();
            file = fopen("LOG.txt", "ab+");
            fprintf(file,"）----------\n");
            fclose(file);
        }
    }
    //读取文件操作完成，指针指向头结点，开始进入菜单界面
    member=memberlist.head;
    printf("欢迎来到计费管理系统面板\n");
    do
    {
        for (j = 0; j < 20; j++)
            t[j] = '\0';
        outputmenu();
        gets(t);
        // 检测输入信息是否合法
        if (t[1] != '\0')
        {
            printf("Error：无法识别输入信息，请重新操作！\n");
            continue;
        }
        n = t[0];
        // 信息合法则开始对应相应的功能
        switch (n)
        {
        case '0':
            i = func0();
            break;
        case '1':
            memberlist = func1(member, memberlist);
            break;
        case '2':
            func2(memberlist);
            break;
        case '3':
            func3(memberlist);
            break;
        case '4':
            func4(memberlist);
            break;
        case '5':
            func5(memberlist);
            break;
        case '6':
            func6(memberlist);
            break;
        case '7':
            func7(memberlist);
            break;
        case 'r':
            remove("member.xls");
            remove("member.txt");
            remove("hash.txt");
            printf("已清除所有数据！\n");
            //写入日志文件
            if ((file = fopen("LOG.txt", "ab+")) == NULL)
	        {
		        printf("Error：无法写入日志文件！");
		        exit(0);
	        }
            log();
            fprintf(file," 管理员清空了所有的成员数据！程序已终止！\n");
            fclose(file);
            return 0;
            break;
        case '8':
            func8(memberlist);
            break;
        case 'd':
            memberlist=dele(memberlist);
            break;
        case 'i':
            memberlist=insert(memberlist);
            break;
        case 'l':
            log();
            break;
        default:
            printf("Error：无法识别输入信息，请重新操作！\n");
        }
    } while (i != '0');
    //退出程序时，释放链表的内存
    member=memberlist.head;
    free(member);
    //写入日志文件
    if ((file = fopen("LOG.txt", "ab+")) == NULL)
	{
		printf("Error：无法写入日志文件！");
		exit(0);
	}
	fprintf(file,"--------------------程序正常退出（");
    fclose(file);
    log();
    file = fopen("LOG.txt", "ab+");
    fprintf(file,"）----------\n");
    fclose(file);
    return 0;
}
