/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:16:31 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 02:11:25 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

bool        Multiplexer::InitMultiplexer()
{
    EpollFd = epoll_create1(0);
    return (true);
}

Multiplexer::Multiplexer()
{

}
