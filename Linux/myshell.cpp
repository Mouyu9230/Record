#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>//基础
#include <sys/wait.h>//wait

using namespace std;

//思路：先基于管道符把输入分段，解析每段命令用结构体存信息以处理重定向，依次执行，特殊处理cd，关键在于对端口的设定
//基础版：

void command_analyse();

int main(){
    while(1){
        cout<<"myshell> "<<endl;
        string line;
        getline(cin,line);
        if(line.empty()){
            continue;
        }
        if(line=="exit"){
            break;
        }

        istringstream iss(line);
        vector<string>vargv;
        string temp;
        
        while(iss>>temp){
            vargv.push_back(temp);
        }
        if(vargv.empty())continue;

        if(vargv[0]!="ls"&&vargv[0]!="cat"){
            cout<<"unknown command"<<endl;
            continue;
        }

        //转化为char*
        vector<char*>argv;
        for(auto& x:vargv){
            argv.push_back(const_cast<char*>(x.c_str()));//转换为const char*再去掉const
        }
        argv.push_back(nullptr);

        pid_t pid=fork();
        if(pid<0){
            cout<<"error when fork"<<endl;
            continue;
        }
        if(pid==0){
            execvp(argv[0],argv.data());//如果成功则不会返回;vec.data()是指向首元素指针，适配c风格的接口
            cout<<"error when exec"<<endl;
            _exit(1);
        }else{
            waitpid(pid,nullptr,0);
        }
    }
    return 0;

}