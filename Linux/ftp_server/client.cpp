#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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
std::string recv_resp(ftp_client* cli);

void handle_login(ftp_client* cli);
void handle_pasv(ftp_client* cli);
void handle_list(ftp_client* cli);
void handle_retr(ftp_client* cli, const std::string& file);
void handle_stor(ftp_client* cli, const std::string& file);

int parse_pasv_port(const std::string& resp);
std::string parse_pasv_ip(const std::string& resp);

