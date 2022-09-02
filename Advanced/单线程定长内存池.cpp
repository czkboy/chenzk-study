#include <new>      // placement new
#include <cstddef>  // ptrdiff_t size_t
#include <cstdlib>  // exit
#include <climits>  // UINT_MAX
#include <iostream> // cerr
template<typename T>
class allcator_1
{
	    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

    public:
        static pointer allocate(size_type n, const void* hint = 0)
        {
			return static_cast<pointer>(operator new(n));
        }

		static void deallocate(pointer ptr) {
			operator delete(static_cast<void*>(ptr));
		}

		static void construct(pointer ptr) {
			new (ptr) T;
		}
		static void destroy(pointer ptr) {
			ptr->~T();
		}
private:
	struct obj {
		unsigned long mile;
		char type;
	};
	union
	{
		obj b;
		allcator_1* next;
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
	static allcator_1* m_FreePosi;
	// 总是指向一块可以分配出去的内存的首地址
	static int m_sTrunkCount;
	//一次分配多少倍该类的内存
};
template<typename T>
void* allcator_1<T>::operator new(size_t size)
{
	allcator_1* p = m_FreePosi;
	if (p)	//如果p有效就把链表头部向下移
		m_FreePosi = p->next;
	else
	{
		//如果链表已空，申请一大块内存
		int size =   (sizeof(allcator_1) > sizeof(T)) ? sizeof(allcator_1) : sizeof(T);
		allcator_1* newBlock = static_cast<allcator_1*>(::operator new(m_sTrunkCount * size));

		//将小块穿成一个freelist
		for (int i = 1; i < m_sTrunkCount - 1; ++i)
			newBlock[i].next = &newBlock[i + 1];
		newBlock[m_sTrunkCount - 1].next = 0;
		p = newBlock;
		m_FreePosi = &newBlock[1];
	}
	return p;
}
template<typename T>
void allcator_1<T> :: operator delete(void* phead)
{
	//free(phead);
	//不再用传统方式实现，针对内存池有特别的实现
	(static_cast <allcator_1*>(phead))->next = m_FreePosi;
	m_FreePosi = static_cast <allcator_1*>(phead);
}
template<typename T>
int allcator_1<T>::m_iCount = 0;
template<typename T>
int allcator_1<T>::m_iMallocCount = 0;
template<typename T>
allcator_1<T>* allcator_1<T>::m_FreePosi = nullptr;
template<typename T>
int allcator_1<T>::m_sTrunkCount = 5;
//一次分配5倍的该类内存作为内存池的大小

class MyClass
{
public:
	MyClass();
	~MyClass();
	void getA() {
		std::cout << a << std::endl;
	}

private:
	int a;
	int b;
	int c;

};

MyClass::MyClass():a(100),b(200),c(300)
{
	std::cout << "MyClass()" << std::endl;
	
}

MyClass::~MyClass()

{
	getA();
	std::cout << "~MyClass()" << std::endl;
}
int main() {
	allcator_1<MyClass> alloc;
	MyClass* m = alloc.allocate(4);
	printf("%p\n", m);
	alloc.construct(m);
	alloc.destroy(m);
	alloc.deallocate(m);

}