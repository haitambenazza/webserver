/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 03:12:13 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/16 21:20:49 by hbenazza         ###   ########.fr       */
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
		// u_int16_t	fd;
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

        void            SetServer( Block& block);
        std::map < std::string, std::vector< std::string > >    GetCommands();
        std::vector < Location >&                               GetLocations();
        void	        StringToMap( std::string &s, std::map<std::string, std::vector< std::string> >& Mp, int flag );
        std::vector<std::string>                                GetKeys();
        int Getfd() const;
};
