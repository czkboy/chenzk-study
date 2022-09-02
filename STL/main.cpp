//
// Created by 陈子康 on 2021/7/30.
//

#include <iostream>
#include <vector>
#include "allocator.h"
class test
{
private:
    int a;
    char b;
    /* data */
public:
    test(/* args */);
    ~test();
};

test::test(/* args */)
{
    std::cout<<"------------test()--------------"<<std::endl;
}

test::~test()
{
    std::cout<<"------------~test()--------------"<<std::endl;
}

int main()
{
    kstd::allocator<int> alloc;
    std::vector<int,kstd::allocator<int>>();
    
    
    std::cout<<"helloworld";
}
