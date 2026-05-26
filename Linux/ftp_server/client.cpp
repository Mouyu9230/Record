#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2100

#define BUF_SIZE 4096

struct ftp_client
{
    int ctrl_fd;
    int data_fd;
    std::string server_ip;
    int server_port;
    bool is_login;
};

int connect_server(ftp_client* cli);

void send_cmd(ftp_client* cli, const std::string& cmd);
string recv_resp(ftp_client* cli);
int handle_command(ftp_client* cli, const string& cmd);

void handle_pasv(ftp_client* cli);
void handle_list(ftp_client* cli);
void handle_retr(ftp_client* cli, const std::string& file);
void handle_stor(ftp_client* cli, const std::string& file);


int main(){
    ftp_client cli;

    cli.server_ip=SERVER_IP;
    cli.server_port=SERVER_PORT;
    cli.is_login=false;
    cli.data_fd=-1;

    if(connect_server(&cli)<0){
        return -1;
    }
    cout<<recv_resp(&cli);
    while(1){

        string cmd;
        cout<<"ftp> ";

        getline(cin, cmd);

        if(cmd.empty()){
            continue;
        }

        if(handle_command(&cli,cmd)){
            break;
        }
    }
    close(cli.ctrl_fd);
    if(cli.data_fd>0){
        close(cli.data_fd);
    }
    return 0;
}

int connect_server(ftp_client* cli){

    cli->ctrl_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(cli->ctrl_fd<0){
        cout<<"[CLIENT] failed to create socket"<<endl;
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(cli->server_port);

    if(inet_pton(AF_INET,cli->server_ip.c_str(),&server_addr.sin_addr)<=0){
        cout<<"[CLIENT] invalid server ip"<<endl;
        close(cli->ctrl_fd);
        return -1;
    }

    if(connect(cli->ctrl_fd,(sockaddr*)&server_addr,sizeof(server_addr))<0){
        cout<<"[CLIENT] failed to connect server"<<endl;
        close(cli->ctrl_fd);
        return -1;
    }

    cout<<"[CLIENT] connected to server"<<endl;

    return 0;
}

void send_cmd(ftp_client* cli, const string& cmd)
{

    string real_cmd=cmd;

    if(real_cmd.size()<2||real_cmd.substr(real_cmd.size()-2)!="\r\n"){
        real_cmd+="\r\n";
    }

    send(cli->ctrl_fd,real_cmd.c_str(),real_cmd.size(),0);

    cout << "[CLIENT] " << real_cmd;
}
string recv_resp(ftp_client* cli){

    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));

    int n=recv(cli->ctrl_fd,buf,sizeof(buf),0);

    if(n<=0){
        return "[CLIENT] server disconnected\r\n";
    }

    //转成string返回
    return string(buf);
}
int handle_command(ftp_client* cli, const string& cmd)
{
    istringstream iss(cmd);
    string cut;
    vector<string> cmds;

    while(iss >> cut){
        cmds.push_back(cut);
    }
    if(cmds.empty()){
        return 0;
    }

    if(cmds[0]=="USER"){

        if(cmds.size()<2){
            cout<<"[CLIENT] missing username"<<endl;
            return 0;
        }
        send_cmd(cli, cmd);
        cout<<recv_resp(cli);

    }else if(cmds[0]=="PASS"){

        if(cmds.size()<2){
            cout<<"[CLIENT] missing password" << endl;
            return 0;
        }

        send_cmd(cli, cmd);
        string resp=recv_resp(cli);

        cout<<resp;
        if(resp.substr(0,3)=="230"){
            cli->is_login=true;
        }
    }else if(cmds[0]=="PASV"){

        handle_pasv(cli);

    }else if(cmds[0]=="LIST"){

        handle_list(cli);

    }else if(cmds[0]=="RETR"){

        if(cmds.size()<2){
            cout<<"[CLIENT] missing filename"<<endl;
            return 0;
        }
        handle_retr(cli, cmds[1]);

    }else if(cmds[0]=="STOR"){

        if(cmds.size()<2){
            cout<<"[CLIENT] missing filename"<<endl;
            return 0;
        }
        handle_stor(cli, cmds[1]);
    }else if(cmds[0]=="QUIT"){

        send_cmd(cli, "QUIT");
        cout<<recv_resp(cli);
        cout<<"[CLIENT] quiting.."<<endl;
        return 1;
    }else{
        send_cmd(cli, cmd);
        cout<<recv_resp(cli);
    }

    return 0;
}

void handle_pasv(ftp_client* cli){

    int data_port;
    string resp;

    string cmd="PASV";
    send_cmd(cli, cmd);
    resp=recv_resp(cli);
    cout<<resp;

//------------------------------解析
    //查找括号
    int left=resp.find('(');
    int right=resp.find(')');

    if(left==string::npos||right==string::npos){
        cout<<"[CLIENT] invalid PASV response"<<endl;
        return;
    }

    string data=resp.substr(left+1,right-left-1);

    stringstream ss(data);
    string cut;
    vector<int> nums;

    while(getline(ss,cut,',')){
        nums.push_back(stoi(cut));
    }

    if(nums.size()!=6){
        cout<<"[CLIENT] invalid PASV data"<<endl;
        return;
    }
    data_port=nums[4]*256+nums[5];
//----------------------------------


    cli->data_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(cli->data_fd<0){
        cout<<"[CLIENT] failed to create socket"<<endl;
        return;
    }
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(data_port);

    if(inet_pton(AF_INET,cli->server_ip.c_str(),&server_addr.sin_addr)<=0){
        cout<<"[CLIENT] invalid server ip"<<endl;
        close(cli->data_fd);
        return;
    }

    if(connect(cli->data_fd,(sockaddr*)&server_addr,sizeof(server_addr))<0){
        cout<<"[CLIENT] failed to connect server"<<endl;
        close(cli->data_fd);
        return;
    }

    cout<<"[CLIENT] server entered PASV "<<endl;

    return;

}
void handle_list(ftp_client* cli){

}
void handle_retr(ftp_client* cli, const std::string& file){

}
void handle_stor(ftp_client* cli, const std::string& file){
    
}

