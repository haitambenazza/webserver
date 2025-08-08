#pragma once
#include "Includes.hpp"

class Multiplexer
{
    private:
        int                                             EpollFd;
        int                                             NumFds;
        int                                             NewConnection;//remove
        struct epoll_event                              Events[MAX_EVENT];
        std::vector<Client>                             Clients;

    public:
        Multiplexer();
        Multiplexer( const Multiplexer& copy );
		Multiplexer(std::vector<Server> &server);
        Multiplexer& operator=( const Multiplexer& copy );
        ~Multiplexer();

        bool                        InitMultiplexer( const std::vector<Server>& server );
        int                         GetEpollFd() const;
        int                         GetNumFd() const;
        int                         GetClientFd() const;
        void                        SetEpollFd(int epollFd);
        void                        SetNumFd(int numfd);
        void                        SetClientFd(int clientfd);
        struct epoll_event*         GetEvents();
        void                        AddClient( Client& NewClient );
        std::vector<Client>&        GetClient();
        void                        RemoveClient(int i);
};


/*
    base class event handler -> constructor(fd) , read()  = 0 write()  = 0, acceptClient() = 0

    serverHandler = accetClient. read , write {}

    clientHandler = read, write, accept {}

    multiplexer :
    {
        epoll fd
        map<fd, event*> = [
            {fd, event *}. server
            {fd, event *}. server
            {fd, event *}. server
            {fd, event *}. server
        ]
    }

    while (running)
    {
        loop through fd you get from epoll

        if fd is a server  use dynamic casting.
        if server:
            clienthandler = castfrom event.accept() -> new instance of client handler(fd client)
            {
                add to epoll with flags : epollin and epollout
                multiplexer map<fd, event> += fd, clientHandler
            }
        if client :
            handle client() --> state;
            state : {read, body, write, close}
            if state is close:
                remove from epoll
                remove from map


    }


    handle new cl --- read request response close
*/

//hhh in in in in in read out out out
