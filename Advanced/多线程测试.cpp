#include<iostream>
#include<thread>
using namespace std;
class X {
public:
    void hello(int x) {
        cout << x;
    }
    
};
X x;

int main3() {
    thread t(&X::hello,x,2022);
    t.join();
    return 1;
}