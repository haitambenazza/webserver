/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:16:31 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/22 21:36:56 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

bool        Multiplexer::InitMultiplexer( const Server& server )
{
    int i;

    i = 0;
    EpollFd = epoll_create1(0);
    if ( EpollFd == -1 )
    {
        perror("Epoll_create");
        return (false);
    }
    Event.events = EPOLLIN ;
    Event.data.fd = server.Getfd();
    if (epoll_ctl(EpollFd, EPOLL_CTL_ADD, Event.data.fd, &Event) == -1)
    {
        perror("Epoll_ctl");
        return (false);
    }
    return (true);
}

Multiplexer::Multiplexer()
{
    EpollFd = -1;
    NumFds = -1;
    NewConnection = -1;
}


Multiplexer::Multiplexer(const Server &server)
{
    EpollFd = -1;
    NumFds = -1;
    NewConnection = -1;
    InitMultiplexer(server);

}

Multiplexer::~Multiplexer()
{
    std::cout << "CLIENT IS DEAD\n";
    close(NewConnection);
    close(EpollFd);
}

int Multiplexer::GetClientFd() const
{
    return (NewConnection);
}

int Multiplexer::GetEpollFd() const
{
    return (EpollFd);
}

int Multiplexer::GetNumFd() const
{
    return (NumFds);
}

void         Multiplexer::SetEpollFd(int epollFd)
{
    EpollFd = epollFd;
}
void         Multiplexer::SetNumFd(int numfd)
{
    NumFds = numfd;
}
void         Multiplexer::SetClientFd(int clientfd)
{
    NewConnection = clientfd;
}
struct epoll_event*      Multiplexer::GetEvent()
{
    return (&Event);
}
struct epoll_event*      Multiplexer::GetEvents()
{
    return (Events);
}

void Multiplexer::SetEvent(int event, int fd)
{
    Event.data.fd = fd;
    Event.events = event;
}
