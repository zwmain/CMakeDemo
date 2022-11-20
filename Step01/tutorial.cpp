#include "TutorialConfig.h"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::cout << "主版本：" << Tutorial_VERSION_MAJOR << std::endl;
    if (argc != 2) {
        std::cout << "参数错误" << std::endl;
        return 0;
    }
    double val = std::stod(argv[1]);
    std::cout << "输入值为：" << val << std::endl;
    return 0;
}
