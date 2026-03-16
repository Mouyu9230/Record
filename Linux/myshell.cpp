#include<iostream>
#include<sstream>
#include<vector>
#include<fcntl.h>                                               
#include<unistd.h>//基础
#include<sys/wait.h>//wait
#include<signal.h>

using namespace std;




vector<string> system_commands={"ls","cat","echo","mkdir","wc","grep","tac","pwd","rm","cp","mv","touch","chmod"};
char old_wd[256];
char cur_wd[256];

typedef struct command{

    vector<string> basic_command;
    string input_from;
    string output_to;
    vector<string> cd_command;
    bool plus=false;//>>
    bool run_in_back=false;//&
    bool is_path=false;// ./

}command;

void command_analyse(vector<string> v,command &com){
    for(size_t i=0;i<v.size();i++){

        if((v[i][0]=='.'&&v[i][1]=='/')||v[i][0]=='/'){
            com.is_path=true;
        }

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

void command_cut(string s,vector<string>& v){
    string temp;
    for (char c:s){
        if(c=='|'){
            v.push_back(temp);
            temp.clear();
        }else{
            temp+=c;
        }
    }
    v.push_back(temp);
}

void command_iss(vector<string> v,vector<vector<string>>& for_analyse){//施工中：越界风险？

    string atom;
    int size=v.size();
    for(int i=0;i<size;i++){
        for_analyse.push_back(vector<string>());//预先分配空间
        istringstream iss(v[i]);
        while (iss>>atom) {
            for_analyse[i].push_back(atom);
        }
    }
}//略过空格

void sigint_handler(int x){

    write(STDOUT_FILENO,"\nreceived a SIGINT!\n",21);

}

int main(){

    signal(SIGINT,sigint_handler);

    while(1){
        int wrong_command_flag=0;//判断未定义输入
        int command_find_flag=0;//遍历system_commmand，检查指令是否支持
        int old_dir_flag=0;//若输入为cd -，跳过正常cd失败检验

        getcwd(cur_wd,256);
        cout<<"\033[32mmyshell>\033[0m"<<":";
        string cppwd(cur_wd);
        cout<<"\033[34m"<<cppwd<<"\033[0m"<<"$ ";

        string line;
        if(!getline(cin,line)){
                cin.clear();
                continue;
        }
        
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

        if(analyzed_commands.size()==1&&!analyzed_commands[0].cd_command.empty()){


            if(analyzed_commands[0].cd_command[1]=="-"){
                if(chdir(old_wd)!=0){
                    cout<<"error when change to prev dir,you may cd to anywhere first"<<endl;
                }
                old_dir_flag=1;
            }
            getcwd(old_wd,256);

            if(analyzed_commands[0].cd_command.size()<2){
                cout<<"...cd to where?"<<endl;
            }else{

                if(chdir(analyzed_commands[0].cd_command[1].c_str())!=0&&!old_dir_flag){
                    cout<<"error when cd"<<endl;
                }

            }
            continue;
        }//特殊处理cd

        

        

        for(auto x:analyzed_commands){
            if(x.basic_command.empty()&&x.cd_command.empty()){
                cout<<"command empty"<<endl;
                wrong_command_flag=1;
            }

            for(auto com:system_commands){
                if(x.basic_command[0]==com){
                    command_find_flag=1;
                }
            }

            if(!command_find_flag&&!x.is_path){//若输入文件路径，跳过检验
            cout<<"an unknown command"<<endl;
            wrong_command_flag=1;
            break;
            }
        }
        if(wrong_command_flag==1)continue;//判断未定义输入

//-------------------------------------------------------------------------输入处理部分结束

        vector<pid_t> pids;
        int size=analyzed_commands.size();
        int prev_reader=-1;
        bool is_run_back=false;

        for(int i=0;i<size;i++){

            int fd[2];

            if(i<pipe_num){
                if(pipe(fd)==-1){
                    cout<<"error when pipe()"<<endl;
                    break;
                }
            }

            pid_t pid=fork();
            if(pid==-1){
                cout<<"error when fork"<<endl;
                break;
            }
            if(pid==0){//子进程部分

                signal(SIGINT, SIG_DFL);

                if(!analyzed_commands[i].input_from.empty()){//处理<
                    int in_fd=open(analyzed_commands[i].input_from.c_str(),O_RDONLY);
                    if(in_fd==-1){
                        perror("open input file");
                        exit(1);
                    }
                    dup2(in_fd, STDIN_FILENO);
                    close(in_fd);
                }else if(prev_reader!=-1){//无<,但前有管道
                    dup2(prev_reader,STDIN_FILENO);
                }

                if(!analyzed_commands[i].output_to.empty()){//处理> >>
                    int out_fd;
                    if(analyzed_commands[i].plus){

                        out_fd=open(analyzed_commands[i].output_to.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);//若为>>，追加
                    }else{

                        out_fd=open(analyzed_commands[i].output_to.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);//覆盖
                    }

                    if(out_fd==-1){
                        perror("open output file");
                        exit(1);
                    }
                    dup2(out_fd, STDOUT_FILENO);
                    close(out_fd);
                }else if(i<pipe_num){//无重定向，且不是最后一条，则写入当前管道
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                }

               if (prev_reader!=-1)close(prev_reader);//关闭多余fd
                if(i<pipe_num){
                    close(fd[0]);
                    close(fd[1]);
                }
        
            vector<char*> cform_commands;//转c风格
                for (auto &s:analyzed_commands[i].basic_command){
                    cform_commands.push_back(const_cast<char*>(s.c_str()));
                }
                cform_commands.push_back(nullptr);//c风格结尾\0


                execvp(cform_commands[0],cform_commands.data());//执行
                perror("execvp failed");
                exit(1);//返回则代表出错了


            }else{//父进程部分，上面的if里是（pid==0）

                pids.push_back(pid);

                if(analyzed_commands[i].run_in_back){
                    is_run_back=true;
                }
                if(prev_reader!=-1){//关闭上一条管道读端
                    close(prev_reader);
                }
                if(i<pipe_num){//若有新管道，存读端，关闭写端
                    close(fd[1]);
                    prev_reader=fd[0];
                }


            }
        }//执行流程完毕

        if(prev_reader!=-1){
            close(prev_reader);
        }

        if(is_run_back==false){//无&，前台运行
            for(auto pid:pids){
                waitpid(pid,nullptr,0);
            }
        }else{
            cout<<"processes are running in background!"<<endl;
        }



        
        
        
    }



    return 0;

}