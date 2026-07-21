//vector:动态数组容器，定义在<vector>中
//特性：
//动态大小，不用预先指定
//自动内存管理
//安全，编译时检查类型
//对象销毁时自动释放内存
//连续存储
//可赋值/拷贝

#include<iostream>
#include<vector>
using namespace std;




int main(){

    vector<int> v;// 创建int类型的空vector

    v.push_back(10);// 尾部添加元素
    v.push_back(20);
    v.push_back(30);

    cout<<v[0]<<endl;// 下标访问（不越界检查）
    cout<<v.at(1)<<endl; // at()带越界检查

    cout<<"size:"<<v.size()<<endl;// 当前元素个数
    cout<<"cap:"<<v.capacity()<<endl;// 当前分配容量

    for(int x:v) cout<<x<<" ";//范围for遍历
    //简洁且安全，要像c一样修改vector里元素需用引用&x
    cout<<endl;

    v.pop_back();// 删除尾部元素
    v.clear();// 清空所有元素

    vector<double> vi={1.1,2.2,3.3};//初始化
    vector<int> vxi(5,99);// 5个元素，初值99

    return 0;
}


