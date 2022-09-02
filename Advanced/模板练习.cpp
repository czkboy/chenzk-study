//static_assert 与assert
//https://blog.csdn.net/drdairen/article/details/76689014

#include <cassert>
#include <cstring>
using namespace std;

template <typename T, typename U> 
void bit_copy(T& a, U& b){
    assert(sizeof(b) == sizeof(a));
    //static_assert(sizeof(b) == sizeof(a), "template parameter size no equal!");
    memcpy(&a,&b,sizeof(b));
};

int main()
{
    int aaa = 0x2468;
    double bbb;
    bit_copy(aaa, bbb);
    return 0;
}