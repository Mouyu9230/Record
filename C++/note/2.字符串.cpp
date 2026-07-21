//c++字符串：
//自动管理内存
//记录长度（size() / length()）
//可扩容、可拼接

#include<iostream>
#include<string>
#include<sstream>
int n=1;
using std::string;//也可以像这样单独using

//初始化方式
int main(){
    string s1;//初始化空串
    string s2(s1);//等价于s2=s1
    string s3("apple");////等价于s3="apple"
    string s4(n,'?');//初始化为n个'？'组成的字符串

//操作

    s1.empty();            // 若 s 为空返回 true，否则返回 false
    s1.size();             // 返回 s 中字符的个数
    s1[n];            // 返回 s 中第 n 个字符的引用（从 0 开始计数）
    s1 + s2;              // 返回 s1 和 s2 连接后的结果
    s1 = s2;              // 用 s2 的副本代替 s1 中原来的字符
    s1 == s2;             // 若 s1 和 s2 所含字符完全一样则为真（大小写敏感）
    s1 != s2;             // 判断 s1 与 s2 是否不相等（大小写敏感）
    s1 < s2;              // 按字典序比较（大小写敏感）
    s1 <= s2;             // 按字典序比较（大小写敏感）
    s1 > s2;              // 按字典序比较（大小写敏感）
    s1 >= s2;             // 按字典序比较（大小写敏感）


//字符串流：把字符串当成输入输出流来用
//意义：拆分字符串，数值转换等

  std::string line = "10 20 apple";
  std::istringstream iss(line);

  int a, b;
  std::string s;
  iss >> a >> b >> s;//行为和 cin 完全一致，只是输入源是字符串


  std::ostringstream oss;
  oss << 123 << " " << 3.14;
  std::string result = oss.str();//用流方式拼字符串


  std::stringstream ss;//读写一体
  ss << "42 test";
  int x;
  std::string y;
  ss >> x >> y;

  //使用模式：
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss(line);
  //通过完整获取一行再解析来避免直接cin遇到空格停止的问题


}