#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "client.h"

void Client::run(std::string port, std::string ip)
{
    this->ip = ip;

    if (!std::regex_match(port, std::regex("[0-9]+")))
        return;

    this->port = std::stoi(port);

    if (this->port < 5000 || this->port > 64001)
        return;

    this->initialize();
    this->send_server();
}

void Client::initialize()
{
    // setup server address
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    this->serv_addr.sin_port = htons(this->port); // port
}

bool Client::connect_server()
{
    this->fd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->fd < 0)
    {
        return false;
    }

    if (connect(this->fd, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr)) < 0)
    {
        std::cout << "FAILED TO CONNECT" << std::endl;
        return false;
    }

    return true;
}

bool Client::send_server()
{
    std::string val = "";
    char message[1000], server_reply[1000];
    //keep communicating with server
    while (this->get_work(&val))
    {
        if (val == "F")
        {
            std::cout << "ERROR WITH INPUT" << std::endl;
            continue;
        }

        else if (val == "S")
        {
            continue;
        }

        if (!this->connect_server())
            return false;

        memset(message, 0, 1000);
        strcpy(message, val.c_str());
        message[val.size()] = '\0';

        if (send(this->fd, message, strlen(message), 0) < 0)
        {
            puts("Send failed");
            return false;
        }

        std::cout << "waiting" << std::endl;

        //Receive a reply from the server
        memset(server_reply, 0, 1000);
        if (recv(this->fd, server_reply, 1000, 0) < 0)
        {
            puts("recv failed");
            break;
        }

        close(this->fd);
    }

    return true;
}

bool Client::get_work(std::string *num)
{
    std::vector<std::string> parsed;
    bool more = this->handler.get_input(parsed);

    if (!more)
        return false;

    std::string input = parsed[0];
    // get the the number from the input
    std::string num_tran = input.substr(1);

    std::cout << input << std::endl;

    // check if it is a numbre
    if (!std::regex_match(num_tran, std::regex("[0-9]+")))
    {
        *num = "F";
        return true;
    }

    // if it is for a consumer, put it in the queue
    if (input.at(0) == 'T')
    {
        this->count++;
        *num = num_tran;
        return true;
    }

    // if it is sleep, put the producer to sleep
    else if (input.at(0) == 'S')
    {
        Sleep(std::stoi(num_tran));
        this->count++;
        *num = "S";
        return true;
    }

    else
        *num = "F";
    return true;
}
