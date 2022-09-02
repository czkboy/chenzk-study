#include<iostream>
using namespace std;
class TestEP
{
public:
	int m_i;
	int m_j;

public:
	struct obj //结构        //定义一个类型，不放在外部，污染全局变量
	{
		//成员，是个指针
		struct obj* next;  //这个next就是个嵌入式指针
							   //自己是一个obj结构对象，那么把自己这个对象的next指针指向 另外一个obj结构对象，最终，把多个自己这种类型的对象通过链串起来；
	};
};

int main6()
{
	TestEP mytest;
	cout << sizeof(mytest) << endl; //8
	TestEP::obj* ptemp;  //定义一个指针
	ptemp = (TestEP::obj*)&mytest; //把对象mytest首地址给了这个指针ptemp，这个指针ptemp指向对象mytest首地址；
	cout << sizeof(ptemp->next) << endl; //4
	cout << sizeof(TestEP::obj) << endl; //4
	ptemp->next = nullptr;
	return 1;
}
