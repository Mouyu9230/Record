#include <iostream>
#include <sstream>
#include <vector>
                                                          
#include <unistd.h>//基础
#include <sys/wait.h>//wait

using namespace std;


vector<string> system_commands={"ls","cat"};//施工中：后续把输入判断做成遍历这个vector

typedef struct command{

    vector<string> basic_command;
    string input_from;
    string output_to;
    vector<string> cd_command;
    bool plus=false;//>>
    bool run_in_back=false;//&

}command;

void command_analyse(vector<string> v,command &com){
    for(size_t i=0;i<v.size();i++){
        if(v[0]=="cd"){
            com.cd_command.push_back(v[i]);
            continue;
        }
        if(v[i]=="<"){
            if(i+1<v.size()){
                com.input_from=v[i+1];
                i++;
            }
        }
        else if(v[i]==">"){
            if(i+1<v.size()){
                com.output_to=v[i+1];
                com.plus=false;
                i++;
            }
        }
        else if(v[i]==">>"){
            if(i+1<v.size()){
                com.output_to=v[i+1];
                com.plus=true;
                i++;
            }
        }
        else if(v[i]=="&"){
            com.run_in_back=true;
        }
        else{
            com.basic_command.push_back(v[i]);
        }
    }


}

void command_cut(string s,vector<string>& v){//越界问题？
    int fastflag=-1;
    int slowflag=0;
    int vcount=0;
    for(auto& x:s){
        fastflag++;
        if(x=='|'){
            for(int i=slowflag;i<fastflag;i++){
                v[vcount].push_back(s[i]);
            }
            vcount++;
            slowflag=fastflag+1;
        }
    }//pushback空格以处理||情况？
    for(int i=slowflag;i<=fastflag;i++){
            v[vcount].push_back(s[i]);
    }
    
}

void command_iss(vector<string> v,vector<vector<string>>& for_analyse){//施工中：越界风险？

    string atom;
    int size=v.size();
    for(int i=0;i<size;i++){
        istringstream iss(v[i]);
        while(iss>>atom)for_analyse[i].push_back(atom);
    }
}//略过空格

int main(){
    while(1){
        int wrong_command_flag=0;//判断未定义输入

        cout<<"myshell> "<<endl;
        string line;
        getline(cin,line);
        if(line.empty()){
            continue;
        }
        if(line=="exit"){
            break;
        }

        vector<string> cutted;//依管道符切割结果
        vector<vector<string>> for_analyze;//解析cutted
        vector<command> analyzed_commands;//解析每个vector为command以处理重定义

        command_cut(line,cutted);
        int pipe_num=cutted.size()-1;
        command_iss(cutted,for_analyze);

        for(auto x:for_analyze){
            command com;
            command_analyse(x,com);
            analyzed_commands.push_back(com);
        }

        for(auto x:analyzed_commands){
            if(x.basic_command.empty()){
                cout<<"basic command empty"<<endl;
                wrong_command_flag=1;
            }
            if(x.basic_command[0]!="ls"&&x.basic_command[0]!="cat"){
            cout<<"an unknown command"<<endl;
            wrong_command_flag=1;
            break;
            }
        }
        if(wrong_command_flag==1)continue;//判断未定义输入

        vector<char*> cform_commands;//转c风格施工中---------------------------

        
        



        
        
    }
    return 0;

}