#ifndef GLOBAL_H
#define GLOBAL_H

#include <queue>
#include <istream>
#include <iostream>
#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

struct consumer
{
    int work = 0;
    int ask = 0;
    int recieve = 0;
    int complete = 0;
    int sleep = 0;
    std::vector<int> finshed;
};

#endif