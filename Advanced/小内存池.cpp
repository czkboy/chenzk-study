#include<new>
#include<ctime>
#include<iostream>
#include<cstdio>
class A
{
public:
	A() {
		printf("next=%p\n", next);
	};
	static void* operator new(size_t size);
	static void operator delete(void* phead);
	static int m_iCount;
	// 用于分配计数统计，每new一次 + 1
	static int m_iMallocCount;
	A* next;
	//用于统计ma11oc次数，每ma11oc一次+1
private:

	static A* m_FreePosi;
	// 总是指向一块可以分配出去的内存的首地址
	static int m_sTrunkCount;
	//一次分配多少倍该类的内存
};

void* A::operator new(size_t size)
{
	//A ppoint =(A *)malloc(size);
	//不再用传统方式实现，而是用内存池实现
	//return ppoint;
	A* tmplink;
	if (m_FreePosi == nullptr)
	{
		//为空，我们要申请内存，申请的是很大一块内存
		size_t realsize = m_sTrunkCount * size;//申请m_sTrunkCount这么多倍的内存
		m_FreePosi = reinterpret_cast<A*>(new char[realsize]); // 这是传统new, 调用底层
		tmplink = m_FreePosi;
		//把分配出来的这一大块内存链接起来，供后续使用
		for (; tmplink != &m_FreePosi[m_sTrunkCount - 1]; ++tmplink)
		{
			tmplink->next = tmplink + 1;
			printf("%p\n", tmplink->next);
		}
		tmplink->next = nullptr;
		++m_iMallocCount;


	}
	tmplink = m_FreePosi;
	m_FreePosi = m_FreePosi->next;
	++m_iCount;
	return tmplink;
}
void A :: operator delete(void* phead)
{
	//free(phead);
	//不再用传统方式实现，针对内存池有特别的实现
	(static_cast <A*>(phead))->next = m_FreePosi;
	m_FreePosi = static_cast <A*>(phead);
}
int A::m_iCount = 0;
int A::m_iMallocCount = 0;
A* A::m_FreePosi = nullptr;
int A::m_sTrunkCount = 5;

class B
{
private:
	struct obj {
		unsigned long mile;
		char type;
	};
	union
	{
		obj b;
		B* next;
	};

public:
	unsigned long getMiles() { return b.mile; }
	char getType() { return b.type; }
	void set(unsigned long m, char t) {
		b.mile = m;
		b.type = t;
	}
	
	static void* operator new(size_t size);
	static void operator delete(void* phead);
	static int m_iCount;
	// 用于分配计数统计，每new一次 + 1
	static int m_iMallocCount;
	//用于统计ma11oc次数，每ma11oc一次+1
private:
	static B* m_FreePosi;
	// 总是指向一块可以分配出去的内存的首地址
	static int m_sTrunkCount;
	//一次分配多少倍该类的内存
};

void* B::operator new(size_t size)
{
	B* p = m_FreePosi;
	if (p)	//如果p有效就把链表头部向下移
		m_FreePosi = p->next;
	else
	{
		//如果链表已空，申请一大块内存
		B* newBlock = static_cast<B*>(::operator new(m_sTrunkCount * sizeof(B)));

		//将小块穿成一个freelist
		for (int i = 1; i < m_sTrunkCount - 1; ++i)
			newBlock[i].next = &newBlock[i + 1];
		newBlock[m_sTrunkCount - 1].next = 0;
		p = newBlock;
		m_FreePosi = &newBlock[1];
	}
	return p;
}
	void B :: operator delete(void* phead)
	{
		//free(phead);
		//不再用传统方式实现，针对内存池有特别的实现
		(static_cast <B*>(phead))->next= m_FreePosi;
		m_FreePosi= static_cast <B*>(phead);
	}
	int B :: m_iCount =0;
	int B :: m_iMallocCount =0;
	B* B::m_FreePosi= nullptr;
	int B :: m_sTrunkCount =5;
	//一次分配5倍的该类内存作为内存池的大小

	class C
	{
	public:
		C();
		~C();

	private:

	};

	C::C()
	{
	}

	C::~C()
	{
	}
	int main8() {
		clock_t start, end,end2;
		start = clock();
		for (int i = 0; i < 5000000; ++i)
		{
			B* pa = new B();
			//printf("%p\n", pa);
			//pa->set(i, i+'a');
			//std::cout << pa->getMiles() << pa->getType() << std::endl;
		}
		end = clock();
		for (int i = 0; i < 5000000; ++i)
		{
			C* pa = new C();
			//printf("%p\n", pa);
			//pa->set(i, i+'a');
			//std::cout << pa->getMiles() << pa->getType() << std::endl;
		}
		end2 = clock();
		std::cout <<"使用内存池" << end - start << std::endl;
		std::cout << "不使用内存池" << end2 - end << std::endl;
		return 1;
		/*
		使用内存池322
		不使用内存池907
		*/
	}