#include "MyMath.h"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "参数错误！" << std::endl;
        return 0;
    }
    int val = std::stoi(argv[1]);
    std::cout << val << "的平方为：" << zwn::mySqure(val) << std::endl;
    return 0;
}

