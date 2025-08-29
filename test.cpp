#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sys/wait.h>
#include <dirent.h>


void  testets(std::string s)
{
    DIR             *dir;
    struct dirent   *first;

    dir = opendir(s.c_str());
    if (dir == NULL)
    {
        perror("opendir");
        return ;
    }
    first = readdir(dir);
    while (first != NULL)
    {
        std::cout << first->d_name ;
        first = readdir(dir);
    }
    closedir(dir);
}