//迭代器：泛化的指针，用于遍历和操作容器元素，定义在各容器内部
//统一访问接口（适用于vector/list/map等）
//支持解引用，自增等指针式操作
//不同容器提供不同能力的迭代器

#include<iostream>
#include<vector>
using namespace std;

int main(){
    vector<int> v={10,20,30};

    //begin()返回首元素迭代器，end()返回尾后迭代器
    for(vector<int>::iterator it=v.begin();it!=v.end();it++){
        cout<<*it<<" "; // *it获取元素值
    }
    cout<<endl;

    //修改元素：通过迭代器赋值（类似指针）
    for(auto it=v.begin();it!=v.end();++it){
        *it *= 2; //原地修改
    }

    //范围for本质是迭代器的简化写法：for(int x:v) 等价于上面的for(it...)

    //vector迭代器支持随机访问
    auto it=v.begin();
    cout<<*(it+1)<<endl; //直接跳转到第2个元素

    return 0;
}