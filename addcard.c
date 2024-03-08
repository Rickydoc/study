#include "data.h"
//由于链表节点的创建是动态的，所以会重复多次，因此用函数来做比较合适
card* addcard()    //函数的返回值为card的指针类型
{
	//定义一个变量作为返回值，它需要指向一个结构体
	card* stu;
	//使用malloc（memory allocation）函数分配一块内存，malloc是标准库函数，接受一个整数作为形参，也就是需要分配内存的大小
	//sizeof用于获取结构的大小，然后好让malloc函数为其分配相应的内存
	//但是此时，它仅仅是一块空白内存，并不知道它需要存储什么类型的数据结构，需要用强制类型转换进行标识
	stu = (card*)malloc(sizeof(card));
	//分配完地址后，让next指向NULL，不要让它成为野指针
	stu->next = NULL;
	//返回创建节点的地址
	return stu;
}
