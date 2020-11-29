#include "client.h"

Client::Client(std::string ip)
{
    // create client name and set ip
    int size = 100000;
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

    this->handler = StringHandler(this->name.substr(0, this->name.size() - 1));
}

void Client::run(std::string port)
{
    // check if port is integer in the correct range
    if (!std::regex_match(port, std::regex("[0-9]+")))
        return;

    this->port = std::stoi(port);

    if (this->port < 5000 || this->port > 64001)
    {
        this->handler.finalize();
        return;
    }

    this->initialize();

    // get input from terminal and send messages to the server
    if (this->get_work())
    {
        printf("\t\tSent %d transactions\n", this->count);
    }
    else
    {
        this->handler.finalize();
    }
}
void Client::initialize()
{
    // setup server address
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    this->serv_addr.sin_port = htons(this->port); // port

    printf("\t\tUsing port %d\n\t\tUsing server address %s\n\t\tHost %s\n", this->port, this->ip.c_str(), this->name.c_str());
}

bool Client::connect_server()
{   
    // create socket and connect to server
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

    // get input until EOF is found
    while (this->handler.get_input(parsed))
    {
        std::string input = parsed[0];
        // get the the number from the input
        std::string num_tran = input.substr(1);

        // check if it is a number
        if (!std::regex_match(num_tran, std::regex("[0-9]+")))
        {
            continue;
        }

        // if it is work, send work
        if (input.at(0) == 'T')
        {
            if (!this->connect_server())
            {
                close(this->fd);
                return false;
            }

            if (!this->send_message(num_tran) || !this->get_response())
            {
                close(this->fd);
                return false;
            }

            close(this->fd);
        }

        // if it is sleep, put the client to sleep
        else if (input.at(0) == 'S')
        {
            Sleep(std::stoi(num_tran));
            this->handler.print(num_tran, "sleep");
        }

        parsed = std::vector<std::string>();
    }

    return true;
}

bool Client::send_message(std::string val)
{
    // formate message to send to server
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
    this->handler.print(val, "send");

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

    this->handler.print(server_reply, "recv");

    return true;
}
