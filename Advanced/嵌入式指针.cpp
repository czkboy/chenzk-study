#include<iostream>
using namespace std;
class TestEP
{
public:
	int m_i;
	int m_j;

public:
	struct obj //�ṹ        //����һ�����ͣ��������ⲿ����Ⱦȫ�ֱ���
	{
		//��Ա���Ǹ�ָ��
		struct obj* next;  //���next���Ǹ�Ƕ��ʽָ��
							   //�Լ���һ��obj�ṹ������ô���Լ���������nextָ��ָ�� ����һ��obj�ṹ�������գ��Ѷ���Լ��������͵Ķ���ͨ������������
	};
};

int main6()
{
	TestEP mytest;
	cout << sizeof(mytest) << endl; //8
	TestEP::obj* ptemp;  //����һ��ָ��
	ptemp = (TestEP::obj*)&mytest; //�Ѷ���mytest�׵�ַ�������ָ��ptemp�����ָ��ptempָ�����mytest�׵�ַ��
	cout << sizeof(ptemp->next) << endl; //4
	cout << sizeof(TestEP::obj) << endl; //4
	ptemp->next = nullptr;
	return 1;
}
