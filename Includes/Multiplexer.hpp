/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:08:58 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/23 06:06:30 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Includes.hpp"

class Multiplexer
{
    private:
        int                     EpollFd;
        int                     NumFds;
        int                     NewConnection;
        struct epoll_event      Event;
        struct epoll_event      Events[MAX_EVENT];

    public:
        Multiplexer();
        Multiplexer( const Multiplexer& copy );
        Multiplexer(const Server &server);
        Multiplexer& operator=( const Multiplexer& copy );
        ~Multiplexer();

        bool        InitMultiplexer( const Server& server );
        int         GetEpollFd() const;
        int         GetNumFd() const;
        int         GetClientFd() const;
        void         SetEpollFd(int epollFd);
        void         SetNumFd(int numfd);
        void         SetClientFd(int clientfd);
        struct epoll_event*      GetEvent();
        struct epoll_event*      GetEvents();
        void SetEvent(int event, int fd);
};
