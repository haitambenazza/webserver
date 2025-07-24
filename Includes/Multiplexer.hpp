#pragma once
#include "Includes.hpp"

class Multiplexer
{
    private:
        int                     EpollFd;
        int                     NumFds;
        int                     NewConnection;
        struct epoll_event      Events[MAX_EVENT];

    public:
        Multiplexer();
        Multiplexer( const Multiplexer& copy );
		Multiplexer(std::vector<Server> &server);
        Multiplexer& operator=( const Multiplexer& copy );
        ~Multiplexer();

        bool        InitMultiplexer( const std::vector<Server>& server );
        int         GetEpollFd() const;
        int         GetNumFd() const;
        int         GetClientFd() const;
        void         SetEpollFd(int epollFd);
        void         SetNumFd(int numfd);
        void         SetClientFd(int clientfd);
        struct epoll_event*      GetEvents();
};
