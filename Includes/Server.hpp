/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/16 21:02:00 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Includes.hpp"
#include "../Includes/Block.hpp"
#include "../headers/webserver.hpp"

class Server
{
    private :
        std::vector <std::string>                               Data;
        std::vector <std::string>                               keys;
        std::map < std::string, std::vector< std::string > >    Commands;
        std::vector < Location >                                Locations;
        int                                                     fd;
    public :
        Server();
        Server( const Server& copy );
        Server& operator=( const Server& copy );
        ~Server();

        void            SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        void	        StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag );
        std::vector<std::string>                                GetKeys();
};
