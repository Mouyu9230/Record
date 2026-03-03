#include<iostream>
void f(int& x) {
    x = 100;
}

int main(){

    int year;
    std::cout<<"how old are you?"<<std::endl;
    std::cin>>year;
    std::cout<<year<<"? thats nice.\n";
     //可通过使用using声明省去使用命名空间内名字时添加的前缀 using namespace::std 但不能随意使用
    //endl会输出换行符并清空输出缓冲区 

    //引用
    int a = 10;
    f(a);
    // a == 100

    //auto类型 让编译器根据初始化表达式自动推导变量类型
    auto x = 10;        // int
    auto y = 3.14;      // double
    auto z = x + y;     // double
    //一旦推导完成，类型就固定，不会动态改变

    //decltype同样是推导类型
    int x = 10;

    decltype(x)            a = x;   // int 写变量名则得到声明时的类型
    decltype((x))          b = x;   // int& (x)为左值表达式，得到类型的引用
    decltype(x + 1)        c = 0;   // int 右值表达式，得到对应的类型

    
    struct S {
    int a = 1; //可以初始化成员
    double b = 2.5;
    };
    return 0;

}
