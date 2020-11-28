#include "client.h"

Client::Client(std::string ip)
{
    int size = 1000;
    this->ip = ip;
    int pid = getpid();
    char name[size];

    if (pid < 0)
    {
        perror("Could not get process");
    }

    gethostname(name, size);

    this->name = name;
    this->name += "." + std::to_string(pid) + "\n";
}

void Client::run(std::string port)
{
    if (!std::regex_match(port, std::regex("[0-9]+")))
        return;

    this->port = std::stoi(port);

    if (this->port < 5000 || this->port > 64001)
        return;

    this->initialize();
    if (this->get_work())
    {
        printf("\tSent %d transactions\n", this->count);
    }
}

void Client::print(std::string val, std::string type)
{
    auto current = std::chrono::system_clock::now();
    double current_epoch = std::chrono::duration<double>(current.time_since_epoch()).count();
    if (type == "send")
    {
        printf("T%-7s%.2f: Send (T%3s)\n", val.c_str(), current_epoch, val.c_str());
    }
    else if (type == "recv")
    {
        printf("\t%-10.2f: Recv (D%3s)\n", current_epoch, val.c_str());
    }
    else if (type == "sleep")
    {
        printf("Sleep %s units\n", val.c_str());
    }
}

void Client::initialize()
{
    // setup server address
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    this->serv_addr.sin_port = htons(this->port); // port

    printf("\tUsing port %d\n\tUsing server address %s\n\tHost %s\n", this->port, this->ip.c_str(), this->name.c_str());
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

bool Client::get_work()
{
    std::vector<std::string> parsed;

    while (this->handler.get_input(parsed))
    {
        std::string input = parsed[0];
        // get the the number from the input
        std::string num_tran = input.substr(1);

        // check if it is a numbre
        if (!std::regex_match(num_tran, std::regex("[0-9]+")))
        {
            continue;
        }

        // if it is for a consumer, put it in the queue
        if (input.at(0) == 'T')
        {
            if (!this->connect_server())
                return false;

            if (!this->send_message(num_tran) || !this->get_response())
            {
                return false;
            }

            close(this->fd);
        }

        // if it is sleep, put the producer to sleep
        else if (input.at(0) == 'S')
        {
            Sleep(std::stoi(num_tran));
            this->print(num_tran, "sleep");
        }

        parsed = std::vector<std::string>();
    }

    return true;
}

bool Client::send_message(std::string val)
{
    std::string mess = val + " " + this->name;
    char message[mess.size() + 1];
    strcpy(message, mess.c_str());
    message[mess.size()] = '\0';

    if (send(this->fd, message, strlen(message), 0) < 0)
    {
        puts("Send failed");
        return false;
    }

    this->count++;
    this->print(val, "send");

    return true;
}

bool Client::get_response()
{
    char server_reply[1000];
    //Receive a reply from the server
    memset(server_reply, 0, 1000);
    if (recv(this->fd, server_reply, 1000, 0) < 0)
    {
        puts("recv failed");
        return false;
    }

    this->print(server_reply, "recv");

    return true;
}
