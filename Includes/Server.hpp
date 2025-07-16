/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/15 23:43:38 by hbenazza         ###   ########.fr       */
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
        std::map < std::string, std::vector< std::string > >    Commands;
        std::vector < Location >                                Locations;
		u_int16_t	fd;
		// u_int16_t	port;
		// u_int32_t	ip;
		// std::string	server_name;
		// std::string root;
		// std::string index;
		// u_int64_t	max_body_size;
		// std::map<u_int16_t , std::string>error_map;

    public :
        Server();
        Server( const Server& copy );
        Server& operator=( const Server& copy );
        ~Server();

        void            SetData( std::string s );
        void            SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        u_int16_t GetFd() const ;

};
