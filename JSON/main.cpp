#include <iostream>
#include <thread>
#include "json.hpp"
using nlohmann::json;
//方式一：
int main()
{
    json j1;
    j1["name"] = "LeBorn Jame";                                  //字符串
    j1["number"] = 23;                                           //整数
    j1["man"] = true;                                            //布尔值
    j1["children"] = {"LeBorn Jr", "Bryce Maximus", "Zhuri"};    //数组
    j1["behavior"]["funny"] = "gigigigigigi";                    //对象中元素值
    j1["wife"] = {{"name", "Savannah Brinson"}, {"man", false}}; //对象

    //方式二
    json j2 = {
        {"name", "LeBorn Jame"},
        {"number", 23},
        {"man", true},
        {"children", {"LeBorn Jr", "Bryce Maximus", "Zhuri"}},
        {"behavior", {{"funny", "gigigigigigi"}}}};

    json j = {
        {"name", "LeBorn Jame"},
        {"number", 23},
        {"children", {"LeBorn Jr", "Bryce Maximus", "Zhuri"}},
        {"man", true},
        {"wife", {"name", "Savannah Brinson"}}};
    auto name = j["name"].get<std::string>(); //方式一
    int number = 0;
    j["number"].get_to(number);                             //方式二
    auto childOne = j["children"][0].get<std::string>();    //获取数组元素方式一，越界后不会抛出异常，直接中断
    auto childTwo = j["children"].at(1).get<std::string>(); //越界后抛出异常
    auto wifeName = j["wife"]["name"].get<std::string>();

    // bool man=j["man"];//不推荐使用这种隐式转换的方式
}
