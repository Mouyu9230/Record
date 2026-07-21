#include <iostream>
using namespace std;

//基类
class Employee{
protected://与private区别是派生类可访问
    string name;
    int id;

public:
    Employee(const string& n,int i):name(n),id(i){}

    //虚函数：计算工资 
    virtual double salary()const=0;

    // 公共接口
    virtual void print()const{
        cout<<"id: "<<id<<endl;
        cout<<"name: "<<name<<endl;
    }

    virtual ~Employee(){}//虚析构
};

//派生类
class Worker:public Employee{
private:
    double base; 

public:
    Worker(const string& n,int i,double b)
        :Employee(n,i),base(b){}

    double salary()const{
        return base;
    }

    void print()const{
        Employee::print(); //复用基类输出
        cout<<"salary: "<<salary()<<endl;
    }
};

//派生类
class Manager:public Employee{
private:
    double base; 
    double bonus;

public:
    Manager(const string& n,int i,double b,double bonus)
        :Employee(n,i),base(b),bonus(bonus){}

    double salary()const{
        return base+bonus;
    }

    void print()const{
        Employee::print();
        cout<<"salary: "<<salary()<<endl;
    }
};

int main(){
    Employee* e1=new Worker("Alice",1001,5000);
    Employee* e2=new Manager("Bob",2001,8000,3000);

    e1->print();//多态调用
    e2->print();

    delete e1;
    delete e2;
    return 0;
}
