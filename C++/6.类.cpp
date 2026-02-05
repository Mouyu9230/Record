#include <iostream>
#include <string>
using namespace std;

class student{
    private:

    string name;
    int id;
    double score;
    mutable int count=0;//mutable：允许在const函数中修改

    public:
    student(const string& n,int i,double s):name(n),id(i),score(s){}
    //构造函数：初始化
    //const string& n部分传入原字符串的只读的引用，避免拷贝带来的开销
    //结尾的函数体可以放额外的语句，比如检验数据符不符合要求

    student():name("xilanai"),id(0),score(0.0){}//默认构造

    //只读的操作加const
    string get_name()const{
        return name;
    }
    int get_id()const{
        return id;
    }
    double get_score()const{
        count++;//mutable成员可在const中被修改
        return score;
    }
    void print_all()const{
        cout<<"id: "<<id<<endl;
        cout<<"name: "<<name<<endl;
        cout<<"score: "<<score<<endl;
        cout<<"score have been search for "<<count<<" times"<<endl;
        //mutable作用
    }

    //修改状态的操作
    void change_score(double s){
        if(s>=0){
            score=s;
        }
    }

};
int main(){
    student s1("mouyu",9230,149);
    const student s2("bydzsh",6666,666);//无法调用非const成员函数
    student s0;//默认

    s1.change_score(148);
    s1.print_all();

    s2.print_all();


    s0.print_all();

    return 0;
}