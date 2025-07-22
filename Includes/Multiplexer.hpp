/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:08:58 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/22 00:19:02 by kbassim          ###   ########.fr       */
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
        Multiplexer& operator=( const Multiplexer& copy );
        ~Multiplexer();
        
        bool        InitMultiplexer( const Server& server );
};