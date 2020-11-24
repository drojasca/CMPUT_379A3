#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include "tands.h"

void Client::run(std::string port, std::string ip)
{
    this->ip = ip;

    if (!std::regex_match(port, std::regex("[0-9]+")))
        return;

    this->port = std::stoi(port);

    if (this->port < 5000 || this->port > 64001)
        return;

    if (this->initialize())
    {
        this->send_server();
    }
}

bool Client::initialize()
{

    struct sockaddr_in serv_addr;

    // setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    serv_addr.sin_port = htons(this->port); // port

    this->fd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->fd < 0)
    {
        return false;
    }

    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // bind socket to the server address
        return false;

    return true;
}

bool Client::send_server()
{
    std::string val = "";
    char message[1000], server_reply[1000];
    //keep communicating with server
    while (this->get_work(val))
    {
        if (val == "F")
        {
            std::cout << "ERROR WITH INPUT" << std::endl;
        }

        memset(message, 0, 1000);
        scanf(val.c_str(), message);

        puts("SENT:");
        puts(val.c_str());

        if (send(this->fd, message, strlen(message), 0) < 0)
        {
            puts("Send failed");
            return false;
        }

        //Receive a reply from the server
        memset(server_reply, 0, 1000);
        if (recv(this->fd, server_reply, 1000, 0) < 0)
        {
            puts("recv failed");
            break;
        }

        // TODO reply
    }

    return true;
}

bool Client::get_work(std::string &num)
{
    std::vector<std::string> parsed;
    bool more = this->handler.get_input(parsed);

    if (!more)
        return false;

    std::string input = parsed[0];

    // get the the number from the input
    std::string num_tran = std::string(input.begin() + 1, input.end());

    // check if it is a numbre
    if (!std::regex_match(num_tran, std::regex("[0-9]+")))
        num = "F";
    return true;

    // if it is for a consumer, put it in the queue
    if (input.at(0) == 'T')
    {
        num = num_tran;
        return true;
    }

    // if it is sleep, put the producer to sleep
    else if (input.at(0) == 'S')
    {
        sleep(std::stoi(num_tran));
        num = "S";
        return true;
    }

    else
        num = "F";
    return true;
}
