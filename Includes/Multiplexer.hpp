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
