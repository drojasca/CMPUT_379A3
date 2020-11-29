#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include "server.h"

void Server::run(std::string port)
{
    if (!std::regex_match(port, std::regex("[0-9]+")))
        return;

    this->port = std::stoi(port);

    if (this->port < 5000 || this->port > 64001)
        return;

    if (!this->initialize())
    {
        perror("Could not create/bind socket");
        return;
    }

    if (!this->listen_client())
    {
        perror("Error Listening");
        return;
    }

    this->handler.print_statistics(this->clients, this->first, this->count, this->start);
}

bool Server::initialize()
{
    this->fd = socket(AF_INET,     // an Internet socket
                      SOCK_STREAM, // reliable stream-like socket
                      0);
    if (fd < 0)
        return false; // something went wrong

    struct sockaddr_in serv_addr;

    // setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->port); // port

    // bind socket to the server address
    if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        close(this->fd);
        return false;
    }

    if (listen(this->fd, 1) < 0)
    {
        close(this->fd);
        return false;
    }

    this->fds[0].fd = this->fd;
    this->fds[0].events = POLLIN;

    return true;
}

bool Server::listen_client()
{
    int read_size;
    char buffer[1000];

    printf("Using port %d\n", this->port);

    while (1)
    {
        memset(buffer, 0, 1000);
        this->rc = poll(fds, this->nfs, this->timeout);

        if (rc < 0)
        {
            close(this->fd);
            perror("  poll() failed");
            return false;
        }

        if (rc == 0)
        {
            break;
        }

        int c = sizeof(struct sockaddr_in);
        int client;

        //accept connection from an incoming client
        int client_fd = accept(this->fd, (struct sockaddr *)&client, (socklen_t *)&c);
        if (client_fd < 0)
        {
            close(this->fd);
            perror("accept failed");
            return false;
        }

        std::string message = "";
        bool received = false;

        while (message[message.size() - 1] != '\n' && (read_size = recv(client_fd, buffer, 1000, 0)) > 0)
        {
            //Send the message back to client
            received = true;
            message += buffer;
            memset(buffer, 0, 1000);
        }

        if (!received)
        {
            close(this->fd);
            return false;
        }

        std::string client_name = handleMessage(message);
        std::string res = std::to_string(this->count);
        char response[res.size() + 1];
        strcpy(response, res.c_str());
        response[res.size()] = '\0';

        write(client_fd, response, strlen(response));

        auto current = std::chrono::system_clock::now();
        double current_epoch = std::chrono::duration<double>(current.time_since_epoch()).count();
        this->handler.print("done", client_name, this->count, current_epoch);
    }

    close(this->fd);
    return true;
}

std::string Server::handleMessage(std::string message)
{
    this->count++;
    std::vector<std::string> parsed_input;
    this->handler.parse_input(parsed_input, message);

    std::string work = parsed_input[0];
    std::string client_name = parsed_input[1];

    client_name = client_name.substr(0, client_name.size() - 1);

    if (this->clients.find(client_name) == this->clients.end())
    {
        this->clients.insert({client_name, 0});
    }

    auto current = std::chrono::system_clock::now();
    double current_epoch = std::chrono::duration<double>(current.time_since_epoch()).count();

    if (!this->first)
    {
        this->start = current_epoch;
        this->first = true;
    }

    this->clients[client_name]++;
    this->handler.print("work", client_name, this->count, current_epoch, work);

    Trans(std::stoi(work));
    return client_name;
}
