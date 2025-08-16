#pragma once
#include "Includes.hpp"
#include "Client.hpp"
#include "Server.hpp"
class Multiplexer
{
    private:
        int                                             EpollFd;
        int                                             NumFds;
        int                                             NewConnection;//remove
        struct epoll_event                              Events[MAX_EVENT];
        std::vector<Client>                             Clients;
        std::vector<unsigned char>                      DataRead;
        

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
        void                        SetDataRead( const unsigned char *buff, size_t size );
        std::vector<unsigned char>  GetDataRead();
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
