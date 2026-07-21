#include<iostream>
using namespace std;


//函数重载：同一作用域中，函数名相同但参数列表（类型/数量/顺序）不同
//仅返回值不同不构成重载
int add(int a,int b){
    return a+b;
}                  
double add(double a,double b){
    return a+b;
}        
int add(int a,int b,int c){
    return a+b+c;
}    

//默认参数：在函数声明中为参数指定默认值，调用时可省略右侧参数。
//必须从右向左连续设置

void greet(string name,string msg="Hello",char end='!'){ // msg和end有默认值
    cout<<msg<<", "<<name<<end<<endl;
}


//内联函数：用inline建议编译器将函数体直接插入调用处，避免函数调用开销
//适用于短小、频繁调用的函数。

inline int pluser(int x){
    return x*x;
}               // 可能展开为x*x

int main(){
    cout<<add(2,3)<<endl;          
    cout<<add(2.5,3.1)<<endl;   
    cout<<add(1,2,3)<<endl;        
    greet("Tom");                
    greet("Jerry","Hi");             
    greet("Jane","Hey",'?');     
    cout<<pluser(4)<<endl;        
    return 0;
}