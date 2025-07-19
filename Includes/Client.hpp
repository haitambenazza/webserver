/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:08:01 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/18 23:08:04 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include "Includes.hpp"


class Client
{
    private :
        int16_t   fd;
        // int16_t    endpoint_fd;
    public :
        Client();
        Client( const Client& copy );
        Client& operator=( const Client& copy );
        bool	SetClient();
        ~Client();

};