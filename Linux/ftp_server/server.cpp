#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>

#define SERVER_PORT 2100
#define BUF_SIZE 4096

// FTP session状态
typedef struct
{
    int ctrl_fd;//控制连接柄
    int data_listen_fd;//监听柄
    int data_fd;//数据连接柄
    int pasv_port;//开放端口
    bool is_right_user;
    bool is_login;
    char cwd[512];//server当前目录路径

}ftp_session;


using namespace std;

//工具函数
int create_server_socket(int port);
void send_response(int fd, const string& msg);

//FTP功能模块声明
void client_handler(int client_fd);
int handle_command(ftp_session* sess, const string& cmd);

void handle_user(ftp_session* sess, const string& arg);
void handle_pass(ftp_session* sess, const string& arg);
void handle_quit(ftp_session* sess);

void handle_pasv(ftp_session* sess);
void handle_list(ftp_session* sess);
void handle_retr(ftp_session* sess, const string& file);
void handle_stor(ftp_session* sess, const string& file);


int main()
{
    int server_fd=create_server_socket(SERVER_PORT);//控制连接监听柄

    cout << "[SERVER] Listening on 2100..." << endl;

    while(1){
        int client_fd=accept(server_fd, nullptr, nullptr);//监听柄完成与2100bind并listen等待connect,accept拿出一个握完手的连接，生成连接柄

        thread t(client_handler, client_fd);
        t.detach();
    }

    return 0;
}

//客户端会话线程handler
void client_handler(int client_fd)
{
    ftp_session sess;
    memset(&sess, 0, sizeof(sess));
    sess.ctrl_fd=client_fd;

    send_response(client_fd,"220 FTP server ready");

    char buf[BUF_SIZE];

    while(true)
    {
        memset(buf, 0, sizeof(buf));
        int n=recv(client_fd, buf, sizeof(buf), 0);


        if(n<=0){
            break;
        }
        string cmd(buf);
        while(!cmd.empty()&&(cmd.back()=='\r'||cmd.back()=='\n'))
        {
            cmd.pop_back();
        }

        if(handle_command(&sess,cmd)){
            break;
        }
    }

    close(client_fd);
}


int handle_command(ftp_session* sess, const string& cmd)
{

    istringstream iss(cmd);
    string cut;
    vector<string> cmds;

    while(iss>>cut){
        cmds.push_back(cut);
    }

    if(cmds.empty()){
        return 0;
    }


    if(cmds[0]=="PASV"){
        handle_pasv(sess);
    }else if(cmds[0]=="LIST"){
        handle_list(sess);
    }else if(cmds[0]=="RETR"){
        if(cmds.size()<2){
            send_response(sess->ctrl_fd,"501 Missing filename");
            return 0;
        }
        handle_retr(sess,cmds[1]);
    }else if(cmds[0]=="STOR"){
        if(cmds.size()<2){
            send_response(sess->ctrl_fd,"501 Missing filename");
            return 0;
        }
        handle_stor(sess,cmds[1]);
    }else if(cmds[0]=="USER"){
        if(cmds.size()<2){
            send_response(sess->ctrl_fd,"501 Missing username");
            return 0;
        }
        handle_user(sess,cmds[1]);
    }else if(cmds[0]=="PASS"){
        if(cmds.size()<2){
            send_response(sess->ctrl_fd,"501 Missing password");    
            return 0;
        }
        handle_pass(sess,cmds[1]);
    }else if(cmds[0]=="QUIT"){
        handle_quit(sess);
        return 1;
    }else{
        send_response(sess->ctrl_fd,"500 Unknown command");
    }
    return 0;
    


    
}

void handle_user(ftp_session* sess, const string& arg)
{
    if(arg!="Mouyu"){
        send_response(sess->ctrl_fd,"530 Wrong username\r\n");
        return;
    }else{
        send_response(sess->ctrl_fd,"331 Username correct,password required\r\n");       
        return;   
    }

    sess->is_right_user=true;
}

void handle_pass(ftp_session* sess, const string& arg){
    if(sess->is_right_user==false){
        send_response(sess->ctrl_fd,"530 No username\r\n");
        return;      
    }
    if(arg!="123456"){
        send_response(sess->ctrl_fd,"530 Wrong password\r\n");   
        return;
    }
    send_response(sess->ctrl_fd,"230 Login successful\r\n");   
    sess->is_login=true;
}

void handle_quit(ftp_session* sess){

    send_response(sess->ctrl_fd,"221 Bye");
    close(sess->ctrl_fd);
    close(sess->data_fd);
    close(sess->data_listen_fd);

}


void handle_pasv(ftp_session* sess)
{
    int listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd<0){
        send_response(sess->ctrl_fd,"425 Cannot create data socket");
        return;
    }

    int opt=1;
    setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=0;//系统分配端口

    if(bind(listen_fd,(sockaddr*)&addr,sizeof(addr))<0){
        send_response(sess->ctrl_fd,"425 Failed to bind");
        close(listen_fd);
        return;
    }

    if(listen(listen_fd,1)<0){
        send_response(sess->ctrl_fd,"425 Failed to listen");
        close(listen_fd);
        return;
    }

    socklen_t len=sizeof(addr);
    getsockname(listen_fd,(sockaddr*)&addr,&len);

    int port=ntohs(addr.sin_port);//主机序转换

    sess->data_listen_fd = listen_fd;
    sess->pasv_port=port;

    int p1=port/256;
    int p2=port%256;

    string ip="127,0,0,1";
    string resp="227 Entering Passive Mode (" +ip + "," +to_string(p1) + "," +to_string(p2) + ")";

    send_response(sess->ctrl_fd, resp);
}


void handle_list(ftp_session* sess)
{
    
}


void handle_retr(ftp_session* sess, const string& file)
{
    
}


void handle_stor(ftp_session* sess, const string& file)
{
    
}


int create_server_socket(int port){

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(SERVER_PORT);
    server_addr.sin_addr.s_addr=INADDR_ANY;

    int server_fd=socket(AF_INET,SOCK_STREAM,0);//IPV4,流式
    if(server_fd==-1){
        cout<<"[SERVER] failed to create socket"<<endl;
        return -1;
    }

    if(bind(server_fd,(sockaddr*)&server_addr,sizeof(server_addr))){
        cout<<"[SERVER] failed to bind"<<endl;
        close(server_fd);
        return -1;
    }
    if(listen(server_fd,SOMAXCONN)<0){
        cout<<"[SERVER] listen failed"<<endl;
        close(server_fd);
        return -1;
    }



    return server_fd;
}

void send_response(int fd, const string& msg){
    string real_msg=msg;

    if(real_msg.size()<2||real_msg.substr(real_msg.size()-2)!="\r\n")
    {
        real_msg+="\r\n";
    }

    send(fd,real_msg.c_str(),real_msg.size(),0);

    cout << "[SERVER] " << real_msg;
}