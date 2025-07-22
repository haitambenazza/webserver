/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:16:31 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/22 01:10:20 by kbassim          ###   ########.fr       */
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
    if (epoll_ctl(EpollFd, EPOLL_CTL_ADD, Event.data.fd, &Event))
    {
        perror("Epoll_ctl");
        return (false);
    }
    while (true)
    {
        NumFds = epoll_wait(EpollFd, Events, MAX_EVENT, -1);
        if (NumFds == -1)
        {
            perror("Epoll_wait");
            return (false);
        }
        while (i < NumFds)
        {
            if (Events[i].data.fd == server.Getfd())
            {
                NewConnection = accept(server.Getfd(), NULL, NULL);
                if (NewConnection == -1)
                {
                    perror("Accept");
                    return (false);
                }
            }
            else
            {
                //else if (client is already connected)
                    //HandleRequest;
                //else
                    //Handle cgi
                std::cout << "Still working on it \n";
                break ;
            }
            i++;
        }
    }
}

// Multiplexer::Multiplexer()
// {

// }
