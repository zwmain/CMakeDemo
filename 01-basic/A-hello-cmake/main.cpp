#include <iostream>

void swap(int &a, int &b);

int main()
{
    std::cout << "这里是VS Code" << std::endl;
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    swap(a, b);
    std::cout << "a=" << a << "\t"
              << "b=" << b << std::endl;
    return 0;
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
