/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:08:58 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 02:06:51 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Includes.hpp"

class Multiplexer
{
    private:
        int     EpollFd;
    public:
        Multiplexer();
        Multiplexer( const Multiplexer& copy );
        Multiplexer& operator=( const Multiplexer& copy );
        ~Multiplexer();
        bool        InitMultiplexer();
};