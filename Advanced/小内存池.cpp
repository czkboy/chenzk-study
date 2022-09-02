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
	// ���ڷ������ͳ�ƣ�ÿnewһ�� + 1
	static int m_iMallocCount;
	A* next;
	//����ͳ��ma11oc������ÿma11ocһ��+1
private:

	static A* m_FreePosi;
	// ����ָ��һ����Է����ȥ���ڴ���׵�ַ
	static int m_sTrunkCount;
	//һ�η�����ٱ�������ڴ�
};

void* A::operator new(size_t size)
{
	//A ppoint =(A *)malloc(size);
	//�����ô�ͳ��ʽʵ�֣��������ڴ��ʵ��
	//return ppoint;
	A* tmplink;
	if (m_FreePosi == nullptr)
	{
		//Ϊ�գ�����Ҫ�����ڴ棬������Ǻܴ�һ���ڴ�
		size_t realsize = m_sTrunkCount * size;//����m_sTrunkCount��ô�౶���ڴ�
		m_FreePosi = reinterpret_cast<A*>(new char[realsize]); // ���Ǵ�ͳnew, ���õײ�
		tmplink = m_FreePosi;
		//�ѷ����������һ����ڴ�����������������ʹ��
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
	//�����ô�ͳ��ʽʵ�֣�����ڴ�����ر��ʵ��
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
	// ���ڷ������ͳ�ƣ�ÿnewһ�� + 1
	static int m_iMallocCount;
	//����ͳ��ma11oc������ÿma11ocһ��+1
private:
	static B* m_FreePosi;
	// ����ָ��һ����Է����ȥ���ڴ���׵�ַ
	static int m_sTrunkCount;
	//һ�η�����ٱ�������ڴ�
};

void* B::operator new(size_t size)
{
	B* p = m_FreePosi;
	if (p)	//���p��Ч�Ͱ�����ͷ��������
		m_FreePosi = p->next;
	else
	{
		//��������ѿգ�����һ����ڴ�
		B* newBlock = static_cast<B*>(::operator new(m_sTrunkCount * sizeof(B)));

		//��С�鴩��һ��freelist
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
		//�����ô�ͳ��ʽʵ�֣�����ڴ�����ر��ʵ��
		(static_cast <B*>(phead))->next= m_FreePosi;
		m_FreePosi= static_cast <B*>(phead);
	}
	int B :: m_iCount =0;
	int B :: m_iMallocCount =0;
	B* B::m_FreePosi= nullptr;
	int B :: m_sTrunkCount =5;
	//һ�η���5���ĸ����ڴ���Ϊ�ڴ�صĴ�С

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
		std::cout <<"ʹ���ڴ��" << end - start << std::endl;
		std::cout << "��ʹ���ڴ��" << end2 - end << std::endl;
		return 1;
		/*
		ʹ���ڴ��322
		��ʹ���ڴ��907
		*/
	}