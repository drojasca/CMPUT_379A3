#ifndef CLIENT_H
#define CLIENT_H
#include "string_handler.h"
#include "../global.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
class Client
{
public:
    void run(std::string port, std::string ip);
    void initialize();
    bool send_server();
    bool get_work(std::string *num);
    bool connect_server();

private:
    int port;
    int fd;
    std::string ip;
    StringHandler handler;
    struct sockaddr_in serv_addr;
};

#endif