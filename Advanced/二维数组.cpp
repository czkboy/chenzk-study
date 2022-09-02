/*

验证c语言风格二维数组是连续存放的


*/
#include <iostream>
int main2() {
    int board[3][3] = {};
    auto b = board;

    int count = 1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = count++;
        }
    }
    auto c = *b;
    std::cout << "*b="<<*c<< std::endl;
    std::cout << "*b+5=" << *(c+5) << std::endl;
    return 1;
}
